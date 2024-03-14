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

	// Removing the starting tabs from the QTabWidget
	tab_widget->clear();

	calibrate_tab_->SetMainCallback([this, mirror_action](bool isCurrentlyShowingPreview)
		{
			mirror_action->setEnabled(isCurrentlyShowingPreview);
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
}


ValveCentralHole::ValveCentralHole(QWidget* parent)
	: QMainWindow(parent)
{
	ui.setupUi(this);

	InitializeUIElements();
	ConnectEventListeners();
}


ValveCentralHole::~ValveCentralHole()
{}
