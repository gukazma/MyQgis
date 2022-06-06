#include "MainWindow.h"
#include "ui_Mainwindow.h"
#include <Components/View.h>
#include <Item/ImageItem.h>
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
	}
}