// TrianglePointMinDist.h

#ifndef __TrianglePointMinDist_h__
#define __TrianglePointMinDist_h__


struct VPoint;
struct VTriangleE;

class TrianglePointMinDist
{
public:
	vreal MinDist ( const VTriangleE & tri , const VPoint & p , VPoint & pointontri ) const;
	bool PointVSTriPlane ( const VTriangleE & tri , const VPoint & p , vreal & d , VPoint & pointonplane ) const; 
	bool PointVSEdge ( const VPoint & lp , const VPoint & lv , const VPoint & p , vreal & d , VPoint & pointonedge ) const;
	vreal PointVSVertex ( const VPoint & vp , const VPoint & p ) const;

};

#endif