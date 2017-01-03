#ifndef OVERLAYWIDGET_H
#define OVERLAYWIDGET_H

#include <QApplication>
#include <QMainWindow>
#include <QResizeEvent>
#include <QPainter>

class OverlayWidget : public QWidget
{
public:
    explicit OverlayWidget(QWidget * parent = 0);

protected:
    //! Catches resize and child events from the parent widget
    bool eventFilter(QObject * obj, QEvent * ev);

    //! Tracks parent widget changes
    bool event(QEvent* ev);
};

class SelectionOverlay : public OverlayWidget
{
public:
    SelectionOverlay(QWidget * parent = 0);

    bool isSet();

    void setInitialPoint(int x, int y);

    void movementPoint(int x, int y);

    void setFinalPoint(int x, int y);

    inline int left()   { return dims.left();   }

    inline int right()  { return dims.right();  }

    inline int top()    { return dims.top();    }

    inline int bottom() { return dims.bottom(); }

protected:
    void paintEvent(QPaintEvent *);

private:
    QRect dims;

    bool set;
};

#endif // OVERLAYWIDGET_H
