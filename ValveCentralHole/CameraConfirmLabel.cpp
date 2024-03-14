#include "CameraConfirmLabel.h"
#include <QMouseEvent>
#include <QBrush>
#include <QPainter>


constexpr int RECT_HEIGHT_OFFSET = 60;


void CameraConfirmLabel::SetCropCallback(const std::function<void(int, int, int, int)>& callback)
{
	crop_callback_ = callback;
}



void CameraConfirmLabel::paintEvent(QPaintEvent* event)
{
	QLabel::paintEvent(event);

	if (!pixmap().isNull() && original_pixmap_.isNull())
	{
		original_pixmap_ = pixmap();
	}


	if (!rectangle_start_point.isNull() && !rectangle_end_point.isNull())
	{
		setPixmap(original_pixmap_);
		QPixmap label_pixmap = pixmap();
		QPainter painter(&label_pixmap);
		painter.setPen(QPen(QBrush(QColor(255, 0, 0)), 5));

		if (should_apply_y_offset)
		{
			painter.drawRect(rectangle_start_point.x(), rectangle_start_point.y() + RECT_HEIGHT_OFFSET,
				rectangle_end_point.x() - rectangle_start_point.x(), rectangle_end_point.y() - rectangle_start_point.y());
		}
		else
		{
			painter.drawRect(rectangle_start_point.x(), rectangle_start_point.y(),
				rectangle_end_point.x() - rectangle_start_point.x(), rectangle_end_point.y() - rectangle_start_point.y());
		}

		setPixmap(label_pixmap);

		// This if statement will be true after the user is done dragging their mouse
		if (!is_user_currently_dragging_ && crop_callback_)
		{
			qDebug() << "Converting cropped image region into new cv::Mat...";

			// Before sending the coordinates of the rectangle's top left corner, we have to check if the user dragged from left to right, or right to left
			// In the case of right to left, the start point's x coordinate will be larger than the end point's x coordinate, so we should send the x coordinate of the end point instead
			if (rectangle_start_point.x() < rectangle_end_point.x())
			{
				crop_callback_(rectangle_start_point.x(), rectangle_start_point.y(),
					rectangle_end_point.x() - rectangle_start_point.x(), rectangle_end_point.y() - rectangle_start_point.y());

			}
			else if (rectangle_start_point.x() > rectangle_end_point.x())
			{
				crop_callback_(rectangle_end_point.x(), rectangle_start_point.y(),
					rectangle_end_point.x() - rectangle_start_point.x(), rectangle_end_point.y() - rectangle_start_point.y());

			}
			rectangle_start_point = QPoint();
		}
		rectangle_end_point = QPoint();
	}
}



void CameraConfirmLabel::mousePressEvent(QMouseEvent* event)
{
	if (is_crop_enabled_)
	{
		qDebug() << "Mouse Click : " << event->pos();
		rectangle_start_point = event->pos();
	}
}

void CameraConfirmLabel::mouseReleaseEvent(QMouseEvent* event)
{
	is_user_currently_dragging_ = false;
	if (is_crop_enabled_)
	{
		qDebug() << "Mouse Release : " << event->pos();
		rectangle_end_point = event->pos();
		update();
	}
}

void CameraConfirmLabel::mouseMoveEvent(QMouseEvent* event)
{
	QLabel::mouseMoveEvent(event);
	is_user_currently_dragging_ = true;
	if (is_crop_enabled_)
	{
		qDebug() << "Mousing moving to: " << event->pos();
		rectangle_end_point = event->pos();
		update();
	}
}

void CameraConfirmLabel::ClearCropLines()
{
	if (!original_pixmap_.isNull())
	{
		setPixmap(original_pixmap_);
	}
}


void CameraConfirmLabel::SetShouldApplyYOffset(bool flag)
{
	should_apply_y_offset = flag;
}


void CameraConfirmLabel::SetNewOriginalPixmap(const QPixmap& pixmap)
{
	original_pixmap_ = pixmap;
}