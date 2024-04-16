#pragma once

#include <QMainWindow>
#include <QTabWidget>
#include <QVBoxLayout>
#include <QFileDialog>
#include <fstream>
#include <memory>
#include <vector>
#include "CalculationHistoryDialog.h"
#include "measurewidget.h"
#include "SelectImageCropDialog.h"
#include "FileWriter.h"
#include "ui_ValveCentralHole.h"


enum OptionsMenuSelection
{
	GAUGE_HELPER_TOOL,
	APPLY_LAST_SAVED_PARAMETERS,
	CAMERA
};

enum class ClearTable
{
	CALIBRATION_TABLE,
	VALVE_TABLE
};

class ValveCentralHole : public QMainWindow
{
	Q_OBJECT

public:
	ValveCentralHole(QWidget* parent = nullptr);
	~ValveCentralHole();

signals:
	void OnCalibrateDataCleared();
	void OnMeasureDataCleared();

private:
	Ui::ValveCentralHoleClass ui;
	std::unique_ptr<QMenuBar> menu_bar_;
	std::unique_ptr<QTabWidget> tab_widget;
	std::unique_ptr<CalibrateWidget> calibrate_tab_;
	std::unique_ptr<MeasureWidget> measure_tab_;
	std::unique_ptr<QMenu> options_menu_;
	std::unique_ptr<QMenu> history_menu_;
	std::unique_ptr<QFileDialog> file_dialog_;
	QMenu* helper_tool_submenu_;
	std::unique_ptr<bool> is_gauge_helper_tool_active_;

	std::vector<CalibrateData> calibrate_history_;
	std::vector<MeasureData> measure_history_;

	std::queue<ClearTable> clear_thread_queue_;
	std::mutex clear_thread_wait_mutex_;
	std::mutex clear_thread_queue_mutex_;
	std::condition_variable clear_thread_cv_;

	std::unique_ptr<CalculationHistoryDialog> history_dialog_;

	void InitializeUIElements();
	void ConnectEventListeners();
	void LaunchDatabaseClearThread();
};
