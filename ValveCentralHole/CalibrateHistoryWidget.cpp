#include "CalibrateHistoryWidget.h"


CalibrateHistoryWidget::CalibrateHistoryWidget(const std::vector<CalibrateData>& calibrate_history, QWidget* parent)
	: QWidget(parent), calibrate_history_(calibrate_history)
{
	ui.setupUi(this);

	history_table_.reset(ui.history_table_widget);
	clear_btn_.reset(ui.clear_history_btn);

	history_table_->setColumnCount(3);
	history_table_->verticalHeader()->setVisible(false);
	history_table_->horizontalHeader()->setVisible(false);
	history_table_->setColumnWidth(0, history_table_->width() / 3);
	history_table_->setColumnWidth(1, history_table_->width() / 3);
	history_table_->setColumnWidth(2, history_table_->width() / 3);
	history_table_->insertRow(history_table_->rowCount());
	history_table_->setItem(0, FILENAME, new QTableWidgetItem("Image Filename"));
	history_table_->setItem(0, CALIBRATION_FACTOR, new QTableWidgetItem("Calculated Calibration Factor"));
	history_table_->setItem(0, TIME, new QTableWidgetItem("Time of Measurement"));
	SetTableData();
}


void CalibrateHistoryWidget::AddTableRow(const CalibrateData& data)
{
	history_table_->insertRow(history_table_->rowCount());
	history_table_->setItem(history_table_->rowCount() - 1, FILENAME, new QTableWidgetItem(QString::fromStdString(data.GetFileName())));
	switch (data.GetUnits())
	{
	case UnitSelection::MILLIMETERS:
	{
		history_table_->setItem(history_table_->rowCount() - 1, CALIBRATION_FACTOR, new QTableWidgetItem(QString::number(data.GetCalibrationFactor()) + " mm^2"));
		break;
	}
	case UnitSelection::INCHES:
	{
		history_table_->setItem(history_table_->rowCount() - 1, CALIBRATION_FACTOR, new QTableWidgetItem(QString::number(data.GetCalibrationFactor()) + " in^2"));
		break;
	}
	}
	history_table_->setItem(history_table_->rowCount() - 1, TIME, new QTableWidgetItem(QString::fromStdString(data.GetTimeProcessed())));
}


void CalibrateHistoryWidget::SetTableData()
{
	if (!calibrate_history_.empty())
	{
		// Iterate through the vector in reverse so that the latest is at the top
		for (int i = calibrate_history_.size() - 1; i >= 0; i--)
		{
			AddTableRow(calibrate_history_[i]);
		}
	}
}