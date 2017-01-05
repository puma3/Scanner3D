#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <stdio.h>
#include <stdlib.h>

//#define _NO_WEBCAM_
#define PERIOD 68700
#define ANGLE_CAMERA 45.0
//#define ANGLE_CAMERA 30.0

using namespace std;
using namespace cv;

Scan scan;

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    highlight(false),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    setupCamera();
    cameraShow();
    brightestPixls = new int[height];
    rndr = new Renderer(&capWebCam, brightestPixls, this);  //New Renderer
    selection = new SelectionOverlay(ui->camera);
    selection->installEventFilter(this);
    connect(this, SIGNAL(startCapturing(int, int)), rndr, SLOT(captureFrames(int, int)));
    connect(ui->commandLinkButton, SIGNAL(clicked(bool)), rndr, SLOT(turnOnLaser()));
    connect(rndr, SIGNAL(finishedPixelCalculation()), this, SLOT(highlight_bright_pixels()));
    rndr->setFrameSize(width, height);
}

MainWindow::~MainWindow()
{
    delete []brightestPixls;
    delete timer;
    delete ui;
}

void MainWindow::setupCamera()
{

    capWebCam.open(1);
    if(!capWebCam.isOpened()) {
        capWebCam.open(0);
        if(!capWebCam.isOpened()) {
            cout << "Camera couldn't start" << endl;
            return;
        }
    }
    width = capWebCam.get(CV_CAP_PROP_FRAME_WIDTH);
    height = capWebCam.get(CV_CAP_PROP_FRAME_HEIGHT);
    v_guideline_pos = width / 2;
    h_guideline_pos = height * 9 / 10;
}

void MainWindow::cameraShow()
{
    timer = new QTimer(this);
    connect(timer, SIGNAL(timeout()), this, SLOT(showCameraFrame()));
    timer->start(17);
}

bool MainWindow::eventFilter(QObject *obj, QEvent *event)
{
    if(qobject_cast<QWidget*>(obj)==selection && event->type() == QEvent::MouseButtonPress) {
//        qDebug() << "Cl" << static_cast<QMouseEvent*>(event)->x();
        selection->setInitialPoint(static_cast<QMouseEvent*>(event)->x(),
                                   static_cast<QMouseEvent*>(event)->y());
        selection->movementPoint(static_cast<QMouseEvent*>(event)->x(),
                                 static_cast<QMouseEvent*>(event)->y());
        return true;
    }
    if(qobject_cast<QWidget*>(obj)==selection && event->type() == QEvent::MouseMove) {
//        qDebug() << "Mv" << static_cast<QMouseEvent*>(event)->x();
        selection->movementPoint(static_cast<QMouseEvent*>(event)->x(),
                                 static_cast<QMouseEvent*>(event)->y());
        return true;
    }
    if(qobject_cast<QWidget*>(obj)==selection && event->type() == QEvent::MouseButtonRelease) {
//        qDebug() << "Rl" << static_cast<QMouseEvent*>(event)->x();
        selection->setFinalPoint(static_cast<QMouseEvent*>(event)->x(),
                                 static_cast<QMouseEvent*>(event)->y());
        return true;
    }
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
    for (int i = selection->isSet() ? selection->top() : 0;
         i < (selection->isSet() ? selection->bottom() : height);
         i++)
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
    if(selection->isSet())
        rndr->setRange(selection->left(), selection->top(), selection->right(), selection->bottom());
    ui->pushButton_2->setEnabled(true);
    emit startCapturing(ui->frames_spinBox->value(), PERIOD / ui->frames_spinBox->value());
}
