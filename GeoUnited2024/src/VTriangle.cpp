// VTriangle.cpp

#include "stdafx.h"
#include "VTriangle.h"
#include "TransformationMatrix.h"

VTriangle::VTriangle()
{

}

VTriangle::VTriangle (const VPoint &c1
					  ,const VPoint &c2
					  ,const VPoint &c3)
{
	corner[0] = c1;
	corner[1] = c2;
	corner[2] = c3;
}

VTriangle::VTriangle (const VTriangle &toCopy)
{
	corner[0] = toCopy.corner[0];
	corner[1] = toCopy.corner[1];
	corner[2] = toCopy.corner[2];
}

VTriangle & VTriangle::operator = (const VTriangle &toCopy)
{
	corner[0] = toCopy.corner[0];
	corner[1] = toCopy.corner[1];
	corner[2] = toCopy.corner[2];
	return *this;
}

sVector<double,3> VTriangle::GetMin()const
{
	sVector<double,3> tmp(MAX_NUMBER_LIMIT);
	tmp.ReplaceMin (corner[0].GetMin());
	tmp.ReplaceMin (corner[1].GetMin());
	tmp.ReplaceMin (corner[2].GetMin());
	return tmp;
}
sVector<double,3> VTriangle::GetMax()const
{
	sVector<double,3> tmp (MIN_NUMBER_LIMIT);
	tmp.ReplaceMax(corner[0].GetMax());
	tmp.ReplaceMax(corner[1].GetMax());
	tmp.ReplaceMax(corner[2].GetMax());
	return tmp;
}

void VTriangle::Transform(const TransformationMatrix &M)
{
	corner[0] = M.Transform (corner[0]);
	corner[1] = M.Transform (corner[1]);
	corner[2] = M.Transform (corner[2]);
}

const VPoint & VTriangle::GetCorner(int i)const
{
	if (i < 0 || i > 2)
		throw VException("VTriangle::GetCorner exception , corner index out of range!",1);
	return corner[i];
}
const VPoint & VTriangle::corner1()const
{
	return corner[0];
}
const VPoint & VTriangle::corner2()const
{
	return corner[1];
}
const VPoint & VTriangle::corner3()const
{
	return corner[2];
}
VPoint & VTriangle::corner1()
{
	return corner[0];
}
VPoint & VTriangle::corner2()
{
	return corner[1];
}
VPoint & VTriangle::corner3()
{
	return corner[2];
}