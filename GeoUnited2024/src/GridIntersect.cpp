#include "stdafx.h"
#include "GridIntersect.h"
#include "VPoint2d.h"
#include "math.h"


GridIntersect::GridIntersect(void)
{
}

GridIntersect::~GridIntersect(void)
{
}

void GridIntersect::SortedPolygon(const std::vector<int> &interi, 
							 const std::vector<int> &loopPoints, 
							 const std::vector<int> &corners, 
							 const std::vector<VPoint2d> &pointPool
							 ,std::vector<int> &pol)
{
	int cornerSize = (int)corners.size();
	int interiSize = (int)interi.size();
	int loopPointSize = (int)loopPoints.size();
	int in(-1), out(-1);
	for(int i = 0; i<interiSize; i++)
	{
		if ( interi[i] == -1 )
			out = interi[i-1];
		if ( interi[i] == -2 ) 
			in = interi[i-1];
	}

	if (in == -1 || out == -1)
		throw VException (( IDS_GriEntOrLeaPoiI),1);

	if ( cornerSize == 1 )
	{
		pol.push_back(in);
		for ( int i = 0; i < loopPointSize; i++ )
			pol.push_back(loopPoints[i]);
		pol.push_back(out);
	}
		
	else if ( cornerSize == 2 )
	{
		int firstCorner, lastCorner;
		double x1 = pointPool.operator [](corners[0]).x();
		double y1 = pointPool.operator [](corners[0]).y();

		double x2 = pointPool.operator [](corners[1]).x();
		double y2 = pointPool.operator [](corners[1]).y();

		double x3 = pointPool.operator [](out).x();
		double y3 = pointPool.operator [](out).y();

		double xd = x3 - x1;
		double yd = y3 - y1;

		double distance0out2 = xd*xd+yd*yd;
	
		xd = x3 - x2;
		yd = y3 - y2;

		double distance1out2 = xd*xd+yd*yd;

		if ( distance0out2 < distance1out2 )
		{
			firstCorner = corners[0];
			lastCorner = corners[1];
		}
		else //if ( distance0out2 > distance1out2 )
		{
			firstCorner = corners[1];
			lastCorner = corners[0];
		}

		pol.push_back(in);
		for ( int i = 0; i < loopPointSize; i++ )
			pol.push_back(loopPoints[i]);
		pol.push_back(out);
		pol.push_back(firstCorner);
		pol.push_back(lastCorner);
		
	}

	else if ( cornerSize == 3 )
	{
		int firstCorner, middleCorner, lastCorner;
		for(int i = 0; i<3; i++)
		{
			if ( fabs(pointPool.operator [](corners[i]).x() - pointPool.operator [](in).x() ) > 1e-8 && 
				 fabs(pointPool.operator [](corners[i]).y() - pointPool.operator [](in).y() ) > 1e-8 &&
				 fabs(pointPool.operator [](corners[i]).x() - pointPool.operator [](out).x() ) > 1e-8 && 
				 fabs(pointPool.operator [](corners[i]).y() - pointPool.operator [](out).y() ) > 1e-8 )
					middleCorner = corners[i];
		}
		std::vector<int> cornerNew;
		for(int i = 0; i < 3; i++)
		{
			if( corners[i] != middleCorner)
				cornerNew.push_back(corners[i]);
		}

		double x1 = pointPool.operator [](cornerNew[0]).x();
		double y1 = pointPool.operator [](cornerNew[0]).y();

		double x2 = pointPool.operator [](cornerNew[1]).x();
		double y2 = pointPool.operator [](cornerNew[1]).y();

		double x3 = pointPool.operator [](out).x();
		double y3 = pointPool.operator [](out).y();

		double xd = x3 - x1;
		double yd = y3 - y1;

		double distance0out2 = xd*xd+yd*yd;
	
		xd = x3 - x2;
		yd = y3 - y2;

		double distance1out2 = xd*xd+yd*yd;

		if ( distance0out2 < distance1out2 )
		{
			firstCorner = corners[0];
			lastCorner = corners[1];
		}
		else //if ( distance0out2 > distance1out2 )
		{
			firstCorner = corners[1];
			lastCorner = corners[0];
		}

		pol.push_back(in);
		for ( int i = 0; i < loopPointSize; i++ )
			pol.push_back(loopPoints[i]);
		pol.push_back(out);
		pol.push_back(firstCorner);
		pol.push_back(middleCorner);
		pol.push_back(lastCorner);
	}
}
