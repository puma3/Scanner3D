#ifndef RENDERER_H
#define RENDERER_H

#include <QDialog>
#include <QDebug>
#include "scan.h"

namespace Ui {
class Renderer;
}

class Renderer : public QDialog
{
    Q_OBJECT

public:
    explicit Renderer(QWidget *parent = 0);
    ~Renderer();

public slots:
    void brightestPixels();

private:
    Ui::Renderer *ui;

signals:
    void finished();
};

#endif // RENDERER_H
