#pragma once
#include <QLabel>
#include <vector>


class CameraConfirmLabel : public QLabel
{
private:
	bool& is_crop_enabled_;
	QPoint rectangle_start_point;
	QPoint rectangle_end_point;
	QPixmap original_pixmap_;
	std::function<void(int, int, int, int)> crop_callback_;
	bool is_user_currently_dragging_ = false;
	bool should_apply_y_offset = false;
public:
	CameraConfirmLabel(bool& is_crop_enabled, QWidget* parent = nullptr) : is_crop_enabled_(is_crop_enabled), QLabel(parent) {}
	void SetCropCallback(const std::function<void(int, int, int, int)>& callback);
	void SetNewOriginalPixmap(const QPixmap& pixmap);
	virtual void mousePressEvent(QMouseEvent* event) override;
	virtual void mouseReleaseEvent(QMouseEvent* event) override;
	virtual void mouseMoveEvent(QMouseEvent* event) override;
	virtual void paintEvent(QPaintEvent* event) override;
	void ClearCropLines();
	void SetShouldApplyYOffset(bool flag);
};

