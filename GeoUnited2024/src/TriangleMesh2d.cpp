/******************************************************************************
*               File: TriangleMesh2d.cpp										  *
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
#include "BasicDefinitions.h"
#include "TriangleMesh2d.h"
#include "DoubleList.h"
#include "VLine.h"


TriangleMesh2d::TriangleMesh2d ()
{
	tri.reserve(VGlobalValues::DEFAULT_INITIAL_SIZE);
	m_min=GETMAXPOINT2D();
	m_max=GETMINPOINT2D();
}

//#############################################################################

TriangleMesh2d::TriangleMesh2d ( const int &initialsize )
{
	tri.reserve (initialsize);
	m_min=GETMAXPOINT2D();
	m_max=GETMINPOINT2D();
}

//#############################################################################

TriangleMesh2d:: TriangleMesh2d ( DoubleList & mylist , unsigned int normalstate )
{
	tri.reserve(VGlobalValues::DEFAULT_INITIAL_SIZE);
	m_min=GETMAXPOINT2D();
	m_max=GETMINPOINT2D();

	position pos = mylist.GetHeadPosition();
	VPoint2d cor1 , cor2 , cor3;
	vreal mypoint[2];

	while ( pos != mylist.End() )
	{
		bool chk = mylist.GetPoint2dAtPos ( mypoint , pos );

		if ( !chk )
			break;
			// throw exception
		
		chk = mylist.GetPoint2dAtPos ( mypoint , pos );
		if ( !chk )
			break;
		
		cor1.x( mypoint[0] );
		cor1.y( mypoint[1] );
		
		chk = mylist.GetPoint2dAtPos ( mypoint , pos );
		if ( !chk )
			break;

		cor2.x( mypoint[0] );
		cor2.y( mypoint[1] );

		chk = mylist.GetPoint2dAtPos ( mypoint , pos );
		if ( !chk )
			break;
		
		cor3.x( mypoint[0] );
		cor3.y( mypoint[1] );

		AddTriangle ( cor1 , cor2 , cor3 );	
	}
}
		
//#############################################################################

TriangleMesh2d::TriangleMesh2d ( const TriangleMesh2d & Foo )
:tri(Foo.tri)
{
	m_min=Foo.m_min;
	m_max=Foo.m_max;
}

//#############################################################################

TriangleMesh2d::~TriangleMesh2d()
{
}

//#############################################################################

bool TriangleMesh2d::AddTriangle ( const VPoint2d &corner1 , const VPoint2d &corner2 , const VPoint2d &corner3 )
{
	VTriangle2dE newtri ( corner1 , corner2 , corner3 );
	return AddTriangle (newtri);
}

//#############################################################################

long TriangleMesh2d::Size() const
{
	//return tri.Size();
	return (long)tri.size();
}

//#############################################################################

void TriangleMesh2d::UpdateMinMax ()
{
	m_min=GETMAXPOINT2D();
	m_max=GETMINPOINT2D();

	for ( long i = 0; i < Size(); i++ )
	{
		VTriangle2dE *currtri = (*this)[i];
		m_min.ReplaceMin (currtri->GetCorner1());
		m_min.ReplaceMin (currtri->GetCorner2());
		m_min.ReplaceMin (currtri->GetCorner3());

		m_max.ReplaceMax (currtri->GetCorner1());
		m_max.ReplaceMax (currtri->GetCorner2());
		m_max.ReplaceMax (currtri->GetCorner3());
	}

}

//#############################################################################

void TriangleMesh2d::UpdateMinMaxForSingleTri ( const VTriangle2dE &newtri )
{
	m_min.ReplaceMin (newtri.GetCorner1());
	m_min.ReplaceMin (newtri.GetCorner2());
	m_min.ReplaceMin (newtri.GetCorner3());

	m_max.ReplaceMax (newtri.GetCorner1());
	m_max.ReplaceMax (newtri.GetCorner2());
	m_max.ReplaceMax (newtri.GetCorner3());
}

//#############################################################################

const VPoint2d & TriangleMesh2d::GetMin() const
{
	return m_min;
}

//#############################################################################

const VPoint2d & TriangleMesh2d::GetMax() const
{
	return m_max;
}

//#############################################################################

void TriangleMesh2d::Clear()
{
	//tri.Clear();
	tri.clear();
}

//#############################################################################

VTriangle2dE * TriangleMesh2d::operator [] ( const long &index ) const
{
	if ( index >= Size() || index < 0 )
		return NULL;

	//return tri[index];
	return (VTriangle2dE *)&tri[index];
}

//#############################################################################

bool TriangleMesh2d::AddTriangle ( const VTriangle2dE &newtri )
{
	if ( newtri.IsCorrupted() == true )
	{
		#if _DEBUG	
			return false;	
		//throw VException ( ("corrupted triangle given to mesh") , 1 );
		#else
			return false;
		#endif		
	}
	
	//tri.AddItem ( newtri );
	tri.push_back(newtri);
	UpdateMinMaxForSingleTri(newtri);
	return true;	
}

//#############################################################################

TriangleMesh2d & TriangleMesh2d::operator = (const TriangleMesh2d &toCopy)
{
	m_min = toCopy.m_min;
	m_max = toCopy.m_max;
	tri = toCopy.tri;
	return *this;
}

bool TriangleMesh2d::operator == (const TriangleMesh2d &toCompare)const
{
	if(this==&toCompare)
		return true;
	
	if(m_min!=toCompare.m_min)
		return false;
	if(m_max!=toCompare.m_max)
		return false;
	if(tri!=toCompare.tri)
		return false;
	
	return true;
}

bool TriangleMesh2d::operator != (const TriangleMesh2d &toCompare)const
{
	return !(this->operator ==(toCompare));
}
