#include "BinaryGaugeLabel.h"



void BinaryGaugeLabel::mouseMoveEvent(QMouseEvent* event)
{
	QLabel::mouseMoveEvent(event);
}


void BinaryGaugeLabel::mousePressEvent(QMouseEvent* event)
{
	QLabel::mousePressEvent(event);
}

void BinaryGaugeLabel::mouseReleaseEvent(QMouseEvent* event)
{
	QLabel::mouseReleaseEvent(event);
}

void BinaryGaugeLabel::paintEvent(QPaintEvent* event)
{
	QLabel::paintEvent(event);

	if (!line_draw_start_.isNull() && !line_draw_end_.isNull() && !pixmap().isNull())
	{
		QPixmap current_pixmap = pixmap();
		QPainter painter(&current_pixmap);
		QPen pen(QBrush(QColor(255, 0, 0)), 5);
		painter.setPen(pen);

		painter.drawLine(line_draw_start_, line_draw_end_);
		line_draw_start_ = QPoint();
		line_draw_end_ = QPoint();
		setPixmap(current_pixmap);
	}
}