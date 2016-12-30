#include "renderer.h"
#include "ui_renderer.h"

Renderer::Renderer(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::Renderer)
{
    ui->setupUi(this);
    brightestPixels();
}

Renderer::~Renderer()
{
    delete ui;
}

void Renderer::brightestPixels()
{
    //Obtener imagen desde OpenCV
    Mat currentMat,
        transformationMat;
    currentMat = imread("output/scan40.jpg", CV_LOAD_IMAGE_COLOR);

    transformationMat = currentMat;
//    cvtColor(currentMat, transformationMat, CV_BGR2GRAY);    //Imagen en HLS
//    threshold(transformationMat, transformationMat, 240, 255, 3);
//    equalizeHist( transformationMat, transformationMat);

//    //Reducir la saturacion
//    for (int i=0; i < transformationMat.rows ; i++)
//        for(int j=0; j < transformationMat.cols; j++)
//            transformationMat.at<cv::Vec3b>(i,j)[1] = 50;



//    cvtColor(transformationMat, transformationMat, CV_GRAY2BGR);
    cvtColor(transformationMat, transformationMat, CV_BGR2HLS);    //Imagen en HLS

    //Procesar pixel mas brillante para cada Y
    int brightestPixls[transformationMat.rows];
    for (int i=0; i < transformationMat.rows ; i++) {
        brightestPixls[i] = 0;
        for(int j=1; j < transformationMat.cols; j++)
            if(transformationMat.at<cv::Vec3b>(i,j)[1] >= transformationMat.at<cv::Vec3b>(i,brightestPixls[i])[1])
                brightestPixls[i] = j;
    }

    //Imagen en RGB
    cvtColor(transformationMat, currentMat, CV_HLS2RGB);

    //Load image using QImage
    QImage img = QImage((uchar*) currentMat.data,
                        currentMat.cols,
                        currentMat.rows,
                        currentMat.step,
                        QImage::Format_RGB888);

    //Escribir sobre pixels mas brillantes
    for (int i=0; i < transformationMat.rows ; i++)
        img.setPixel(brightestPixls[i], i, qRgb(0, 0, 255));
    qDebug() << transformationMat.rows << endl << transformationMat.cols;

    //Mandar imagen a label: lbl_Pic
    ui->lbl_Pic->setPixmap(QPixmap::fromImage(img));

    emit finished();
}
