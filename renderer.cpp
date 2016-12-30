#include "renderer.h"
#include "ui_renderer.h"

Renderer::Renderer(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::Renderer),
    iterator(0)
{
    ui->setupUi(this);
    ui->openGLWidget->setPointCloud(&points);
    setFrameSize(640, 480);
    setAngles(45.0, 5.14286);
    frameBrightestPixels();
}

Renderer::~Renderer()
{
    delete []brightestPixls;
    delete ui;
}

void Renderer::setFrameSize(int _w, int _h)
{
    width = _w;
    height = _h;
    middle_x = _w / 2;

    brightestPixls = new int[height];
}

void Renderer::setAngles(float _laserAngle, float _stepAngle)
{
    laserAngle = _laserAngle * 3.1416 / 180;
    stepAngle = _stepAngle * 3.1416 / 180;
//    stepAngle = 2 * 3.1416 / 70;
}

void Renderer::frameBrightestPixels()
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
    cvtColor(currentMat, currentMat, CV_BGR2RGB);

    //Load image using QImage
    QImage img = QImage((uchar*) currentMat.data,
                        currentMat.cols,
                        currentMat.rows,
                        currentMat.step,
                        QImage::Format_RGB888);

    //Escribir sobre pixels mas brillantes
    for (int i=0; i < height ; i++)
        img.setPixel(brightestPixls[i], i, qRgb(0, 0, 255));

    //Mandar imagen a label: lbl_Pic
    ui->lbl_Pic->setPixmap(QPixmap::fromImage(img));

//    emit finishedPixelCalculation();
    processSlice();
    }
}

void Renderer::processSlice()
{
    float dAngle = stepAngle * iterator++,
          dx, dz, x, z;
    for (int i=0; i < height ; i++) {
        dx = brightestPixls[i] - middle_x;
        dz = dx / sin(laserAngle);

        x = dz * cos(dAngle);
        z = dz * sin(dAngle);

        points.push_back(x, height - i * 1.0, z);
    }

    emit finishedSliceProcessing();
}
