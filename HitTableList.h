#pragma once

#include "HitTable.h"

class HitTableList : public HitTable
{
public:
	HitTableList(){}
	virtual bool Hit(const Ray& r, float tMin, float tMax, HitRecord& result) const override;


	HitTableList(std::vector<std::shared_ptr<HitTable>>& l);

	std::vector<std::shared_ptr<HitTable>> HitList;
	
	int ListSize = 0;	
};