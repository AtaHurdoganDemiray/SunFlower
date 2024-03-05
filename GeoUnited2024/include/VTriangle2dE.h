/******************************************************************************
*               File: VTriangle2dE.h											  *
*******************************************************************************
*               Description:                                                  *
*                                                                             *
*******************************************************************************
*               History:                                                      *
*  01.07.2003 16:11:30 Created by:    Ata Hurdogan Demiray                    *
*******************************************************************************
*               (C) 2003 by Pozitif GmbH                                      *
******************************************************************************/

#ifndef __VTriangle2dE_h__
#define __VTriangle2dE_h__

#include "VTriangle2d.h"
class Bifstream;
class Bofstream;

struct VTriangle2dE
{
public:
	VTriangle2dE ();
	VTriangle2dE 
		( const VPoint2d & cor1 , const VPoint2d & cor2 , const VPoint2d & cor3 );
	VTriangle2dE ( vreal cor1[3] , vreal cor2[3] , vreal cor3[3] );
	VTriangle2dE ( const VTriangle2dE &Foo );
	VTriangle2dE ( VIFStream & infile );
	~VTriangle2dE();
	VTriangle2dE & operator = ( const VTriangle2dE &Foo );
	const VPoint2d & GetCorner1() const;
	const VPoint2d & GetCorner2() const;
	const VPoint2d & GetCorner3() const;
	VPoint2d & GetCorner1();
	VPoint2d & GetCorner2();
	VPoint2d & GetCorner3();

	const VPoint2d & GetEdge1() const;
	const VPoint2d & GetEdge2() const;
	const VPoint2d & GetEdge3() const;
	
	VPoint2d & GetEdge1();
	VPoint2d & GetEdge2();
	VPoint2d & GetEdge3();

	const VPoint2d & operator[] ( int index ) const;
	//VPoint2d GetMin() const;
	//VPoint2d GetMax() const;
	void SetEdges ();
	void SetNormal ();
	void SetCorner1 ( const vreal newcorner[] );
	void SetCorner2 ( const vreal newcorner[] );
	void SetCorner3 ( const vreal newcorner[] );
	bool IsInTriangle ( const VPoint2d &apoint ) const;
	//int IsInTriangleTol ( const VPoint2d &apoint , const vreal & offsettol ) const;//return value : 0 out 1,2,3 on edges 4 in triangle
	VPoint2d GetCenterOfMass() const;
	bool IsPointOnVertices ( const VPoint2d & p  , const vreal & tol ) const;
	void operator << ( VIFStream & infile );
	void operator >> ( VofStream & outfile ) const;
	void Serialize (Bofstream &binfile) const;
	void Serialize (Bifstream &binfile);
	VTriangle2dE (Bifstream &binfile);	
	
	bool Compare ( const VTriangle2dE & comp , const vreal & tol ) const;

	/*
	inline bool IsCorrupted () const
	{
		if ( fabs( edge1^edge2 ) < EPSILON )
			return false;
		
		vreal per2 = edge1.GetLength2() + edge2.GetLength2() + edge3.GetLength2();
		
		if ( per2 > EPSILON )
			return false;

		return true;//exception 
	};
	*/
	bool IsCorrupted ()const;
	vreal GetArea () const;
	vreal GetPerimeter () const;
	bool operator == (const VTriangle2dE &toCompare)const;
	bool operator != (const VTriangle2dE &toCompare)const;
	void UpdateMinMax(VPoint2d &minp,VPoint2d &maxp)const;
	void GetUV (const VPoint2d &p , vreal &u , vreal &v)const;
	VPoint2d GetP (const vreal &u , const vreal &v)const;
	VTriangle2d & GetTri ();
	const VTriangle2d & GetTri()const;
	bool IsNeighbor (const VTriangle2dE &t , VPoint2d &common1 , VPoint2d &common2)const;
	sVector<double,2> GetMin()const;
	sVector<double,2> GetMax()const;
	bool IsInTriangle (const VPoint2d &apoint , double tol);
private:
	//VPoint2d corner1;
	//VPoint2d corner2;
	//VPoint2d corner3;
	VTriangle2d m_tri;
	VPoint2d edge1;
	VPoint2d edge2;
	VPoint2d edge3;

};

#endif //__VTriangle2dE_h__
