#include "CalibrateHistoryWidget.h"
#include <algorithm>
#include <QDateTime>
#include <future>


static std::vector<CalibrateData> SortCalibrateData(const std::vector<CalibrateData>& calibrate_data, const CalibrateHistoryWidget::SortOption& option)
{
	std::vector<CalibrateData> result = calibrate_data;

	switch (option)
	{
	case CalibrateHistoryWidget::SortOption::FILENAME_ASCENDING:
	{
		std::sort(result.begin(), result.end(), [](const CalibrateData& a, const CalibrateData& b)
			{
				return a.GetFileName() < b.GetFileName();
			});
		break;
	}
	case CalibrateHistoryWidget::SortOption::FILENAME_DESCENDING:
	{
		std::sort(result.begin(), result.end(), [](const CalibrateData& a, const CalibrateData& b)
			{
				return a.GetFileName() > b.GetFileName();
			});
		break;
	}
	case CalibrateHistoryWidget::SortOption::CALIBRATION_ASCENDING:
	{
		std::sort(result.begin(), result.end(), [](const CalibrateData& a, const CalibrateData& b)
			{
				return a.GetCalibrationFactor() < b.GetCalibrationFactor();
			});
		break;
	}
	case CalibrateHistoryWidget::SortOption::CALIBRATION_DESCENDING:
	{
		std::sort(result.begin(), result.end(), [](const CalibrateData& a, const CalibrateData& b)
			{
				return a.GetCalibrationFactor() > b.GetCalibrationFactor();
			});
		break;
	}
	case CalibrateHistoryWidget::SortOption::TIME_ASCENDING:
	{
		std::sort(result.begin(), result.end(), [](const CalibrateData& a, const CalibrateData& b)
			{
				return QDateTime::fromString(QString::fromStdString(a.GetTimeProcessed())) < QDateTime::fromString(QString::fromStdString(b.GetTimeProcessed()));
			});
		break;
	}
	case CalibrateHistoryWidget::SortOption::TIME_DESCENDING:
	{
		std::sort(result.begin(), result.end(), [](const CalibrateData& a, const CalibrateData& b)
			{
				return QDateTime::fromString(QString::fromStdString(a.GetTimeProcessed())) > QDateTime::fromString(QString::fromStdString(b.GetTimeProcessed()));
			});
		break;
	}
	}
	return result;
}


CalibrateHistoryWidget::CalibrateHistoryWidget(const std::vector<CalibrateData>& calibrate_history, QWidget* parent)
	: QWidget(parent), calibrate_history_(calibrate_history)
{
	ui.setupUi(this);

	history_table_.reset(ui.history_table_widget);
	clear_btn_.reset(ui.clear_history_btn);

	InitializeHistoryTable();
	SetTableData(calibrate_history_);

	connect(clear_btn_.get(), &QPushButton::clicked, this, [this]()
		{
			if (clear_calibrate_history_callback_)
			{
				clear_calibrate_history_callback_();
			}
		});

	connect(history_table_.get(), &QTableWidget::cellClicked, [this](int row, int column)
		{
			if (row == 0)
			{
				QTableWidgetItem* selected_column_header = history_table_->item(row, column);
				SortOption sort_option;
				switch (column)
				{
				case 0:
				{
					if (sort_mapping_[selected_column_header])
					{
						selected_column_header->setIcon(QIcon("down_arrow.png"));
						sort_option = SortOption::FILENAME_DESCENDING;
					}
					else
					{
						selected_column_header->setIcon(QIcon("up_arrow.png"));
						sort_option = SortOption::FILENAME_ASCENDING;
					}
					break;
				}
				case 1:
				{
					if (sort_mapping_[selected_column_header])
					{
						selected_column_header->setIcon(QIcon("down_arrow.png"));
						sort_option = SortOption::CALIBRATION_DESCENDING;
					}
					else
					{
						selected_column_header->setIcon(QIcon("up_arrow.png"));
						sort_option = SortOption::CALIBRATION_ASCENDING;
					}
					break;
				}
				case 2:
				{
					if (sort_mapping_[selected_column_header])
					{
						selected_column_header->setIcon(QIcon("down_arrow.png"));
						sort_option = SortOption::TIME_ASCENDING;
					}
					else
					{
						selected_column_header->setIcon(QIcon("up_arrow.png"));
						sort_option = SortOption::TIME_DESCENDING;
					}
					break;
				}
				}
				sort_mapping_[history_table_->item(row, column)] = !sort_mapping_[history_table_->item(row, column)];
				auto sort_thread = std::async(std::launch::async, [this, sort_option]()
					{
						std::vector<CalibrateData> sorted = SortCalibrateData(calibrate_history_, sort_option);
						emit this->DisplaySortedTable(sorted);
					});
			}
		});

	connect(this, &CalibrateHistoryWidget::DisplaySortedTable, this, [this](const std::vector<CalibrateData>& sorted)
		{
			SetTableData(sorted, true);
		});
}

void CalibrateHistoryWidget::InitializeHistoryTable(bool is_refresh)
{
	if (!is_refresh)
	{
		history_table_->setRowCount(0);
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

		// Setting icons for all of the column headers
		QTableWidgetItem* filename_column_header = history_table_->item(0, 0);
		QTableWidgetItem* calibration_factor_column_header = history_table_->item(0, 1);
		QTableWidgetItem* time_column_header = history_table_->item(0, 2);

		filename_column_header->setIcon(QIcon("down_arrow.png"));
		calibration_factor_column_header->setIcon(QIcon("down_arrow.png"));
		time_column_header->setIcon(QIcon("down_arrow.png"));

		sort_mapping_[filename_column_header] = false;
		sort_mapping_[calibration_factor_column_header] = false;
		sort_mapping_[time_column_header] = false;
	}
	else
	{
		// Remove all rows except the first
		int temp = history_table_->rowCount() - 1;
		for (int i = 0; i < temp; i++)
		{
			history_table_->removeRow(history_table_->rowCount() - 1);
		}
	}
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


void CalibrateHistoryWidget::SetTableData(const std::vector<CalibrateData>& data, bool is_refresh)
{
	InitializeHistoryTable(is_refresh);
	if (!data.empty())
	{
		// Iterate through the vector in reverse so that the latest is at the top
		for (int i = data.size() - 1; i >= 0; i--)
		{
			AddTableRow(data[i]);
		}
	}
}

void CalibrateHistoryWidget::SetClearCalibrateHistoryCallback(const std::function<void()>& callback)
{
	clear_calibrate_history_callback_ = callback;
}

void CalibrateHistoryWidget::ClearTable()
{
	history_table_->clear();
	history_table_->setRowCount(1);
	InitializeHistoryTable();
}
