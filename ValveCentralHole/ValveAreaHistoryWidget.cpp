#include "ValveAreaHistoryWidget.h"

#include <future>
#include <QDateTime>


static std::vector<MeasureData> SortValveData(const std::vector<MeasureData>& valve_data, const ValveAreaHistoryWidget::SortOption& option)
{
	std::vector<MeasureData> result = valve_data;

	switch (option)
	{
	case ValveAreaHistoryWidget::SortOption::FILENAME_ASCENDING:
	{
		std::sort(result.begin(), result.end(), [](const MeasureData& a, const MeasureData& b)
			{
				return a.GetFileName() < b.GetFileName();
			});
		break;
	}
	case ValveAreaHistoryWidget::SortOption::FILENAME_DESCENDING:
	{
		std::sort(result.begin(), result.end(), [](const MeasureData& a, const MeasureData& b)
			{
				return a.GetFileName() > b.GetFileName();
			});
		break;
	}
	case ValveAreaHistoryWidget::SortOption::AREA_ASCENDING:
	{
		std::sort(result.begin(), result.end(), [](const MeasureData& a, const MeasureData& b)
			{
				return a.GetMeasuredValveArea() < b.GetMeasuredValveArea();
			});
		break;
	}
	case ValveAreaHistoryWidget::SortOption::AREA_DESCENDING:
	{
		std::sort(result.begin(), result.end(), [](const MeasureData& a, const MeasureData& b)
			{
				return a.GetMeasuredValveArea() > b.GetMeasuredValveArea();
			});
		break;
	}
	case ValveAreaHistoryWidget::SortOption::TIME_ASCENDING:
	{
		std::sort(result.begin(), result.end(), [](const MeasureData& a, const MeasureData& b)
			{
				return QDateTime::fromString(QString::fromStdString(a.GetTimeProcessed())) < QDateTime::fromString(QString::fromStdString(b.GetTimeProcessed()));
			});
		break;
	}
	case ValveAreaHistoryWidget::SortOption::TIME_DESCENDING:
	{
		std::sort(result.begin(), result.end(), [](const MeasureData& a, const MeasureData& b)
			{
				return QDateTime::fromString(QString::fromStdString(a.GetTimeProcessed())) > QDateTime::fromString(QString::fromStdString(b.GetTimeProcessed()));
			});
		break;
	}
	}
	return result;
}


static ValveAreaHistoryWidget::SortOption OnColumnHeaderClicked(QTableWidgetItem* clicked_header, std::unordered_map<QTableWidgetItem*, bool>& sort_mapping, int selected_col)
{
	ValveAreaHistoryWidget::SortOption result;
	switch (selected_col)
	{
	case 0:
	{
		if (sort_mapping.at(clicked_header))
		{
			clicked_header->setIcon(QIcon("down_arrow.png"));
			result = ValveAreaHistoryWidget::SortOption::FILENAME_DESCENDING;
		}
		else
		{
			clicked_header->setIcon(QIcon("up_arrow.png"));
			result = ValveAreaHistoryWidget::SortOption::FILENAME_ASCENDING;
		}
		break;
	}
	case 1:
	{
		if (sort_mapping.at(clicked_header))
		{
			clicked_header->setIcon(QIcon("down_arrow.png"));
			result = ValveAreaHistoryWidget::SortOption::AREA_DESCENDING;
		}
		else
		{
			clicked_header->setIcon(QIcon("up_arrow.png"));
			result = ValveAreaHistoryWidget::SortOption::AREA_ASCENDING;
		}
		break;
	}
	case 2:
	{
		if (sort_mapping.at(clicked_header))
		{
			clicked_header->setIcon(QIcon("down_arrow.png"));
			result = ValveAreaHistoryWidget::SortOption::TIME_DESCENDING;
		}
		else
		{
			clicked_header->setIcon(QIcon("up_arrow.png"));
			result = ValveAreaHistoryWidget::SortOption::TIME_ASCENDING;
		}
		break;
	}
	}
	sort_mapping[clicked_header] = !sort_mapping[clicked_header];
	return result;
}

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
	history_table_->setItem(0, 2, new QTableWidgetItem("Time of Measurement"));

	QTableWidgetItem* filename_header = history_table_->item(0, 0);
	QTableWidgetItem* area_header = history_table_->item(0, 1);
	QTableWidgetItem* time_header = history_table_->item(0, 2);
	sorted_mapping_[filename_header] = false;
	sorted_mapping_[area_header] = false;
	sorted_mapping_[time_header] = false;

	filename_header->setIcon(QIcon("down_arrow.png"));
	area_header->setIcon(QIcon("down_arrow.png"));
	time_header->setIcon(QIcon("down_arrow.png"));

	SetTableData(measure_history_);

	connect(clear_btn_.get(), &QPushButton::clicked, this, [this]()
		{
			if (clear_valve_area_history_callback_)
			{
				clear_valve_area_history_callback_();
			}
		});

	connect(history_table_.get(), &QTableWidget::cellClicked, this, [this](int row, int col)
		{
			if (row == 0)
			{
				QTableWidgetItem* selected_header = history_table_->item(row, col);
				SortOption sort_option = OnColumnHeaderClicked(selected_header, sorted_mapping_, col);

				auto sort_thread = std::async(std::launch::async, [this, sort_option]()
					{
						emit this->OnSortComplete(SortValveData(measure_history_, sort_option));
					});
			}
		});

	connect(this, &ValveAreaHistoryWidget::OnSortComplete, this, [this](const std::vector<MeasureData>& sorted)
		{
			ClearTable(true);
			SetTableData(sorted);
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

void ValveAreaHistoryWidget::SetTableData(const std::vector<MeasureData>& data)
{
	if (!data.empty())
	{
		// Iterate through the vector in reverse to have the latest log at the top
		for (int i = data.size() - 1; i >= 0; i--)
		{
			AddRowToTable(data[i]);
		}
	}
}

void ValveAreaHistoryWidget::SetClearValveAreaHistoryCallback(const std::function<void()>& callback)
{
	clear_valve_area_history_callback_ = callback;
}

void ValveAreaHistoryWidget::ClearTable(bool exclude_first_row)
{
	if (!exclude_first_row)
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
	else
	{
		int temp = history_table_->rowCount() - 1;
		for (int i = 0; i < temp; i++)
		{
			history_table_->removeRow(history_table_->rowCount() - 1);
		}
	}

}