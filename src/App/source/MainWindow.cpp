#include "MainWindow.h"
#include "ui_Mainwindow.h"
#include <View.h>
#include <ImageItem.h>
#include <QFileDialog>
#include <QMessageBox>
#include <gdal_priv.h>
namespace Stone
{
	MainWindow::MainWindow(QWidget* parent)
		: QMainWindow(parent)
		, ui(new Ui::MainWindow)
	{
		ui->setupUi(this);
		View* view = new View();
		m_Scene = new QGraphicsScene;
		view->m_GraphicsView->setScene(m_Scene);
		setCentralWidget(view);
		connect(ui->actionOpenImage, &QAction::triggered, this, &MainWindow::openImage);
	}

	void MainWindow::openImage()
	{
		QString fileName = QFileDialog::getOpenFileName(this, tr("open file"), "", tr("GeoTIFF (*.tif)"
		));
		if (fileName.isEmpty()) return;
		GDALAllRegister();
		auto m_pDataset = (GDALDataset*)GDALOpen(fileName.toLatin1().data(), GA_ReadOnly);
		if (m_pDataset == NULL)
		{
			QMessageBox::about(this, u8"提示", u8"指定的文件不能打开！");
			return ;
		}
		int  ret = QMessageBox::question(this, "question", u8"是否构建金字塔？", QMessageBox::No | QMessageBox::Yes);
		switch (ret) {
		case QMessageBox::Yes:
		{
			int  anOverviewList[8] = { 2, 4, 8, 16, 32, 64, 128, 256 };
			GDALProgressFunc pfnProgress = GDALTermProgress;
			m_pDataset->BuildOverviews("NEAREST", 7, anOverviewList, 0, nullptr, pfnProgress, nullptr);    //构造金字塔
			break;
		}
		case QMessageBox::No:
			break;
		default:
			break;
		}

		ImageItem* item = new ImageItem(nullptr, fileName.toStdString());
		m_Scene->addItem(item);
	}
}