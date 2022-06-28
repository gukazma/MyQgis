#include "MainWindow.h"
#include "ui_Mainwindow.h"
#include <View.h>
#include <ImageItem.h>
#include <QFileDialog>
#include <QMessageBox>
#include <iostream>
namespace Stone
{
	MainWindow::MainWindow(QWidget* parent)
		: QMainWindow(parent)
		, ui(new Ui::MainWindow)
	{
		ui->setupUi(this);
		m_View = new View();
		auto scene = new QGraphicsScene;
		m_View->m_GraphicsView->setScene(scene);
		setCentralWidget(m_View);
		connect(ui->actionOpenImage, &QAction::triggered, m_View, &View::slotOpenImage);
	}
}