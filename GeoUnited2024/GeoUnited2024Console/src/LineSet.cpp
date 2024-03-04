/******************************************************************************
*               File: LineSet.cpp											  *
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

#include "LineSet.h"
#include "DoubleList.h"
#include "TransformationMatrix.h"
#include "VectorSerializer.h"
#include "SequentialLineSet.h"
#include "LineSet2d.h"
#include "VPlane.h"

LineSet::LineSet()
:PrimitiveGeoVec<VLine,3>(VGlobalValues::DEFAULT_INITIAL_SIZE)
{
}

LineSet::LineSet(int initialcapacity)
:PrimitiveGeoVec<VLine,3>(initialcapacity)
{
}

//#############################################################################

LineSet::LineSet ( DoubleList &mylist )
:PrimitiveGeoVec<VLine,3> ((int)(mylist.GetCount()/ 6 + 2))
{
	position pos = mylist.GetHeadPosition ();
	vreal p1[3] , p2[3];
	bool chk;

	while ( pos!=mylist.End() )
	{
		chk = mylist.GetPointAtPos ( p1 , pos );
		if ( !chk )
			return;//Exception

		chk = mylist.GetPointAtPos ( p2 , pos );
		if ( !chk )
			return;// Exception
		
		AddLine ( p1 , p2 );
	}
}

LineSet::LineSet ( const LineSet & Foo )
:PrimitiveGeoVec<VLine,3>(Foo)
{
}

//#############################################################################

LineSet::LineSet (const std::vector<VLine> &std_vec)
:PrimitiveGeoVec<VLine,3>(std_vec)
{
	
}

LineSet::LineSet (const LineSet2d &ls , const vreal z)
: PrimitiveGeoVec<VLine,3> (ls.Size())
{
	int mysize = ls.Size();
	for (int i=0;i<mysize;++i)
	{
		const VLine2d *l = ls[i];
		VLine myl3d (VPoint (l->GetP1().x(),l->GetP1().y(),z)
					,VPoint(l->GetP2().x(),l->GetP2().y(),z));
		PushBack (myl3d);
	}
}

LineSet::LineSet (const SequentialLineSet &v)
: PrimitiveGeoVec<VLine,3>(v.Size())
{
	int mysize = v.Size();
	for (int i=0;i<mysize;++i)
	{
		VLine currl = v[i];
		PushBack (currl);
	}
}

LineSet& LineSet::operator = (const std::vector<VLine> &std_vec)
{
	PrimitiveGeoVec<VLine,3>::operator =(std_vec);
	return *this;
}

LineSet::~LineSet()
{
}

//#############################################################################

//void LineSet::UpdateMinMax()
//{
//	m_min=GETMAXPOINT();
//	m_max=GETMINPOINT();
//
//	for ( long i = 0; i < Size(); i++ )
//	{
//		UpdateMinMaxForLine(*(*this)[i]);
//	}
//}

//#############################################################################

//void LineSet::UpdateMinMaxForLine ( const VLine & newline )
//{
//	m_min.ReplaceMin ( newline.GetP1 () );
//	m_min.ReplaceMin ( newline.GetP2 () );
//	
//	m_max.ReplaceMax ( newline.GetP1() );
//	m_max.ReplaceMax ( newline.GetP2() );
//}

//#############################################################################

//long LineSet::Size() const
//{
//	return (long)( lines.size());
//}

//#############################################################################

void LineSet::AddLine ( const VLine & newline )
{
	PrimitiveGeoVec<VLine,3>::PushBack (newline);
}

//#############################################################################

void LineSet::AddLine ( const VPoint &p1 , const VPoint &p2 )
{
	VLine newline ( p1 , p2 );
	PrimitiveGeoVec<VLine,3>::PushBack (newline);
}

//#############################################################################

void LineSet::AddLine ( const vreal *p1 , const vreal *p2 )
{
	VLine newline ( p1 , p2 );
	PrimitiveGeoVec<VLine,3>::PushBack (newline);
}

//#############################################################################

//const VPoint & LineSet::GetMin () const
//{
//	return m_min;
//}

//#############################################################################

//const VPoint & LineSet::GetMax () const 
//{
//	return m_max;
//}

//#############################################################################

//void LineSet::Clear()
//{
//	lines.clear();
//}

//#############################################################################

VLine * LineSet::operator [] ( const long &index )
{
	return &(PrimitiveGeoVec<VLine,3>::At(index));
}

//#############################################################################

const VLine * LineSet::operator [] (const long &index)const
{
	return &(PrimitiveGeoVec<VLine,3>::At(index));
}

//#############################################################################

void LineSet::Serialize (Bofstream &outfile)const
{
	int currversion=3;
	outfile.write ((VChr*)&currversion,sizeof(int));
	//if ( currversion==2)
	//{
	//	m_min.Serialize(outfile);
	//	m_max.Serialize(outfile);
	//	//lines.Serialize(outfile);
	//	SerializeTVec(outfile,lines);
	//}
	//else 
	if (currversion == 3)
	{
		PrimitiveGeoVec<VLine,3>::Serialize(outfile);
	}
	else
	{
		throw VException(( IDS_WroVerNum),4);
	}
}

void LineSet::Serialize (Bifstream &infile)
{
	int currversion;
	infile.read((VChr*)&currversion,sizeof(int));
	
	if(currversion==2)
	{	
		VPoint mymin , mymax;
		mymin.Serialize(infile);
		mymax.Serialize(infile);
		VectorSerializer::SerializeTVec(infile,m_vec);
		m_aabb.Clear();
		UpdateAABB (mymin , m_aabb);
		UpdateAABB (mymax , m_aabb);
	}
	else if (currversion==1)
	{
		VPoint mymin,mymax;
		mymin.Serialize (infile);
		mymax.Serialize (infile);
		//lines.Serialize (infile);
		VectorSerializer::SerializeTVec001(infile,m_vec);
		m_aabb.Clear();
		UpdateAABB (mymin , m_aabb);
		UpdateAABB (mymax , m_aabb);
	}
	else if (currversion == 3)
	{
		PrimitiveGeoVec<VLine,3>::Serialize (infile);
	}
	else
	{
		throw VException(( IDS_WroVerNum),5);
	}
}

LineSet & LineSet::operator =(const LineSet &toCopy)
{
	PrimitiveGeoVec<VLine,3>::operator = (toCopy);
	return *this;
}

void LineSet::Transform(const TransformationMatrix &tm)
{
	m_aabb.Clear();
	for ( int i=0; i < Size(); i++ )
	{
		VLine * currline = (*this)[i];
		*currline = tm.Transform ( *currline );
		UpdateAABB (*currline , m_aabb);		
	}
}

//bool LineSet::operator == (const LineSet &toCompare)const
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

//bool LineSet::operator != (const LineSet &toCompare)const
//{
//	return !(this->operator ==(toCompare));
//}

void LineSet::ReverseOrder ()  // the last line will be the first and first will be last , for 4ax wire toolpath
{
	int mysize = Size();
	LineSet reversed (mysize);
	for (int i=mysize - 1; i>=0; --i)
	{
		const VLine *l = this->operator [] (i);
		reversed.AddLine (*l);
	}
	*this = reversed;
}

VPoint LineSet::GetMinPointInCs (const CoordinateSystem &cs)const
{
	VPoint mn (MAX_NUMBER_LIMIT);
	int mysize = Size();
	for (int i=mysize - 1; i>=0; --i)
	{
		const VLine *l = this->operator [] (i);
		mn.ReplaceMin (cs.AbsoluteCoordToRelativeCoord (l->GetP1()));
		mn.ReplaceMin (cs.AbsoluteCoordToRelativeCoord(l->GetP2()));
	}
	return mn;
}
	
VPoint LineSet::GetMaxPointInCs (const CoordinateSystem &cs)const
{
	VPoint mx (MIN_NUMBER_LIMIT);
	int mysize = Size();
	for (int i=mysize-1;i>=0;--i)
	{
		const VLine *l = this->operator[] (i);
		mx.ReplaceMax (cs.AbsoluteCoordToRelativeCoord (l->GetP1()));
		mx.ReplaceMax (cs.AbsoluteCoordToRelativeCoord(l->GetP2()));
	}
	return mx;
}

void LineSet::XFillBoundary3D(const CoordinateSystem &cs, std::map<vreal, std::multimap<vreal, VPoint>> &xCoordYCoordPointMap, vreal tol)
{
	// for all lines
	int lsize = Size();
	for (int li = 0; li<lsize; ++li)
	{
		const VLine *curl = this->operator[](li);
		VLine tline(cs.AbsoluteCoordToRelativeCoord(curl->GetP1())
			, cs.AbsoluteCoordToRelativeCoord(curl->GetP2()));
		VPoint lmin = tline.GetMin();
		VPoint lmax = tline.GetMax();
		std::map<vreal, std::multimap<vreal, VPoint>>::const_iterator xi = xCoordYCoordPointMap.lower_bound(lmin.x());//xCoordYCoordPointMap.begin();
		std::map<vreal, std::multimap<vreal, VPoint>>::const_iterator xe = xCoordYCoordPointMap.end();
		for (xi; xi != xe; ++xi)
		{
			if (xi->first >= lmin.x() - tol && xi->first <= lmax.x() + tol)
			{
				VPoint origin(lmin);
				origin.x(xi->first);
				VPlane pln(origin, UX);
				vreal intt;
				VPlane::IntType ck = pln.Int(tline, intt);
				if (ck == VPlane::REGULAR)
				{
					if (intt > -EPSILON && intt < 1 + EPSILON)
					{
						VPoint ipoint = tline.GetP(intt);
						std::multimap<vreal, VPoint> &curmap = xCoordYCoordPointMap[xi->first];
						curmap.insert(std::pair<vreal, VPoint>(ipoint.y(), ipoint));
					}
				}
			}
			else
				break;
		}
	}
}
void LineSet::YFillBoundary3D(const CoordinateSystem &cs, std::map<vreal, std::multimap<vreal, VPoint>> &yCoordXCoordPointMap, vreal tol)
{
	// for all lines
	int lsize = Size();
	for (int li = 0; li<lsize; ++li)
	{
		const VLine *curl = this->operator[](li);
		VLine tline(cs.AbsoluteCoordToRelativeCoord(curl->GetP1())
			, cs.AbsoluteCoordToRelativeCoord(curl->GetP2()));
		VPoint lmin = tline.GetMin();
		VPoint lmax = tline.GetMax();
		std::map<vreal, std::multimap<vreal, VPoint>>::const_iterator yi = yCoordXCoordPointMap.lower_bound(lmin.y());//yCoordXCoordPointMap.begin();
		std::map<vreal, std::multimap<vreal, VPoint>>::const_iterator ye = yCoordXCoordPointMap.end();
		for (yi; yi != ye; ++yi)
		{
			if (yi->first >= lmin.y() - tol && yi->first <= lmax.y() + tol)
			{
				VPoint origin(lmin);
				origin.y(yi->first);
				VPlane pln(origin, UY);
				vreal intt;
				VPlane::IntType ck = pln.Int(tline, intt);
				if (ck == VPlane::REGULAR)
				{
					if (intt > -EPSILON && intt < 1 + EPSILON)
					{
						VPoint ipoint = tline.GetP(intt);
						std::multimap<vreal, VPoint> &curmap = yCoordXCoordPointMap[yi->first];
						curmap.insert(std::pair<vreal, VPoint>(ipoint.x(), ipoint));
					}
				}
			}
			else
				break;
		}
	}
}