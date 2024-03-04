// BSplineM.h

#ifndef __BSplineM_h__
#define __BSplineM_h__

#include "Matrix.h"

class BSplineM
{
public:
	BSplineM ( int row , int col )
		: m ( row , col )
	{

	};

	Matrix m;

};

#endif //__BSplineM_h__
