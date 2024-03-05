/******************************************************************************
*               File: gmQuad2d.cpp											  *
*******************************************************************************
*               Description:                                                  *
*                                                                             *
*******************************************************************************
*               History:                                                      *
*  06.01.2004 11:32:52 Created by:    Ata Hurdogan Demiray                    *
*******************************************************************************
*               (C) 2003 by Positive GmbH                                  *
******************************************************************************/

#include "stdafx.h"
#include "gmQuad2d.h"

//#include "LineIntersection2d.h"
#include "VLine2d.h"



gmQuad2d::gmQuad2d ( const VPoint2d &cor1 , const VPoint2d &cor2 , const VPoint2d &cor3 , const VPoint2d &cor4 )
{
	m_cor1 = cor1;
	m_cor2 = cor2;
	m_cor3 = cor3;
	m_cor4 = cor4;

	m_edge1 = m_cor2 - m_cor1;
	m_edge2 = m_cor3 - m_cor2;
	m_edge3 = m_cor4 - m_cor3;
	m_edge4 = m_cor1 - m_cor4;

	UpdateMinMax();
}

//#############################################################################

gmQuad2d::gmQuad2d (const VPoint2d &mn , const VPoint2d &mx)
{
	m_cor1 = mn;
	m_cor3 = mx;
	m_cor2 = mn;
	m_cor2.x(mx.x());
	m_cor4 = mx;
	m_cor4.x(mn.x());
	m_edge1 = m_cor2 - m_cor1;
	m_edge2 = m_cor3 - m_cor2;
	m_edge3 = m_cor4 - m_cor3;
	m_edge4 = m_cor1 - m_cor4;
	min = mn;
	max = mx;
}

gmQuad2d::gmQuad2d ( const gmQuad2d & Foo )
{
	(*this) = Foo;
}

//#############################################################################

gmQuad2d & gmQuad2d::operator = ( const gmQuad2d & Foo )
{
	m_cor1 = Foo.m_cor1;
	m_cor2 = Foo.m_cor2;	
	m_cor3 = Foo.m_cor3;	
	m_cor4 = Foo.m_cor4;	

	m_edge1 = Foo.m_edge1;
	m_edge2 = Foo.m_edge2;
	m_edge3 = Foo.m_edge3;
	m_edge4 = Foo.m_edge4;
	
	min = Foo.min;
	max = Foo.max;
	
	return (*this);
}

//#############################################################################

gmQuad2d::~gmQuad2d() 
{

}

//#############################################################################

VPoint2d gmQuad2d::GetCor1()
{
	return m_cor1;
}

//#############################################################################

VPoint2d gmQuad2d::GetCor2()
{
	return m_cor2;
}

//#############################################################################

VPoint2d gmQuad2d::GetCor3()
{
	return m_cor3;
}

//#############################################################################

VPoint2d gmQuad2d::GetCor4()
{
	return m_cor4;
}

//#############################################################################

const VPoint2d & gmQuad2d::GetCor1() const
{
	return m_cor1;
}

//#############################################################################

const VPoint2d & gmQuad2d::GetCor2() const
{
	return m_cor2;
}

//#############################################################################

const VPoint2d & gmQuad2d::GetCor3() const
{
	return m_cor3;
}

//#############################################################################

const VPoint2d & gmQuad2d::GetCor4() const
{
	return m_cor4;
}

//#############################################################################

VPoint2d gmQuad2d::GetEdge1()
{
	return m_edge1;
}

//#############################################################################

VPoint2d gmQuad2d::GetEdge2()
{
	return m_edge2;
}

//#############################################################################

VPoint2d gmQuad2d::GetEdge3()
{
	return m_edge3;
}

//#############################################################################

VPoint2d gmQuad2d::GetEdge4()
{
	return m_edge4;
}

//#############################################################################

const VPoint2d & gmQuad2d::GetEdge1() const
{
	return m_edge1;
}

//#############################################################################

const VPoint2d & gmQuad2d::GetEdge2() const
{
	return m_edge2;
}

//#############################################################################

const VPoint2d & gmQuad2d::GetEdge3() const
{
	return m_edge3;
}

//#############################################################################

const VPoint2d & gmQuad2d::GetEdge4() const
{
	return m_edge4;
}

//#############################################################################

VPoint2d gmQuad2d::GetMin()
{
	return min;
}

//#############################################################################

VPoint2d gmQuad2d::GetMax()
{
	return max;
}

//#############################################################################

const VPoint2d & gmQuad2d::GetMin() const
{
	return min;
}

//#############################################################################

const VPoint2d & gmQuad2d::GetMax() const
{
	return max;
}

//#############################################################################

void gmQuad2d::UpdateMinMax()
{
	min = m_cor1;
	min.ReplaceMin ( m_cor2 );
	min.ReplaceMin ( m_cor3 );
	min.ReplaceMin ( m_cor4 );

	max = m_cor1;
	max.ReplaceMax ( m_cor2 );
	max.ReplaceMax ( m_cor3 );
	max.ReplaceMax ( m_cor4 );
}

//#############################################################################

bool gmQuad2d::IsInQuad ( const VPoint2d &apoint ) const
{
	if ( apoint.x() <= max.x() && apoint.x() >= min.x() )
		if ( apoint.y() <= max.y() && apoint.y() >= min.y() )
				return true;

	return false;
}

//void gmQuad2d::EdgeLineIntersection 
//( const VLine2d & l , int edgeno , int & intno , VPoint2d intp[] , vreal sofline[] ) const
//{
//	VLine2d qe;
//	if ( edgeno == 1 )
//	{
//		qe.SetP1( m_cor1 );
//		qe.SetP2( m_cor2 );
//	}
//	else if ( edgeno == 2 )
//	{
//		qe.SetP1( m_cor2 );
//		qe.SetP2( m_cor3 );
//	}
//	else if ( edgeno == 3 )
//	{
//		qe.SetP1( m_cor3 );
//		qe.SetP2( m_cor4 );
//	}
//	else if ( edgeno == 4 )
//	{
//		qe.SetP1( m_cor4 );
//		qe.SetP2( m_cor1 );
//	}
//	else
//		throw ( VException ( ("quad2d edge no out of range") , 1 ) );
//
//	LineIntersection2d calc;
//	LineIntersection2d::LineIntersectionData2d result 
//		= calc.CalcInt ( l , qe );
//
//	for ( int j=0; j<result.intno; j++ )
//	{
//
//		if ( intno+j > 3 )
//			throw ( VException ( ("too many quad intersections") , 0 )); 		
//		
//		intp[intno+j] = result.intpoint[j];
//		sofline[intno+j] = result.sofline1[j];
//	}
//	
//	intno += result.intno;
//
//}

//#############################################################################

//bool gmQuad2d::IsIntersectingWithEdges ( const VLine2d &l , int & intno , VPoint2d intp[] , vreal sofline[] ) const
//{
//	intno = 0;
//	for ( int i=1; i<5; i++ )
//	{
//		 EdgeLineIntersection ( l , i , intno , intp , sofline );
//	}
//	
//	if ( intno > 0 )
//		return true;
//	else
//		return false;
//
//}

//#############################################################################

VPoint2d gmQuad2d::GetCenter () const
{
	VPoint2d result = ( m_cor1 + m_cor3 ) / 2;
	return result;
}

//#############################################################################

//bool gmQuad2d::IsIntersectingWithDiagonal ( const VLine2d &l , int & intno , VPoint2d intp[] , vreal sofline[] ) const
//{
//	intno = 0;
//	// quad's diagonal
//	VLine2d qd;
//	qd.SetP1 ( m_cor1 );
//	qd.SetP2 ( m_cor3 );
//
//	LineIntersection2d calc;
//	LineIntersection2d::LineIntersectionData2d result 
//		= calc.CalcInt ( l , qd );
//
//	for ( int j=0; j<result.intno; j++ )
//	{
//		if ( intno+j > 3 )
//			throw ( VException ( ("too many quad intersections") , 0 )); 
//
//		intp[intno+j] = result.intpoint[j];
//		sofline[intno+j] = result.sofline1[j];
//	}
//	
//	intno += result.intno;
//	if ( intno > 0 )
//		return true;
//	else
//		return false;
//}
