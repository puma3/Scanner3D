#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <stdio.h>
#include <stdlib.h>

using namespace std;
using namespace cv;

Scan scan;

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow),
    iterator(0)
{
    ui->setupUi(this);

    //std::thread thread1(startCamera);
    //thread1.join();

    //Inicializar la comunicacion serial
//    std::thread t(&MainWindow::startCamera, this);
//    initThread();
    startCamera();
    initSerial();

//    QThread* thread = new QThread;
//    Worker* worker = new Worker();
//    worker->setup(this, &capWebCam);
//    worker->moveToThread(thread);
////    connect(worker, SIGNAL (error(QString)), this, SLOT (errorString(QString)));
//    connect(thread, SIGNAL (started()), worker, SLOT (process()));
//    connect(worker, SIGNAL (finished()), thread, SLOT (quit()));
//    connect(worker, SIGNAL (finished()), worker, SLOT (deleteLater()));
//    connect(thread, SIGNAL (finished()), thread, SLOT (deleteLater()));
//    thread->start();
}

void MainWindow::initThread()
{
//    t.join();
}

void MainWindow::startCamera()
{
    capWebCam.open(1);
    if(!capWebCam.isOpened())
    {
        cout<<"Error"<<endl;
        return;
    }

    Timer = new QTimer(this);
    connect(Timer, SIGNAL(timeout()),this, SLOT(on_actionCapture()));
    Timer->start(1000);
//    Timer->start();
}

void MainWindow::initSerial()
{
    QSerialPortInfo info("usbmodem621");
    // Check info of the port
    qDebug() << "Name        : " << info.portName();
    qDebug() << "Manufacturer: " << info.manufacturer(); //if showing manufacturer, means Qstring &name is good
    qDebug() << "Busy: " << info.isBusy() << endl;

    // Initialize Serial
    QSerialPort serial;
    serial.setPortName("usbmodem621");
    serial.open(QIODevice::ReadWrite);
    serial.setBaudRate(QSerialPort::Baud9600);
    serial.setDataBits(QSerialPort::Data8);
    serial.setParity(QSerialPort::NoParity);
    serial.setStopBits(QSerialPort::OneStop);
    serial.setFlowControl(QSerialPort::NoFlowControl);

    if (serial.isOpen() && serial.isWritable()) {
        QByteArray ba("R");
        serial.write(ba);
        serial.flush();
        qDebug() << "data has been send" << endl;
        serial.close();
    }

    else {
        qDebug() << "An error occured" << endl;
    }
}


void MainWindow::on_actionCapture()
{

    capWebCam >> matOriginal;
//    for(;;)
    {
        Mat frame;
        capWebCam >> frame;
        scan.listFrames.push_back(frame);
        QString path = "output/scan" + QString::number(iterator++) + ".jpg";
        imwrite(path.toStdString(), frame );
    }

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



