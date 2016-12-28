#include "worker.h"

Worker::Worker(QObject *parent) : QObject(parent)
{

}

Worker::~Worker()
{

}

void Worker::process()
{
    qDebug("Hello World!");

    othr_capWebCam->open(1);
    if(!othr_capWebCam->isOpened())
    {
        cout<<"Error"<<endl;
        return;
    }

    Timer = new QTimer(othr_ui);
    connect(Timer, SIGNAL(timeout()), othr_ui, SLOT(on_actionCapture()));
    Timer->start();

    emit finished();
}

void Worker::setup(QMainWindow *_ui, VideoCapture *_capWebCam)
{
    othr_ui = _ui;
    othr_capWebCam = _capWebCam;
}
