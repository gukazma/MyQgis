#pragma once
#include <QMainWindow>
#include <QGraphicsScene>
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
        QGraphicsScene* m_Scene;
        Ui::MainWindow* ui;
    };

}
