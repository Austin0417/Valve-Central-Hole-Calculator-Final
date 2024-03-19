#include "BinaryDetailsLabel.h"
#include <QToolTip>


QString BinaryDetailsLabel::GenerateTooltipText()
{
	if (*total_num_white_pixels_ == 0)
	{
		return "Total Num. Pixels: " + QString::number(*total_num_pixels_) + "\nTotal Num. White Pixels: "
			+ QString::number(*total_num_white_pixels_)
			+ "\nTotal Num. Black Pixels : " + QString::number(0);
	}

	return "Total Num. Pixels: " + QString::number(*total_num_pixels_) + "\nTotal Num. White Pixels: "
		+ QString::number(*total_num_white_pixels_)
		+ "\nTotal Num. Black Pixels : " + QString::number(*total_num_pixels_ - *total_num_white_pixels_);
}


void BinaryDetailsLabel::enterEvent(QEnterEvent* event)
{
	QLabel::enterEvent(event);

	QToolTip::showText(event->globalPosition().toPoint(), GenerateTooltipText(), this, rect(), 10000);
}

void BinaryDetailsLabel::paintEvent(QPaintEvent* event)
{
	QLabel::paintEvent(event);
}
