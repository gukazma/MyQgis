#pragma once
#include <QGraphicsView>
#include <iostream>
#include <QWheelEvent>
#include <QDragMoveEvent>
// TODO: some code here
class View : public QGraphicsView
{
    Q_OBJECT
public:
    View() {

    }
    void wheelEvent(QWheelEvent* event) override
    {
        if (event->delta()>0)
        {
            zoomIn();
        }
        else
        {
            zoomOut();
        }
    }

public slots:
    void zoomIn() { scale(1.2, 1.2); }
    void zoomOut() { scale(1 / 1.2, 1 / 1.2); }
};