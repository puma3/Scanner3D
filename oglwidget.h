#ifndef OGLWIDGET_H
#define OGLWIDGET_H

#include <QWidget>
#include <QOpenGLWidget>
//#include <GL/glu.h>
//#include <GL/gl.h>
#include <GL/freeglut.h>
#include "pointcloud.h"
#include <iostream>
#include <QDebug>

using namespace std;

class OGLWidget : public QOpenGLWidget
{
    Q_OBJECT
public:
    OGLWidget(QWidget *parent = 0);
    ~OGLWidget();

    void setPointCloud(PointCloud *ptr);

    PointCloud *cloud;

    void resizeGL(int w, int h);

protected:
    void initializeGL();
    void paintGL();
};

#endif // OGLWIDGET_H
