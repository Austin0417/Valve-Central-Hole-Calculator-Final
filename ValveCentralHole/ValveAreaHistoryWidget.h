#pragma once
#include <QWidget>
#include <QTableWidget>
#include <QPushButton>
#include "MeasureData.h"
#include "ui_ValveAreaHistoryWidget.h"



class ValveAreaHistoryWidget : public QWidget
{
private:
	enum RowFormat
	{
		FILENAME,
		MEASURED_AREA,
		TIME
	};

	std::unique_ptr<QTableWidget> history_table_;
	std::unique_ptr<QPushButton> clear_btn_;
	const std::vector<MeasureData>& measure_history_;
	std::function<void()> clear_valve_area_history_callback_;

	Ui::ValveAreaHistoryWidget ui;
	void AddRowToTable(const MeasureData& data);
	void SetTableData();
public:
	ValveAreaHistoryWidget(const std::vector<MeasureData>& measure_history, QWidget* parent = nullptr);
	void SetClearValveAreaHistoryCallback(const std::function<void()>& callback);
	void ClearTable();
};

