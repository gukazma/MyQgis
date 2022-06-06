#include "ImageItem.h"
#include <QPainter>
#include <string>
namespace Stone
{
	ImageItem::ImageItem(QGraphicsItem* parent, const std::string& filename)
		: QGraphicsItem(parent)
	{
		m_Image = new QImage(filename.c_str());
		x = -m_Image->width() / 2.0;
		y = -m_Image->height() / 2.0;
		width = m_Image->width();
		height = m_Image->height();
	}
	void ImageItem::paint(QPainter* painter, const QStyleOptionGraphicsItem* item, QWidget* widget)
	{
		painter->drawImage(QRect(-m_Image->width()/2.0, -m_Image->height() / 2.0, -m_Image->width(), -m_Image->height()), *m_Image);
	}

	QRectF ImageItem::boundingRect() const
	{
		return QRectF(-width*4, -height * 2, 8 * width, 4 * height);
	}
	QPainterPath ImageItem::shape() const
	{
		QPainterPath path;
		path.addRect(x, y, width, height);
		return path;
	}
}