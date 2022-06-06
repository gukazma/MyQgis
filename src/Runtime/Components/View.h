#pragma once
#include <QFrame>
#include <QGraphicsView>
namespace Stone
{
	class GraphicsView : public QGraphicsView
	{
	public:

	private:

	};

	class View : public QFrame
	{
		Q_OBJECT
	public:
		View();
	private:
		GraphicsView* m_GraphicsView;
	};
}