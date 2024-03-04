// TwoDLoopTriangulator.h

#ifndef __TwoDLoopTriangulator_h__
#define __TwoDLoopTriangulator_h__

#include "VPoint2d.h"

class SequentialLineSet2d;
class VRefTriangleMesh;
namespace geo
{
	class BspTree;
};

class TwoDLoopTriangulator
{
public:
	TwoDLoopTriangulator (const SequentialLineSet2d *outerLoop,
		const std::vector <SequentialLineSet2d *> &innerLoopVec,
		const std::vector<vreal> &ugrid,
		const std::vector<vreal> &vgrid);
	~TwoDLoopTriangulator ();
	VRefTriangleMesh * Triangulate ();
	VRefTriangleMesh * TriangulateOuterLoop (const vreal step);
	std::vector<SequentialLineSet2d *> loops;
private:
	VRefTriangleMesh * m_res;
	const SequentialLineSet2d *m_outerLoop;
	const std::vector <SequentialLineSet2d *> &m_innerLoopVec;
	geo::BspTree *m_bspOuter;
	std::vector <geo::BspTree *> m_bspInnerVec;

	void MakeBspTrees ();
	geo::BspTree * MakeSingleBsp (const SequentialLineSet2d *loop);

	const std::vector<vreal> &m_ugrid;
	const std::vector<vreal> &m_vgrid;

	struct Quad
	{
		Quad (const vreal leftu , const vreal rightu,
			const vreal tipv , const vreal topv );
	
		VPoint2d c[4];
	};
	std::vector<Quad> m_qv;

	void MakeQuads ();
	geo::BspTree * MakeQuadBsp (const Quad &q);
	geo::BspTree * QuadBspLoopsInt (geo::BspTree *quadbsp);
	void TriangulateQuadLoop (std::vector<int> &polygonloop);
	void TriangulateLoopInsertPoints (std::vector<int> &polygonloop,const vreal step);
	void DivideTriTill (const int i[3],const vreal step,std::vector<int> &tris,int &recCount);

};

#endif