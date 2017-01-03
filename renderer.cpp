#include "renderer.h"
#include "ui_renderer.h"

//#define _WORKING_WITH_FILES_

Renderer::Renderer(VideoCapture *capturer, int *_brightestPixls, QWidget *parent) :
    QDialog(parent),
    ui(new Ui::Renderer),
    capWebCam(capturer),
    iterator(0),
    brightestPixls(_brightestPixls),
    timer(this)
{
    ui->setupUi(this);
    ui->openGLWidget->setPointCloud(&points);
    initSerial();               //Inicializar la comunicacion serial
#ifndef _WORKING_WITH_FILES_
    connect(this, SIGNAL(finishedPixelCalculation()), this, SLOT(processSlice()));
#endif
}

Renderer::~Renderer()
{
    delete ui;
}

void Renderer::setFrameSize(int _w, int _h)
{
    width = _w;
    height = _h;
    middle_x = _w / 2;

    r_x0 = 0;
    r_y0 = 0;
    r_xf = width;
    r_yf = height;
}

void Renderer::setAngles(float _laserAngle, float _stepAngle)
{
    laserAngle = _laserAngle * 3.1416 / 180;
    stepAngle = _stepAngle * 3.1416 / 180;
}

void Renderer::setRange(int x0, int y0, int xf, int yf)
{
    qDebug() << "Setting: " << x0 << y0 << xf << yf;
    r_x0 = x0;
    r_y0 = y0;
    r_xf = xf;
    r_yf = yf;
}

void Renderer::initSerial()
{
    QSerialPortInfo info("COM1");
    // Check info of the port
    qDebug() << "Name        : " << info.portName();
    qDebug() << "Manufacturer: " << info.manufacturer(); //if showing manufacturer, means Qstring &name is good
    qDebug() << "Busy: " << info.isBusy() << endl;

    // Initialize Serial
    QSerialPort serial;
    serial.setPortName("Arduino");
    serial.open(QIODevice::ReadWrite);
    serial.setBaudRate(QSerialPort::Baud9600);
    serial.setDataBits(QSerialPort::Data8);
    serial.setParity(QSerialPort::NoParity);
    serial.setStopBits(QSerialPort::OneStop);
    serial.setFlowControl(QSerialPort::NoFlowControl);
}

void Renderer::captureFrames(int n, int rate) {
    n_frames = n;
    iterator = 0;
    qDebug() << "Will capture" << n_frames << "frames at" << rate << "ms";
    qDebug() << "Range: (" << r_x0 << ", " << r_y0 << ") -> (" << r_xf << ", " << r_yf << ")";
#ifdef _WORKING_WITH_FILES_
    frameBrightestPixels_();
#endif
    connect(&timer, SIGNAL(timeout()), this, SLOT(frameBrightestPixels()));
    timer.start(rate);

//    if (serial.isOpen() && serial.isWritable()) {
//        QByteArray ba("R");
//        serial.write(ba);
//        serial.flush();
//        qDebug() << "data has been send" << endl;
//        serial.close();
//    }

//    else
//        qDebug() << "An error occured" << endl;
}

void Renderer::frameBrightestPixels()
{
    //Obtener imagen desde OpenCV
    (*capWebCam) >> transformationMat;

    threshold(transformationMat, transformationMat, 240, 255, 3);
    cvtColor(transformationMat, transformationMat, CV_BGR2HLS);    //Imagen en HLS

    //Procesar pixel mas brillante para cada Y
    for (int i=r_y0; i < r_yf; i++) {
        brightestPixls[i] = 0;
        for(int j=r_x0; j < r_xf; j++)
            if(transformationMat.at<cv::Vec3b>(i,j)[1] >= transformationMat.at<cv::Vec3b>(i,brightestPixls[i])[1])
                brightestPixls[i] = j;
    }

    emit finishedPixelCalculation();
}

void Renderer::frameBrightestPixels_()
{
    for(int it = 5; it < 72; ++it) {
    //Obtener imagen desde OpenCV
    Mat currentMat,
        transformationMat;
    QString path = "output/scan" + QString::number(it) + ".jpg"; /////////Debug
//    QString path = "output/scan40.jpg"; /////////Debug
    currentMat = imread(path.toStdString(), CV_LOAD_IMAGE_COLOR);

    transformationMat = imread(path.toStdString(), CV_LOAD_IMAGE_COLOR);

    threshold(transformationMat, transformationMat, 240, 255, 3);
    cvtColor(transformationMat, transformationMat, CV_BGR2HLS);    //Imagen en HLS

    //Procesar pixel mas brillante para cada Y
//    int brightestPixls[height];
    for (int i=0; i < height ; i++) {
        brightestPixls[i] = 0;
        for(int j=0; j < width; j++)
            if(transformationMat.at<cv::Vec3b>(i,j)[1] >= transformationMat.at<cv::Vec3b>(i,brightestPixls[i])[1])
                brightestPixls[i] = j;
    }

    //Imagen en RGB
//    cvtColor(transformationMat, currentMat, CV_HLS2RGB);
//    cvtColor(currentMat, currentMat, CV_BGR2RGB);

//    //Load image using QImage
//    QImage img = QImage((uchar*) currentMat.data,
//                        currentMat.cols,
//                        currentMat.rows,
//                        currentMat.step,
//                        QImage::Format_RGB888);

//    //Escribir sobre pixels mas brillantes
//    for (int i=0; i < height ; i++)
//        img.setPixel(brightestPixls[i], i, qRgb(0, 0, 255));

//    //Mandar imagen a label: lbl_Pic
//    ui->lbl_Pic->setPixmap(QPixmap::fromImage(img));

    processSlice();
    }
}

void Renderer::processSlice()
{
    //Stop timer
    if(iterator >= n_frames) {
        timer.stop();
        qDebug() << "Timer was stopped";
    }

    //Calculate position in space
    float dAngle = stepAngle * iterator++,
          dx, dz, x, z;
    for (int i=0; i < height ; i++) {
        dx = brightestPixls[i] - middle_x;
        dz = dx / sin(laserAngle);

        x = dz * cos(dAngle);
        z = dz * sin(dAngle);

        points.push_back(x, height - i * 1.0, -z);
    }

#ifndef _WORKING_WITH_FILES_
    ui->openGLWidget->update();
#endif

    emit finishedSliceProcessing();
}
