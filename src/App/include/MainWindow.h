#pragma once
#include "ProcessDialog.h"
#include <QMainWindow>
#include <QGraphicsScene>
#include <gdal_priv.h>
#include <View.h>
QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

namespace Stone
{
    class MainWindow : public QMainWindow
    {
        Q_OBJECT

    public:
        MainWindow(QWidget* parent = nullptr);
        ~MainWindow() = default;
    private:
        View* m_View;
        Ui::MainWindow* ui;
    };

}
