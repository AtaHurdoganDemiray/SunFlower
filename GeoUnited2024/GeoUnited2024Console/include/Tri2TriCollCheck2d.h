// Tri2TriCollCheck2d.h

#ifndef __Tri2TriCollCheck2d_h__
#define __Tri2TriCollCheck2d_h__

struct VTriangle2dE;
struct VLine2d;
class PointSet2d;

class Tri2TriCollCheck2d
{
public:
	Tri2TriCollCheck2d();
	~Tri2TriCollCheck2d();
	bool CheckKoll ( const VTriangle2dE &tri1 , const VTriangle2dE &tri2 );
	bool DoesAnyEdgeIntersectToLine ( const VTriangle2dE &tri1 , const VLine2d &newline );
	bool GetIntersectionPoints 
	( const VTriangle2dE & tri1 , const VTriangle2dE & tri2 , PointSet2d & intp );
private:
	bool DoesAnyEdgesIntersect ( const VTriangle2dE &tri1 , const VTriangle2dE &tri2 );
	bool IsAnyTriCompletelyInOther ( const VTriangle2dE &tri1 , const VTriangle2dE &tri2 );

};

#endif //__Tri2TriCollCheck2d_h__






