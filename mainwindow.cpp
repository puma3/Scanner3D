#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <stdio.h>
#include <stdlib.h>

#define _NO_WEBCAM_
#define PERIOD 68700
//#define PERIOD 5000
#define ANGLE_CAMERA 45.0

using namespace std;
using namespace cv;

Scan scan;

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    iterator(0),
    highlight(false),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    setupCamera();
    cameraShow();
    brightestPixls = new int[height];
    rndr = new Renderer(&capWebCam, brightestPixls, this);  //New Renderer
    connect(this, SIGNAL(startCapturing(int, int)), rndr, SLOT(captureFrames(int, int)));
    connect(rndr, SIGNAL(finishedPixelCalculation()), this, SLOT(highlight_bright_pixels()));
    rndr->setFrameSize(width, height);
    initSerial();               //Inicializar la comunicacion serial
}

MainWindow::~MainWindow()
{
    delete []brightestPixls;
    delete timer;
    delete ui;
}

void MainWindow::setupCamera()
{
#ifndef _NO_WEBCAM_
    capWebCam.open(1);
#else
    capWebCam.open(0);
#endif
    if(!capWebCam.isOpened()) {
        cout << "Camera couldn't start" << endl;
        return;
    }
    width = capWebCam.get(CV_CAP_PROP_FRAME_WIDTH);
    height = capWebCam.get(CV_CAP_PROP_FRAME_HEIGHT);
    v_guideline_pos = width / 2;
    h_guideline_pos = height * 9 / 10;
}

void MainWindow::initSerial()
{
//    QSerialPortInfo info("usbmodem621");
//    // Check info of the port
//    qDebug() << "Name        : " << info.portName();
//    qDebug() << "Manufacturer: " << info.manufacturer(); //if showing manufacturer, means Qstring &name is good
//    qDebug() << "Busy: " << info.isBusy() << endl;

//    // Initialize Serial
//    QSerialPort serial;
//    serial.setPortName("usbmodem621");
//    serial.open(QIODevice::ReadWrite);
//    serial.setBaudRate(QSerialPort::Baud9600);
//    serial.setDataBits(QSerialPort::Data8);
//    serial.setParity(QSerialPort::NoParity);
//    serial.setStopBits(QSerialPort::OneStop);
//    serial.setFlowControl(QSerialPort::NoFlowControl);

//    if (serial.isOpen() && serial.isWritable()) {
//        QByteArray ba("R");
//        serial.write(ba);
//        serial.flush();
//        qDebug() << "data has been send" << endl;
//        serial.close();
//    }

//    else {
//        qDebug() << "An error occured" << endl;
    //    }
}

void MainWindow::cameraShow()
{
    timer = new QTimer(this);
    connect(timer, SIGNAL(timeout()), this, SLOT(showCameraFrame()));
    timer->start(17);
}

void MainWindow::showCameraFrame()
{
    capWebCam >> matOriginal;

    cvtColor(matOriginal,matOriginal,CV_BGR2RGB);
    camera_frame = QImage((uchar*) matOriginal.data,
                          matOriginal.cols,
                          matOriginal.rows,
                          matOriginal.step,
                          QImage::Format_RGB888);

    //Inicializamos el Painter
    QPainter painter;
    painter.begin(&camera_frame);
    painter.setCompositionMode(QPainter::RasterOp_SourceXorDestination);
    painter.setPen(QColor(0x33, 0xff, 0x33));

    //Dibujamos las lineas guia
    painter.drawLine(v_guideline_pos, 0, v_guideline_pos, height);
    painter.drawLine(0, h_guideline_pos, width, h_guideline_pos);

    //Escribir sobre pixels mas brillantes
    if(highlight) {
    for (int i=0; i < height ; i++)
        camera_frame.setPixel(brightestPixls[i], i, qRgb(0, 0, 255));
    }

    //Mandar imagen a label: camera
    ui->camera->setPixmap(QPixmap::fromImage(camera_frame));
}

void MainWindow::highlight_bright_pixels()
{
    highlight = true;
}

void MainWindow::on_pushButton_2_clicked() //Mesh rendering button
{
    rndr->show();
}

void MainWindow::on_scan_bttn_clicked()
{
    rndr->setAngles(ANGLE_CAMERA, 360.0 / ui->frames_spinBox->value());
    ui->pushButton_2->setEnabled(true);
    emit startCapturing(ui->frames_spinBox->value(), PERIOD / ui->frames_spinBox->value());
}
