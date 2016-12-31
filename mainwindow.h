#ifndef MAINWINDOW_H
#define MAINWINDOW_H
#include "scan.h"
#include <QTimer>
#include <QMainWindow>
#include <QPainter>
#include <QtSerialPort/QSerialPort>
#include <QtSerialPort/QSerialPortInfo>
#include <QDebug>
#include <QThread>
#include "worker.h"
#include "renderer.h"

using namespace std;
using namespace cv;

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

    void start();

//    void initThread();

    void setupCamera();

    void initSerial();

    void cameraShow();

    VideoCapture capWebCam;
    QTimer * timer;
    uint iterator;

private:
    Renderer *rndr;

    QImage camera_frame;

    int *brightestPixls;

    int width,
        height,
        v_guideline_pos,
        h_guideline_pos;

    bool highlight;

protected slots:
    void showCameraFrame();

    void highlight_bright_pixels();

protected:
    Ui::MainWindow *ui;

//    Mat matProcessed;
    Mat matOriginal;

    QImage qimgOriginal;
    QImage qimgProcessed;

private slots:
    void on_pushButton_2_clicked();

    void on_scan_bttn_clicked();

signals:
    void startCapturing(int, int);
};

#endif // MAINWINDOW_H
