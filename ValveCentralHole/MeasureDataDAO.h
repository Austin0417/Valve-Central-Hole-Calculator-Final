#pragma once
#include "Database.h"
#include "MeasureData.h"


class MeasureDataDAO
{
public:
	bool InsertMeasureData(const MeasureData& data) const;
	bool ClearMeasureData() const;
	std::vector<MeasureData> GetAllMeasureData() const;
};

