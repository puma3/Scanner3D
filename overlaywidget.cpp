#include "overlaywidget.h"

OverlayWidget::OverlayWidget(QWidget *parent) : QWidget(parent) {
    if (parent) {
        parent->installEventFilter(this);
        raise();
    }
}

bool OverlayWidget::eventFilter(QObject *obj, QEvent *ev) {
    if (obj == parent()) {
        if (ev->type() == QEvent::Resize) {
            QResizeEvent * rev = static_cast<QResizeEvent*>(ev);
            resize(rev->size());
        }
        else if (ev->type() == QEvent::ChildAdded) {
            raise();
        }
    }
    return QWidget::eventFilter(obj, ev);
}

bool OverlayWidget::event(QEvent *ev) {
    if (ev->type() == QEvent::ParentAboutToChange) {
        if (parent()) parent()->removeEventFilter(this);
    }
    else if (ev->type() == QEvent::ParentChange) {
        if (parent()) {
            parent()->installEventFilter(this);
            raise();
        }
    }
    return QWidget::event(ev);
}

SelectionOverlay::SelectionOverlay(QWidget *parent) :
    OverlayWidget(parent),
    dims(0, 0, 0, 0),
    set(false)
{
    setAttribute(Qt::WA_TranslucentBackground);
}

void SelectionOverlay::paintEvent(QPaintEvent *)
{
    if(set) {
        QPainter p(this);
        p.fillRect(dims, QColor(40, 70, 200, 50));
    }
}

bool SelectionOverlay::isSet()
{
    return set;
}

void SelectionOverlay::setInitialPoint(int x, int y)
{
    dims.setTopLeft(QPoint(x, y));
}

void SelectionOverlay::movementPoint(int x, int y)
{
    dims.setBottomRight(QPoint(x, y));
}

void SelectionOverlay::setFinalPoint(int x, int y)
{
    if(!set)
        set = true;
    dims.setBottomRight(QPoint(x, y));
}

