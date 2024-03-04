// BSplineControlPoints

#ifndef __BSplineControlPoints_h__
#define __BSplineControlPoints_h__

#include "PointSet.h"

class BSplineControlPoints
{
public:
	BSplineControlPoints ( int pointno , const std::vector<vreal> &controlpoints );
	BSplineControlPoints ( int _upointno , int _vpointno , const std::vector<vreal> &controlpoints ); 
	const VPoint & GetPoint ( int i ) const; 
	const VPoint & GetPoint ( int iu , int iv ) const;	
protected:
	int GetNo ( int iu , int iv ) const;
	int upointno;
	PointSet _points;

};

#endif