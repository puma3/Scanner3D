#ifndef MAINWINDOW_H
#define MAINWINDOW_H
#include "scan.h"
#include <QTimer>
#include <QMainWindow>
#include <QPainter>

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

    VideoCapture capWebCam;
    QTimer * Timer;

private slots:
    void on_actionCapture();


private:
    Ui::MainWindow *ui;

    Mat matProcessed;
    Mat matOriginal;

    QImage qimgOriginal;
    QImage qimgProcessed;


};

#endif // MAINWINDOW_H
