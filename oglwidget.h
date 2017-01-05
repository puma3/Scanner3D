#ifndef OGLWIDGET_H
#define OGLWIDGET_H

#include <QWidget>
#include <QOpenGLWidget>
#include <QtGlobal>
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

    void resizeGL(int w, int h);

    PointCloud *cloud;

public slots:
    void setRotX(int _rot)  {   rotX = _rot;    update();   }

protected:
    void initializeGL();
    void paintGL();

private:
    int rotX;
};

#endif // OGLWIDGET_H
