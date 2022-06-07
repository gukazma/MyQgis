#include "MainWindow.h"
#include "ui_Mainwindow.h"
#include <View.h>
#include <ImageItem.h>
#include <QFileDialog>
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

		ImageItem* item = new ImageItem(nullptr, fileName.toStdString());
		m_Scene->addItem(item);
	}
}