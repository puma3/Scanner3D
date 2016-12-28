#ifndef WORKER_H
#define WORKER_H

#include <QObject>
#include <ui_mainwindow.h>
#include <QTimer>
#include "scan.h"

class Worker : public QObject
{
    Q_OBJECT
public:
    Worker(QObject *parent = 0);
    ~Worker();

public slots:
    void process();

    void setup(QMainWindow *_ui, VideoCapture *_capWebCam);

signals:
    void finished();
    void error(QString err);

private:
    QMainWindow *othr_ui;
    VideoCapture *othr_capWebCam;
    QTimer *Timer;
};

#endif // WORKER_H
