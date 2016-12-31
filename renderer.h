#ifndef RENDERER_H
#define RENDERER_H

#include <QDialog>
#include <QDebug>
#include <QTimer>
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
    explicit Renderer(VideoCapture *capturer, int *_brightestPixls, QWidget *parent = 0);
    ~Renderer();

    void setFrameSize(int _w = 640, int _h = 480);

    void setAngles(float _laserAngle, float _stepAngle);

    void frameBrightestPixels_();

public slots:
    void captureFrames(int n, int rate);

    void frameBrightestPixels();

    void processSlice();

private:
    Ui::Renderer *ui;

    VideoCapture *capWebCam;

    Mat transformationMat;

    int width,
        height,
        middle_x,
        n_frames,
        iterator;

    int *brightestPixls;

    float laserAngle,
          stepAngle;

    PointCloud points;

    QTimer timer;

signals:
    void finishedPixelCalculation();

    void finishedSliceProcessing();
};

#endif // RENDERER_H
