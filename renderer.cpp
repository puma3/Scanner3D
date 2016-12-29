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
    Mat currentMat;
    cvtColor(imread("output/scan40.jpg", CV_LOAD_IMAGE_COLOR),      //Imagen en canales BGR
             currentMat,                                            //Imagen en RGB
             CV_BGR2RGB);

    //Procesar pixel mas brillante para cada Y
//    for (int j = 0; j < currentMat.rows; ++j) {

//    }

    //Load image using QImage
    QImage img = QImage((uchar*) currentMat.data,
                        currentMat.cols,
                        currentMat.rows,
                        currentMat.step,
                        QImage::Format_RGB888);

    //Escribir sobre pixels mas brillantes

    //Mandar imagen a label: lbl_Pic
    ui->lbl_Pic->setPixmap(QPixmap::fromImage(img));

    emit finished();
}
