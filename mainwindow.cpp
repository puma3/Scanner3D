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


    std::thread t(&MainWindow::startCamera,this);
        t.join();

}

void MainWindow::startCamera()
{
    capWebCam.open(0);
    if(!capWebCam.isOpened())
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

    //Inicializamos el Painter
    QPainter painter;
    painter.begin(&img);
    painter.setCompositionMode(QPainter::RasterOp_SourceXorDestination);
    painter.setPen(QColor(0x11, 0xff, 0x22));

    //Dibujamos las lineas guia
    int w = img.width() / 2,
        h = img.height() * 9 / 10;
    painter.drawLine(w, 0, w, img.height());
    painter.drawLine(0, h, img.width(), h);

    ui->camera->setPixmap(QPixmap::fromImage(img));
}

MainWindow::~MainWindow()
{
    delete ui;
}



