// SurfaceQuadTrimmer2.h

#ifndef __SurfaceQuadTrimmer2_h__
#define __SurfaceQuadTrimmer2_h__
#include "PosCubes2d.h"
#include "VPoint.h"

struct VPoint2d;
struct VLine2d;
class SequentialLineSet2d;
class VRefTriangleMesh;
class UntrimmedSurface;
class dSurface;

class SurfaceQuadTrimmer2
{
public:
	SurfaceQuadTrimmer2 (const std::vector <vreal> &ugrid
						,const std::vector<vreal> &vgrid
						,UntrimmedSurface &surf
						,const SequentialLineSet2d *outerLoop
						,const std::vector<SequentialLineSet2d *> &innerLoopVec
						,dSurface &dsurf);
	VRefTriangleMesh * Triangulate ();
public:
	typedef std::vector<vreal>::const_iterator Itr;
private:
	const std::vector<vreal> &m_ugrid;
	const std::vector<vreal> &m_vgrid;
	UntrimmedSurface &m_surf;
	const SequentialLineSet2d *m_outerLoop;
	const std::vector<SequentialLineSet2d *> &m_innerLoopVec;
	dSurface &m_dsurf;
private:
	int m_uno;
	int m_vno;
	int m_quno;
	int m_qvno;
	std::vector<std::vector<int>> m_qi;
	std::vector<std::vector<int>> m_qv;
	std::vector<std::vector<int>> m_ql;
	VRefTriangleMesh *m_res;
	const Itr m_ub;
	const Itr m_ue;
	const Itr m_vb;
	const Itr m_ve;
	PosCubes2d m_qcub;
	VPoint m_fa;
private:
	int GetQIndex (const int quno , const int qvno)const;
	void GetQCoord (const int qi , int &quno , int &qvno)const;
	int CastUCoord (const vreal ucoord)const;
	int CastVCoord (const vreal vcoord)const;
	VLine2d GetQTop (const int quno , const int qvno)const;
	VLine2d GetQBottom (const int quno ,const int qvno)const;
	VLine2d GetQLeft (const int quno , const int qvno)const;
	VLine2d GetQRight (const int quno , const int qvno)const;
	VPoint2d GetQUpperLeft (const int quno,const int qvno)const;
	VPoint2d GetQUpperRight (const int quno,const int qvno)const;
	VPoint2d GetQLowerLeft (const int quno,const int qvno)const;
	VPoint2d GetQLowerRight (const int quno,const int qvno)const;
	VPoint2d GetQMid (const int quno , const int qvno)const;
	bool IsInsideQ (const int quno,const int qvno,const VPoint2d &p)const;

	void AddLoop (const SequentialLineSet2d *loop);
	void AddQPointsIf ();
	void AddQVertex (const VPoint2d &p , const int u,const int v);
	void AddRightQVertex (const VPoint2d &p,const int u,const int v);
	void AddUpQVertex (const VPoint2d &p,const int u,const int v);
	void AddUpRightQVertex (const VPoint2d &p,const int u,const int v);
	bool IsExiting (const VLine2d &l , const VPoint2d &intp , const int qu , const int qv)const;
	void AddIntPoint (const VLine2d &l
						,const VPoint2d &intp
						,const int currqu
						,const int currqv
						,const int currqi
						,const int qi_second);
	void TriangulateQuad (const int qi);
	void FindLoopQuadIntPoints ();
	void FindLoopQuadIntVer ();
	void FindLoopQuadIntHor ();
	void CollectUntrimmedQVertexHor (const std::vector<VPoint2d> &svec);

public:
	struct SortStruct
	{
	public:
		std::vector<int> m_rvec;
		vreal m_teta;
		void SetTeta(const VPoint2d &p
					,const VPoint2d &qmid);
	};
};

#endif