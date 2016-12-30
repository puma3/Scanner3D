#include "oglwidget.h"

OGLWidget::OGLWidget(QWidget *parent) :
    QOpenGLWidget(parent)
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

    glutInitDisplayMode (GLUT_DOUBLE | GLUT_RGB);
    glClearColor (0.0, 0.0, 0.0, 0.0);
    glShadeModel (GL_FLAT);

    glColor3f(1.0, 1.0, 1.0);
    glPointSize(1.0);
}

void OGLWidget::resizeGL(int w, int h)
{
//    glViewport(0,0,w,h);
//    glMatrixMode(GL_PROJECTION);
//    glLoadIdentity();
//    gluPerspective(45, (float)w/h, 0.01, 100.0);
//    glMatrixMode(GL_MODELVIEW);
//    glLoadIdentity();
//    gluLookAt(0,0,5,0,0,0,0,1,0);


    int width = 640,
        height = 480;
    glViewport (0, 0, (GLsizei) w, (GLsizei) h);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
//    glOrtho(-width/2.0, width/2.0, -height/2.0, height/2.0, -1.0, 1.0);
    glOrtho(-width/2.0, width/2.0, 0.0, height, -width/2.0, width/2.0);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
}

void OGLWidget::paintGL()
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

/////TRIANGLE
//    glBegin(GL_TRIANGLES);
//        glColor3f(1.0, 0.0, 0.0);
//        glVertex3f(-0.5, -0.5, 0);
//        glColor3f(0.0, 1.0, 0.0);
//        glVertex3f( 0.5, -0.5, 0);
//        glColor3f(0.0, 0.0, 1.0);
//        glVertex3f( 0.0,  0.5, 0);
//    glEnd();

    qDebug() << cloud->cloud.size();
    glBegin(GL_POINTS);
        for (auto it = cloud->cloud.begin(); it != cloud->cloud.end(); ++it)
            glVertex3f(it->x, it->y, it->z);
    glEnd();
}
