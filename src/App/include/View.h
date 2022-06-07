#pragma once
#include <QFrame>
#include <QGraphicsView>
namespace Stone
{
	class View;
	class GraphicsView : public QGraphicsView
	{
		Q_OBJECT
	public:
		GraphicsView(View* view)
			:m_View(view)
		{}

	protected:
#if QT_CONFIG(wheelevent)
		void wheelEvent(QWheelEvent*) override;
#endif
	private:
		View* m_View;
	};

	class View : public QFrame
	{
		Q_OBJECT
	public:
		View();
		GraphicsView* m_GraphicsView;
	public slots:
		void zoomIn();
		void zoomOut();
	private:
	};
}