// GrowingTriangles.h

#ifndef __GrowingTriangles_h__
#define __GrowingTriangles_h__

class HalfEdge;
class SurfPointsPackBucketed;
struct VPoint2d;
class SequentialLineSet2d;

class GrowingTriangles
{
public:
	GrowingTriangles (SurfPointsPackBucketed &surfPack);
	~GrowingTriangles();
	void DestroyHalfEdges();
	HalfEdge *GetPointLocation(int p , bool &isOnEdge , bool &isOnVertex);	
	void MakeInitialSquareTris (const std::vector<vreal> &ugrid , const std::vector<vreal> &vgrid);	// The square is from 0,0 to 1,1
	void SplitOneTriWithPointInside (HalfEdge *e , int p);
	void SplitOneEdgeWithPointOn (HalfEdge *e , int p);
	void InsertPoint (int p);
	void SplitEdgeLineIf (int p0,int p1, int loop0 , std::vector<VPoint2d> &intPoints);
	struct Loop
	{
	public:
		int m_i0,m_i1,m_i2;// indices are for half edges in vector m_halfEdges
	};
	struct TrimLoop
	{
	public:
		std::vector<int> m_ivec; // indices are for half edges in vector m_halfEdges
	};
	void InsertOuterTrimLoop (const SequentialLineSet2d &sl);
	void InsertInnerTrimLoop (const SequentialLineSet2d &sl);
private:
	SurfPointsPackBucketed &m_surfPack;
	std::vector<HalfEdge *> m_halfEdges;
	std::vector<Loop> m_loops;
	std::vector<TrimLoop> m_trimLoops;
	HalfEdge *m_midEdge;
	static double m_epsilon;
};

#endif