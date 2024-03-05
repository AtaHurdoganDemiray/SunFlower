#include "stdafx.h"
#include "VPoint2d.h"
#include "VPolygon.h"
#include "Math.h"
#include "SequentialLineSet2d.h"

//
//VPolygon::VPolygon(void)
//{
//}

VPolygon::~VPolygon(void)
{
}
//VPolygon::VPolygon(std::vector<VPoint2d> Vec)
//{
//	m_vector = Vec;
//}

VPolygon::VPolygon(const std::vector<VPoint2d> &pointPool, std::vector<int> &polygonPoints)
:m_pointPool(&pointPool)
,m_polygonPoints(polygonPoints)
{

}

const VPoint2d & VPolygon::GetPoint(int i)const
{
	int index = m_polygonPoints[i];
	return m_pointPool->operator [](index);
}

double VPolygon::Kross2d(VPoint2d &sv1, VPoint2d &sv2)const
{
	return ( sv1.x() * sv2.y() ) - ( sv1.y() * sv2.x() ); 
}

bool VPolygon::SegmentInCone(const VPoint2d &v0, const VPoint2d &v1, const VPoint2d &vM, const VPoint2d &vP)const
{
	// assert: vN, v0, vP are not collinear

	VPoint2d diff = v1 - v0;
	VPoint2d edgeL = vM - v0;
	VPoint2d edgeR = vP - v0;
	//vreal krs = edgeR ^ edgeL;
	if( Kross2d(edgeR, edgeL) > 0 )	//vertex is convex
	{
		bool c1 = (Kross2d( diff, edgeR ) < 0) ;
		bool c2 = (Kross2d( diff, edgeL ) > 0) ;
		return ( Kross2d( diff, edgeR ) < 0 && Kross2d( diff, edgeL ) > 0 );
	}
	else							//vertex is reflex
		return ( Kross2d( diff, edgeR ) < 0 || Kross2d( diff, edgeL ) > 0 );

}

bool VPolygon::SegmentIntersect( const VPoint2d &P0, const VPoint2d &D0, const VPoint2d &P1, const VPoint2d &D1 )const
{
	VPoint2d E = P1 - P0;

	double kross = D0.x()*D1.y() - D0.y()*D1.x();
	double sqrKross = kross*kross;
	double sqrLen0 = D0.x()*D0.x() + D0.y()*D0.y();
	double sqrLen1 = D1.x()*D1.x() + D1.y()*D1.y();
	double sqrEpsilon = 1E-12;
	
	if( sqrKross > sqrEpsilon * sqrLen0 * sqrLen1 )
	{
		//lines of segments are not parellel
		double s = (E.x()*D1.y() - E.y()*D1.x()) / kross;
		if( s < 0 || s > 1) //intersection of lines is not a point on segment P0 + s * D0
			return false;

		double t = (E.x()*D0.y() - E.y()*D0.x()) / kross;
		if( t < 0 || t > 1 )//intersection of lines is not a point on segment P1 + t * D0
			return false;

		else
			return true;	
	}

	//lines of segments are parellel
	double sqrLenE = E.x()*E.x() + E.y()*E.y();
	kross = E.x() * D0.y() - E.y()*D0.x();
	sqrKross = kross*kross;
	if( sqrKross > sqrEpsilon * sqrLen0 * sqrLenE ) //lines of segments are different
		return false;
	
	return false; //lines of segments are the same. Need to test for overlap of segments

}

bool VPolygon::IsDiagonal(int i0, int i1)const
{
	int n = (int)(m_polygonPoints.size());	
	if (abs (i0-i1) < 2)
		return false;
	if (i0 == 0 && i1 == n-1)
		return false;
	if (i0 == n-1 && i1 == 0)
		return false;


	int iM = (i0-1) % n;
	if ( iM < 0 )
		iM += n;
	int iP = (i0+1) % n;
	
	bool ck = SegmentInCone(GetPoint(i0), GetPoint(i1), GetPoint(iM), GetPoint(iP));
	if( ck == false )
		return false;

	//test segment i0,i1 to see if it is a diagonal
	int j0,j1;
	for(j0 = 0, j1 = n-1; j0 < n; j1 = j0, j0++)
	{
		if (j0 != i0 && j0 != i1 && j1 != i0 && j1 != i1) 
		{
			if(SegmentIntersect(GetPoint(i0),GetPoint(i1)-GetPoint(i0), GetPoint(j0), GetPoint(j1)-GetPoint(j0)))
				return false;
		}
	}

	return true;
}

void VPolygon::Triangulate(std::vector<VPolygon> &triangles, VPolygon &left, VPolygon &right)
{
 	int n = (int)(m_polygonPoints.size());
	if( n == 3 )
	{
		triangles.push_back(*this);
		return;
	}
	int i0, i1;
	if(GetADiagonal(i0, i1))
	{
		left = GetLeftPolygon(i0, i1);
		right = GetRightPolygon(i0, i1);
	}
}
void VPolygon::Triangulate(std::vector<VPolygon> &triangles)
{
 	int n = (int)(m_polygonPoints.size());
	if( n == 3 )
	{
		triangles.push_back(*this);
		return;
	}
	int i0, i1;
	if(GetADiagonal(i0, i1))
	{
		VPolygon left = GetLeftPolygon(i0, i1);
		VPolygon right = GetRightPolygon(i0, i1);
		left.Triangulate(triangles);
		right.Triangulate(triangles);
	}
}

bool VPolygon::GetADiagonal(int &i0, int &i1)
{
	int n = (int)(m_polygonPoints.size());
	for(int i = 0; i < n; i++) {
		for(int j = i+2; j < n; j++) {
			if(IsDiagonal( i, j))
			{
				i0 = i;
				i1 = j;
				return true;
			}
		}
	}
	return false;
}

VPolygon VPolygon::GetRightPolygon(int i0, int i1)
{	
	std::vector<int> points;
	int x = i1;
	int n = (int)(m_polygonPoints.size());
	points.push_back(m_polygonPoints[i1]);
	while ( x != i0)
	{
		x = (x+1)%n;
		points.push_back(m_polygonPoints[x]);
	}

	VPolygon p(*m_pointPool, points);
	return p;
}

VPolygon VPolygon::GetLeftPolygon(int i0, int i1)
{
	std::vector<int> points;
	int x = i0;
	int n = (int)(m_polygonPoints.size());
	points.push_back(m_polygonPoints[i0]);
	while ( x != i1)
	{
		x = (x+1)%n;
		points.push_back(m_polygonPoints[x]);
	}
	
	VPolygon p(*m_pointPool, points);
	return p;
}

int VPolygon::GetVertexCount() const
{
	return (int)(m_polygonPoints.size());
}

//void VPolygon::RemoveCollinear(VPolygon &vPol)
//{
//	bool ck = true;
//	bool check = true;
//	int size = (int)(m_pointPool->size());
//	for( int i = 0; i < size; ++i )
//	{
//		ck = true;
//		if ( i == size-2)
//		{
//			const VPoint2d &vp1 = m_pointPool->at(i);
//			const VPoint2d &vp2 = m_pointPool->at(i+1);
//			vPol.m_pointPool->push_back(vp1);
//			vPol.m_pointPool->push_back(vp2);
//			i++;
//		}
//		else if ( i == size-1 )
//		{
//			const VPoint2d &vp1 = m_pointPool->at(i);
//			vPol.m_pointPool->push_back(vp1);
//		}
//		if ( check == false )
//			break;
//	
//		VPoint2d &vp1 = m_pointPool->at(i);
//		vPol.m_pointPool->push_back(vp1);
//
//		for( int k = i+1; ck  == true; )
//		{
//			//VPoint2d &vp1 = m_pointPool->at(i);
//			VPoint2d &vp2 = m_pointPool->at(k);
//			VPoint2d &vp3 = m_pointPool->at((k+1)%(size));
//			
//			if(	fabs(((vp1.x())*(vp2.y()-vp3.y()) + 
//				(vp2.x())*(vp3.y()-vp1.y()) + 
//				(vp3.x())*(vp1.y()-vp2.y()))) < 1e-8)
//			{
//				i= k-1;
//			}
//			else
//			{
//				ck = false;
//				i= k-1;
//			}
//			if ( k+1 != (k+1)%(size-1) )
//				check = false;
//			k = (k+1)%(size);
//			
//		}
//	}
//	
//	int size2 = (int)(vPol.m_pointPool->size());
//	for ( int j = 0; j < size2 ; j++ )
//	{
//		vPol.m_polygonPoints.push_back(j);
//	}
//}

SequentialLineSet2d * VPolygon::MakeSequentialLineSet() 
{
	int mysize = (int)m_polygonPoints.size();
	SequentialLineSet2d * sl = new SequentialLineSet2d (mysize);
	for (int i=0;i<mysize;++i)
	{
		sl->InsertPoint ((*m_pointPool) [m_polygonPoints[i]]);
	}	
	return sl;
}

bool VPolygon::PolygonIsConvex() const 
{
	int mysize = (int)m_polygonPoints.size();
	for (int i=0;i<mysize;++i)
	{
		for (int j=i+2;j<mysize;++j)
		{
			if (i==0 && j==mysize-1)
				continue;
			if (IsDiagonal (i,j) == false)
				return false;
		}
	}
	return true;
}

bool VPolygon::IsCCWPolygonConvex () const
{
	int mysize = (int)m_polygonPoints.size();
	for (int i=0;i<mysize-2;++i)
	{
		int i0 = i;
		int i1 = i+1;
		int i2 = i1 +1;

		VPoint2d v1 = (*m_pointPool)[m_polygonPoints[i1]] - (*m_pointPool)[m_polygonPoints[i0]];
		VPoint2d v2 = (*m_pointPool)[m_polygonPoints[i2]] - (*m_pointPool)[m_polygonPoints[i1]];
		if ( (v1 ^ v2) < 0.0)
			return false;
	}
	return true;
}
