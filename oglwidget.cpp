#include "oglwidget.h"

OGLWidget::OGLWidget(QWidget *parent) :
    QOpenGLWidget(parent),
    rotX(0)
{
}

OGLWidget::~OGLWidget()
{
}

void OGLWidget::setPointCloud(PointCloud *ptr)
{
    cloud = ptr;
}

void OGLWidget::initializeGL()
{
//    glClearColor(0, 0, 0, 1);
//    glEnable(GL_DEPTH_TEST);
//    glEnable(GL_LIGHT0);
//    glEnable(GL_LIGHTING);
//    glColorMaterial(GL_FRONT_AND_BACK, GL_AMBIENT_AND_DIFFUSE);
//    glEnable(GL_COLOR_MATERIAL);


    //glutInitDisplayMode (GLUT_DOUBLE | GLUT_RGB);

    glutInitDisplayMode (GLUT_SINGLE | GLUT_RGB | GLUT_DEPTH);
    glClearColor (0.0, 0.0, 0.0, 0.0);
    glShadeModel (GL_FLAT);

//    LIGHTS
//    GLfloat ambient[] = { 0.2, 0.2, 0.2, 1.0 };
    GLfloat ambient[] = { 1.0, 1.0, 1.0, 1.0 };
    GLfloat position[] = { 0.0, 240.0, 1.0, 1.0 };
    GLfloat mat_diffuse[] = { 0.6, 0.6, 0.6, 0.5 };
    GLfloat mat_specular[] = { 1.0, 1.0, 1.0, 1.0 };
    GLfloat mat_shininess[] = { 100.0 };

    glEnable(GL_LIGHTING);
    glEnable(GL_LIGHT0);

    glLightfv(GL_LIGHT0, GL_AMBIENT, ambient);
    glLightfv(GL_LIGHT0, GL_POSITION, position);

    glMaterialfv(GL_FRONT, GL_DIFFUSE, mat_diffuse);
    glMaterialfv(GL_FRONT, GL_SPECULAR, mat_specular);
    glMaterialfv(GL_FRONT, GL_SHININESS, mat_shininess);

//    glEnable(GL_DEPTH_TEST);
//    glEnable(GL_CULL_FACE);
//    glShadeModel(GL_SMOOTH);
//    glEnable(GL_LIGHTING);
//    glEnable(GL_LIGHT0);

    static GLfloat lightPosition[4] = { 0, 0, 0, 0.5 };
    glLightfv(GL_LIGHT0, GL_POSITION, lightPosition);

    //Points
    glColor3f(1.0, 1.0, 1.0);
    glPointSize(1.5);
}

void OGLWidget::resizeGL(int w, int h)
{
    int width = 640,
        height = 480;
    glViewport (0, 0, (GLsizei) w, (GLsizei) h);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glOrtho(-width/2.0, width/2.0, 0.0, height, width/2.0, 0.0);
    glMatrixMode(GL_MODELVIEW);
}

void OGLWidget::paintGL()
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
//    glTranslatef(0.0, 0.0, 240);

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

//            glVertex3f(it->edge_2->x, it->edge_2->y, it->edge_2->z);
//            glVertex3f(it->edge_3->x, it->edge_3->y, it->edge_3->z);
//            glVertex3f(it->edge_4->x, it->edge_4->y, it->edge_4->z);
        }
    glEnd();

    glPushMatrix();//GL_MODELVIEW);
    glLoadIdentity();
    glPopMatrix();//GL_MODELVIEW);
}
