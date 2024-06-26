#include "CalculationHistoryDialog.h"

#include "ValveAreaHistoryWidget.h"


CalculationHistoryDialog::CalculationHistoryDialog(QWidget* parent) :
	QDialog(parent)
{
	ui.setupUi(this);

	calibration_gauge_history_ = std::make_unique<CalibrateHistoryWidget>(this);
	valve_area_history_ = std::make_unique<ValveAreaHistoryWidget>(this);

	calibration_gauge_history_->SetClearCalibrateHistoryCallback([this]()
		{
			if (clear_calibrate_history_callback_)
			{
				clear_calibrate_history_callback_();
			}
		});
	valve_area_history_->SetClearValveAreaHistoryCallback([this]()
		{
			if (clear_valve_area_history_callback_)
			{
				clear_valve_area_history_callback_();
			}
		});

	tab_widget_.reset(ui.history_tab_widget);
	tab_widget_->clear();

	tab_widget_->addTab(calibration_gauge_history_.get(), "Calibration Gauge History");
	tab_widget_->addTab(valve_area_history_.get(), "Valve Area Calculation History");

	connect(tab_widget_.get(), &QTabWidget::currentChanged, this, [this](int index)
		{
			// TODO Load either the calibration gauge history or valve area history depending on the index here
			switch (index)
			{
			case 0:
			{
				qDebug() << "Now displaying Calibration Gauge History...";
				break;
			}
			case 1:
			{
				qDebug() << "Now displaying Valve Area Calculation History...";
				break;
			}
			}
		});
}

void CalculationHistoryDialog::SetClearCalibrateHistoryCallback(const std::function<void()>& callback)
{
	clear_calibrate_history_callback_ = callback;
}

void CalculationHistoryDialog::SetClearValveAreaHistoryCallback(const std::function<void()>& callback)
{
	clear_valve_area_history_callback_ = callback;
}

std::unique_ptr<CalibrateHistoryWidget>& CalculationHistoryDialog::GetCalibrateHistoryWidget()
{
	return calibration_gauge_history_;
}

std::unique_ptr<ValveAreaHistoryWidget>& CalculationHistoryDialog::GetValveAreaHistoryWidget()
{
	return valve_area_history_;
}

int CalculationHistoryDialog::exec()
{
	calibration_gauge_history_->RefreshTableData();
	valve_area_history_->RefreshTableData();

	return QDialog::exec();
}