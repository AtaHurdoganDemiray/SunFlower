// MedianFilter.cpp

#include "stdafx.h"
#include "MedianFilter.h"

void MedianFilter::Filter (const std::map<vreal,vreal> &original , std::map<vreal,vreal> &result)
{
	std::map<vreal,vreal>::const_iterator i = original.begin();
	std::map<vreal,vreal>::const_iterator e = original.end();
	std::map<vreal,vreal>::const_iterator pi = i;
	for (i;i!=e;++i)
	{
		std::map<vreal,vreal>::const_iterator ni = i;
		++ni;
		if (ni == e)
			ni = i;
		std::multimap<vreal,vreal> tmp;
		tmp.insert(std::pair<vreal,vreal>(pi->second , pi->second));
		tmp.insert(std::pair<vreal,vreal>(i->second , i->second));
		tmp.insert(std::pair<vreal,vreal>(ni->second , ni->second));
		std::multimap<vreal,vreal>::const_iterator median = tmp.begin();
		++median;
		result[i->first] = median->first;

		pi = i;	
	}
}