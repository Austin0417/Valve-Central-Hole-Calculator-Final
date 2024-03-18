#include "CalculationHistoryDialog.h"

#include "ValveAreaHistoryWidget.h"


CalculationHistoryDialog::CalculationHistoryDialog(const std::vector<CalibrateData>& calibrate_history, const std::vector<MeasureData>& measure_history, QWidget* parent) :
	QDialog(parent), calibrate_history_(calibrate_history), measure_history_(measure_history)
{
	ui.setupUi(this);

	calibration_gauge_history_ = std::make_unique<CalibrateHistoryWidget>(calibrate_history_, this);
	valve_area_history_ = std::make_unique<ValveAreaHistoryWidget>(measure_history_, this);

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
