#include "ValveAreaHistoryWidget.h"

#include <future>
#include <QDateTime>

#include "MeasureDataDAO.h"


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

ValveAreaHistoryWidget::ValveAreaHistoryWidget(QWidget* parent) : QWidget(parent)
{
	ui.setupUi(this);

	history_table_.reset(ui.history_table);
	clear_btn_.reset(ui.clear_btn);
	export_csv_btn_.reset(ui.export_csv_btn);
	save_csv_file_dialog_ = std::make_unique<QFileDialog>(this);
	save_csv_file_dialog_->setFileMode(QFileDialog::Directory);

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

	filename_header->setFlags(Qt::ItemIsSelectable | Qt::ItemIsUserCheckable | Qt::ItemIsEnabled);
	area_header->setFlags(Qt::ItemIsSelectable | Qt::ItemIsUserCheckable | Qt::ItemIsEnabled);
	time_header->setFlags(Qt::ItemIsSelectable | Qt::ItemIsUserCheckable | Qt::ItemIsEnabled);
	filename_header->setIcon(QIcon("down_arrow.png"));
	area_header->setIcon(QIcon("down_arrow.png"));
	time_header->setIcon(QIcon("down_arrow.png"));

	//SetTableData(measure_history_);
	auto fetch_from_database = std::async(std::launch::async, [this]()
		{
			MeasureDataDAO dao;
			measure_history_ = dao.GetAllMeasureData();
			emit this->OnDataInitialLoadComplete();
		});


	connect(this, &ValveAreaHistoryWidget::OnDataInitialLoadComplete, this, [this]()
		{
			SetTableData(measure_history_);
		});

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

	connect(export_csv_btn_.get(), &QPushButton::clicked, this, [this]()
		{
			save_csv_file_dialog_->exec();
		});

	connect(save_csv_file_dialog_.get(), &QFileDialog::fileSelected, [this](const QString& directory)
		{
			// Two threads, one will retrieve all of the CSV data, such as the headers and all of the row data.
			// The other will initialize and open an output file stream that creates the CSV file at the desired file location
			auto output_file_stream_thread = std::async(std::launch::async, [this, directory]()
				{
					// Initially create the output CSV file and write the headers, then wait until the data retrieval thread has finished processing
					std::ofstream output_csv(directory.toStdString() + "/valve_area_history.csv");
					if (output_csv.is_open())
					{
						output_csv << "Image Filename, Measured Area, Time of Measurement\n";
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

	connect(this, &ValveAreaHistoryWidget::OnTableRefreshComplete, this, [this]()
		{
			ClearTable(true);
			SetTableData(measure_history_);
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

void ValveAreaHistoryWidget::RefreshTableData()
{
	auto refresh_thread = std::async(std::launch::async, [this]()
		{
			MeasureDataDAO dao;
			measure_history_ = dao.GetAllMeasureData();
			emit this->OnTableRefreshComplete();
		});
}


std::vector<QTableWidgetItem*> ValveAreaHistoryWidget::GetItemsFromRow(int row)
{
	std::vector<QTableWidgetItem*> result;
	for (int i = 0; i < history_table_->columnCount(); i++)
	{
		result.push_back(history_table_->item(row, i));
	}
	return result;
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