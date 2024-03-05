/******************************************************************************
*               File: PointSet.cpp											  *
*******************************************************************************
*               Description:                                                  *
*                                                                             *
*******************************************************************************
*               History:                                                      *
*  02.07.2003 18:23:08 Created by:    Ata Hurdogan Demiray                    *
*******************************************************************************
*               (C) 2003 by Pozitif GmbH                                     *
******************************************************************************/

#include "stdafx.h"
#include "PointSet.h"
#include "DoubleList.h"

#include "TransformationMatrix.h"
#include "VectorSerializer.h"
#include "PosCubes.h"

PointSet::PointSet()
	:PrimitiveGeoVec(2)
{

}

PointSet::PointSet( const long &initialcapacity )
:PrimitiveGeoVec<VPoint,3>(initialcapacity)
{

}

//#############################################################################

PointSet::PointSet ( const long &initialcapacity , DoubleList &mylist )
:PrimitiveGeoVec<VPoint,3> (initialcapacity)
{
	vreal mypoint[3];
	position pos = mylist.GetHeadPosition();
	bool chk = mylist.GetPointAtPos ( mypoint , pos );

	while ( chk )
	{
		InsertPoint ( mypoint );
		chk = mylist.GetPointAtPos ( mypoint , pos );
	}

}

//#############################################################################

PointSet::PointSet ( const PointSet & Foo )
:PrimitiveGeoVec<VPoint,3> (Foo)
{
}

//#############################################################################

PointSet::PointSet (const std::vector<VPoint> &pvec)
:PrimitiveGeoVec<VPoint,3> (pvec)
{
}

//#############################################################################

PointSet::~PointSet()
{
}

//#############################################################################

void PointSet::InsertPoint ( const VPoint & newpoint )
{
	//points.push_back(newpoint);
	//UpdateMinMaxForSinglePoint ( newpoint );
	PrimitiveGeoVec <VPoint,3>::PushBack (newpoint);
}

//#############################################################################

void PointSet::InsertPoint ( const vreal *coord )
{
	VPoint newpoint ( coord );
	//points.push_back(newpoint);
	//UpdateMinMaxForSinglePoint ( newpoint );
	PrimitiveGeoVec<VPoint,3>::PushBack (newpoint);
}

//#############################################################################

const VPoint * PointSet::operator [] (const long &index)const
{
	return &(PrimitiveGeoVec<VPoint,3>::At(index));
}

//#############################################################################

VPoint * PointSet::operator [] ( const long &index )
{
	//if ( index >= Size() || index < 0 )
	//	throw(VException(("pointset invalid index"),0));

	//return (VPoint*)(&(points[index]));
	return &(PrimitiveGeoVec<VPoint,3>::At(index));
}

//#############################################################################

bool PointSet::GetIndex ( const VPoint &p , int & index , const vreal tol) const
{
	for ( int i=0; i < Size(); i++ )
	{
		const VPoint * cp = (*this)[i];
		if (  p.Compare (*cp,tol) )
		{
			index = i;
			return true;
		}
	}
	
	index = -1;
	return false;
}

//#############################################################################

void PointSet::Transform ( const TransformationMatrix & tm )
{
	m_aabb.Clear();
	for ( int i=0; i < Size(); i++ )
	{
		VPoint * currpoint = (*this)[i];
		VPoint transformed = tm.Transform ( *currpoint );
		*currpoint = transformed;
		UpdateAABB (*currpoint , m_aabb);
	}
}

void PointSet::TransformAsVectors (const TransformationMatrix &tm)
{
	m_aabb.Clear();
	for (int i=0; i < Size(); i++)
	{
		VPoint *currpoint = (*this)[i];
		VPoint transformed = tm.TransformVec ( *currpoint );
		*currpoint = transformed;
		UpdateAABB (*currpoint , m_aabb);
	}
}

void PointSet::TransformAsUnitVectors(const TransformationMatrix &tm)
{
	m_aabb.Clear();
	for (int i = 0; i < Size(); i++)
	{
		VPoint *currpoint = (*this)[i];
		VPoint transformed = tm.TransformVec(*currpoint);
		transformed.NormThis();
		*currpoint = transformed;
		UpdateAABB(*currpoint, m_aabb);
	}
}

void PointSet::Serialize (Bofstream &binfile)const
{
	int currversion =3;
	binfile.write((VChr*)&currversion,sizeof(int));
//	if (currversion==2)
//	{
//		SerializeTVec(binfile,points);
//		m_min.Serialize (binfile);
//		m_max.Serialize (binfile);
//	}
//	else
//	{
//		throw VException(("wrong version number"),1);
//	}
	if (currversion == 3)
	{
		PrimitiveGeoVec<VPoint,3>::Serialize (binfile);
	}
}

void PointSet::Serialize (Bifstream &binfile)
{
	int currversion;
	binfile.read ((VChr*)&currversion,sizeof(int));
	if (currversion==1)
	{
		VectorSerializer::SerializeTVec001(binfile,PrimitiveGeoVec<VPoint,3>::m_vec);
		VPoint mymin;
		VPoint mymax;
		mymin.Serialize(binfile);
		mymax.Serialize(binfile);
		m_aabb.Clear();
		UpdateAABB (mymin , m_aabb);
		UpdateAABB (mymax , m_aabb);
	}
	else if (currversion==2)
	{
		VectorSerializer::SerializeTVec(binfile,PrimitiveGeoVec<VPoint,3>::m_vec);
		VPoint mymin;
		VPoint mymax;
		mymin.Serialize(binfile);
		mymax.Serialize(binfile);
		m_aabb.Clear();
		UpdateAABB (mymin , m_aabb);
		UpdateAABB (mymax , m_aabb);
	}
	else if (currversion==3)
	{
		PrimitiveGeoVec<VPoint,3>::Serialize(binfile);	
	}
	else
	{
		throw VException(( IDS_WroVerNum),1);
	}
}

PointSet & PointSet::operator = (const PointSet &toCopy)
{
	/*m_min = toCopy.m_min;
	m_max = toCopy.m_max;
	points = toCopy.points;*/
	PrimitiveGeoVec <VPoint,3>::operator = (toCopy);
	return *this;
}

void PointSet::RemoveDuplicate (std::vector<int> &removed)
{
	PosCubes cubes (*this);
	vreal comptol = 1.e-8;
	int vertexno = Size();
	PointSet tmp (vertexno);

	for ( int i=0; i<vertexno; i++ )
	{	
		VPoint *currp = (*this)[i];
		LongVec relind , nii;
		cubes.SetRelatedIndiceListForPoint (*currp , relind , nii);
		bool coinc = false;
		int mysize = relind.Size();
		for (int j=0;j<mysize;j++)
		{
			int currind = relind[j];
			if (currind >= i)//deal only with added points
				continue;
			VPoint *p = (*this)[currind];
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
void PointSet::AddVecBack(PointSet *ps)
{
	m_vec.insert(m_vec.end(), ps->GetVec().begin(), ps->GetVec().end());
}
