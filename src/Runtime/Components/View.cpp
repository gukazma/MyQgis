#include "View.h"
#include <QGridLayout>
namespace Stone
{
	View::View()
	{
		m_GraphicsView = new GraphicsView;
		auto layout = new QGridLayout;
		layout->addWidget(m_GraphicsView, 0, 0);
		setLayout(layout);
	}
}