#include "View.h"
#include <QGridLayout>
#include <QWheelEvent>
#include <iostream>
#include <QGraphicsPixmapItem>
#include <QFileDialog>
#include <QMessageBox>
#include <thread>
#include "ProcessBar.h"
#include "ProcessDialog.h"
namespace Stone
{
#if QT_CONFIG(wheelevent)
	void GraphicsView::wheelEvent(QWheelEvent* e)
	{
		if (e->modifiers() & Qt::ControlModifier) {
			if (e->angleDelta().y() > 0)
				m_View->zoomIn();
			else
				m_View->zoomOut();
			e->accept();
		}
		else {
			QGraphicsView::wheelEvent(e);
		}
	}
#endif

	View::View()
	{
		m_GraphicsView = new GraphicsView(this);
		m_GraphicsView->setDragMode(QGraphicsView::ScrollHandDrag);
		m_GraphicsView->setInteractive(false);
		auto layout = new QGridLayout;
		layout->addWidget(m_GraphicsView, 0, 0);
		setLayout(layout);
	}

	void View::zoomIn()
	{
		m_GraphicsView->scale(1.2, 1.2);
		Scale_level += 1.2;
	}
	void View::zoomOut()
	{
		m_GraphicsView->scale(1 / 1.2, 1 / 1.2);
		Scale_level -= 1.2;
	}
	void View::slotRefreshImage()
	{

	}
	void View::slotOpenImage()
	{
		QString file = QFileDialog::getOpenFileName(this,
			tr("Open Image"), ".",
			tr("Open tiff files(*.tiff *.tif)"));
		GDALAllRegister();
		CPLSetConfigOption("GDAL_FILENAME_IS_UTF8", "NO");
		poDataset = (GDALDataset*)GDALOpen(file.toStdString().c_str(), GA_ReadOnly);

		if (poDataset == NULL)
		{
			QMessageBox::about(this, QString::fromLocal8Bit("错误"), QString::fromLocal8Bit("GDAL 初始化失败"));
			return;
		}

		int  ret = QMessageBox::question(this, "question", QString::fromLocal8Bit("是否构建图像金字塔"), QMessageBox::No | QMessageBox::Yes);
		switch (ret) {
		case QMessageBox::Yes:
		{
			int  anOverviewList[9] = { 2, 4, 6, 8, 10, 20, 50, 100, 128};
			ProcessDialog* processDialog = new ProcessDialog();
			GDALProgressFunc pfnProgress = Stone::GDALTermProgress;
			std::thread thread([&](){
				poDataset->BuildOverviews("NEAREST", 9, anOverviewList, 0, nullptr, pfnProgress, (void*) processDialog);
			});

			thread.detach();
			processDialog->exec();
			break;
		}
		case QMessageBox::No:
			break;
		default:
			break;
		}

		int pyramidCount = poDataset->GetRasterBand(1)->GetOverviewCount();
		if (pyramidCount == 0)
		{
			QMessageBox::about(this, QString::fromLocal8Bit("错误"), QString::fromLocal8Bit("未构建图像金字塔"));
			return;
		}

		QList<GDALRasterBand*> bandList;
		bandList.append(poDataset->GetRasterBand(1)->GetOverview(0));
		bandList.append(poDataset->GetRasterBand(2)->GetOverview(0));
		bandList.append(poDataset->GetRasterBand(3)->GetOverview(0));
		try
		{
			getImgbyBand(&bandList);
			
		}
		catch (const std::exception& e)
		{
			std::cerr << e.what() << std::endl;
		}
		GDALClose(poDataset);
	}
	void View::refreshImage()
	{

	}
	void View::getImgbyBand(QList<GDALRasterBand*>* imgBand)
	{
		if (imgBand->size() != 3)
		{
			throw std::runtime_error("Number of image band must be 3!");
			return;
		}

		int imgWidth = imgBand->at(0)->GetXSize();
		int imgHeight = imgBand->at(0)->GetYSize();

		float m_scaleFactor = this->height() * Scale_level / imgHeight;

		int iScaleWidth = (int)(imgWidth * m_scaleFactor);
		int iScaleHeight = (int)(imgHeight * m_scaleFactor);

		GDALDataType dataType = imgBand->at(0)->GetRasterDataType();

		float* rBand = new float[iScaleWidth * iScaleHeight];
		float* gBand = new float[iScaleWidth * iScaleHeight];
		float* bBand = new float[iScaleWidth * iScaleHeight];

		unsigned char* rBandUC, * gBandUC, * bBandUC;

		imgBand->at(0)->RasterIO(GF_Read, 0, 0, imgWidth, imgHeight, rBand, iScaleWidth, iScaleHeight, GDT_Float32, 0, 0);
		imgBand->at(1)->RasterIO(GF_Read, 0, 0, imgWidth, imgHeight, gBand, iScaleWidth, iScaleHeight, GDT_Float32, 0, 0);
		imgBand->at(2)->RasterIO(GF_Read, 0, 0, imgWidth, imgHeight, bBand, iScaleWidth, iScaleHeight, GDT_Float32, 0, 0);

		rBandUC = imgSketch(rBand, imgBand->at(0), iScaleWidth * iScaleHeight, imgBand->at(0)->GetNoDataValue());
		gBandUC = imgSketch(gBand, imgBand->at(1), iScaleWidth * iScaleHeight, imgBand->at(1)->GetNoDataValue());
		bBandUC = imgSketch(bBand, imgBand->at(2), iScaleWidth * iScaleHeight, imgBand->at(2)->GetNoDataValue());

		int bytePerLine = (iScaleWidth * 3);
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

		imgItem = new QGraphicsPixmapItem(QPixmap::fromImage(QImage(allBandUC, iScaleWidth, iScaleHeight, bytePerLine, QImage::Format_RGB888)));
		m_GraphicsView->scene()->addItem(imgItem);
	}
	unsigned char* View::imgSketch(float* buffer, GDALRasterBand* currentBand, int bandSize, double noValue)
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
}