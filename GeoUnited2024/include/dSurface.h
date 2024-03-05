// dSurface.h

#ifndef __dSurface_h__
#define __dSurface_h__
#include "VRefTriangleMesh.h"
#include "SequentialLineSet2d.h"
#include "SequentialLineSet.h"
#include "LineIntersection2d.h"
#include "UntrimmedSurface.h"
#include "AnalyzeSurface.h"
#include "TransformationMatrix.h"

class UntrimmedSurface;
class ProcessedTrimmedSurface;
class ProcessedBoundedSurface;
class IgesTrimLoop2d;
class IgesTrimLoop;
class IgesEntity;
class IgesEntitySet;
class PosCubes2d;
class CurvePointGenerator;
class dCurve;
class SurfaceQuadTrimmer;
class MoveMatrix;
class ScaleMatrix;
#include "PointSet.h"
#include "LineSet.h"

namespace geo
{
	class BspTree;
};
class TriangleMesh2d;
struct VTriangle2dE;
struct RefTriangle;
class PlaneEntity;
class ProcessedFace;

class dSurface
{
public:
	friend class SurfaceBspTrimmer;
	friend class SurfacePolygonTrimmer;
	struct SurfTypeData
	{
	public:
		bool m_isClosedCylinder;
		vreal m_h,m_rad;
		VPoint m_center,m_ax;
		AnalyzeSurface::SurfaceType m_ck;
	};

	enum SurfaceType {none,planar,cylinder};
	enum SurfaceSubType {bspline,tabulatedcylinder,revolved,parametricspline,ruled,offset,plane};
	dSurface (const int meshsize , bool useposcubestrim=false);
	dSurface (IgesEntity *ent ,IgesEntitySet &entset , const vreal &tol,bool triangulate,bool changeBigSurfaceTol , double bigSurfaceLength , double bigSurfaceTol,bool useposcubestrim=false);
	dSurface (const vreal &tol , ProcessedTrimmedSurface &pts , bool triangulate,bool changeBigSurfaceTol , double bigSurfaceLength , double bigSurfaceTol,bool useposcubestrim=false);
	dSurface (const vreal &tol , ProcessedBoundedSurface &pbs , bool triangulate,bool changeBigSurfaceTol , double bigSurfaceLength , double bigSurfaceTol,bool useposcubestrim=false);
	dSurface (SurfacePointGenerator *uSurf , IgesEntitySet &entset);
	dSurface(const vreal &tol, ProcessedFace &pf, bool triangulate, bool changeBigSurfaceTol, double bigSurfaceLength, double bigSurfaceTol, bool useposcubestrim = false);
	virtual ~dSurface();
	VRefTriangleMesh * GetTri ();
	SequentialLineSet2d * GetOuterLoop ();
	std::vector<IgesEntity *> &  GetOuterLoop3d ();
	std::vector<SequentialLineSet2d *> & GetInnerLoopVec ();
	std::vector<std::vector<IgesEntity *>> & GetInnerLoop3dVec ();
	const VRefTriangleMesh * GetTri ()const;
	const SequentialLineSet2d * GetOuterLoop ()const;
	const std::vector<IgesEntity *> &  GetOuterLoop3d ()const;
	const std::vector<SequentialLineSet2d *> & GetInnerLoopVec ()const;
	const std::vector<std::vector<IgesEntity *>> & GetInnerLoop3dVec ()const;
	const VRefTriangleMesh * GetTrimmedTri ()const;
	VRefTriangleMesh * GetTrimmedTri();
	const UntrimmedSurface & GetSurface ()const;
	UntrimmedSurface & GetSurface ();
	int GetDeNo()const;
	void SetDeNo(int deno);
	void Transform (const TransformationMatrix &tm);
	LineSet & GetSnapLines();
	const LineSet & GetSnapLines()const;
	PointSet & GetSnapPoints();
	const PointSet & GetSnapPoints()const;
	VPoint2d GetMinPar()const;
	VPoint2d GetMaxPar()const;
	const LineSet * GetWireFrame ()const;
	void FormTrimmedTri (bool surfQuadTrimmerMakeBoundary);
	SurfaceQuadTrimmer * FormTrimmedTriTest ();
	bool IsPointTrimmed (const VPoint2d &p);
	void AllLoopsConstULineInt (const vreal &u,std::vector <VPoint2d> &sortedVec);
	void AllLoopsConstVLineInt (const vreal &v,std::vector <VPoint2d> &sortedVec);
	void AllLoopsLineInt (const VLine2d &l , std::vector<VPoint2d> &sortedVec);
	void ProjectModelSpaceTrimLoops (const SequentialLineSet &modelLoop 
									, SequentialLineSet2d &paramLoop
									, std::vector<VPoint> &witnessPoints);
	const SurfTypeData & GetSurfTypeData ()const;
	const VPoint GetMin()const;
	const VPoint GetMax()const;
	VRefTriangleMesh * MakeTriangleMesh (const vreal tol);
	const double GetTolerance ()const;
	static bool IsParLoopCCW (SequentialLineSet2d *sl,PosCubes2d *cubes);
	void ChangeToleranceForVeryBigSurfaces ();
	bool IsBigSurface ()const;
	void Retesselate (double tol);
	void RollbackPreviousTransform();
private:
	VRefTriangleMesh * CalculateTriangles (const vreal tol,bool surfQuadTrimmerMakeBoundary);
	vreal m_tol;
	VRefTriangleMesh *m_tri;
	VRefTriangleMesh *m_trimmedTri;
	VRefTriangleMesh *m_roughTri;
	std::vector<IgesEntity *> m_outerLoopVec;
	SequentialLineSet2d *p_outerLoop;
	std::vector <std::vector<IgesEntity *>> m_innerLoopVec;
	std::vector<SequentialLineSet2d*> p_innerLoopVec;
	PosCubes2d * cubesOuterLoop;
	std::vector<PosCubes2d *> cubesInnerLoopVec;
	UntrimmedSurface m_surf;
	SurfTypeData m_surftypedata;

	int m_deno;
	void DigitiseSurface (UntrimmedSurface &surf);
	void MakeLoops ( IgesTrimLoop &m_outerloop
					,  std::vector<IgesTrimLoop *> &m_innerloop 
					,  IgesEntitySet &allent , UntrimmedSurface &surf);
	void MakeLoops2d ( IgesTrimLoop2d &outerloop 
					,  std::vector<IgesTrimLoop2d *> &innerloop 
					,  IgesEntitySet &allent , UntrimmedSurface &surf);

	void DigitiseLoop ( IgesEntity *ent 
						, IgesEntitySet &allEnt
						, UntrimmedSurface &surf
						, SequentialLineSet2d &loop
						, bool isReversed);
	void DigitiseLoop3d ( IgesEntity *ent 
						  , IgesEntitySet &allEnt
						  , bool isReversed
						  , std::vector<dCurve *> &dcrvVec);
	bool IsLoopCurveReversed (CurvePointGenerator &loopCurve , const VPoint &lastPoint);
	void TrimTriangles (const UntrimmedSurface &surf);
	geo::BspTree * MakeBspTree (const SequentialLineSet2d &loop)const;
	geo::BspTree * GetTriangleBsp (int untrimmedTriangleIndex);
	void AddToTrimmedTriangles (int i , const TriangleMesh2d &tri);
	RefTriangle MakeTriangle ( const VTriangle2dE &t 
					      , const RefTriangle &r
						  , const VTriangle2dE &p
						  , const VTriangleE &m 
						  , const VTriangleE &n);
	int AddNewPointToTrianglesSurfPack (const VPoint2d &newp 
									  , const VTriangle2dE &p 
									  , const VTriangleE &m 
									  , const VTriangleE &n);
	void AddAllTrimLoopsToSurfPack (const UntrimmedSurface &surf);
	void AddTrimLoopToSurfPack (const SequentialLineSet2d &sl ,const UntrimmedSurface &surf);
	void MakeTrimmedTri (int mysize);
	void FillTrimmedTriSurfPack (const SurfPointsPack &surfpack 
								, const geo::BspTree *outerLoop 
								, const std::vector<geo::BspTree *> &innerLoopVec);
	bool IsPointTrimmed (const VPoint2d &p 
						, const geo::BspTree *outerLoop 
						, const std::vector<geo::BspTree *> &innerLoopVec);
	int outerLoopInsideSign;
	std::vector<int> innerLoopsInsideSign;
	void SetTrimLoopInsideSigns (const geo::BspTree *outerLoop 
								, const std::vector<geo::BspTree *> &innerLoopVec);
	void TrimPointsAndDelaunayTriangulate (const UntrimmedSurface &surf 
											, geo::BspTree *outerTrimLoop 
											, std::vector<geo::BspTree *> &innerTrimLoopVec);
	LineIntersection2d calc;
	void MakeLoopsForBoundedSurf (ProcessedBoundedSurface &pbs);
	bool ShouldIReverse (const CurvePointGenerator &crv , const SequentialLineSet2d &sl , const UntrimmedSurface &surf);
	void MakeLoopsForPlaneSurface ( IgesTrimLoop &m_outerloop
									, std::vector<IgesTrimLoop *> &m_innerloop 
									, IgesEntitySet &allent , UntrimmedSurface &surf);
	void Make2dLoopsFrom3d (ProcessedTrimmedSurface *pts , IgesEntitySet &entset);
	void UseNewTrim();
	LineSet * m_wireframe;
	PointSet  m_snapPoints;
	LineSet m_snapLines;
	void MakeUSection (const vreal &u , int vno , LineSet &lines);
	void MakeSect (const VPoint2d &p1,const VPoint2d &p2,int stepno,LineSet &lines);
	void MakeVSection (const vreal &v , int uno , LineSet &lines);
	void DoPolygonTriangulation();
	vreal m_parSpaceTol;
	void CalcParSpaceTol();
	void ReverseOuterLoopIf ();
	void ReverseInnerLoopIf (int seqref);
	bool m_useposcubestrim;
	void GetParamLoop (const SequentialLineSet &loop , SequentialLineSet2d &paramLoop);
	void MyTwoDLoopTriangulate ();
	int GetDivideCount (const vreal tol);
	bool DoesIntervalCrossGrids (const vreal i0,const vreal i1
								,const std::vector<vreal> &grids);
	bool InsertParIfNoCrossToQuads (const SequentialLineSet2d *loop,std::vector<vreal> &ugrids,const std::vector<vreal> &vgrids);
	void MakeBoundaryCurvesFromSQT (SurfaceQuadTrimmer &sqt);
	//void ExtendGridsIf (std::vector<vreal> &ugrid , std::vector<vreal> &vgrid , UntrimmedSurface &surf , SequentialLineSet2d *outerLoop);
	//bool ShiftSurfaceParIf (const SequentialLineSet2d *outerLoop , UntrimmedSurface &surf);
	//void ShiftAllLoops (UntrimmedSurface &surf);
	//void ShiftSingleLoop (SequentialLineSet2d *curloop , UntrimmedSurface &surf);
	bool m_changeBigSurfaceTol; 
	double m_bigSurfaceLength; 
	double m_bigSurfaceTol;
	VRefTriangleMesh *m_trimmedTriBackUp;
	bool m_isTransformedBackUp;
	TransformationMatrix m_tmBackUp;
public:
	void AddTrimLoopToPointVector (const SequentialLineSet2d &sl , const UntrimmedSurface &surf , std::vector<VPoint> &vec,const VPoint &mymn , const VPoint &mymx);
private:
	//std::map<double,VRefTriangleMesh *> m_tolTriMap;
	std::map<double,VRefTriangleMesh *> m_tolTrimmedTriMap;
public:
	//std::map<double,VRefTriangleMesh *> & GetTolTriMap();
	std::map<double,VRefTriangleMesh *> & GetTolTrimmedTriMap();
	void BuildWireFrame ();
	VRefTriangleMesh * GetRoughTri();
	void SetRoughTri (VRefTriangleMesh *roughTri);
	SurfaceSubType GetSurfaceSubType()const;
	void UpdateBSplineControlPointsIf();
	void One3dLoopToSl(std::vector<IgesEntity *> loopVec, SequentialLineSet &merged,IgesEntitySet &allent);
	void Make2dLoopsFrom3d(ProcessedFace &pf);
	void One3dLoopToSl(std::vector<dCurve*> &dcrvVec , SequentialLineSet &merged);
private:
	bool m_transformParametricLoop;
	MoveMatrix *m_MParametricLoop;
	ScaleMatrix *m_SParametricLoop;
public:
	void TransformParametricLoopIf(int surfEntityType , SequentialLineSet2d *loop);
	void CalculateLoopTransformationIf(int surfEntityType , SequentialLineSet2d *outerLoop);
};
#endif