#ifndef MAINWINDOW_H
#define MAINWINDOW_H
#include "scan.h"
#include <QTimer>
#include <QMainWindow>
#include <QPainter>
#include <QDebug>
#include <QThread>
#include "renderer.h"
#include "overlaywidget.h"

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

    void setupCamera();

    void cameraShow();

    bool eventFilter(QObject *obj, QEvent *event);

//public members
    VideoCapture capWebCam;
    QTimer * timer;

private:
    Renderer *rndr;

    QImage camera_frame;

    int *brightestPixls;

    int width,
        height,
        v_guideline_pos,
        h_guideline_pos;

    bool highlight;

    SelectionOverlay *selection;

protected slots:
    void showCameraFrame();

    void highlight_bright_pixels();

protected:
    Ui::MainWindow *ui;

    Mat matOriginal;

private slots:
    void on_pushButton_2_clicked();

    void on_scan_bttn_clicked();

signals:
    void startCapturing(int, int);
};

#endif // MAINWINDOW_H
