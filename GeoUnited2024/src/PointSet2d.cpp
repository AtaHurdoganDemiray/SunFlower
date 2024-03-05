// PointSet2d.cpp

#include "stdafx.h"
#include "PointSet2d.h"
#include "DoubleList.h"
#include "VectorSerializer.h"

#include "PosCubes2d.h"

PointSet2d::PointSet2d ()
:PrimitiveGeoVec<VPoint2d,2>()
{
}

PointSet2d::PointSet2d( int initialcapacity )
:PrimitiveGeoVec<VPoint2d,2>(initialcapacity)
{
}

//#############################################################################

PointSet2d::PointSet2d ( DoubleList &mylist )
:PrimitiveGeoVec<VPoint2d,2>(static_cast<int>(mylist.GetCount()/3))
{	
	vreal mypoint[2];
	position pos = mylist.GetHeadPosition();

	bool chk = mylist.GetPoint2dAtPos ( mypoint , pos );

	while ( chk )
	{
		InsertPoint ( mypoint );
		chk = mylist.GetPoint2dAtPos ( mypoint , pos );
	}

}

PointSet2d::PointSet2d ( const PointSet2d & Foo )
:PrimitiveGeoVec<VPoint2d,2>(Foo)
{

}

//#############################################################################

PointSet2d::~PointSet2d()
{
}

//#############################################################################

void PointSet2d::InsertPoint ( const VPoint2d & newpoint )
{
	//points.push_back( newpoint );
	//UpdateMinMaxForSinglePoint ( newpoint );
	PrimitiveGeoVec<VPoint2d,2>::PushBack(newpoint);
}

//#############################################################################

void PointSet2d::InsertPoint ( const vreal *coord )
{
	VPoint2d  newpoint ( coord );
	//points.push_back(newpoint);
	//UpdateMinMaxForSinglePoint ( newpoint );
	PrimitiveGeoVec<VPoint2d,2>::PushBack(newpoint);
}

//#############################################################################

const VPoint2d * PointSet2d::operator [] ( const long& index ) const
{
	//if ( index >= Size() || index < 0 )
	//	throw(VException(("pointset2d invalid index"),0));

	//return (VPoint2d*)&points[index];
	return &(PrimitiveGeoVec<VPoint2d,2>::At(index));
}

//#############################################################################

VPoint2d * PointSet2d::operator [] ( const long& index )
{
	//if ( index >= Size() || index < 0 )
	//	throw(VException(("pointset2d invalid index"),0));

	//return (VPoint2d*)&points[index];
	return &(PrimitiveGeoVec<VPoint2d,2>::At(index));
}

//#############################################################################

bool PointSet2d::GetIndex ( const VPoint2d &p , int & index , vreal cointol) const
{
	int mysize=Size();
	for ( int i=0; i < mysize; ++i )
	{
		const VPoint2d * cp = (*this)[i];
		if ( cp->Compare(p,cointol) )
		{
			index = i;
			return true;
		}
	}
	
	index = -1;
	return false;
}

//#############################################################################

PointSet2d & PointSet2d::operator = (const PointSet2d &toCopy)
{
	//m_min = toCopy.m_min;
	//m_max = toCopy.m_max;
	//points = toCopy.points;
	PrimitiveGeoVec<VPoint2d,2>::operator =(toCopy);
	return *this;
}

void PointSet2d::RemoveDuplicate (std::vector<int> &removed)
{
	PosCubes2d cubes (*this);
	vreal comptol = 1.e-8;
	int vertexno = Size();
	PointSet2d tmp (vertexno);

	for ( int i=0; i<vertexno; i++ )
	{	
		VPoint2d *currp = (*this)[i];
		LongVec relind , nii;
		cubes.SetRelatedIndiceListForPoint (*currp , relind , nii);
		bool coinc = false;
		int mysize = relind.Size();
		for (int j=0;j<mysize;j++)
		{
			int currind = relind[j];
			if (currind >= i)//deal only with added points
				continue;
			VPoint2d *p = (*this)[currind];
			if (currp->Compare (*p , comptol))
			{
				coinc = true;
				break;
			}
		}
		if (coinc == false)
		{
			tmp.InsertPoint(*currp);
		}
		else
		{
			removed.push_back(i);
		}
	}
	(*this) = tmp;
}

int PointSet2d::InsertPointIf (const VPoint2d &p , bool &isexist)
{
	int i;
	isexist = GetIndex (p,i,EPSILON);
	if (isexist)
		return i;
	else
	{
		InsertPoint (p);
		return Size()-1;
	}
}
