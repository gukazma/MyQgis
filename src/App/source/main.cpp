#include <iostream>
#include <QApplication>
#include <QGraphicsView>
#include "Core/test.h"
#include "test.h"
int main(int argc, char *argv[])
{
    test();
    QApplication* app = new QApplication(argc, argv);
    QGraphicsScene scene;
    scene.addText("Hello, world!");
    QGraphicsRectItem* rect = scene.addRect(0, 0, 100, 100);

    View view;
    view.setScene(&scene);
    view.show();
    app->exec();
}
