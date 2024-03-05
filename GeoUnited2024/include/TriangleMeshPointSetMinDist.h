// TriangleMeshPointSetMinDist.h

#ifndef __TriangleMeshPointSetMinDist_h__
#define __TriangleMeshPointSetMinDist_h__

#include "PosCubes.h"
class MultiTriangles;
class MultiPoints;

struct VPoint;

class TriangleMeshPointSetMinDist
{
public:
	TriangleMeshPointSetMinDist (const MultiTriangles &triangles , const MultiPoints &points);
	~TriangleMeshPointSetMinDist ();
	void MinDistAllTrianglesAndAllPoints (std::vector <vreal> &allmindist 
											, LineSet &minlines)const;
	void MinDistAllTrianglesAndAllPoints (std::vector <vreal> &allmindist 
											, std::vector<int> &triangleind
											, std::vector<VPoint> &witnessPoints)const;
protected:
	vreal GetMinDist (const VPoint &p 
						, const LongVec &relTriangleInd 
						, VPoint &witnessPoint)const;
	int GetMinDist (const VPoint &p 
					, const LongVec &relTriangleInd 
					, vreal &minDist
					, VPoint &witnessPoint)const;
	const MultiTriangles * m_triangles;
	const MultiPoints * m_points;
	PosCubes *m_trianglesCubes;
	PosCubes *m_pointsCubes;
};

#endif