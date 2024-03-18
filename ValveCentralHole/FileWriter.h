#pragma once
#include <fstream>
#include "CalibrateData.h"
#include "MeasureData.h"
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
	void AppendToCalibrateHistory(const ValveData& data);
	void AppendToValveAreaHistory(const ValveData& measure_data);

	static std::string CALIBRATE_HISTORY_FILENAME;
	static std::string MEASURE_AREA_HISTORY_FILENAME;
};

