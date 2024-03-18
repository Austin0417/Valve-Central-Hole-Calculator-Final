#pragma once
#include <fstream>
#include "CalibrateData.h"
#include "MeasureData.h"
#include <queue>
#include <vector>


class FileWriter
{
private:
	std::ifstream read_calibrate_history_;
	std::ifstream read_valve_area_history_;
	std::ofstream write_calibrate_history_;
	std::ofstream write_valve_area_history_;
public:
	FileWriter();
	std::vector<CalibrateData> ReadCalibrateHistory();
	std::vector<MeasureData> ReadValveAreaHistory();
	std::queue<CalibrateData> ReadCalibrateHistoryQueue();
	std::queue<MeasureData> ReadValveAreaHistoryQueue();
	void AppendToCalibrateHistory(const ValveData& data);
	void AppendToValveAreaHistory(const ValveData& measure_data);
	void TrimCalibrateHistory();	// trims the calibrate history text file so that only the latest 20 logs are kept
	void TrimValveAreaHistory();

	static std::string CALIBRATE_HISTORY_FILENAME;
	static std::string MEASURE_AREA_HISTORY_FILENAME;
};

