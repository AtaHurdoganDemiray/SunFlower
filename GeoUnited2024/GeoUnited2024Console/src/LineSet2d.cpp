/******************************************************************************
*               File: LineSet2d.cpp											  *
*******************************************************************************
*               Description:                                                  *
*                                                                             *
*******************************************************************************
*               History:                                                      *
*  02.07.2003 18:29:09 Created by:    Ata Hurdogan Demiray                    *
*******************************************************************************
*               (C) 2003 by Pozitif GmbH                                      *
******************************************************************************/

#include "stdafx.h"
#include "LineSet2d.h"
#include "DoubleList.h"
#include "VectorSerializer.h"
#include "SequentialLineSet2d.h"
#include "VLine2dE.h"

LineSet2d::LineSet2d()
:PrimitiveGeoVec<VLine2d , 2>()
{
	//lines.reserve( VGlobalValues::DEFAULT_INITIAL_SIZE );
	//m_min=GETMAXPOINT2D();
	//m_max=GETMINPOINT2D();
}

LineSet2d::LineSet2d(int initialcapacity)
:PrimitiveGeoVec<VLine2d , 2>(initialcapacity)
{
	//lines.reserve(initialcapacity);
	//m_min=GETMAXPOINT2D();
	//m_max=GETMINPOINT2D();

}

//#############################################################################

LineSet2d::LineSet2d ( DoubleList &mylist )
:PrimitiveGeoVec<VLine2d , 2> (mylist.GetCount())
{
	//lines.reserve( (int)((mylist.GetCount() / 6)+2) );
//	m_min=GETMAXPOINT2D();
//	m_max=GETMINPOINT2D();

	position pos = mylist.GetHeadPosition ();
	vreal p1[2] , p2[2];
	bool chk;

	while ( pos!=mylist.End() )
	{
		chk = mylist.GetPoint2dAtPos ( p1 , pos );
		if ( !chk )
			return;//Exception

		chk = mylist.GetPoint2dAtPos ( p2 , pos );
		if ( !chk )
			return;// Exception
		
		VLine2d tmp (p1 , p2);
		PrimitiveGeoVec<VLine2d , 2>::PushBack (tmp);
	}
}

LineSet2d::LineSet2d ( const LineSet2d & Foo )
: PrimitiveGeoVec<VLine2d , 2> (Foo)
//:lines ( Foo.lines )
{
	//m_min=Foo.m_min;
	//m_max=Foo.m_max;
}


LineSet2d::LineSet2d (const std::vector<VLine2d> &v)
: PrimitiveGeoVec<VLine2d , 2> (v)
{

}

LineSet2d::LineSet2d (const SequentialLineSet2d &v)
: PrimitiveGeoVec<VLine2d,2> (v.Size())
{
	int mysize = v.Size();
	for (int i=0;i<mysize;++i)
	{
		VLine2d currl = v[i];
		PushBack (currl);
	}
}

//#############################################################################

LineSet2d::LineSet2d (const std::vector<VLine2dE> &v)
: PrimitiveGeoVec<VLine2d,2> ((int)v.size())
{
	int mysize = (int)v.size();
	for (int i=0;i<mysize;++i)
	{
		const VLine2dE &cl = v[i];
		PushBack (cl.L());
	}
}

//#############################################################################

LineSet2d::~LineSet2d()
{
}

//#############################################################################

//void LineSet2d::UpdateMinMax()
//{
//	m_min=GETMAXPOINT2D();
//	m_max=GETMINPOINT2D();
//
//	for ( long i = 0; i < Size(); i++ )
//	{
//		UpdateMinMaxForLine(*(*this)[i]);
//	}
//}

//#############################################################################
	
//void LineSet2d::UpdateMinMaxForLine ( const VLine2d & newline )
//{
//	m_min.ReplaceMin ( newline.GetP1 () );
//	m_min.ReplaceMin ( newline.GetP2 () );
//	
//	m_max.ReplaceMax ( newline.GetP1() );
//	m_max.ReplaceMax ( newline.GetP2() );
//}

//#############################################################################

//long LineSet2d::Size() const
//{
//	return (long)( lines.size());
//}

//#############################################################################

void LineSet2d::AddLine ( const VLine2d &newline )
{
	//lines.push_back (newline);
	//UpdateMinMaxForLine ( newline );
	PrimitiveGeoVec<VLine2d , 2>::PushBack (newline);
}

//#############################################################################

void LineSet2d::AddLine ( const VPoint2d &p1 , const VPoint2d &p2 )
{
	VLine2d newline (p1,p2);
	//AddLine(newline);
	PrimitiveGeoVec<VLine2d , 2>::PushBack (newline);
}

//#############################################################################

void LineSet2d::AddLine ( const vreal *p1 , const vreal *p2 )
{
	VLine2d newline (p1,p2);
	//AddLine(newline);
	PrimitiveGeoVec<VLine2d , 2>::PushBack (newline);
}

//#############################################################################

//VPoint2d LineSet2d::GetMin () const
//{
//	return m_min;
//}

//#############################################################################

//VPoint2d LineSet2d::GetMax () const
//{
//	return m_max;
//}

//#############################################################################

//void LineSet2d::Clear()
//{
//	lines.clear();
//}

//#############################################################################

VLine2d * LineSet2d::operator [] ( const long &index )
{
	//if ( index < 0 || index >= Size() )
	///	return NULL;

	//return (VLine2d*)(&lines[index]);
	return &(PrimitiveGeoVec<VLine2d , 2>::At (index));
}

//#############################################################################

//void Line
const VLine2d * LineSet2d::operator [] ( const long &index ) const
{
	//if ( index < 0 || index >= Size() )
	///	return NULL;

	//return (VLine2d*)(&lines[index]);
	return &(PrimitiveGeoVec<VLine2d , 2>::At (index));
}

//#############################################################################

//void LineSet2d::Serialize (Bofstream &outfile)const
//{
//	int currversion=1;
//	outfile.write((VChr*)&currversion,sizeof(int));
//	
//	if ( currversion==1)
//	{
//		VPoint2d mymin (GetMin());
//		VPoint2d mymax (GetMax());
//		mymin.Serialize(outfile);
//		mymax.Serialize(outfile);
//		//lines.Serialize(outfile);
//		SerializeTVec(outfile,m_vec);
//	}
//	else
//	{
//		throw VException(("wrong version number"),6);
//	}
//}

//void LineSet2d::Serialize (Bifstream &infile)
//{
//	int currversion=1;
//	infile.read((VChr*)&currversion,sizeof(int));
//	
//	if (currversion==1)
//	{
//		VPoint2d mymin (GetMin());
//		VPoint2d mymax (GetMax());
//		mymin.Serialize(infile);
//		mymax.Serialize(infile);
//		//lines.Serialize(infile);
//		SerializeTVec(infile,m_vec);
//	}
//	else
//	{
//		throw VException (("wrong version number"),7);
//	}
//}

LineSet2d & LineSet2d::operator = (const LineSet2d &toCopy)
{
	//m_min = toCopy.m_min;
	//m_max = toCopy.m_max;
	//lines = toCopy.lines;
	PrimitiveGeoVec<VLine2d , 2>::operator = (toCopy);
	return *this;
}

//bool LineSet2d::operator == (const LineSet2d &toCompare)const
//{
//	if(this==&toCompare)
//		return true;
//	if(lines.size()!=toCompare.lines.size())
//		return false;
//
//	if(m_min!=toCompare.m_min)
//		return false;
//	if(m_max!=toCompare.m_max)
//		return false;
//	if(lines!=toCompare.lines)
//		return false;
//	
//	return true;
//}

//bool LineSet2d::operator != (const LineSet2d &toCompare)const
//{
//	return !(this->operator ==(toCompare));
//}

void LineSet2d::AddBack(const SequentialLineSet2d &sl)
{
	int slsize = sl.Size();
	for (int i=0;i<slsize;++i)
	{
		VLine2d l=sl[i];
		AddLine (l);
	}
}
