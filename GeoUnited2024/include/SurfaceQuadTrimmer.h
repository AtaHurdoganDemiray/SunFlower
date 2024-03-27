// SurfaceQuadTrimmer.h

#ifndef __SurfaceQuadTrimmer_h__
#define __SurfaceQuadTrimmer_h__
#include "VPoint2d.h"
#include <map>
class UntrimmedSurface;
class SequentialLineSet2d;
class VRefTriangleMesh;
struct VLine2d;
struct VLine2dE;
class PosCubes2d;
struct RefTriangle;
//#include "PointSet2d.h"
namespace geo
{
	class BspTree;
};

class SurfaceQuadTrimmer
{
public:
	SurfaceQuadTrimmer (bool mydebug , UntrimmedSurface &surf);
	SurfaceQuadTrimmer (const std::vector<vreal> &ugrid
						, const std::vector<vreal> &vgrid
						,UntrimmedSurface &surf
						, SequentialLineSet2d *outerLoop
						, const std::vector<SequentialLineSet2d*> &innerLoopVec
						, PosCubes2d *cubesOuterLoop
						, std::vector<PosCubes2d *> &cubesInnerLoopVec
						,bool useposcubesfortrim=false
						,bool makeboundarycurve = false);
	enum IsInLoopAlgo {bsp,poscubes};
	virtual ~SurfaceQuadTrimmer();
	VRefTriangleMesh * TrimSurface();
	static bool RegularLineInt (const VLine2d &l1,const VLine2d &l2,VPoint2d &intp);
	bool IsPointInLoop ( const VPoint2d &p
								, const SequentialLineSet2d *sl 
								, PosCubes2d *cubes);
	struct rqp
	{
	public:
		friend class std::vector<rqp>;
		rqp (int myloopno
			,vreal mylpointno
			,int myrefi
			,int myqeno)
			:loopno(myloopno),lpointno(mylpointno),refi(myrefi),qeno(myqeno)
		{};
		int loopno;
		vreal lpointno;
		int refi;
		int qeno;
	//private:
		rqp ()
		:loopno(-1),lpointno(-1),refi(-1),qeno(-1)
		{};
	};
	struct qsort
	{
	public:
		qsort (const std::vector<VPoint2d> *myvec,const VPoint2d &myqcent , int p)
		:m_vec(myvec),m_qcent(myqcent)
		{
			m_seq.push_back (p);		
		};
		const std::vector<VPoint2d> *m_vec;
		VPoint2d m_qcent;
		std::list<int> m_seq;
	};

private:
	struct RefQuad
	{
		RefQuad()
		{
			mi[0]=-1;mi[1]=-1;mi[2]=-1;mi[3]=-1;
		};
		// quads top-right corner is mi1 , top-left corner is mi2
		// quads bottom-left corner is mi3, bottom right corner is mi4
		int mi[4];//1,mi2,mi3,mi4;
	};
	
	int m_uno;
	int m_vno;
	int m_qno;

	std::list<rqp> *m_q;
	std::vector<vreal> m_ugrid;
	std::vector<vreal> m_vgrid;
	typedef std::vector<vreal>::const_iterator Itr;
	
	RefQuad *m_qvrefmeshi;
	//static VRefTriangleMesh *m_res;
	
	struct Mesh
	{
		Mesh ();
		int AddPoint (const VPoint2d &p);
		int AddTri (const RefTriangle &t);
		const std::vector<VPoint2d> & Vec()const;	
		std::vector<VPoint2d> & GetVec();
		VRefTriangleMesh * GetRes (UntrimmedSurface &surf);
		void Clear();
	private:
		int m_usedp;
		int m_usedtri;
		//for each point a list of tri index is kept for all tris that use the point
		std::vector<std::list<int>> m_rel;
		// point pool
		std::vector<VPoint2d> m_vec;
		// triangles
		std::vector<RefTriangle> m_tri;
		// temp vec for making VRefTriangleMesh
		std::vector<VPoint2d> m_temp;

		void ExpandVector();
	};

	Mesh m_res;

	void LoopQuadsInt (const SequentialLineSet2d *loop,const int loopno);
	int CastUCoord (const double &ucoord);
	int CastVCoord (const double &vcoord);
	VLine2d GetQTop (int quno , int qvno);
	VLine2d GetQBottom (int quno , int qvno);
	VLine2d GetQLeft (int quno , int qvno);
	VLine2d GetQRight (int quno , int qvno);
	VPoint2d GetQUpperLeft (int quno,int qvno);
	VPoint2d GetQUpperRight (int quno,int qvno);
	VPoint2d GetQLowerLeft (int quno,int qvno);
	VPoint2d GetQLowerRight (int quno,int qvno);
	bool IsQIndexValid (const int qi)const;
	bool IsInQ (int quno,int qvno,const VPoint2d &p);
	bool IsPointTrimmed (const VPoint2d &p);
	UntrimmedSurface &m_surf;
	SequentialLineSet2d *m_outerLoop;
	const std::vector<SequentialLineSet2d*> &m_innerLoopVec;
	PosCubes2d *m_cubesOuterLoop;
	std::vector<PosCubes2d *> &m_cubesInnerLoopVec;
	void RecordQuadLinesInt (const VLine2dE &l 
							,const int loopno
							,const int lpointno);
	void AddQuadCorners ();
	void MakeBsp ();
	geo::BspTree * m_bspouter;
	std::vector<geo::BspTree *> m_bspinnervec;
	IsInLoopAlgo m_isinloopalgo;
	bool m_useposcubesfortrim;
	int AddMyTriangle (const RefTriangle &rt);
	void TriangulateQuadLoop (std::vector<int> &polygonloop);
	vreal m_lpointnoOffs;
	vreal GetLpointNo (const int lpointno,const vreal intp , const int qno );
	void DoItOldDelaunayWay (int myqno);
	void IsLoopInsideQuad (const SequentialLineSet2d *loop , vreal &uToAdd , vreal &vToAdd);
	void IsIntervalInsideQuad (const vreal pmin , const vreal pmax
								, const std::vector<vreal> &grid
								, vreal &toAdd );
	bool SingleQTriangulate (const int qno);
	bool CacheQEdgePars (const int qno , std::vector<std::list<rqp>> &sequences
						 ,std::vector<vreal> &startQEdgeParVec
						 ,std::vector<vreal> &endQEdgeParVec);
	int GetNearestSequence (const int qno
							,const int curri 
							,const std::vector<vreal> &startQEdgeParVec
							,const std::vector<vreal> &endQEdgeParVec
							,int quadCornersBetween[4]
							,bool quadCornerExist[4]);
	//gets sequence that start with nearest quad edge parameter to latest sequence end

	int GetSequence(const std::vector<bool> &usedVec);
	void AddSequenceToPolygon (std::vector<std::list<rqp>> &sequences
								,const int currs
								,std::vector<int> &polyLoop);
	void AddSorted (std::list<rqp> &qlist,rqp &q);
	void MakeSequences (const std::list<rqp> &ql , std::vector<std::list<rqp>> &sequences);
	void GetSortedQListForQuad (const int qno,std::list<rqp> &sortedl);
	vreal m_uStartPar,m_vStartPar,m_uEndPar,m_vEndPar;
	enum BoundaryParanthesis {open,closed};
	BoundaryParanthesis m_boundaryParanthesis;
	int m_loopBeginsQuadNo;

	public:
		// Boundary curve extraction data
		std::vector<std::vector<VPoint2d>> m_boundaryCurves;
	
		struct sortboupoints
		{
		public:
			double par;
			VPoint2d solidp;
		};
		std::vector<sortboupoints> m_localIntPar;
		bool m_makeboundarycurve;
};

#endif