// SequentialLinesSubset.cpp

#include "stdafx.h"
#include "SequentialLinesSubset.h"
#include "SequentialLineSet2d.h"

void SequentialLinesSubset::GetSubset(const SequentialLineSet2d &sl
									  , const std::vector<vreal> &param
									  , const vreal t0
									  , const vreal t1
									  , SequentialLineSet2d &sub)
{
	int mysize = sl.GetPointSize();
	for (int i=0;i<mysize;++i)
	{
		const vreal &currt = param[i];
		if (currt > t0 && currt < t1)
		{
			sub.InsertPoint (*sl.GetPoint(i));
		}
		else if (currt > t1)
			break;
	}
}
