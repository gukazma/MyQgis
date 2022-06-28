#include "MainWindow.h"
#include "ui_Mainwindow.h"
#include <View.h>
#include <ImageItem.h>
#include <QFileDialog>
#include <QMessageBox>
#include <iostream>
namespace Stone
{
	void MainWindow::showImg(QList<GDALRasterBand*>* imgBand) {
		if (imgBand->size() != 3)
		{
			return;
		}

		int imgWidth = imgBand->at(0)->GetXSize();
		int imgHeight = imgBand->at(0)->GetYSize();

		auto m_scaleFactor = this->height() * 5.0 / imgHeight;

		int iScaleWidth = (int)(imgWidth * m_scaleFactor - 1);
		int iScaleHeight = (int)(imgHeight * m_scaleFactor - 1);

		GDALDataType dataType = imgBand->at(0)->GetRasterDataType();

		// 首先分别读取RGB三个波段
		float* rBand = new float[iScaleWidth * iScaleHeight];
		float* gBand = new float[iScaleWidth * iScaleHeight];
		float* bBand = new float[iScaleWidth * iScaleHeight];

		unsigned char* rBandUC, * gBandUC, * bBandUC;

		// 根据是否显示彩色图像，判断RGB三个波段的组成方式，并分别读取
		imgBand->at(0)->RasterIO(GF_Read, 0, 0, imgWidth, imgHeight, rBand, iScaleWidth, iScaleHeight, GDT_Float32, 0, 0);
		imgBand->at(1)->RasterIO(GF_Read, 0, 0, imgWidth, imgHeight, gBand, iScaleWidth, iScaleHeight, GDT_Float32, 0, 0);
		imgBand->at(2)->RasterIO(GF_Read, 0, 0, imgWidth, imgHeight, bBand, iScaleWidth, iScaleHeight, GDT_Float32, 0, 0);

		// 分别拉伸每个波段并将Float转换为unsigned char
		rBandUC = imgSketch(rBand, imgBand->at(0), iScaleWidth * iScaleHeight, imgBand->at(0)->GetNoDataValue());
		gBandUC = imgSketch(gBand, imgBand->at(1), iScaleWidth * iScaleHeight, imgBand->at(1)->GetNoDataValue());
		bBandUC = imgSketch(bBand, imgBand->at(2), iScaleWidth * iScaleHeight, imgBand->at(2)->GetNoDataValue());
		
		// 将三个波段组合起来
		int bytePerLine = (iScaleWidth * 24 + 31) / 8;
		unsigned char* allBandUC = new unsigned char[bytePerLine * iScaleHeight];
		for (int h = 0; h < iScaleHeight; h++)
		{
			for (int w = 0; w < iScaleWidth; w++)
			{
				allBandUC[h * bytePerLine + w * 3 + 0] = rBandUC[h * iScaleWidth + w];
				allBandUC[h * bytePerLine + w * 3 + 1] = gBandUC[h * iScaleWidth + w];
				allBandUC[h * bytePerLine + w * 3 + 2] = bBandUC[h * iScaleWidth + w];
			}
		}

		// 构造图像并显示
		QGraphicsPixmapItem* imgItem = new QGraphicsPixmapItem(QPixmap::fromImage(QImage(allBandUC, iScaleWidth, iScaleHeight, bytePerLine, QImage::Format_RGB888)));
		m_Scene->addItem(imgItem);
	}

	unsigned char* MainWindow::imgSketch(float* buffer, GDALRasterBand* currentBand, int bandSize, double noValue)
	{
		unsigned char* resBuffer = new unsigned char[bandSize];
		double max, min;
		double minmax[2];
		currentBand->ComputeRasterMinMax(1, minmax);
		min = minmax[0];
		max = minmax[1];
		if (min <= noValue && noValue <= max)
		{
			min = 0;
		}
		for (int i = 0; i < bandSize; i++)
		{
			if (buffer[i] > max)
			{
				resBuffer[i] = 255;
			}
			else if (buffer[i] <= max && buffer[i] >= min)
			{
				resBuffer[i] = static_cast<uchar>(255 - 255 * (max - buffer[i]) / (max - min));
			}
			else
			{
				resBuffer[i] = 0;
			}
		}

		return resBuffer;
	}

	
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
		QString file = QFileDialog::getOpenFileName(this,
			tr("Open Image"), ".",
			tr("Open tiff files(*.tiff *.tif)"));
		GDALAllRegister();
		CPLSetConfigOption("GDAL_FILENAME_IS_UTF8", "NO");
		auto poDataset = (GDALDataset*)GDALOpen(file.toStdString().c_str(), GA_ReadOnly);
		
		if (poDataset == NULL)
		{
			QMessageBox::about(this, u8"提示", u8"指定的文件不能打开！");
			return;
		}

		int  ret = QMessageBox::question(this, "question", u8"是否构建金字塔？", QMessageBox::No | QMessageBox::Yes);
		switch (ret) {
		case QMessageBox::Yes:
		{
			int  anOverviewList[8] = { 2, 4, 8, 16, 32, 64, 128, 256 };
			GDALProgressFunc pfnProgress = GDALTermProgress;
			poDataset->BuildOverviews("NEAREST", 7, anOverviewList, 0, nullptr, pfnProgress, nullptr);
			break;
		}
		case QMessageBox::No:
			break;
		default:
			break;
		}
		
		//确认影像已经有金字塔
		int pyramidCount = poDataset->GetRasterBand(1)->GetOverviewCount();
		if (pyramidCount == 0)
		{
			QMessageBox::about(this, u8"提示", u8"影像文件未建立金字塔！");
			return;
		}

		QList<GDALRasterBand*> bandList;
		bandList.append(poDataset->GetRasterBand(1)->GetOverview(0));
		bandList.append(poDataset->GetRasterBand(2)->GetOverview(0));
		bandList.append(poDataset->GetRasterBand(3)->GetOverview(0));
		showImg(&bandList);
		GDALClose(poDataset);
	}
}