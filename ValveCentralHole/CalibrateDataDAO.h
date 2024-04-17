#pragma once
#include "CalibrateData.h"
#include "Database.h"
#include <vector>



class CalibrateDataDAO
{
public:
	std::optional<CalibrateData> InsertCalibrateData(const CalibrateData& data) const;
	std::optional<CalibrateData> FindCalibrateData(const CalibrateData& data) const;
	bool ClearCalibrateData() const;
	std::vector<CalibrateData> GetAllCalibrateData() const;
	bool DeleteSelectedData(const CalibrateData& data) const;
	bool DeleteSelectedDataList(const std::vector<CalibrateData>& data_to_delete) const;
};

