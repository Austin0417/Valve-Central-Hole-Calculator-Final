#include "ValveAreaHistoryWidget.h"


ValveAreaHistoryWidget::ValveAreaHistoryWidget(const std::vector<MeasureData>& measure_history, QWidget* parent) : measure_history_(measure_history), QWidget(parent)
{
	ui.setupUi(this);

	history_table_.reset(ui.history_table);
	clear_btn_.reset(ui.clear_btn);

	history_table_->clear();

	// Initialize the first row (headers row)
	history_table_->insertRow(history_table_->rowCount());
	history_table_->setColumnCount(3);
	history_table_->verticalHeader()->setVisible(false);
	history_table_->horizontalHeader()->setVisible(false);
	history_table_->setColumnWidth(0, history_table_->width() / 3);
	history_table_->setColumnWidth(1, history_table_->width() / 3);
	history_table_->setColumnWidth(2, history_table_->width() / 3);
	history_table_->setItem(0, 0, new QTableWidgetItem("Image Filename"));
	history_table_->setItem(0, 1, new QTableWidgetItem("Measured Area"));
	history_table_->setItem(0, 2, new QTableWidgetItem("Time at Measurement"));

	SetTableData();

	connect(clear_btn_.get(), &QPushButton::clicked, this, [this]()
		{
			if (clear_valve_area_history_callback_)
			{
				clear_valve_area_history_callback_();
			}
		});
}

void ValveAreaHistoryWidget::AddRowToTable(const MeasureData& data)
{
	history_table_->insertRow(history_table_->rowCount());
	history_table_->setItem(history_table_->rowCount() - 1, FILENAME, new QTableWidgetItem(QString::fromStdString(data.GetFileName())));
	switch (data.GetUnits())
	{
	case UnitSelection::MILLIMETERS:
	{
		history_table_->setItem(history_table_->rowCount() - 1, MEASURED_AREA, new QTableWidgetItem(QString::number(data.GetMeasuredValveArea()) + " mm^2"));
		break;
	}
	case UnitSelection::INCHES:
	{
		history_table_->setItem(history_table_->rowCount() - 1, MEASURED_AREA, new QTableWidgetItem(QString::number(data.GetMeasuredValveArea()) + " in^2"));
		break;
	}
	}
	history_table_->setItem(history_table_->rowCount() - 1, TIME, new QTableWidgetItem(QString::fromStdString(data.GetTimeProcessed())));
}

void ValveAreaHistoryWidget::SetTableData()
{
	if (!measure_history_.empty())
	{
		// Iterate through the vector in reverse to have the latest log at the top
		for (int i = measure_history_.size() - 1; i >= 0; i--)
		{
			AddRowToTable(measure_history_[i]);
		}
	}
}

void ValveAreaHistoryWidget::SetClearValveAreaHistoryCallback(const std::function<void()>& callback)
{
	clear_valve_area_history_callback_ = callback;
}

void ValveAreaHistoryWidget::ClearTable()
{
	history_table_->clear();
	history_table_->setRowCount(1);
	history_table_->insertRow(history_table_->rowCount());
	history_table_->setColumnWidth(0, history_table_->width() / 3);
	history_table_->setColumnWidth(1, history_table_->width() / 3);
	history_table_->setColumnWidth(2, history_table_->width() / 3);
	history_table_->setItem(0, 0, new QTableWidgetItem("Image Filename"));
	history_table_->setItem(0, 1, new QTableWidgetItem("Measured Area"));
	history_table_->setItem(0, 2, new QTableWidgetItem("Time at Measurement"));
}