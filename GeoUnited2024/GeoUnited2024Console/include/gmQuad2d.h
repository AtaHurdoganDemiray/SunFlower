/******************************************************************************
*               File: gmQuad2d.h											  *
*******************************************************************************
*               Description:                                                  *
*                                                                             *
*******************************************************************************
*               History:                                                      *
*  06.01.2004 11:19:45 Created by:    Ata Hurdogan Demiray                    *
*******************************************************************************
*               (C) 2003 by Positive GmbH		                               *
******************************************************************************/

#ifndef __gmQuad2d_h__
#define __gmQuad2d_h__

#include "VPoint2d.h"
struct VLine2d;

struct gmQuad2d
{
public:
	gmQuad2d ( const VPoint2d &cor1 , const VPoint2d &cor2 , const VPoint2d &cor3 , const VPoint2d &cor4 );
	gmQuad2d (const VPoint2d &mn , const VPoint2d &mx);
	gmQuad2d ( const gmQuad2d & Foo );
	gmQuad2d & operator = ( const gmQuad2d & Foo )	;
	
	~gmQuad2d();
	
	bool IsInQuad ( const VPoint2d &apoint ) const;
	//bool IsIntersectingWithEdges 
	//	( const VLine2d & l , int & intno , VPoint2d intp[] , vreal sofline[] ) const;	
	//void EdgeLineIntersection 
	//	( const VLine2d & l , int edgeno , int & intno , VPoint2d intp[] , vreal sofline[] ) const;
	//bool IsIntersectingWithDiagonal ( const VLine2d &l , int & intno , VPoint2d intp[] , vreal sofline[] ) const;

	VPoint2d GetMin();
	VPoint2d GetMax();
	const VPoint2d &GetMin() const;
	const VPoint2d &GetMax() const;
	VPoint2d GetCenter () const;
	const VPoint2d &GetCor1() const;
	const VPoint2d &GetCor2() const;
	const VPoint2d &GetCor3() const;
	const VPoint2d &GetCor4() const;
	
	VPoint2d GetCor1();
	VPoint2d GetCor2();
	VPoint2d GetCor3();
	VPoint2d GetCor4();
	
	const VPoint2d &GetEdge1() const;
	const VPoint2d &GetEdge2() const;
	const VPoint2d &GetEdge3() const;
	const VPoint2d &GetEdge4() const;
	
	VPoint2d GetEdge1();
	VPoint2d GetEdge2();
	VPoint2d GetEdge3();
	VPoint2d GetEdge4();
	
	void UpdateMinMax();
	
private:
	VPoint2d m_cor1 , m_cor2 , m_cor3 , m_cor4;
	VPoint2d m_edge1 , m_edge2 , m_edge3 , m_edge4;
	VPoint2d min , max;

};

#endif //__gmQuad2d_h__
