/******************************************************************************
*               File: VTriangleE.cpp											  *
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
#include "VTriangleE.h"
#include "CoordinateSystem.h"
#include "VPlane.h"
#include "LinearEquations.h"
#include "RotationMatrix.h"

#include "LinearEquations.h"

VTriangleE::VTriangleE()
{

}

//#############################################################################

VTriangleE::VTriangleE ( const VTriangleE &Foo )
:m_tri(Foo.m_tri)
{
	edge1 = Foo.edge1;
	edge2 = Foo.edge2;
	edge3 = Foo.edge3;

	normal = Foo.normal;
}

//#############################################################################

VTriangleE::VTriangleE ( vreal cor1[] , vreal cor2[] , vreal cor3[] )
{
	m_tri.corner[0] = cor1;
	m_tri.corner[1] = cor2;
	m_tri.corner[2] = cor3;
	SetEdges();
	SetNormal();
}

//#############################################################################

VTriangleE::VTriangleE 
( const VPoint & cor1 , const VPoint & cor2 , const VPoint & cor3 )
:m_tri (cor1,cor2,cor3)
{
	SetEdges();
	SetNormal();
}

//#############################################################################

VTriangleE::~VTriangleE()
{

}

//#############################################################################

const VPoint & VTriangleE::GetCorner1 () const
{
	return m_tri.corner[0];
}

//#############################################################################

const VPoint & VTriangleE::GetCorner2 () const
{
	return m_tri.corner[1];
}

//#############################################################################

const VPoint & VTriangleE::GetCorner3 () const
{
	return m_tri.corner[2];
}

//#############################################################################

VPoint & VTriangleE::GetCorner1 () 
{
	return m_tri.corner[0];
}

//#############################################################################

VPoint & VTriangleE::GetCorner2 ()
{
	return m_tri.corner[1];
}

//#############################################################################

VPoint & VTriangleE::GetCorner3 ()
{
	return m_tri.corner[2];
}

//#############################################################################

const VPoint & VTriangleE::GetNormal () const 
{
	return normal;
}

//#############################################################################

const VPoint & VTriangleE::GetEdge1 () const 
{
	return edge1;
}

//#############################################################################

const VPoint & VTriangleE::GetEdge2 () const
{
	return edge2;
}

//#############################################################################

const VPoint & VTriangleE::GetEdge3 () const
{
	return edge3;
}

//#############################################################################

VPoint & VTriangleE::GetNormal ()
{
	return normal;
}

//#############################################################################

VPoint & VTriangleE::GetEdge1 ()
{
	return edge1;
}

//#############################################################################

VPoint & VTriangleE::GetEdge2 ()
{
	return edge2;
}

//#############################################################################

VPoint & VTriangleE::GetEdge3 ()
{
	return edge3;
}

//#############################################################################

const VPoint & VTriangleE:: operator[] ( int index ) const
{
	if ( index == 0 )
		return m_tri.corner[0];
	
	if ( index == 1 )
		return m_tri.corner[1];
	
	if ( index == 2 )
		return m_tri.corner[2];

	return normal;
}
	
//#############################################################################

//VPoint VTriangleE::GetMin() const
//{
//	VPoint min = m_tri.corner[0];
//
//	if ( min.x() > m_tri.corner[1].x() )
//		min.x( m_tri.corner[1].x() );
//
//	if ( min.x() > m_tri.corner[2].x() )
//		min.x ( m_tri.corner[2].x() );
//
//	if ( min.y() > m_tri.corner[1].y() )
//		min.y( m_tri.corner[1].y() );
//
//	if ( min.y() > m_tri.corner[2].y() )
//		min.y ( m_tri.corner[2].y() );
//
//	if ( min.z() > m_tri.corner[1].z() )
//		min.z( m_tri.corner[1].z() );
//
//	if ( min.z() > m_tri.corner[2].z() )
//		min.z ( m_tri.corner[2].z() );
//
//	return min;
//}

//#############################################################################

//VPoint VTriangleE::GetMax() const
//{
//	VPoint max = m_tri.corner[0];
//
//	if ( max.x() < m_tri.corner[1].x() )
//		max.x( m_tri.corner[1].x() );
//
//	if ( max.x() < m_tri.corner[2].x() )
//		max.x ( m_tri.corner[2].x() );
//
//	if ( max.y() < m_tri.corner[1].y() )
//		max.y( m_tri.corner[1].y() );
//
//	if ( max.y() < m_tri.corner[2].y() )
//		max.y ( m_tri.corner[2].y() );
//
//	if ( max.z() < m_tri.corner[1].z() )
//		max.z( m_tri.corner[1].z() );
//
//	if ( max.z() < m_tri.corner[2].z() )
//		max.z ( m_tri.corner[2].z() );
//
//	return max;
//}

//#############################################################################

void VTriangleE::SetEdges ()
{
	edge1 = m_tri.corner[1] - m_tri.corner[0];
	//edge2 = m_tri.corner[2] - m_tri.corner[0];
	//edge3 = m_tri.corner[2] - m_tri.corner[1];
	edge2 = m_tri.corner[2] - m_tri.corner[1];
	edge3 = m_tri.corner[0] - m_tri.corner[2];
}

//#############################################################################

void VTriangleE::SetNormal()
{
	normal = ( edge1^edge2 );
	
	//if ( fabs(normal.GetLength2()) < EPSILON )
	//	throw VException (("VTriangleE::SetNormal exception,edges are degenerate."),1);
	
	normal.NormThis();
}

//#############################################################################

void VTriangleE::SetCorner1 ( const vreal  newcorner[] )
{
	m_tri.corner[0].x ( newcorner[0] );
	m_tri.corner[0].y ( newcorner[1] );
	m_tri.corner[0].z ( newcorner[2] );
}

//#############################################################################

void VTriangleE::SetCorner2 ( const vreal  newcorner[] )
{
	m_tri.corner[1].x ( newcorner[0] );
	m_tri.corner[1].y ( newcorner[1] );
	m_tri.corner[1].z ( newcorner[2] );
}

//#############################################################################

void VTriangleE::SetCorner3 ( const vreal  newcorner[] )
{
	m_tri.corner[2].x ( newcorner[0] );
	m_tri.corner[2].y ( newcorner[1] );
	m_tri.corner[2].z ( newcorner[2] );
}

//#############################################################################

bool VTriangleE::IsInTriangle ( const VPoint & apoint ) const
{

	VPoint edge1perp = normal^edge1;
	VPoint edge2perp = normal^edge2;
	VPoint edge3perp = normal^edge3;
	//edge1perp.NormThis();
	//edge2perp.NormThis();
	//edge3perp.NormThis();

	VPoint diff1 = apoint - m_tri.corner[1];
	VPoint diff3 = apoint - m_tri.corner[2];

	vreal dotp1 = edge1perp % diff1;
	vreal dotp2 = edge2perp % diff1;
	vreal dotp3 = edge3perp % diff3;

	if ( (dotp1 >= -EPSILON && dotp2 >= -EPSILON && dotp3 >= -EPSILON)
		|| (dotp1 <= EPSILON && dotp2 <= EPSILON && dotp3 <= EPSILON) )
		return true; // point is in triangle

	return false; // point is not in triangle
}

//#############################################################################

bool VTriangleE::IsInTriangle ( const VPoint & apoint , const vreal & offsettol ) const
{

	VPoint edge1perp = normal^edge1;
	VPoint edge2perp = normal^edge2;
	VPoint edge3perp = normal^edge3;
	//edge1perp.NormThis();
	//edge2perp.NormThis();
	//edge3perp.NormThis();

	VPoint diff1 = apoint - m_tri.corner[1];
	VPoint diff3 = apoint - m_tri.corner[2];

	vreal dotp1 = edge1perp % diff1;
	vreal dotp2 = edge2perp % diff1;
	vreal dotp3 = edge3perp % diff3;

	if ( (dotp1 >= -offsettol && dotp2 >= -offsettol && dotp3 >= -offsettol)
		|| (dotp1 <= offsettol && dotp2 <= offsettol && dotp3 <= offsettol) )
		return true; // point is in triangle

	return false; // point is not in triangle
}

//#############################################################################

VPoint VTriangleE::GetCenterOfMass() const
{
	VPoint temp = edge1 * 0.5;
	temp += m_tri.corner[0];

	VPoint diff = m_tri.corner[2] - temp;
	diff *= 0.33333333333333333333333333333333;//(1.0/3.0);
	temp += diff;

	return temp;

}

//#############################################################################

void VTriangleE::operator = ( const VTriangleE &Foo )
{
	m_tri.operator = (Foo.m_tri);
	
	edge1 = Foo.edge1;
	edge2 = Foo.edge2;
	edge3 = Foo.edge3;
	
	normal = Foo.normal;

}

//#############################################################################

VTriangleE::VTriangleE ( VIFStream & infile )
{
	(*this)<<infile;
}

void VTriangleE::operator << ( VIFStream & infile )
{
	m_tri.corner[0]<<infile;
	m_tri.corner[1]<<infile;
	m_tri.corner[2]<<infile;
	edge1<<infile;
	edge2<<infile;
	edge3<<infile;
	normal<<infile;
}

void VTriangleE::operator >> ( VofStream & outfile ) const
{
	m_tri.corner[0]>>outfile;
	m_tri.corner[1]>>outfile;
	m_tri.corner[2]>>outfile;
	edge1>>outfile;
	edge2>>outfile;
	edge3>>outfile;
	normal>>outfile;
}

VTriangleE::VTriangleE ( Bifstream & binfile )
{
	Serialize (binfile);
}

void VTriangleE::Serialize (Bifstream &binfile)
{
	m_tri.corner[0].Serialize (binfile);
	m_tri.corner[1].Serialize (binfile);
	m_tri.corner[2].Serialize (binfile);
	edge1.Serialize (binfile);
	edge2.Serialize (binfile);
	edge3.Serialize (binfile);
	normal.Serialize (binfile);
}

void VTriangleE::Serialize (Bofstream &binfile) const
{
	m_tri.corner[0].Serialize (binfile);
	m_tri.corner[1].Serialize (binfile);
	m_tri.corner[2].Serialize (binfile);
	edge1.Serialize (binfile);
	edge2.Serialize (binfile);
	edge3.Serialize (binfile);
	normal.Serialize (binfile);
}

vreal VTriangleE::GetArea () const
{
	// A = 0.25 * sqrt ( U * ( U - 2 l1 ) * ( U - 2 l2 ) * ( U - 2 l3 ) )
//	vreal U = GetPerimeter();
//	vreal l1 = edge1.GetLength();
//	vreal l2 = edge2.GetLength();
//	vreal l3 = edge3.GetLength();
//	vreal A = 0.25 * sqrt ( U * ( U - 2 * l1 ) * ( U - 2 * l2 ) * ( U - 2 * l3 ) );
//	return A;

	VPoint tmp = (m_tri.corner[0] ^ m_tri.corner[1]) + (m_tri.corner[1] ^ m_tri.corner[2]) + (m_tri.corner[2] ^ m_tri.corner[0]);
	return 0.5 * normal % tmp;
}

vreal VTriangleE::GetPerimeter () const
{
	vreal per = edge1.GetLength() + edge2.GetLength() + edge3.GetLength();
	return per;
}

bool VTriangleE::operator == (const VTriangleE &toCompare)const
{
	if(this==&toCompare)
		return true;

	if(m_tri.corner[0]!=toCompare.m_tri.corner[0])
		return false;
	if(m_tri.corner[1]!=toCompare.m_tri.corner[1])
		return false;
	if(m_tri.corner[2]!=toCompare.m_tri.corner[2])
		return false;

	return true;
}

bool VTriangleE::operator != (const VTriangleE &toCompare)const
{
	return !(this->operator ==(toCompare));
}

void VTriangleE::UpdateMinMax(VPoint &minp,VPoint &maxp)const
{
	minp.ReplaceMin (m_tri.corner[0]);
	minp.ReplaceMin (m_tri.corner[1]);
	minp.ReplaceMin (m_tri.corner[2]);

	maxp.ReplaceMax (m_tri.corner[0]);
	maxp.ReplaceMax (m_tri.corner[1]);
	maxp.ReplaceMax (m_tri.corner[2]);
}

bool VTriangleE::GetUV (const VPoint &p , vreal &u , vreal &v)const
{
	VVector d = p - m_tri.corner[0];
	if ( fabs(d % normal) > 1.0e-4)
		return false;

	VPoint xdir = edge1.Unit();
	VPoint ydir = normal ^ xdir;
	//ydir.NormThis();

	vreal v1x = xdir % edge1;
	vreal v1y = 0.0;
	vreal v2x = xdir % edge2;
	vreal v2y = ydir % edge2;
	vreal dx = xdir % d;
	vreal dy = ydir % d;

	//vreal div = v2y * v1x - v2x * v1y;
	//if (fabs(div)<EPSILON)
	//	throw VException (("VTriangleE::GetUV Determinant 0!"),1);
	//u = (dx * v2y - dy * v2x) / div;
	//v = (dy * v1x - dx * v1y) / div;
	bool ck = LinearEquations::SolveLinear2Unknown(v1x,v2x,-dx,v1y,v2y,-dy,u,v);
	// for debug 
	//CString logstr;
	//logstr.Format("U:%.4f V:%.4f", u, v);
	//LogFileWriter::WriteUnknownExceptionLog((LPCTSTR)logstr);
	// for debug
	return ck;
}

VPoint VTriangleE::GetP (const vreal &u , const vreal &v)const
{
	VPoint res = m_tri.corner[0] + u * edge1 + v * edge2;
	return res;
}

VTriangle & VTriangleE:: GetTri ()
{
	return m_tri;
}

const VTriangle & VTriangleE::GetTri()const
{
	return m_tri;
}

bool VTriangleE::IsNeighbor (const VTriangleE &t , VPoint &common1 , VPoint &common2 , vreal tol)const
{
	int commonno = 0;
	VPoint *c = &common1;
	if (t.GetCorner1().Compare(m_tri.corner[0],tol)) 
	{
		commonno++;
		(*c) = m_tri.corner[0];
		c = &common2;
	}
	else if (t.GetCorner1().Compare(m_tri.corner[1],tol))
	{
		commonno++;
		(*c) = m_tri.corner[1];
		c = &common2;
	}
	else if (t.GetCorner1().Compare(m_tri.corner[2],tol))
	{
		commonno++;
		(*c) = m_tri.corner[2];
		c = &common2;
	}

	if (t.GetCorner2().Compare(m_tri.corner[0],tol)) 
	{
		commonno++;
		(*c) = m_tri.corner[0];
		c = &common2;
	}
	else if (t.GetCorner2().Compare(m_tri.corner[1],tol))
	{
		commonno++;
		(*c) = m_tri.corner[1];
		c = &common2;
	}
	else if (t.GetCorner2().Compare(m_tri.corner[2],tol))
	{
		commonno++;
		(*c) = m_tri.corner[2];
		c = &common2;
	}

	if (t.GetCorner3().Compare(m_tri.corner[0],tol)) 
	{
		commonno++;
		(*c) = m_tri.corner[0];
		c = &common2;
	}
	else if (t.GetCorner3().Compare(m_tri.corner[1],tol))
	{
		commonno++;
		(*c) = m_tri.corner[1];
		c = &common2;
	}
	else if (t.GetCorner3().Compare(m_tri.corner[2],tol))
	{
		commonno++;
		(*c) = m_tri.corner[2];
		c = &common2;
	}
	
	if (commonno == 2)
		return true;
	else
		return false;
}

bool VTriangleE::IsNeighbor(const VTriangleE &t, VPoint &common1, VPoint &common2 , VPoint &myother , VPoint &tother , vreal tol)const
{
	int commonno = 0;
	VPoint *c = &common1;
	int commonindexthis[2], commonindext[2];
	commonindexthis[0] = -1;
	commonindexthis[1] = -1;
	commonindext[0] = -1;
	commonindext[1] = -1;

	for (int i = 0; i < 3; ++i)
	{
		for (int j = 0; j < 3; ++j)
		{
			if (m_tri.corner[i].Compare(t.m_tri.corner[j], tol))
			{
				commonindexthis[commonno] = i;
				commonindext[commonno] = j;
				++commonno;
				(*c) = m_tri.corner[i];
				c = &common2;
			}
		}
	}
	if (commonno != 2)
		return false;
	for (int i = 0; i < 3; ++i)
	{
		if (commonindexthis[0] != i && commonindexthis[1] != i)
			myother = m_tri.corner[i];
		if (commonindext[0] != i && commonindext[1] != i)
			tother = t.m_tri.corner[i];
	}
	return true;
}

//#############################################################################
bool VTriangleE::IsCorrupted () const
{
	if ( m_tri.corner[0].IsCorrupted()
		|| m_tri.corner[1].IsCorrupted()
		|| m_tri.corner[2].IsCorrupted() )
	{
		return true;
	}
	
	//if ( fabs(GetArea()) < EPSILON )
	//{
	vreal U = GetPerimeter();
	vreal l1 = edge1.GetLength();
	vreal l2 = edge2.GetLength();
	vreal l3 = edge3.GetLength();
	vreal A = /*0.25 * sqrt */ fabs( U * ( U - 2 * l1 ) * ( U - 2 * l2 ) * ( U - 2 * l3 ) ) ;	
	if (A<EPSILON)
		return true;
	//}

	return false;	
}
//#############################################################################

sVector<double,3> VTriangleE::GetMin()const
{
	return m_tri.GetMin();
}	
sVector<double,3> VTriangleE::GetMax()const
{
	return m_tri.GetMax();
}

//vreal VTriangleE::CalculateProjVolumeToPlane (const VPoint &planepoint , const VPoint &planenormal)const
//{
//	vreal dotp1 = normal % planenormal;
//	if (fabs (dotp1) < EPSILON)
//		return 0.0; //triangle is perp to projection plane , no projected volume
//	//Calculating c1 and cn
//	VVector xdir = edge1.Unit();
//	VVector ydir = planenormal ^ xdir;
//	vreal c1 = xdir % planenormal;
//	vreal cn = ydir % planenormal;
//	// Calculating h and l
//	vreal dotp3 = xdir % edge2;
//	vreal h = sqrt (edge2.GetLength2() - dotp3 * dotp3);
//	vreal l=edge1.GetLength();
//	// Calculating h0
//	VVector diff = m_tri.corner[0] - planepoint;
//	vreal h0 =  diff % planenormal;
//	vreal c2 = dotp3 / h;
//	vreal dotp4 = xdir % edge3;
//	vreal c3 = dotp4 / h;
//	// Calculating x1
//	vreal x1 = edge1.GetLength();
//	// Calculating h0
//	VVector diff = m_tri.corner[0] - planepoint;
//	vreal h0 =  diff % planenormal;
//	// Calculating A and B and C
//	vreal A = (c1 * c3 * c3  - c1 * c2 * c2 ) * 0.5 + cn * c3 - cn * c2;
//	vreal B = c3 * c1 * x1 + h0 * c3 - h0 * c2 + cn * x1;
//	vreal C = c1 * x1 * x1 * 0.5 + h0 * x1;
//	// calculating projected volume
//	vreal projVol = A * h * h * h / 3.0 + B * h * h * 0.5 + C * h;
//	return projVol;
//}
//vreal VTriangleE::CalculateProjVolumeToPlane (const VPoint &planepoint , const VPoint &planenormal)const
//{
//	VPoint prj1 = (edge1 % planenormal) * planenormal;
//	VVector xdir = edge1 - prj1;
//	xdir.NormThis();
//	VVector ydir = planenormal ^ xdir;
//	CoordinateSystem co (m_tri.corner[0] , xdir , ydir , planenormal);
//	VPoint p1 = co.AbsoluteCoordToRelativeCoord (m_tri.corner[0]);
//	VPoint p2 = co.AbsoluteCoordToRelativeCoord (m_tri.corner[1]);
//	VPoint p3 = co.AbsoluteCoordToRelativeCoord (m_tri.corner[2]);
//	
//	VVector e1 = p2 - p1;
//	VVector e2 = p3 - p1;
//	VVector e3 = p3 - p2;
//
//	vreal l = e1.x();
//	vreal c1 = e1.z() / l;
//	vreal ztmp = p3.x()*c1;
//	vreal dzy = p3.z() - ztmp;
//	vreal c2 = dzy / p3.y();
//	vreal h = p3.y();
//	vreal h0 = (m_tri.corner[0] - planepoint) % planenormal;
//
//	vreal vol = 0.5 * (h0 * l * h + c1 * l * l * h * 0.5 + c2 * l * h * h * 0.5);
//	return vol;
//}

//vreal VTriangleE::CalculateProjVolumeToPlane (const VPoint &planepoint , const VPoint &planenormal)const
//{
//	vreal dotp1 = normal % planenormal;
//	if (fabs (dotp1) < EPSILON)
//		return 0.0; //triangle is perp to projection plane , no projected volume
//	VPoint prj1 = (edge1 % planenormal) * planenormal;
//	VVector xdir = edge1 - prj1;
//	xdir.NormThis();
//	VVector ydir = planenormal ^ xdir;
//	CoordinateSystem co (m_tri.corner[0] , xdir , ydir , planenormal);
//	VPoint p1 = co.AbsoluteCoordToRelativeCoord (m_tri.corner[0]);
//	VPoint p2 = co.AbsoluteCoordToRelativeCoord (m_tri.corner[1]);
//	VPoint p3 = co.AbsoluteCoordToRelativeCoord (m_tri.corner[2]);
//	
//	VVector e1 = p2 - p1;
//	VVector e2 = p3 - p1;
//	VVector e3 = p3 - p2;
//
//	vreal l = e1.x();
//	vreal c1 = e1.z() / l;
//	vreal ztmp = p3.x()*c1;
//	vreal dzy = p3.z() - ztmp;
//	vreal cn = dzy / p3.y();
//	vreal h = p3.y();
//	vreal h0 = (m_tri.corner[0] - planepoint) % planenormal;
//	vreal c2 = e2.x() / e2.y();
//	vreal c3 = e3.x() / e3.y();
//	vreal x1 = p2.x();
//
//	// Calculating A and B and C
//	vreal A = (c1 * c3 * c3  - c1 * c2 * c2 ) * 0.5 + cn * c3 - cn * c2;
//	vreal B = c3 * c1 * x1 + h0 * c3 - h0 * c2 + cn * x1;
//	vreal C = c1 * x1 * x1 * 0.5 + h0 * x1;
//	// calculating projected volume
//	vreal projVol = A * h * h * h / 3.0 + B * h * h * 0.5 + C * h;
//	return fabs(projVol);
//	
//}

vreal VTriangleE::CalculateProjVolumeToPlane (const VPlane &plane)const
{

	VPoint p1,p2,p3;
	VPoint n1;
	vreal tmp = normal % plane.normal;
	if ( tmp > 1.e-6)
	{
		p1 = m_tri.corner[0];
		p2 = m_tri.corner[1];
		p3 = m_tri.corner[2];
		n1 = normal;
	}
	else if ( tmp < -1.e-6)
	{
		p1 = m_tri.corner[0];
		p2 = m_tri.corner[2];
		p3 = m_tri.corner[1];
		n1 = -normal;
	}
	else
	{
		return 0.0;// triangle is perp to plane
	}

	VPoint pp1 = plane.ProjectPoint (p1);
	VPoint pp2 = plane.ProjectPoint (p2);
	VPoint pp3 = plane.ProjectPoint (p3);

	vreal c1 = p1 % n1;
	vreal A1 = GetArea();

	vreal c2 , c3 , c4 , c5;
	vreal A2 , A3 , A4 , A5;
	GetAc (p1,pp1,pp2,p2,c2,A2);
	GetAc (p2,pp2,pp3,p3,c3,A3);
	GetAc (p3,pp3,pp1,p1,c4,A4);
	
	VTriangleE myt (pp1,pp3,pp2);
	c5 = pp1 % myt.GetNormal();
	A5 = myt.GetArea();
	
	vreal f1 = c1 * A1;
	vreal f2 = c2 * A2;
	vreal f3 = c3 * A3;
	vreal f4 = c4 * A4;
	vreal f5 = c5 * A5;
	vreal vol = (f1 + f2 + f3 + f4 + f5) / 3.0;
	return vol;
}

void VTriangleE::GetAc (const VPoint &p1,const VPoint &p2,const VPoint &p3,const VPoint &p4,vreal &c , vreal &A)const
{
	VPoint n = (p2-p1) ^ (p3-p2);
	if (n.GetLength2() < EPSILON)
	{
		A = 0.0;
		c = 0.0;
		return;
	}
	
	n.NormThis();
	c = p1 % n;
	VPoint mysigma = (p1 ^ p2) + (p2 ^ p3) + (p3 ^ p4) + (p4 ^ p1);
	A = 0.5 * n % mysigma;
}

void VTriangleE::Digitise (const vreal &step 
						   , std::vector<VPoint> &ps
						   , std::vector<VPoint> &ns)const
{
	vreal lu = edge1.GetLength();
	vreal lv = edge2.GetLength();
	vreal du = lu/step;
	vreal dv = lv/step;
	for (vreal u= 0.;u<1.;u+=du)
	{
		for (vreal v=0.;v<(1.0-u);v+=dv)
		{
			ps.push_back (GetP (u,v));
			ns.push_back (normal);
		}
	}
}

void VTriangleE::Digitise (const vreal step
						   , std::vector<VPoint> &pvec)const
{
	//vreal lu = edge1.GetLength();
	//vreal lv = edge2.GetLength();
	//vreal du = lu < step ? 1. : step/lu;
	//vreal dv = lv < step ? 1. : step/lv;
	//vreal ue = 1.;
	//for (vreal u=0.; u<ue; u+=du)
	//{
	//	DigitiseConstU (u,dv,pvec);
	//}
	//DigitiseConstU(ue,dv,pvec);
	
	
	
	// Check if this is a normal triangle 
	VPoint ck1 = edge1 ^ edge2;
	if (ck1.x() < 1.e-8 && ck1.y() < 1.e-8  && ck1.z() < 1.e-8)
		return;
	VPoint ck2 = edge2 ^ edge3;
	if (ck2.x() < 1.e-8 && ck2.y() < 1.e-8 && ck2.z() < 1.e-8)
		return;
	VPoint ck3 = edge3 ^ edge1;
	if (ck3.x() < 1.e-8 && ck3.y() < 1.e-8 && ck3.z() < 1.e-8)
		return;
	
	CoordinateSystem cs1 = this->GetCs1();
	CoordinateSystem cs2 = this->GetCs2();
	CoordinateSystem cs3 = this->GetCs3();

	// Find the most efficient box 
	VTriangle t1 (cs1.AbsoluteCoordToRelativeCoord(GetCorner1()) , cs1.AbsoluteCoordToRelativeCoord(GetCorner2()) , cs1.AbsoluteCoordToRelativeCoord(GetCorner3()));
	VTriangle t2 (cs2.AbsoluteCoordToRelativeCoord(GetCorner1()) , cs2.AbsoluteCoordToRelativeCoord(GetCorner2()) , cs2.AbsoluteCoordToRelativeCoord(GetCorner3()));
	VTriangle t3 (cs3.AbsoluteCoordToRelativeCoord(GetCorner1()) , cs3.AbsoluteCoordToRelativeCoord(GetCorner2()) , cs3.AbsoluteCoordToRelativeCoord(GetCorner3()));
	
	VPoint mn1 = t1.GetMin();
	VPoint mx1 = t1.GetMax();
	VPoint mn2 = t2.GetMin();
	VPoint mx2 = t2.GetMax();
	VPoint mn3 = t3.GetMin();
	VPoint mx3 = t3.GetMax();
	
	VPoint delta1 = mx1 - mn1;
	VPoint delta2 = mx2 - mn2;
	VPoint delta3 = mx3 - mn3;

	double area1 = MIN_NUMBER_LIMIT;//delta1.x() * delta1.y();
	double area2 = delta2.x() * delta2.y();
	double area3 = delta3.x() * delta3.y();
	if (area1 <= area2 && area1 <= area3)
	{
		// use cs1
		VTriangleE t1E (t1.GetCorner1(),t1.GetCorner2(),t1.GetCorner3());
		int signk = t1.GetCorner3().y() < 0.0 ? -1 : 1;
		double ymax = fabs (t1.GetCorner3().y());
		for (double x=0; x <= t1.GetCorner2().x(); x+=step)
		{
			for (double y=0; y <= ymax; y+=step)
			{
				VPoint cp (x,y*signk,0.0);
				if (t1E.IsInTriangle (cp))
					pvec.push_back (cs1.RelativeCoordToAbsoluteCoord(cp));
			}
		}
	}
	else if (area2 <= area1 && area2 <= area3)
	{
		// use cs2
		VTriangleE t2E (t2.GetCorner1() , t2.GetCorner2() , t2.GetCorner3());
		int signk = t2.GetCorner3().y() < 0.0 ? -1 : 1;
		double ymax = fabs (t2.GetCorner3().y());
		for (double x=0; x <= t2.GetCorner2().x(); x+=step)
		{
			for (double y=0; y <= ymax; y+=step)
			{
				VPoint cp (x,y*signk,0.0);
				if (t2E.IsInTriangle(cp))
					pvec.push_back (cs2.RelativeCoordToAbsoluteCoord(cp));
			}
		}
	}
	else if (area3 <= area1 && area3 <= area2)
	{
		// use cs3
		VTriangleE t3E (t3.GetCorner1() , t3.GetCorner2() , t3.GetCorner3());
		int signk = t3.GetCorner3().y() < 0.0 ? -1 : 1;
		double ymax = fabs (t3.GetCorner3().y());
		for (double x=0; x <= t3.GetCorner2().x(); x+=step)
		{
			for (double y=0; y <= ymax; y+=step)
			{
				VPoint cp (x,y*signk,0.0);
				if (t3E.IsInTriangle (cp))
					pvec.push_back (cs3.RelativeCoordToAbsoluteCoord(cp));
			}
		}
	}
}

void VTriangleE::DigitiseConstU (const vreal u 
								 , const vreal dv
								 , std::vector<VPoint> &pvec)const
{
	vreal ve = (1. - u);
	for (vreal v=0.; v<ve; v+=dv)
	{
		pvec.push_back (GetP (u,v));
	}
	pvec.push_back (GetP(u,ve));
}

VPlane VTriangleE::GetPlane() const
{
	return VPlane(GetCorner1(), GetNormal());
}

CoordinateSystem VTriangleE::GetCs1()const
{
	VPoint dirx = GetEdge1().Unit();
	CoordinateSystem cs (GetCorner1() , dirx , GetNormal() ^ dirx , GetNormal());
	return cs;
}

CoordinateSystem VTriangleE::GetCs2()const
{
	VPoint dirx = GetEdge2().Unit();
	CoordinateSystem cs (GetCorner2() , dirx , GetNormal() ^ dirx , GetNormal());
	return cs;
}

CoordinateSystem VTriangleE::GetCs3()const
{
	VPoint dirx = GetEdge3().Unit();
	CoordinateSystem cs (GetCorner3() , dirx , GetNormal() ^ dirx , GetNormal());
	return cs;
}

VPoint VTriangleE::SpaceConversionFromTriangleToTriangle (const VPoint &p , const VTriangleE &tri1 , const VTriangleE &tri2)
{
	//CoordinateSystem csp = tri1.GetCs1(); 	
	//CoordinateSystem csd = tri2.GetCs1();
	//VPoint lpU = csp.AbsoluteCoordToRelativeCoord (tri1.GetCorner2());
	//VPoint lpV = csp.AbsoluteCoordToRelativeCoord (tri1.GetCorner3());
	//VPoint ldX = csd.AbsoluteCoordToRelativeCoord (tri2.GetCorner2());
	//VPoint ldY = csd.AbsoluteCoordToRelativeCoord (tri2.GetCorner3());
	//if ((tri1.GetEdge1()^tri1.GetEdge2())%tri1.GetNormal() < 0.0)
	//{
	//	// normals don't conform , 
	//	// Rotate second cs
	//	RotationMatrix R ( csd.GetOrigin() , csd.GetDirx() , 180.0);
	//	csd.Transform (R);
	//	ldX = csd.AbsoluteCoordToRelativeCoord (tri2.GetCorner2());
	//	ldY = csd.AbsoluteCoordToRelativeCoord (tri2.GetCorner3());
	//	VpAfxMessageBox ("rotating cs");
	//}
	//VPoint luv = csp.AbsoluteCoordToRelativeCoord (p);
	//double lu = luv.x() / lpU.x();
	//double lv = luv.y() / lpV.y();
	//double lx = ldX.x() * lu;
	//double ly = ldY.y() * lv;
	//VPoint convertedPoint = csd.RelativeCoordToAbsoluteCoord (VPoint(lx,ly,0.0));
	//if (tri2.IsInTriangle (convertedPoint) == false)
	//	LogFileWriter::WriteUnknownExceptionLog (("VTriangleE::SpaceConversionFromTriangleToTriangle warning,parameter point out of param tri!"));
	//return convertedPoint;
	double a1 = tri1.GetEdge1().x();
	double b1 = -tri1.GetEdge3().x();
	double c1 = tri1.GetCorner1().x() - p.x();
	double a2 = tri1.GetEdge1().y();
	double b2 = -tri1.GetEdge3().y();
	double c2 = tri1.GetCorner1().y() - p.y();
	double t,s;
	bool ck = LinearEquations::SolveLinear2Unknown (a1,b1,c1,a2,b2,c2,t,s);
	if (ck == false)
	{
		LogFileWriter::WriteUnknownExceptionLog ( IDS_VTrSpaGauSolFai);
	}
	
	VPoint solpoint ( tri2.GetCorner1() + tri2.GetEdge1 () * t - tri2.GetEdge3() * s );
	return solpoint;
}

VPoint VTriangleE::GetMinInCs(const CoordinateSystem &cs)const
{
	VPoint mn(MAX_NUMBER_LIMIT);
	mn.ReplaceMin(cs.AbsoluteCoordToRelativeCoord(GetCorner1()));
	mn.ReplaceMin(cs.AbsoluteCoordToRelativeCoord(GetCorner2()));
	mn.ReplaceMin(cs.AbsoluteCoordToRelativeCoord(GetCorner3()));
	return mn;
}
VPoint VTriangleE::GetMaxInCs(const CoordinateSystem &cs)const
{
	VPoint mx(MIN_NUMBER_LIMIT);
	mx.ReplaceMax(cs.AbsoluteCoordToRelativeCoord(GetCorner1()));
	mx.ReplaceMax(cs.AbsoluteCoordToRelativeCoord(GetCorner2()));
	mx.ReplaceMax(cs.AbsoluteCoordToRelativeCoord(GetCorner3()));
	return mx;
}

