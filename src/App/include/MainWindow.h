#pragma once
#include <QMainWindow>
#include <QGraphicsScene>
#include <gdal_priv.h>
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
        void showImg(QList<GDALRasterBand*>* imgBand);
        unsigned char* imgSketch(float* buffer, GDALRasterBand* currentBand, int bandSize, double noValue);
    public Q_SLOTS:
        void openImage();
    private:
        QGraphicsScene* m_Scene;
        Ui::MainWindow* ui;
    };

}
