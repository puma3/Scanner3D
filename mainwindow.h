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
    void startCamera();
    void initThread();
    void initSerial();

    VideoCapture capWebCam;
    QTimer * Timer;
    uint iterator;

protected slots:
    void on_actionCapture();

protected:
    Ui::MainWindow *ui;

    Mat matProcessed;
    Mat matOriginal;

    QImage qimgOriginal;
    QImage qimgProcessed;
};

#endif // MAINWINDOW_H
