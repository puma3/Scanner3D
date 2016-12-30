#ifndef OGLWIDGET_H
#define OGLWIDGET_H

#include <QWidget>
#include <QOpenGLWidget>
//#include <GL/glu.h>
//#include <GL/gl.h>
#include <GL/freeglut.h>
#include "pointcloud.h"

class OGLWidget : public QOpenGLWidget
{
    Q_OBJECT
public:
    OGLWidget(QWidget *parent = 0);
    ~OGLWidget();

    void drawCircle();
    void drawPolygon(int n, float r);
    void drawLine(float p1_x, float p1_y, float p2_x, float p2_y);

    int n1,
        n2;
    float x1,
          y1,
          x2,
          y2;

protected:
    void initializeGL();
    void resizeGL(int w, int h);
    void paintGL();
};

#endif // OGLWIDGET_H
