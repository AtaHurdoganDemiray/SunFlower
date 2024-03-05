// LinePointMinDist.h

#ifndef __LinePointMinDist_h__
#define __LinePointMinDist_h__


struct VPoint;
struct VLine;

class LinePointMinDist
{
public:
	vreal MinDist ( const VLine & line , const VPoint & p , VPoint & pointonline ) const;
	bool PointVSEdge ( const VPoint & lp , const VPoint & lv , const VPoint & p , vreal & d , VPoint & pointonedge ) const;
	vreal PointVSVertex ( const VPoint & vp , const VPoint & p ) const;
	double InfiniteLinePointMinDist (const VPoint & lp , const VPoint & lv , const VPoint & p) const;
	double InfiniteLinePointMinDist (const VPoint & lp , const VPoint & lv , const VPoint & p , vreal &s) const;
};

#endif