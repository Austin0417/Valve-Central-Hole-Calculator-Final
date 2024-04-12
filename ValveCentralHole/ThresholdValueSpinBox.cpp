#include "ThresholdValueSpinBox.h"

ThresholdValueSpinBox::ThresholdValueSpinBox(QWidget* parent) : QSpinBox(parent)
{
	setMinimum(0);
	setMaximum(255);
	setFixedWidth(51);
	setFixedHeight(31);
	setValue(127);
	setStyleSheet("background: white;");
	if (parent != nullptr)
	{
		move(800, 60);
	}
}

bool ThresholdValueSpinBox::event(QEvent* event)
{
	if (event->type() == QEvent::KeyPress)
	{
		QKeyEvent* key_event = dynamic_cast<QKeyEvent*>(event);
		if (key_event->key() == Qt::Key_Return)
		{
			emit OnReturnPressed(value());
		}
	}

	return QSpinBox::event(event);
}