// BSplineControlPoints.cpp

#include "stdafx.h"
#include "BSplineControlPoints.h"

BSplineControlPoints::BSplineControlPoints ( int pointno , const std::vector<vreal> &controlpoints )
: _points ( pointno + 1 )
{
	upointno = pointno;
	for ( int i = 0; i < pointno; i++ )
	{
		int startindex = i*3;
		vreal coord[3] =  { controlpoints[startindex]
							, controlpoints[startindex+1]
							, controlpoints[startindex+2] };

		_points.InsertPoint ( coord );
	}
}

BSplineControlPoints::BSplineControlPoints ( int _upointno , int _vpointno , const std::vector<vreal> &controlpoints )
:_points ( _upointno * _vpointno + 1 )
{
	upointno = _upointno;
		
	for ( int j = 0; j < _vpointno; j++ )	
		for ( int i = 0; i < _upointno; i++ )
		{
			int no = GetNo ( i , j );
			int startindex = no * 3;
			vreal coord[3] =  { controlpoints[startindex]
							, controlpoints[startindex+1]
							, controlpoints[startindex+2] };
			
			_points.InsertPoint ( coord );	
		}
}

const VPoint & BSplineControlPoints::GetPoint ( int i ) const
{
	return *( _points[i] );
}

const VPoint & BSplineControlPoints::GetPoint ( int iu , int iv ) const
{
	int no = GetNo ( iu , iv );
	return *( _points[no] );
}

int BSplineControlPoints::GetNo (int iu,int iv) const
{
	int no = iv * upointno + iu;
	return no;
}

