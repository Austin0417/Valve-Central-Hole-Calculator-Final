#include "ValveCentralHole.h"
#include "CameraDisplayDialog.h"
#include "MessageBoxHelper.h"


void ValveCentralHole::InitializeUIElements() {
	// Initialize Tab Widget and add the necessary tabs
	tab_widget.reset(ui.tab_widget);
	menu_bar_.reset(ui.options_menu);
	calibrate_tab_ = std::make_unique<CalibrateWidget>(is_gauge_helper_tool_active_, this);
	measure_tab_ = std::make_unique<MeasureWidget>(this);
	file_dialog_ = std::make_unique<QFileDialog>(this);

	file_dialog_->setStyleSheet("background: white;");

	options_menu_.reset(menu_bar_->addMenu("Options"));
	history_menu_.reset(menu_bar_->addMenu("History"));
	options_menu_->addAction(new QAction("Crop an Image"));
	helper_tool_submenu_ = new QMenu("Helper Gauge Diameter Measure Tool");
	options_menu_->addMenu(helper_tool_submenu_);

	helper_tool_submenu_->addAction("Toggle");
	QAction* mirror_action = new QAction("Mirror Drawn Line to Binary");
	mirror_action->setEnabled(false);
	helper_tool_submenu_->addAction(mirror_action);
	helper_tool_submenu_->addAction("Clear Gauge Helper Lines");

	options_menu_->addAction("Apply Last Saved Gauge Parameters");
	options_menu_->addAction("Activate Camera");


	history_menu_->addAction("View Calculation History");

	// Removing the starting tabs from the QTabWidget
	tab_widget->clear();

	calibrate_tab_->SetMirrorCallback([this, mirror_action](bool isCurrentlyShowingPreview)
		{
			mirror_action->setEnabled(isCurrentlyShowingPreview);
		});
	calibrate_tab_->SetCalibrateDataCallback([this](const CalibrateData& calibrate_data)
		{
			{
				std::unique_lock<std::mutex> lock(file_thread_mutex_);
				data_queue_.push(std::make_unique<CalibrateData>(calibrate_data));
			}
			file_thread_cv_.notify_one();
		});

	measure_tab_->SetMeasureDataCallback([this](const MeasureData& measure_data)
		{
			{
				std::unique_lock<std::mutex> lock(file_thread_mutex_);
				data_queue_.push(std::make_unique<MeasureData>(measure_data));
			}
			file_thread_cv_.notify_one();
		});

	tab_widget->addTab(calibrate_tab_.get(), "Calibrate");
	tab_widget->addTab(measure_tab_.get(), "Measure");

	// Setting the window icon
	setWindowTitle("Valve Central Hole Calculator");
	setWindowIcon(QIcon("window_icon.jpg"));

	is_gauge_helper_tool_active_ = std::make_unique<bool>(false);
}

void ValveCentralHole::ConnectEventListeners() {
	connect(tab_widget.get(), &QTabWidget::tabBarClicked, this, [this](int tab_index) {
		qDebug() << "Switched to tab with index: " << tab_index;
		tab_widget->setCurrentIndex(tab_index);
		if (tab_index == 1)
		{
			measure_tab_->RefreshCalibrationFactor();
		}
		});

	connect(file_dialog_.get(), &QFileDialog::fileSelected, this, [this](const QString& filename)
		{
			if (!filename.contains(".jpg") && !filename.contains(".png"))
			{
				MessageBoxHelper::ShowErrorDialog("Selected file was not in a valid image format (jpg or png)");
				return;
			}
			SelectImageCropDialog* dialog = new SelectImageCropDialog(filename, this);
			dialog->exec();
		});

	QList<QAction*> options_menu_actions = options_menu_->actions();
	if (!options_menu_actions.empty())
	{
		for (QAction* action : options_menu_actions)
		{

			if (action->text() == "Apply Last Saved Gauge Parameters")
			{
				connect(action, &QAction::triggered, this, [this]()
					{
						// Apply the last saved parameters to the Calibrate Widget
						calibrate_tab_->ApplyLastSavedParameters();
					});
			}
			else if (action->text() == "Activate Camera")
			{
				connect(action, &QAction::triggered, this, [this]()
					{
						CameraDisplayDialog* camera_dialog = new CameraDisplayDialog(this);
						camera_dialog->SetOnCameraCompleteCallback([this](Mat confirmed_mat, const QString& image_name)
							{
								calibrate_tab_->ReceiveAndDisplayCameraImage(image_name);
							});
						camera_dialog->exec();
					});
			}
			else if (action->text() == "Crop an Image")
			{
				connect(action, &QAction::triggered, this, [this]()
					{
						file_dialog_->exec();
					});
			}
		}
	}

	QList<QAction*> helper_tool_submenu_actions = helper_tool_submenu_->actions();
	if (!helper_tool_submenu_actions.empty())
	{
		for (QAction* action : helper_tool_submenu_actions)
		{
			if (action->text() == "Toggle")
			{
				connect(action, &QAction::triggered, this, [this, action]()
					{
						*is_gauge_helper_tool_active_ = !(*is_gauge_helper_tool_active_);
						if (*is_gauge_helper_tool_active_)
						{
							action->setIcon(QIcon("checkmark_icon.png"));
						}
						else
						{
							action->setIcon(QIcon());
						}
					});
			}
			else if (action->text() == "Clear Gauge Helper Lines")
			{
				connect(action, &QAction::triggered, this, [this]()
					{
						// TODO Clear gauge helper lines here
						emit calibrate_tab_->ShouldClearHelperGaugeLines();
					});
			}
			else
			{
				connect(action, &QAction::triggered, this, [this]()
					{
						emit calibrate_tab_->MirrorDrawnLinesToPreview();
					});
			}
		}
	}

	QList<QAction*> history_menu_actions = history_menu_->actions();
	if (!history_menu_actions.empty())
	{
		for (QAction* action : history_menu_actions)
		{
			if (action->text() == "View Calculation History")
			{
				connect(action, &QAction::triggered, this, [this](bool checked)
					{
						std::mutex mutex;
						std::condition_variable cv;
						bool has_loaded_calibrate_history = false, has_loaded_valve_area_history = false;

						// Load the history data from both text files
						auto load_calibrate_history = std::async(std::launch::async, [this, &cv, &has_loaded_calibrate_history]()
							{
								calibrate_history_ = file_writer_.ReadCalibrateHistory();
								has_loaded_calibrate_history = true;
								cv.notify_one();
							});

						auto load_valve_area_history = std::async(std::launch::async, [this, &cv, &has_loaded_valve_area_history]()
							{
								measure_history_ = file_writer_.ReadValveAreaHistory();
								has_loaded_valve_area_history = true;
								cv.notify_one();
							});

						// Wait until both files have been loaded
						{
							std::unique_lock<std::mutex> lock(mutex);
							cv.wait(lock, [this, &has_loaded_calibrate_history, &has_loaded_valve_area_history]()
								{
									return has_loaded_calibrate_history && has_loaded_valve_area_history;
								});
						}
						num_lines_calibrate_history_ = calibrate_history_.size();
						num_lines_valve_history_ = measure_history_.size();

						// Launch the CalculationHistoryDialog here
						history_dialog_ = std::make_unique<CalculationHistoryDialog>(calibrate_history_, measure_history_, this);

						// Clear the calibrate history text file here
						history_dialog_->SetClearCalibrateHistoryCallback([this]()
							{
								std::ofstream file_clear(FileWriter::CALIBRATE_HISTORY_FILENAME);
								if (file_clear.is_open())
								{
									MessageBoxHelper::ShowOkDialog("Previous calibration history has been cleared");
									history_dialog_->GetCalibrateHistoryWidget()->ClearTable();
								}
							});

						// Clear Valve Area history text file
						history_dialog_->SetClearValveAreaHistoryCallback([this]()
							{
								std::ofstream file_clear(FileWriter::MEASURE_AREA_HISTORY_FILENAME);
								if (file_clear.is_open())
								{
									MessageBoxHelper::ShowOkDialog("Previous valve area measurement history has been cleared");
									history_dialog_->GetValveAreaHistoryWidget()->ClearTable();
								}
							});
						history_dialog_->exec();
					});
			}
		}
	}
}


void ValveCentralHole::LaunchFileWriterThread()
{
	std::thread([this]()
		{
			while (true)
			{
				{
					std::unique_lock<std::mutex> lock(file_thread_mutex_);
					file_thread_cv_.wait(lock, [this]()
						{
							return !data_queue_.empty();
						});
				}

				std::unique_ptr<ValveData> data = std::move(data_queue_.front());
				{
					std::unique_lock<std::mutex> lock(file_thread_mutex_);
					data_queue_.pop();
				}

				// Write the data to the corresponding text file here (we have to determine the corresponding text file)
				qDebug() << "RECORDED LOG: " << data->ToFileFormat();
				if (data->GetFileNameType() == FileWriter::CALIBRATE_HISTORY_FILENAME)
				{
					file_writer_.AppendToCalibrateHistory(*data);
					num_lines_calibrate_history_++;
				}
				else
				{
					file_writer_.AppendToValveAreaHistory(*data);
					num_lines_valve_history_++;
				}

				// TODO After the latest log has been added, check either of the text files exceed 20 lines
				// If greater than 20 lines, remove the first line (oldest log)
				if (num_lines_calibrate_history_ > 20)
				{
					file_writer_.TrimCalibrateHistory();
					num_lines_calibrate_history_ = 20;
				}
				if (num_lines_valve_history_ > 20)
				{
					file_writer_.TrimValveAreaHistory();
					num_lines_valve_history_ = 20;
				}
			}
		}).detach();
}


ValveCentralHole::ValveCentralHole(QWidget* parent)
	: QMainWindow(parent)
{
	ui.setupUi(this);

	InitializeUIElements();
	ConnectEventListeners();
	LaunchFileWriterThread();
}


ValveCentralHole::~ValveCentralHole()
{}
