#pragma once
#include <QLabel>
#include <QEnterEvent>



class BinaryDetailsLabel : public QLabel
{
private:
	unsigned long long* total_num_pixels_;
	unsigned long long* total_num_white_pixels_;

	QString GenerateTooltipText();
public:
	BinaryDetailsLabel(unsigned long long* total_num_pixels, unsigned long long* total_num_white_pixels, QWidget* parent = nullptr) : total_num_pixels_(total_num_pixels), total_num_white_pixels_(total_num_white_pixels), QLabel(parent) {}
	virtual void enterEvent(QEnterEvent* event) override;
	virtual void paintEvent(QPaintEvent* event) override;
};

