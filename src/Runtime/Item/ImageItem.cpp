#include "ImageItem.h"
#include <QPainter>
namespace Stone
{
	ImageItem::ImageItem(QGraphicsItem* parent)
		: QGraphicsItem(parent)
	{
		m_Image = new QImage("D:/data/imgs/Lenna.png");
		x = -m_Image->width() / 2.0;
		y = -m_Image->height() / 2.0;
		width = m_Image->width();
		height = m_Image->height();
	}
	void ImageItem::paint(QPainter* painter, const QStyleOptionGraphicsItem* item, QWidget* widget)
	{
		
		painter->drawImage(QRect(-m_Image->width()/2.0, -m_Image->height() / 2.0, -m_Image->width(), -m_Image->height()), *m_Image);
		//painter->fillRect(QRectF(0, 0, 60, 60), QColor(1, 0, 0));
	}

	QRectF ImageItem::boundingRect() const
	{
		return QRectF(x, y, width, height);
	}
	QPainterPath ImageItem::shape() const
	{
		QPainterPath path;
		path.addRect(x, y, width, height);
		return path;
	}
}