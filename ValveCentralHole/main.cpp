#include "ValveCentralHole.h"
#include "Database.h"
#include "CalibrateDataDAO.h"
#include "MeasureDataDAO.h"
#include <QtWidgets/QApplication>
#include <opencv2/core.hpp>
#include <opencv2/imgcodecs.hpp>
#include <opencv2/highgui.hpp>
#include <iostream>


using namespace cv;
int main(int argc, char* argv[])
{
	Database::InitializeDatabaseTables();
	QApplication a(argc, argv);
	ValveCentralHole w;

	CalibrateDataDAO calibrate_dao;
	MeasureDataDAO measure_dao;
	std::vector<CalibrateData> calibration_history = calibrate_dao.GetAllCalibrateData();
	std::vector<MeasureData> valve_history = measure_dao.GetAllMeasureData();

	std::cout << "Calibration history : \n";
	for (const auto& calibration : calibration_history)
	{
		std::cout << calibration << "\n";
	}

	std::cout << "Valve History : \n";

	for (const auto& measure : valve_history)
	{
		std::cout << measure << "\n";
	}

	w.show();
	return a.exec();
}
