#include "CalibrateHistoryWidget.h"
#include <algorithm>
#include <QDateTime>
#include <future>

#include "CalibrateDataDAO.h"


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


CalibrateHistoryWidget::CalibrateHistoryWidget(QWidget* parent)
	: QWidget(parent)
{
	ui.setupUi(this);

	history_table_.reset(ui.history_table_widget);
	clear_btn_.reset(ui.clear_history_btn);
	export_csv_btn_.reset(ui.export_csv_btn);
	save_csv_file_dialog_ = std::make_unique<QFileDialog>(this);
	save_csv_file_dialog_->setFileMode(QFileDialog::Directory);

	InitializeHistoryTable();
	//SetTableData(calibrate_history_);

	std::future<void> query_from_database = std::async(std::launch::async, [this]()
		{
			CalibrateDataDAO dao;
			calibrate_history_ = dao.GetAllCalibrateData();
			emit this->OnDataInitialLoadComplete();
		});

	connect(this, &CalibrateHistoryWidget::OnDataInitialLoadComplete, this, [this]()
		{
			SetTableData(calibrate_history_);
		});

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

	connect(export_csv_btn_.get(), &QPushButton::clicked, this, [this]()
		{
			save_csv_file_dialog_->exec();
		});

	connect(save_csv_file_dialog_.get(), &QFileDialog::fileSelected, this, [this](const QString& directory)
		{
			// Two threads, one will retrieve all of the CSV data, such as the headers and all of the row data.
			// The other will initialize and open an output file stream that creates the CSV file at the desired file location
			auto output_file_stream_thread = std::async(std::launch::async, [this, directory]()
				{
					// Initially create the output CSV file and write the headers, then wait until the data retrieval thread has finished processing
					std::ofstream output_csv(directory.toStdString() + "/calibrate_history.csv");
					if (output_csv.is_open())
					{
						output_csv << "Image Filename, Calculated Calibration Factor, Time of Measurement\n";
						{
							std::unique_lock<std::mutex> lock(mutex_);
							cv_.wait(lock, [this]()
								{
									return data_retrieval_complete;
								});
						}
						for (const std::vector<QTableWidgetItem*>& table_row : table_items_)
						{
							for (int i = 0; i < table_row.size(); i++)
							{
								if (i == table_row.size() - 1)
								{
									output_csv << table_row[i]->text().toStdString() << "\n";
								}
								else
								{
									output_csv << table_row[i]->text().toStdString() << ", ";
								}
							}
						}
						output_csv.flush();
						table_items_.clear();
						data_retrieval_complete = false;
					}
				});

			auto data_retrieval_thread = std::async(std::launch::async, [this]()
				{
					for (int i = 1; i < history_table_->rowCount(); i++)
					{
						table_items_.push_back(GetItemsFromRow(i));
					}
					data_retrieval_complete = true;
					cv_.notify_one();
				});
		});

	connect(this, &CalibrateHistoryWidget::OnTableRefreshComplete, this, [this]()
		{
			SetTableData(calibrate_history_, true);
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
		filename_column_header->setFlags(Qt::ItemIsSelectable | Qt::ItemIsUserCheckable | Qt::ItemIsEnabled);
		calibration_factor_column_header->setFlags(Qt::ItemIsSelectable | Qt::ItemIsUserCheckable | Qt::ItemIsEnabled);
		time_column_header->setFlags(Qt::ItemIsSelectable | Qt::ItemIsUserCheckable | Qt::ItemIsEnabled);

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

std::vector<QTableWidgetItem*> CalibrateHistoryWidget::GetItemsFromRow(int row)
{
	std::vector<QTableWidgetItem*> result;
	for (int i = 0; i < history_table_->columnCount(); i++)
	{
		result.push_back(history_table_->item(row, i));
	}
	return result;
}


void CalibrateHistoryWidget::SetClearCalibrateHistoryCallback(const std::function<void()>& callback)
{
	clear_calibrate_history_callback_ = callback;
}

void CalibrateHistoryWidget::RefreshTableData()
{
	auto refresh_thread = std::async(std::launch::async, [this]()
		{
			CalibrateDataDAO dao;
			calibrate_history_ = dao.GetAllCalibrateData();
			emit this->OnTableRefreshComplete();
		});
}


void CalibrateHistoryWidget::ClearTable()
{
	history_table_->clear();
	history_table_->setRowCount(1);
	InitializeHistoryTable();
}
