#include "MainWindow.h"
#include <Components/View.h>
namespace Stone
{
	MainWindow::MainWindow(QWidget* parent)
	{
		View* view = new View();

		setCentralWidget(view);
	}
}