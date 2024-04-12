#pragma once
#include <QEvent>
#include <QKeyEvent>
#include <QSpinBox>


class ThresholdValueSpinBox : public QSpinBox
{
	Q_OBJECT
private:
public:
	ThresholdValueSpinBox(QWidget* parent = nullptr);
	virtual bool event(QEvent* event) override;
	signals:
		void OnReturnPressed(int threshold_value);
};

