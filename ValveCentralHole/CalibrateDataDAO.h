#pragma once
#include "CalibrateData.h"
#include "Database.h"
#include <vector>



class CalibrateDataDAO
{
public:
	bool InsertCalibrateData(const CalibrateData& data) const;
	bool ClearCalibrateData() const;
	std::vector<CalibrateData> GetAllCalibrateData() const;

};

