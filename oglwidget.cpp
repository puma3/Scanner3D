#include "oglwidget.h"

//#define _TESTING_CODE_

OGLWidget::OGLWidget(QWidget *parent) :
    QOpenGLWidget(parent),
    rotX(0)
{}

OGLWidget::~OGLWidget()
{
}

void OGLWidget::setPointCloud(PointCloud *ptr)
{
    cloud = ptr;
}

void OGLWidget::initializeGL()
{
#ifndef _TESTING_CODE_
    glutInitDisplayMode (GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
    glClearColor (0.0, 0.0, 0.0, 0.0);
    glShadeModel (GL_FLAT);

    glEnable(GL_DEPTH_TEST);
    glShadeModel(GL_SMOOTH);
    glEnable(GL_LIGHTING);
    glEnable(GL_LIGHT0);

//    LIGHTS
    GLfloat ambient[] = { 1.0, 1.0, 1.0, 0.5 };
    GLfloat position[] = { 0.0, 160.0, 10.0, 1.0 };
    GLfloat mat_diffuse[] = { 0.6, 0.6, 0.6, 40.0 };
    GLfloat mat_specular[] = { 1.0, 1.0, 1.0, 1.0 };
    GLfloat mat_shininess[] = { 100.0 };

    glLightfv(GL_LIGHT0, GL_AMBIENT, ambient);
    glLightfv(GL_LIGHT0, GL_POSITION, position);

    glMaterialfv(GL_FRONT, GL_DIFFUSE, mat_diffuse);
    glMaterialfv(GL_FRONT, GL_SPECULAR, mat_specular);
    glMaterialfv(GL_FRONT, GL_SHININESS, mat_shininess);
#else
    glClearColor (0.0, 0.0, 0.0, 0.0);
       glShadeModel (GL_SMOOTH);
       glEnable(GL_LIGHTING);
       glEnable(GL_LIGHT0);
       glEnable(GL_DEPTH_TEST);
#endif

    //Points
    glColor3f(1.0, 1.0, 1.0);
    glPointSize(1.5);
}

void OGLWidget::resizeGL(int w, int h)
{
    int width = 640,
        height = 480;
#ifndef _TESTING_CODE_
    glViewport (0, 0, (GLsizei) w, (GLsizei) h);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glOrtho(-width/2.0, width/2.0, 0.0, height, width/2.0, 0.0);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
#else
    glViewport (0, 0, (GLsizei) w, (GLsizei) h);
       glMatrixMode (GL_PROJECTION);
       glLoadIdentity();
       glOrtho(-width/2.0, width/2.0, 0.0, height, width/2.0, 0.0);
       gluPerspective(40.0, (GLfloat) w/(GLfloat) h, 1.0, 20.0);
       glMatrixMode(GL_MODELVIEW);
       glLoadIdentity();
#endif
}

void OGLWidget::paintGL()
{
#ifndef _TESTING_CODE_
//    GLfloat ambient[] = { 1.0, 1.0, 1.0, 0.5 };
//    GLfloat position[] = { 0.0, 160.0, 480.0, 1.0 };
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

//    glPushMatrix();
//        gluLookAt (0.0, 0.0, 150.0, 0.0, 0.0, 0.0, 0.0, 1.0, 0.0);
//        glPushMatrix();
//            glLightfv(GL_LIGHT0, GL_AMBIENT, ambient);
//            glLightfv(GL_LIGHT0, GL_POSITION, position);
//        glPopMatrix();


    //Rotation
    glRotatef(rotX, 0.0, 1.0, 0.0);

    glBegin(GL_POINTS);
        for (auto it = cloud->cloud.begin(); it != cloud->cloud.end(); ++it)
            glVertex3f(it->x, it->y, it->z);
    glEnd();

    glBegin(GL_TRIANGLES);
        for (auto it = cloud->mesh.begin(); it != cloud->mesh.end(); ++it) {
            glVertex3f(it->edge_1->x, it->edge_1->y, it->edge_1->z);
            glVertex3f(it->edge_2->x, it->edge_2->y, it->edge_2->z);
            glVertex3f(it->edge_4->x, it->edge_4->y, it->edge_4->z);

            glVertex3f(it->edge_2->x, it->edge_2->y, it->edge_2->z);
            glVertex3f(it->edge_3->x, it->edge_3->y, it->edge_3->z);
            glVertex3f(it->edge_4->x, it->edge_4->y, it->edge_4->z);
        }
    glEnd();

    glLoadIdentity();
    glPopMatrix();              //GL_MODELVIEW);
    glFlush();
#else
    GLfloat position[] = { 0.0, 0.0, 1.5, 1.0 };

       glClear (GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
       glPushMatrix ();
       glTranslatef (0.0, 0.0, -5.0);

       glPushMatrix ();
       glRotated ((GLdouble) 0.0, rotX/360, 0.0, 0.0);
       glLightfv (GL_LIGHT0, GL_POSITION, position);

       glTranslated (0.0, 0.0, 1.5);
       glDisable (GL_LIGHTING);
       glColor3f (1.0, 1.0, 1.0);

       glBegin(GL_TRIANGLES);
           for (auto it = cloud->mesh.begin(); it != cloud->mesh.end(); ++it) {
               glVertex3f(it->edge_1->x, it->edge_1->y, it->edge_1->z);
               glVertex3f(it->edge_2->x, it->edge_2->y, it->edge_2->z);
               glVertex3f(it->edge_4->x, it->edge_4->y, it->edge_4->z);

               glVertex3f(it->edge_2->x, it->edge_2->y, it->edge_2->z);
               glVertex3f(it->edge_3->x, it->edge_3->y, it->edge_3->z);
               glVertex3f(it->edge_4->x, it->edge_4->y, it->edge_4->z);
           }

       glEnd();
       glEnable (GL_LIGHTING);
       glPopMatrix ();

       glFlush ();
#endif
}
