// VTriangle2d.cpp

#include "stdafx.h"
#include "VTriangle2d.h"


VTriangle2d::VTriangle2d ()
:corner1(0.),corner2(0.),corner3(0.)
{

}

VTriangle2d::VTriangle2d (const VPoint2d &cor1 , const VPoint2d &cor2 , const VPoint2d &cor3)
:corner1(cor1) , corner2(cor2) , corner3(cor3)
{

}

VTriangle2d::VTriangle2d (const vreal cor1[] , const vreal cor2[] , const vreal cor3[])
:corner1(cor1) , corner2(cor2) , corner3(cor3)
{
		
}

VTriangle2d::VTriangle2d (const VTriangle2d &toCopy)
:corner1(toCopy.corner1),corner2(toCopy.corner2),corner3(toCopy.corner3)
{
	
}

VTriangle2d & VTriangle2d::operator = (const VTriangle2d &toCopy)
{
	corner1 = toCopy.corner1;
	corner2 = toCopy.corner2;
	corner3 = toCopy.corner3; 
	return *this;
}

const VPoint2d & VTriangle2d::GetCorner1()const
{
	return corner1;
}

const VPoint2d & VTriangle2d::GetCorner2()const
{
	return corner2;
}

const VPoint2d& VTriangle2d::GetCorner3()const
{
	return corner3;
}

VPoint2d & VTriangle2d::GetCorner1()
{
	return corner1;
}

VPoint2d & VTriangle2d::GetCorner2()
{
	return corner2;
}

VPoint2d & VTriangle2d::GetCorner3()
{
	return corner3;
}

bool VTriangle2d::operator == (const VTriangle2d &toCompare)const
{
	if (this == &toCompare)
	{
		return true;
	}

	if(corner1 != toCompare.corner1)
		return false;

	if (corner2 != toCompare.corner2)
		return false;

	if (corner3 != toCompare.corner3)
		return false;

	return true;
}

bool VTriangle2d::operator != (const VTriangle2d &toCompare)const
{
	return ! operator == (toCompare);
}

void VTriangle2d::Serialize (Bofstream &outfile)const 
{
	//int version = 1;
	//outfile.write ((VChr *) &version , sizeof(int));
	//if(version == 1)
	{
		corner1.Serialize(outfile);
		corner2.Serialize(outfile);
		corner3.Serialize(outfile);
	}
	//else
	//	throw VException (("VTriangle2d exception"),1);

}

void VTriangle2d::Serialize (Bifstream &infile)
{
	//int version;
	//infile.read ((VChr *)&version , sizeof (int));
	//if (version == 1)
	{
		corner1.Serialize(infile);
		corner2.Serialize(infile);
		corner3.Serialize(infile);
	}
	//else 
	//	throw VException (("VTriangle2d serialization exception"),1);
}

sVector<double,2> VTriangle2d::GetMin()const
{
	sVector<double,2> tmp(MAX_NUMBER_LIMIT);
	tmp.ReplaceMin (corner1.GetMin());
	tmp.ReplaceMin(corner2.GetMin());
	tmp.ReplaceMin(corner3.GetMin());
	return tmp;
}
sVector<double,2> VTriangle2d::GetMax()const
{
	sVector<double,2> tmp(MIN_NUMBER_LIMIT);
	tmp.ReplaceMax(corner1.GetMax());
	tmp.ReplaceMax(corner2.GetMax());
	tmp.ReplaceMax(corner3.GetMax());
	return tmp;
}
