// LTable.cpp

#include "stdafx.h"
#include "LTable.h"

LTable::LTable()
{

}

void LTable::Construct(const std::map<vreal,vreal> &table)
{
	m_table = table;
}

bool LTable::LookUp(vreal x, vreal &y)const
{
	std::map<vreal, vreal>::const_iterator fi = m_table.lower_bound(x);
	if (fi == m_table.end()) 
		return false;
	if (fi == m_table.begin())
	{
		if (fabs(x - fi->first) < EPSILON)
		{
			y = fi->second;
			return true;
		}
		else
			return false;
	}
	vreal x2 = fi->first;
	vreal y2 = fi->second;
	--fi;
	vreal x1 = fi->first;
	vreal y1 = fi->second;
	y = y1 + (y2 - y1) * (x - x1) / (x2 - x1);
	return true;
}