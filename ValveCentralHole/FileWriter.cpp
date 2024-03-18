#include "FileWriter.h"
#include <iostream>

std::string FileWriter::CALIBRATE_HISTORY_FILENAME = "calibrate_history.txt";
std::string FileWriter::MEASURE_AREA_HISTORY_FILENAME = "valve_area_history.txt";

static std::vector<std::string> splitString(std::string& line, const std::string& delimiter)
{
	size_t pos = 0;
	std::string token;
	std::vector<std::string> result;
	while ((pos = line.find(delimiter)) != std::string::npos) {
		token = line.substr(0, pos);
		result.push_back(token);
		line.erase(0, pos + delimiter.length());
	}
	result.push_back(line);

	return result;
}


FileWriter::FileWriter()
{
	// If the history files do not exist (first time launching the app), then create them
	write_calibrate_history_.open("calibrate_history.txt", std::ios::out | std::ios::app);
	write_valve_area_history_.open("valve_area_history.txt", std::ios::out | std::ios::app);
	read_calibrate_history_.open("calibrate_history.txt");
	read_valve_area_history_.open("valve_area_history.txt");
}


std::vector<CalibrateData> FileWriter::ReadCalibrateHistory()
{
	std::string line;
	std::vector<CalibrateData> result;
	while (std::getline(read_calibrate_history_, line))
	{
		std::vector<std::string> split_string = splitString(line, ", ");
		std::string tmp = split_string[1];
		std::vector<std::string> value_and_units = splitString(tmp, " ");
		UnitSelection units = UnitSelection::MILLIMETERS;

		if (value_and_units.size() >= 2 && value_and_units[1] == "in^2")
		{
			units = UnitSelection::INCHES;
		}
		result.emplace_back(split_string[0], split_string[2], units, std::stod(split_string[1]));
	}

	// Clear the eof flag and return to the beginning of the file
	read_calibrate_history_.clear();
	read_calibrate_history_.seekg(0, std::ios::beg);

	return result;
}


std::vector<MeasureData> FileWriter::ReadValveAreaHistory()
{
	std::string line;
	std::vector<MeasureData> result;
	while (std::getline(read_valve_area_history_, line))
	{
		std::vector<std::string> split_string = splitString(line, ", ");
		std::string tmp = split_string[1];
		std::vector<std::string> value_and_units = splitString(tmp, " ");
		UnitSelection units = UnitSelection::MILLIMETERS;

		if (value_and_units.size() >= 2 && value_and_units[1] == "in^2")
		{
			units = UnitSelection::INCHES;
		}
		result.emplace_back(split_string[0], split_string[2], units, std::stod(split_string[1]));
	}

	read_valve_area_history_.clear();
	read_valve_area_history_.seekg(0, std::ios::beg);

	return result;
}

void FileWriter::AppendToCalibrateHistory(const ValveData& calibrate_data)
{

	write_calibrate_history_ << calibrate_data.ToFileFormat();
	write_calibrate_history_.flush();
}

void FileWriter::AppendToValveAreaHistory(const ValveData& measure_data)
{
	write_valve_area_history_ << measure_data.ToFileFormat();
	write_valve_area_history_.flush();
}

void FileWriter::TrimCalibrateHistory()
{
	std::vector<CalibrateData> calibrate_history = ReadCalibrateHistory();

	// Grab the latest 20 logs/CalibrateData
	int index = 0;
	while (index < 20)
	{

	}
}

