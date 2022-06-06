#include <iostream>
#include <QApplication>
#include <Components/View.h>
#include "MainWindow.h"
int main(int argc, char *argv[])
{
    QApplication* app = new QApplication(argc, argv);
    Stone::MainWindow* mainwindow = new Stone::MainWindow();
    mainwindow->show();
    app->exec();
}
