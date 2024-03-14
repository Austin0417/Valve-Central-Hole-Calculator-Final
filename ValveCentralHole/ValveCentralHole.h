#pragma once

#include <QMainWindow>
#include <QTabWidget>
#include <QVBoxLayout>
#include <QFileDialog>
#include <memory>
#include <vector>
#include "measurewidget.h"
#include "SelectImageCropDialog.h"
#include "ui_ValveCentralHole.h"


enum OptionsMenuSelection
{
	GAUGE_HELPER_TOOL,
	APPLY_LAST_SAVED_PARAMETERS,
	CAMERA
};

class ValveCentralHole : public QMainWindow
{
	Q_OBJECT

public:
	ValveCentralHole(QWidget* parent = nullptr);
	~ValveCentralHole();

private:
	Ui::ValveCentralHoleClass ui;
	std::unique_ptr<QMenuBar> menu_bar_;
	std::unique_ptr<QTabWidget> tab_widget;
	std::unique_ptr<CalibrateWidget> calibrate_tab_;
	std::unique_ptr<MeasureWidget> measure_tab_;
	std::unique_ptr<QMenu> options_menu_;
	std::unique_ptr<QFileDialog> file_dialog_;
	QMenu* helper_tool_submenu_;
	std::unique_ptr<bool> is_gauge_helper_tool_active_;

	void InitializeUIElements();
	void ConnectEventListeners();
};
