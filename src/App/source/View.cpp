#include "View.h"
#include <QGridLayout>
#include <QWheelEvent>
#include <iostream>
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
}