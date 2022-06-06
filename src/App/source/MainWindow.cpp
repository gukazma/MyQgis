#include "MainWindow.h"
#include <Components/View.h>
#include <Item/ImageItem.h>
namespace Stone
{
	MainWindow::MainWindow(QWidget* parent)
	{
		View* view = new View();
		QGraphicsScene* scene = new QGraphicsScene;
		ImageItem* imageItem = new ImageItem();
		scene->addItem(imageItem);
		view->m_GraphicsView->setScene(scene);
		setCentralWidget(view);
	}
}