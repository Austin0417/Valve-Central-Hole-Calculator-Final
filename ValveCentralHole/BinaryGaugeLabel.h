#pragma once
#include "CalibrationGaugeLabel.h"



class BinaryGaugeLabel : public CalibrationGaugeLabel
{
public:
	BinaryGaugeLabel(const std::unique_ptr<bool>& toggle, QWidget* parent = nullptr) : CalibrationGaugeLabel(toggle, parent) {}
	virtual void mouseMoveEvent(QMouseEvent* event) override;
	virtual void mousePressEvent(QMouseEvent* event) override;
	virtual void mouseReleaseEvent(QMouseEvent* event) override;
	virtual void paintEvent(QPaintEvent* event);
};

