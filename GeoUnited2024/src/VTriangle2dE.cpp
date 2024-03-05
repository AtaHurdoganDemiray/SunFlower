/******************************************************************************
*               File: VTriangle2dE.cpp											  *
*******************************************************************************
*               Description:                                                  *
*                                                                             *
*******************************************************************************
*               History:                                                      *
*  01.07.2003 17:01:38 Created by:    Ata Hurdogan Demiray                    *
*******************************************************************************
*               (C) 2003 by Pozitif GmbH                                      *
******************************************************************************/
#include "stdafx.h"
#include "VTriangle2dE.h"

VTriangle2dE::VTriangle2dE()
{

}

//#############################################################################

VTriangle2dE::VTriangle2dE ( const VTriangle2dE &Foo )
{
	m_tri.corner1 = Foo.m_tri.corner1;
	m_tri.corner2 = Foo.m_tri.corner2;
	m_tri.corner3 = Foo.m_tri.corner3;

	edge1 = Foo.edge1;
	edge2 = Foo.edge2;
	edge3 = Foo.edge3;
}

//#############################################################################

VTriangle2dE::VTriangle2dE ( vreal cor1[] , vreal cor2[] , vreal cor3[] )
{
	m_tri.corner1 = cor1;
	m_tri.corner2 = cor2;
	m_tri.corner3 = cor3;
	SetEdges();
	//SetNormal();
}

//#############################################################################

VTriangle2dE::VTriangle2dE 
( const VPoint2d & cor1 , const VPoint2d & cor2 , const VPoint2d & cor3 )
{
	m_tri.corner1 = cor1;
	m_tri.corner2 = cor2;
	m_tri.corner3 = cor3;
	SetEdges();
	//SetNormal();
}

//#############################################################################

VTriangle2dE::~VTriangle2dE()
{

}

//#############################################################################

const VPoint2d & VTriangle2dE::GetCorner1 () const
{
	return m_tri.corner1;
}

//#############################################################################

const VPoint2d & VTriangle2dE::GetCorner2 () const
{
	return m_tri.corner2;
}

//#############################################################################

const VPoint2d & VTriangle2dE::GetCorner3 () const
{
	return m_tri.corner3;
}

//#############################################################################

VPoint2d & VTriangle2dE::GetCorner1 () 
{
	return m_tri.corner1;
}

//#############################################################################

VPoint2d & VTriangle2dE::GetCorner2 ()
{
	return m_tri.corner2;
}

//#############################################################################

VPoint2d & VTriangle2dE::GetCorner3 ()
{
	return m_tri.corner3;
}

//#############################################################################

const VPoint2d & VTriangle2dE::GetEdge1 () const 
{
	return edge1;
}

//#############################################################################

const VPoint2d & VTriangle2dE::GetEdge2 () const
{
	return edge2;
}

//#############################################################################

const VPoint2d & VTriangle2dE::GetEdge3 () const
{
	return edge3;
}

//#############################################################################

VPoint2d & VTriangle2dE::GetEdge1 ()
{
	return edge1;
}

//#############################################################################

VPoint2d & VTriangle2dE::GetEdge2 ()
{
	return edge2;
}

//#############################################################################

VPoint2d & VTriangle2dE::GetEdge3 ()
{
	return edge3;
}

//#############################################################################

const VPoint2d & VTriangle2dE:: operator[] ( int index ) const
{
	if ( index == 0 )
		return m_tri.corner1;
	
	if ( index == 1 )
		return m_tri.corner2;
	
	return m_tri.corner3;

}
	
//#############################################################################

//VPoint2d VTriangle2dE::GetMin() const
//{
//	VPoint2d min = m_tri.corner1;
//
//	if ( min.x() > m_tri.corner2.x() )
//		min.x( m_tri.corner2.x() );
//
//	if ( min.x() > m_tri.corner3.x() )
//		min.x ( m_tri.corner3.x() );
//
//	if ( min.y() > m_tri.corner2.y() )
//		min.y( m_tri.corner2.y() );
//
//	if ( min.y() > m_tri.corner3.y() )
//		min.y ( m_tri.corner3.y() );
//
//	return min;
//}

//#############################################################################

//VPoint2d VTriangle2dE::GetMax() const
//{
//	VPoint2d max = m_tri.corner1;
//
//	if ( max.x() < m_tri.corner2.x() )
//		max.x( m_tri.corner2.x() );
//
//	if ( max.x() < m_tri.corner3.x() )
//		max.x ( m_tri.corner3.x() );
//
//	if ( max.y() < m_tri.corner2.y() )
//		max.y( m_tri.corner2.y() );
//
//	if ( max.y() < m_tri.corner3.y() )
//		max.y ( m_tri.corner3.y() );
//
//	return max;
//}

//#############################################################################

void VTriangle2dE::SetEdges ()
{
	edge1 = m_tri.corner2 - m_tri.corner1;
	//edge2 = m_tri.corner3 - m_tri.corner1;
	//edge3 = m_tri.corner3 - m_tri.corner2;
	edge2 = m_tri.corner3 - m_tri.corner2;
	edge3 = m_tri.corner1 - m_tri.corner3;
}

//#############################################################################

void VTriangle2dE::SetCorner1 ( const vreal  newcorner[] )
{
	m_tri.corner1.x ( newcorner[0] );
	m_tri.corner1.y ( newcorner[1] );
}

//#############################################################################

void VTriangle2dE::SetCorner2 ( const vreal  newcorner[] )
{
	m_tri.corner2.x ( newcorner[0] );
	m_tri.corner2.y ( newcorner[1] );
}

//#############################################################################

void VTriangle2dE::SetCorner3 ( const vreal  newcorner[] )
{
	m_tri.corner3.x ( newcorner[0] );
	m_tri.corner3.y ( newcorner[1] );
}

//#############################################################################

bool VTriangle2dE::IsInTriangle ( const VPoint2d &apoint ) const
{
	//vreal normal = edge2 ^ edge1;
	//
	//if ( normal > 0.0 )
	//	normal = 1.0;
	//else
	//	normal = -1.0;

	//VPoint2d edge1perp ( normal * edge1.y() , -normal * edge1.x() );
	//VPoint2d edge2perp ( -normal * edge2.y() , normal * edge2.x() );
	//VPoint2d edge3perp ( normal * edge3.y() , -normal * edge3.x() );
	VPoint2d edge1perp (-edge1.y() , edge1.x());
	VPoint2d edge2perp (-edge2.y() , edge2.x());
	VPoint2d edge3perp (-edge3.y() , edge3.x());

	VPoint2d diff1 = apoint - m_tri.corner2;
	VPoint2d diff3 = apoint - m_tri.corner3;

	vreal dotp1 = edge1perp % diff1;
	vreal dotp2 = edge2perp % diff1;
	vreal dotp3 = edge3perp % diff3;

	//if ( dotp1 >= -EPSILON && dotp2 >= -EPSILON && dotp3 >= -EPSILON )
	//	return true; // point is in triangle
	if ( (dotp1 >= -EPSILON && dotp2 >= -EPSILON && dotp3 >= -EPSILON)
		|| (dotp1 <= EPSILON && dotp2 <= EPSILON && dotp3 <= EPSILON) )
		return true; // point is in triangle
	
	return false; // point is not in triangle

}

bool VTriangle2dE::IsInTriangle (const VPoint2d &apoint , double tol)
{
	VPoint2d edge1perp (-edge1.y() , edge1.x());
	VPoint2d edge2perp (-edge2.y() , edge2.x());
	VPoint2d edge3perp (-edge3.y() , edge3.x());

	VPoint2d diff1 = apoint - m_tri.corner2;
	VPoint2d diff3 = apoint - m_tri.corner3;

	vreal dotp1 = edge1perp % diff1;
	vreal dotp2 = edge2perp % diff1;
	vreal dotp3 = edge3perp % diff3;

	//if ( dotp1 >= -EPSILON && dotp2 >= -EPSILON && dotp3 >= -EPSILON )
	//	return true; // point is in triangle
	if ( (dotp1 >= -tol && dotp2 >= -tol && dotp3 >= -tol)
		|| (dotp1 <= tol && dotp2 <= tol && dotp3 <= tol) )
		return true; // point is in triangle
	
	return false; // point is not in triangle
}

//#############################################################################
// return values:
// 0 : out of triangle
// 11: on corner 1
// 22: on corner 2
// 33: on corner 3
// 1 : on edge 1
// 2 : on edge 2
// 3 : on edge 3

//int VTriangle2dE::IsInTriangleTol ( const VPoint2d &apoint , const vreal & offsettol ) const
//{
//	vreal normal = edge2 ^ edge1;
//	
//	if ( normal > 0.0 )
//		normal = 1.0;
//	else
//		normal = -1.0;
//
//	VPoint2d edge1perp ( normal * edge1.y() , -normal * edge1.x() );
//	VPoint2d edge2perp ( -normal * edge2.y() , normal * edge2.x() );
//	VPoint2d edge3perp ( normal * edge3.y() , -normal * edge3.x() );
//	edge1perp.NormThis();
//	edge2perp.NormThis();
//	edge3perp.NormThis();
//
//	VPoint2d diff1 = apoint - m_tri.corner2;
//	VPoint2d diff3 = apoint - m_tri.corner3;
//
//	vreal dotp1 = edge1perp % diff1;
//	vreal dotp2 = edge2perp % diff1;
//	vreal dotp3 = edge3perp % diff3;
//
//	bool ck1 = (fabs(dotp1) <= offsettol);
//	bool ck2 = (fabs(dotp2) <= offsettol);
//	bool ck3 = (fabs(dotp3) <= offsettol);
//
//	if (ck1 && ck2 && dotp3 >= -offsettol)
//		return 11; // point is on corner1
//	
//	if (ck1 && ck3 && dotp2 >= -offsettol)
//		return 22; // point is on corner2
//
//	if (ck2 && ck3 && dotp1 >= -offsettol)
//		return 33; // point is on corner3
//
//	if (ck1 && dotp2 >= -offsettol && dotp3 >= -offsettol)
//		return 1;//point is on edge 1
//
//	if (ck2 && dotp1 >= -offsettol && dotp3 >= -offsettol)
//		return 2;//point is on edge 2
//
//	if (ck3 && dotp1 >= -offsettol && dotp2 >= -offsettol)
//		return 3;// point is on edge 3
//
//	if ( dotp1 >= -offsettol && dotp2 >= -offsettol && dotp3 >= -offsettol )
//		return 4; // point is in triangle
//
//	return 0; // point is not in triangle
//
//}

//#############################################################################


VPoint2d VTriangle2dE::GetCenterOfMass() const
{
	VPoint2d temp = edge1 * 0.5;
	temp += m_tri.corner1;

	VPoint2d diff = m_tri.corner3 - temp;
	diff *= (1.0/3.0);
	temp += diff;

	return temp;

}

//#############################################################################

VTriangle2dE & VTriangle2dE::operator = ( const VTriangle2dE &Foo )
{
	m_tri.corner1 = Foo.m_tri.corner1;
	m_tri.corner2 = Foo.m_tri.corner2;
	m_tri.corner3 = Foo.m_tri.corner3;

	edge1 = Foo.edge1;
	edge2 = Foo.edge2;
	edge3 = Foo.edge3;
	return *this;
}

//#############################################################################

bool VTriangle2dE::IsPointOnVertices ( const VPoint2d & p  , const vreal & tol ) const
{
	if ( p.Compare ( m_tri.corner1 , tol ) )
		return true;
	if ( p.Compare ( m_tri.corner2 , tol ) )
		return true;
	if ( p.Compare ( m_tri.corner3 , tol ) )
		return true;

	return false;	
}

VTriangle2dE::VTriangle2dE ( VIFStream & infile )
{
	(*this)<<infile;
}

void VTriangle2dE::operator << ( VIFStream & infile )
{
	m_tri.corner1<<infile;
	m_tri.corner2<<infile;
	m_tri.corner3<<infile;
	edge1<<infile;
	edge2<<infile;
	edge3<<infile;
}

void VTriangle2dE::operator >> ( VofStream & outfile ) const
{
	m_tri.corner1>>outfile;
	m_tri.corner2>>outfile;
	m_tri.corner3>>outfile;
	edge1>>outfile;
	edge2>>outfile;
	edge3>>outfile;
}

bool VTriangle2dE::Compare ( const VTriangle2dE & comp , const vreal & tol ) const
{
	int coinc = 0;
	if ( IsPointOnVertices ( comp.m_tri.corner1 , tol ) )
		coinc++;
	if ( IsPointOnVertices ( comp.m_tri.corner2 , tol ) )
		coinc++;
	if ( IsPointOnVertices ( comp.m_tri.corner3 , tol ) )
		coinc++;

	if ( coinc == 3 )
		return true;
	else
		return false;
}

VTriangle2dE::VTriangle2dE ( Bifstream & binfile )
{
	Serialize (binfile);
}

void VTriangle2dE::Serialize ( Bifstream & binfile )
{
	m_tri.corner1.Serialize (binfile);
	m_tri.corner2.Serialize (binfile);
	m_tri.corner3.Serialize (binfile);
	edge1.Serialize (binfile);
	edge2.Serialize (binfile);
	edge3.Serialize (binfile);
}

void VTriangle2dE::Serialize (Bofstream &binfile) const
{
	m_tri.corner1.Serialize (binfile);
	m_tri.corner2.Serialize (binfile);
	m_tri.corner3.Serialize (binfile);
	edge1.Serialize (binfile);
	edge2.Serialize (binfile);
	edge3.Serialize (binfile);
}

vreal VTriangle2dE::GetArea () const
{
	// A = 0.25 * sqrt ( U * ( U - 2 l1 ) * ( U - 2 l2 ) * ( U - 2 l3 ) )
	vreal U = GetPerimeter();
	vreal l1 = edge1.GetLength();
	vreal l2 = edge2.GetLength();
	vreal l3 = edge3.GetLength();
	vreal A = 0.25 * sqrt ( U * ( U - 2 * l1 ) * ( U - 2 * l2 ) * ( U - 2 * l3 ) );
	return A;
}

vreal VTriangle2dE::GetPerimeter () const
{
	vreal per = edge1.GetLength() + edge2.GetLength() + edge3.GetLength();
	return per;
}

bool VTriangle2dE::operator == (const VTriangle2dE &toCompare)const
{
	if(this==&toCompare)
		return true;

	if(m_tri.corner1!=toCompare.m_tri.corner1)
		return false;
	if(m_tri.corner2!=toCompare.m_tri.corner2)
		return false;
	if(m_tri.corner3!=toCompare.m_tri.corner3)
		return false;

	return true;
}

bool VTriangle2dE::operator != (const VTriangle2dE &toCompare)const
{
	return !(this->operator ==(toCompare));
}

void VTriangle2dE::UpdateMinMax(VPoint2d &minp,VPoint2d &maxp)const
{
	minp.ReplaceMin (m_tri.corner1);
	minp.ReplaceMin (m_tri.corner2);
	minp.ReplaceMin (m_tri.corner3);

	maxp.ReplaceMax (m_tri.corner1);
	maxp.ReplaceMax (m_tri.corner2);
	maxp.ReplaceMax (m_tri.corner3);
}

bool VTriangle2dE::IsCorrupted()const
{
	if ( m_tri.corner1.IsCorrupted()
		|| m_tri.corner2.IsCorrupted()
		|| m_tri.corner3.IsCorrupted() )
	{
		return true;
	}
	
	if ( fabs(GetArea()) < EPSILON )
	{
		return true;
	}
	return false;	
};

void VTriangle2dE::GetUV (const VPoint2d &p , vreal &u , vreal &v)const
{
	VVector2d d = p - m_tri.corner1;
	vreal div = edge1.x() * edge2.y() - edge1.y() * edge2.x();
	u = (d.x() * edge2.y() - d.y() * edge2.x()) / div;
	v = (d.y() * edge1.x() - d.x() * edge1.y()) / div;
}

VPoint2d VTriangle2dE::GetP (const vreal &u , const vreal &v)const
{
	VPoint2d res = m_tri.corner1 + u * edge1 + v * edge2;
	return res;
}

VTriangle2d & VTriangle2dE:: GetTri ()
{
	return m_tri;
}

const VTriangle2d & VTriangle2dE::GetTri()const
{
	return m_tri;
}

bool VTriangle2dE::IsNeighbor (const VTriangle2dE &t , VPoint2d &common1 , VPoint2d &common2)const
{
	int commonno = 0;
	VPoint2d *c = &common1;
	if (t.GetCorner1() == m_tri.corner1) 
	{
		commonno++;
		(*c) = m_tri.corner1;
		c = &common2;
	}
	else if (t.GetCorner1() == m_tri.corner2)
	{
		commonno++;
		(*c) = m_tri.corner2;
		c = &common2;
	}
	else if (t.GetCorner1() == m_tri.corner3)
	{
		commonno++;
		(*c) = m_tri.corner3;
		c = &common2;
	}

	if (t.GetCorner2() == m_tri.corner1) 
	{
		commonno++;
		(*c) = m_tri.corner1;
		c = &common2;
	}
	else if (t.GetCorner2() == m_tri.corner2)
	{
		commonno++;
		(*c) = m_tri.corner2;
		c = &common2;
	}
	else if (t.GetCorner2() == m_tri.corner3)
	{
		commonno++;
		(*c) = m_tri.corner3;
		c = &common2;
	}

	if (t.GetCorner3() == m_tri.corner1) 
	{
		commonno++;
		(*c) = m_tri.corner1;
		c = &common2;
	}
	else if (t.GetCorner3() == m_tri.corner2)
	{
		commonno++;
		(*c) = m_tri.corner2;
		c = &common2;
	}
	else if (t.GetCorner3() == m_tri.corner3)
	{
		commonno++;
		(*c) = m_tri.corner3;
		c = &common2;
	}
	
	if (commonno == 2)
		return true;
	else
		return false;
}

sVector<double,2> VTriangle2dE::GetMin()const
{
	return m_tri.GetMin();
}	
sVector<double,2> VTriangle2dE::GetMax()const
{
	return m_tri.GetMax();
}
