

#include "HitTableList.h"

HitTableList::HitTableList(std::vector<std::shared_ptr<HitTable>>& l)
{
	HitList = l;
}

bool HitTableList::Hit(const Ray& r, float tMin, float tMax, HitRecord& result) const
{
	HitRecord tempRec;
	bool bHitAnything = false;
	float closeSoFar = tMax;

	for(auto& table : HitList)
	{
		if(table->Hit(r, tMin, tMax, tempRec))
		{
			bHitAnything = true;
			closeSoFar = tempRec.T;
			result = tempRec;
		}
	}
	return bHitAnything;
}
