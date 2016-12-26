#include "mainwindow.h"
#include "ui_mainwindow.h"

using namespace std;
using namespace cv;

Scan scan;

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    //std::thread thread1(startCamera);
    //thread1.join();
    startCamera();

}

void MainWindow::startCamera()
{
    capWebCam.open(0);
    if(capWebCam.isOpened()==false)
    {
        cout<<"Error"<<endl;
        return;
    }
    Timer = new QTimer(this);
    connect(Timer, SIGNAL(timeout()),this, SLOT(on_actionCapture()));
    Timer->start();

}


void MainWindow::on_actionCapture()
{

    capWebCam >> matOriginal;
    /*for(int i=0;i<100;++i)
    {
        scan.listFrames[i].create(50,50,CV_8UC3);
    }*/

    cvtColor(matOriginal,matOriginal,CV_BGR2RGB);
    QImage img=QImage((uchar*) matOriginal.data, matOriginal.cols, matOriginal.rows, matOriginal.step, QImage::Format_RGB888);
    ui->camera->setPixmap(QPixmap::fromImage(img));
}

MainWindow::~MainWindow()
{
    delete ui;
}



