#pragma once
#include <QLabel>
#include <QMouseEvent>
#include <QPainter>
#include <QBrush>
#include <functional>



class CalibrationGaugeLabel : public QLabel
{
	Q_OBJECT
private:
	bool can_draw_ = true;
	bool should_clear_on_repaint = true;
	const std::unique_ptr<bool>& helper_lines_toggled;
	bool is_mouse_currently_dragging = false;
	std::function<void(const QPoint&, const QPoint&)> on_mouse_release_callback_;

protected:
	QPixmap original_pixmap_;
	QPoint line_draw_start_;
	QPoint line_draw_end_;
public:
	CalibrationGaugeLabel(const std::unique_ptr<bool>& toggle, QWidget* parent = nullptr);
	virtual void mouseMoveEvent(QMouseEvent* event) override;
	virtual void mousePressEvent(QMouseEvent* event) override;
	virtual void mouseReleaseEvent(QMouseEvent* event) override;
	virtual void paintEvent(QPaintEvent* event) override;
	void SetOnMouseReleaseCallback(const std::function<void(const QPoint&, const QPoint&)>& callback);
	void SetLineStartPoint(const QPoint& start);
	void SetLineEndPoint(const QPoint& end);
	void SetNewOriginalPixmap(const QPixmap& pixmap);
	void SetCanDraw(bool draw_flag);
	void SetShouldClearOnRepaint(bool flag);
	void PaintEventUpdate();
	void ClearDrawnLines();
};

