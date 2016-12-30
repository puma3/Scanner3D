#ifndef RENDERER_H
#define RENDERER_H

#include <QDialog>
#include <QDebug>
#include <math.h>
#include "scan.h"
#include "pointcloud.h"
#include "oglwidget.h"

namespace Ui {
class Renderer;
}

class Renderer : public QDialog
{
    Q_OBJECT

public:
    explicit Renderer(QWidget *parent = 0);
    ~Renderer();

    void setFrameSize(int _w = 640, int _h = 480);
    void setAngles(float _laserAngle, float _stepAngle);

public slots:
    void frameBrightestPixels();

    void processSlice();

private:
    Ui::Renderer *ui;

    int width,
        height,
        middle_x,
        iterator;

    int *brightestPixls;

    float laserAngle,
          stepAngle;

    PointCloud points;

signals:
    void finishedPixelCalculation();

    void finishedSliceProcessing();
};

#endif // RENDERER_H
