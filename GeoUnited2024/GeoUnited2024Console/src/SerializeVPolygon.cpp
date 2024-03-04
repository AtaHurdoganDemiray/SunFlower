#include "stdafx.h"
#include "VPoint2d.h"
#include "SerializeVPolygon.h"

SerializeVPolygon::SerializeVPolygon(void)
{
}

SerializeVPolygon::~SerializeVPolygon(void)
{
}

void SerializeVPolygon::ReadPolygon(std::vector<VPoint2d> &pointPool, 
									std::vector<int> &polygonPoints, 
									const VStr file)
{
	int size;
	VChr tmp;
	VIFStream mystream(file.data());
	VPoint2d myTemp;

	mystream >> size;
	for ( int i = 0; i < size; i++)
	{
		mystream >> myTemp[0] 
				 >> tmp
				 >> myTemp[1];
		
		pointPool.push_back(myTemp);
	}

	int myTemp2;
	mystream >> size;
	for ( int i = 0; i < size; i++)
	{
		mystream >> myTemp2;		
		polygonPoints.push_back(myTemp2);
	}

}

void SerializeVPolygon::WritePolygon(const std::vector<VPoint2d> &pointPool, 
									 const std::vector<int> &polygonPoints, 
									 const VStr file)
{
	VofStream mystream(file.data());

	std::vector<VPoint2d>::const_iterator b;
	std::vector<VPoint2d>::const_iterator e;

	b = pointPool.begin();
	e = pointPool.end();	
	mystream << pointPool.size() << std::endl;
	for ( b; b != e; b++)
	{
		mystream << b->x()
				 << (",")
				 << b->y()
				 << std::endl;
	}
	
	std::vector<int>::const_iterator i;
	std::vector<int>::const_iterator w;

	i = polygonPoints.begin();
	w = polygonPoints.end();
	mystream << polygonPoints.size() << std::endl;
	for ( i; i != w; i++)
	{
		mystream << (*i)
				 << std::endl;
	}

	mystream.close();
}
