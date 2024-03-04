// MultiPoints.cpp

#include "stdafx.h"
#include "MultiPoints.h"

MultiPoints::MultiPoints (const std::vector <const Points*> &pointsVec)
:MultiGV<VPoint,Points> (pointsVec)
{

}
MultiPoints::MultiPoints (int totalSize , const std::vector <const Points *> &pointsVec)
:MultiGV<VPoint,Points> (totalSize , pointsVec)
{

}
MultiPoints::MultiPoints (const MultiPoints & toCopy)
:MultiGV<VPoint,Points> (toCopy)
{

}

MultiPoints & MultiPoints::operator = (const MultiPoints &toCopy)
{
	if (this != &toCopy)
	{
		MultiGV<VPoint,Points>::operator = (toCopy);
	}
	return *this;
}
