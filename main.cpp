#include "mainwindow.h"
#include <QApplication>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/core/core.hpp>
#include <iostream>

using namespace std;
using namespace cv;

const int NUM_SECOND=10;

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    MainWindow w;
    w.show();

    /*Mat img=imread("pikachu.jpg");
    namedWindow("Imagen",WINDOW_AUTOSIZE);
    imshow("Imagen",img);*/

    return a.exec();
}
