#pragma once
#include <QGraphicsItem>
namespace Stone
{
	class ImageItem : public QGraphicsItem
	{
	public:
		ImageItem(QGraphicsItem* parent, const std::string& filename);
		void paint(QPainter* painter, const QStyleOptionGraphicsItem* item, QWidget* widget) override;
		QRectF boundingRect() const override;
		QPainterPath shape() const override;
	private:
		QImage* m_Image;
		float x, y;
		float width, height;
	};
}