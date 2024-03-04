// dSurface.cpp

#include "stdafx.h"
#include "dSurface.h"
#include "UntrimmedSurface.h"
#include "ProcessedTrimmedSurface.h"
#include "ProcessedBoundedSurface.h"
//#include "SurfaceDigitiser.h"
#include "CurvePointGenerator.h"
#include "CurvePointGeneratorFactory.h"
//#include "CurveDigitiser.h"
#include "BspTreeBooleanOp.h"
#include "BspTree.h"
#include "BspTreeBooleanOp.h"
#include "BspTreeToLineSet2d.h"
#include "LineSet2d2SequentialLineSet2d.h"
#include "PolygonToTriangles.h"
#include "TriangleMesh2d.h"
#include "VDelaunay2d.h"
//#include "TrimInParSpace.h"
#include "gmTriangleUtility.h"
#include "BspTreeConstructor.h"
#include "LineSet2d.h"
#include "PosCubes2d.h"
#include "SequentialLineSet.h"
#include "DoubleVec.h"
#include "PlaneEntity.h"
#include "IgesEntitySet.h"
#include "PlaneEntityPointGenerator.h"
#include "TriangleMeshPointSetMinDist.h"
#include "MultiTriangles.h"
#include "MultiPoints.h"
#include "sPoints.h"
#include "dCurve.h"
#include "TrimmedSurface.h"
#include "BoundedSurface.h"
#include "CopiousData.h"
#include "SurfaceTrimmer.h"
//#include "AnalyzeSurface.h"
#include "SurfaceTrimmerDelaunay.h"
#include "SurfaceQuadTrimmer.h"
#include "SurfacePolygonTrimmer.h"
#include "SurfaceQuadBspTrimmer.h"
#include "SurfaceQuadTrimmer2.h"
#include "VPolygon.h"
#include "LoopLineInt.h"
#include "TrimLoopVerifier.h"
#include "ThreeDLoopToSurfParSpace.h"
#include "TwoDLoopTriangulator.h"
#include "CopiousDataFactory.h"
#include "CompositeCurve.h"
#include "UntypedException.h"
#include <stdexcept>
#include "SurfaceSpace.h"
#include "ViewPointDefaults.h"
#include "BSplineSurfacePointGenerator.h"
#include "dCurve.h"
#include "LoopSort.h"
#include "PlaneSurfacePointGenerator.h"
#include "ProcessedFace.h"
#include "Face.h"
#include "RightCylindricalSurfacePointGenerator.h"
#include "RightConicalSurfacePointGenerator.h"
#include "ScaleMatrix.h"
#include "MoveMatrix.h"

#define BSP_TRIM 0

dSurface::dSurface(const int meshsize,bool useposcubestrim)
	:m_useposcubestrim(useposcubestrim), m_transformParametricLoop(false), m_MParametricLoop(0), m_SParametricLoop(0), m_snapPoints(10), m_snapLines(1)
{
	m_trimmedTri = new VRefTriangleMesh (meshsize);
}

dSurface::dSurface (IgesEntity *ent , IgesEntitySet &entset , const vreal &tol,bool triangulate,bool changeBigSurfaceTol , double bigSurfaceLength , double bigSurfaceTol,bool useposcubestrim)
:m_tol(tol) , m_surf (ent , entset ) , m_deno (ent->directorylineno),m_wireframe (0),m_useposcubestrim(useposcubestrim),m_changeBigSurfaceTol(changeBigSurfaceTol),m_bigSurfaceLength(bigSurfaceLength),m_bigSurfaceTol(bigSurfaceTol)
, m_roughTri(0), m_transformParametricLoop(false), m_MParametricLoop(0), m_SParametricLoop(0), m_snapPoints(10), m_snapLines(1)
{
		ChangeToleranceForVeryBigSurfaces();
		CalcParSpaceTol();		
		m_tri = 0;
		m_trimmedTri = 0;
		p_outerLoop = 0;
		
		int c1i,c2i,c3i,c4i;
		UntrimmedSurface::IsoParametricType t1 (UntrimmedSurface::v0uchanging);
		CopiousData * c1 = m_surf.MakeIsoParametricCurve (t1,entset,tol,c1i);
		c1->DE.pColor = ent->DE.pColor;
		//m_outerLoopVec.push_back (c1);

		UntrimmedSurface::IsoParametricType t2 (UntrimmedSurface::u1vchanging);
		CopiousData * c2 = m_surf.MakeIsoParametricCurve (t2,entset,tol,c2i);
		c2->DE.pColor = ent->DE.pColor;
		//m_outerLoopVec.push_back (c2);
		
		UntrimmedSurface::IsoParametricType t3 (UntrimmedSurface::v1uchanging);
		CopiousData * c3 = m_surf.MakeIsoParametricCurve (t3,entset,tol,c3i);
		c3->DE.pColor = ent->DE.pColor;
		//m_outerLoopVec.push_back (c3);

		UntrimmedSurface::IsoParametricType t4 (UntrimmedSurface::u0vchanging);
		CopiousData * c4 = m_surf.MakeIsoParametricCurve (t4,entset,tol,c4i);
		c4->DE.pColor = ent->DE.pColor;
		//m_outerLoopVec.push_back (c4);

		SequentialLineSet2d sli(4);
		sli.InsertPoint (VPoint2d (0,0));
		sli.InsertPoint (VPoint2d (1,0));
		sli.InsertPoint (VPoint2d (1,1));
		sli.InsertPoint (VPoint2d (0,1));
		sli.InsertPoint (VPoint2d (0,0));
		CopiousDataFactory myfact;
		CopiousData *c5 = myfact.Make(sli);

		// Add iges entities first
		int c5i = entset.InsertEntity(c5);

		// Make the IgesLoops
		IgesTrimLoop2d mytl2d;
		mytl2d.loopcurves.push_back(c5);
		IgesTrimLoop mytl3d;
		mytl3d.loopcurves.push_back(c1);
		mytl3d.m_isReversed.push_back(false);
		mytl3d.loopcurves.push_back(c2);
		mytl3d.m_isReversed.push_back(false);
		mytl3d.loopcurves.push_back(c3);
		mytl3d.m_isReversed.push_back(true);
		mytl3d.loopcurves.push_back(c4);
		mytl3d.m_isReversed.push_back(true);

		std::vector<IgesTrimLoop2d *>innerLoopVec;
		std::vector<IgesTrimLoop *>innerLoopVec3d;
		MakeLoops2d(mytl2d,innerLoopVec,entset,m_surf);
		MakeLoops (mytl3d,innerLoopVec3d,entset,m_surf);
		// Make a composite curve for outerLoop3d
		CompositeCurve *compout = new CompositeCurve;
		compout->N = 4;
		compout->mDE.push_back(c1i);
		compout->mDE.push_back(c2i);
		compout->mDE.push_back(c3i);
		compout->mDE.push_back(c4i);
		int compouti = entset.InsertEntity(compout);
		
		// Make curve on parametric surfaces for outer loop and inner loop as NULL
		


		if (triangulate)
		{
			FormTrimmedTri(false);
		}
		else 
		{
			int stepno = 30;
			m_wireframe = new LineSet (3 * stepno);
			MakeUSection (0.5,stepno,*m_wireframe);
			MakeVSection (0.5,stepno,*m_wireframe);
		}


}	
dSurface::dSurface (SurfacePointGenerator *uSurf , IgesEntitySet &entset)
:m_tol(0.1) , m_surf (uSurf ) , m_deno (0),m_wireframe (0),m_useposcubestrim(true),m_changeBigSurfaceTol(false),m_bigSurfaceLength(1000.0),m_bigSurfaceTol(1.)
, m_roughTri(0), m_transformParametricLoop(false), m_MParametricLoop(0), m_SParametricLoop(0),m_snapPoints(10), m_snapLines(1)
{
		ChangeToleranceForVeryBigSurfaces();
		CalcParSpaceTol();		
		m_tri = 0;
		m_trimmedTri = 0;
		p_outerLoop = 0;
		
		int c1i,c2i,c3i,c4i;
		vreal tol = 0.1;
		UntrimmedSurface::IsoParametricType t1 (UntrimmedSurface::v0uchanging);
		CopiousData * c1 = m_surf.MakeIsoParametricCurve (t1,entset,tol,c1i);
		//c1->DE.pColor = ent->DE.pColor;

		UntrimmedSurface::IsoParametricType t2 (UntrimmedSurface::u1vchanging);
		CopiousData * c2 = m_surf.MakeIsoParametricCurve (t2,entset,tol,c2i);
		//c2->DE.pColor = ent->DE.pColor;
		
		UntrimmedSurface::IsoParametricType t3 (UntrimmedSurface::v1uchanging);
		CopiousData * c3 = m_surf.MakeIsoParametricCurve (t3,entset,tol,c3i);
		//c3->DE.pColor = ent->DE.pColor;

		UntrimmedSurface::IsoParametricType t4 (UntrimmedSurface::u0vchanging);
		CopiousData * c4 = m_surf.MakeIsoParametricCurve (t4,entset,tol,c4i);
		//c4->DE.pColor = ent->DE.pColor;

		SequentialLineSet2d sli(4);
		sli.InsertPoint (VPoint2d (0,0));
		sli.InsertPoint (VPoint2d (1,0));
		sli.InsertPoint (VPoint2d (1,1));
		sli.InsertPoint (VPoint2d (0,1));
		sli.InsertPoint (VPoint2d (0,0));
		CopiousDataFactory myfact;
		CopiousData *c5 = myfact.Make(sli);

		// Add iges entities first
		int c5i = entset.InsertEntity(c5);

		// Make the IgesLoops
		IgesTrimLoop2d mytl2d;
		mytl2d.loopcurves.push_back(c5);
		IgesTrimLoop mytl3d;
		mytl3d.loopcurves.push_back(c1);
		mytl3d.m_isReversed.push_back(false);
		mytl3d.loopcurves.push_back(c2);
		mytl3d.m_isReversed.push_back(false);
		mytl3d.loopcurves.push_back(c3);
		mytl3d.m_isReversed.push_back(true);
		mytl3d.loopcurves.push_back(c4);
		mytl3d.m_isReversed.push_back(true);

		std::vector<IgesTrimLoop2d *>innerLoopVec;
		std::vector<IgesTrimLoop *>innerLoopVec3d;
		MakeLoops2d(mytl2d,innerLoopVec,entset,m_surf);
		MakeLoops (mytl3d,innerLoopVec3d,entset,m_surf);
		//// Make a composite curve for outerLoop3d
		CompositeCurve *compout = new CompositeCurve;
		compout->N = 4;
		compout->mDE.push_back(c1i);
		compout->mDE.push_back(c2i);
		compout->mDE.push_back(c3i);
		compout->mDE.push_back(c4i);
		int compouti = entset.InsertEntity(compout);
		
		// Make curve on parametric surfaces for outer loop and inner loop as NULL
		

		FormTrimmedTri(false);


}	

dSurface::dSurface (const vreal &tol , ProcessedTrimmedSurface &pts , bool triangulate,bool changeBigSurfaceTol , double bigSurfaceLength , double bigSurfaceTol,bool useposcubestrim)
:m_tol(tol) , m_surf (pts.GetSurfaceIgesEntity() , pts.allent),m_deno(pts.GetTrimmedSurface().directorylineno),m_wireframe(0),m_changeBigSurfaceTol(changeBigSurfaceTol),m_bigSurfaceLength(bigSurfaceLength),m_bigSurfaceTol(bigSurfaceTol)
,m_roughTri(0)
, m_useposcubestrim(useposcubestrim), m_transformParametricLoop(false), m_MParametricLoop(0), m_SParametricLoop(0), m_snapPoints(10), m_snapLines(1)
{
		ChangeToleranceForVeryBigSurfaces();
		CalcParSpaceTol();
		m_tri = 0;
		m_trimmedTri = 0;
		p_outerLoop = 0;
		
		int etyp = pts.GetSurfaceIgesEntity()->entitytype;
		//  if plane entity or r
		//if (/*pts.Is2dLoopsGiven() == false && */ (etyp == 108 || etyp == 190 ||etyp == 194|| etyp == 120) &&
		if(pts.Is2dLoopsGiven() == false)
		{
			//this is a plane entity
			//MakeLoopsForPlaneSurface (pts.m_outerloop,pts.m_innerloop,pts.allent,m_surf);
			//throw VException (( IDS_PlaSurNotImpYet),1);
			//if (pts.GetSurfaceIgesEntity()->entitytype == 154)
			//{
			//	VpAfxMessageBox("Right circular cylinder entity");
			//	throw VException("Right circular cylinder entity not implemented",1);
			//}
			LogFileWriter::WriteUnknownExceptionLog("dSurface from 144 , trim parametric curves not given , projecting 3D");
			MakeLoops(pts.m_outerloop, pts.m_innerloop, pts.allent, m_surf);
			Make2dLoopsFrom3d(&pts , pts.allent);
			LogFileWriter::WriteUnknownExceptionLog("Projection done");
		}
		else
		{
			MakeLoops (pts.m_outerloop , pts.m_innerloop , pts.allent , m_surf);
			if (pts.Is2dLoopsGiven () == true)
				MakeLoops2d (pts.outerloop , pts.innerloop , pts.allent , m_surf);
			else
			{
				throw VException (( IDS_DSuDLooNotGiv),1);
				//Make2dLoopsFrom3d (pts.allent);
			}
		}

		if (triangulate)
		{
			FormTrimmedTri(false);
		}
		else
		{
			int stepno = 30;
			VPoint2d mymini = GetMinPar();
			VPoint2d mymaxi = GetMaxPar();
			VPoint2d mymidi = (mymaxi + mymini) * 0.5;
			
			m_wireframe = new LineSet (stepno*3);
			MakeUSection (mymidi.x(),stepno,*m_wireframe);
			MakeVSection (mymidi.y(),stepno,*m_wireframe);
		}
		//UseNewTrim();
}
dSurface::dSurface(const vreal &tol, ProcessedFace &pf, bool triangulate, bool changeBigSurfaceTol, double bigSurfaceLength, double bigSurfaceTol, bool useposcubestrim)
	:m_tol(tol), m_surf(pf.GetSurfaceIgesEntity(), pf.allent), m_deno(pf.GetFace().directorylineno), m_wireframe(0), m_changeBigSurfaceTol(changeBigSurfaceTol), m_bigSurfaceLength(bigSurfaceLength), m_bigSurfaceTol(bigSurfaceTol)
	, m_roughTri(0)
	, m_useposcubestrim(useposcubestrim), m_transformParametricLoop(false), m_MParametricLoop(0), m_SParametricLoop(0), m_snapPoints(10), m_snapLines(1)
{
	//ChangeToleranceForVeryBigSurfaces();
	//CalcParSpaceTol();
	m_parSpaceTol = 0.001;
	m_tri = 0;
	m_trimmedTri = 0;
	p_outerLoop = 0;

	if (pf.Is2dLoopsGiven() == false)
	{
		Make2dLoopsFrom3d(pf);
	}
	else
	{
		m_transformParametricLoop = true;
		SequentialLineSet merged(100);
		One3dLoopToSl(pf.m_outerLoop, merged);

		if (pf.GetSurfaceIgesEntity()->entitytype == 108)
		{
			PlaneEntityPointGenerator *pepg = static_cast<PlaneEntityPointGenerator *>(&m_surf.GetDigitiser());
			pepg->SetMinMax(merged);
		}
		if (pf.GetSurfaceIgesEntity()->entitytype == 190)
		{
			PlaneSurfacePointGenerator *pspg = static_cast<PlaneSurfacePointGenerator *>(&m_surf.GetDigitiser());
			pspg->SetMinMax(merged);
		}
		if (pf.GetSurfaceIgesEntity()->entitytype == 192)
		{
			//VpAfxMessageBox("Right circular cylinder surface");
			//throw VException("Right circular cylinder surface not implemented",1);
			RightCylindricalSurfacePointGenerator *rcpg = static_cast<RightCylindricalSurfacePointGenerator *>(&m_surf.GetDigitiser());
			rcpg->DecideH(merged);
		}
		if (pf.GetSurfaceIgesEntity()->entitytype == 194)
		{
			RightConicalSurfacePointGenerator *rcs = static_cast<RightConicalSurfacePointGenerator *>(&m_surf.GetDigitiser());
			rcs->DecideH(merged);
		}
		MakeLoops2d(pf.outerloop, pf.innerloop, pf.allent, m_surf);
	}
	//ChangeToleranceForVeryBigSurfaces();
	//CalcParSpaceTol();

	if (triangulate)
	{
		FormTrimmedTri(false);
	}
	else
	{
		int stepno = 30;
		VPoint2d mymini = GetMinPar();
		VPoint2d mymaxi = GetMaxPar();
		VPoint2d mymidi = (mymaxi + mymini) * 0.5;

		m_wireframe = new LineSet(stepno * 3);
		MakeUSection(mymidi.x(), stepno, *m_wireframe);
		MakeVSection(mymidi.y(), stepno, *m_wireframe);
	}
	//UseNewTrim();
}
dSurface::dSurface (const vreal &tol , ProcessedBoundedSurface &pbs ,bool triangulate,bool changeBigSurfaceTol , double bigSurfaceLength , double bigSurfaceTol,bool useposcubestrim)
:m_tol(tol) , m_surf (pbs.GetSurfaceIgesEntity() , pbs.allent ),m_deno(pbs.GetBoundedSurface().directorylineno),m_wireframe(0),m_changeBigSurfaceTol(changeBigSurfaceTol),m_bigSurfaceLength(bigSurfaceLength),m_bigSurfaceTol(bigSurfaceTol)
, m_roughTri(0), m_transformParametricLoop(false), m_MParametricLoop(0), m_SParametricLoop(0), m_snapPoints(10), m_snapLines(1)
,m_useposcubestrim(useposcubestrim)
{
	ChangeToleranceForVeryBigSurfaces ();
	CalcParSpaceTol();
		m_tri = 0;
		m_trimmedTri = 0;
		p_outerLoop = 0;
		if (pbs.GetSurfaceIgesEntity()->entitytype == 108)
		{
			//this is a plane surface 
			throw VException (( IDS_PlaSurNotImpYet),1);
		}
		else
		{
			MakeLoopsForBoundedSurf (pbs);	
		}
		if (triangulate)
		{
			FormTrimmedTri(false);
		}
		else
		{
			int stepno = 30;
			VPoint2d mymini = GetMinPar();
			VPoint2d mymaxi = GetMaxPar();
			VPoint2d mymidi = (mymaxi + mymini) * 0.5;
			
			m_wireframe = new LineSet (stepno*3);
			MakeUSection (mymidi.x(),stepno,*m_wireframe);
			MakeVSection (mymidi.y(),stepno,*m_wireframe);
		}
}

dSurface::~dSurface()
{
	if (m_tri == m_trimmedTri)
	{
		//delete m_tri;
	}
	else
	{
		delete m_tri;
		//delete m_trimmedTri;
	}
	std::map<double,VRefTriangleMesh*>::iterator i=m_tolTrimmedTriMap.begin();
	std::map<double,VRefTriangleMesh*>::iterator e=m_tolTrimmedTriMap.end();
	for (i;i!=e;++i)
	{
		delete i->second;
	}
	delete p_outerLoop;
	delete cubesOuterLoop;
	int mySize1 = static_cast<int>(p_innerLoopVec.size());
	for (int i = 0; i<mySize1; i++)
	{
		const SequentialLineSet2d *psls = p_innerLoopVec[i];
		PosCubes2d *cbs = cubesInnerLoopVec[i];
		delete psls;
		delete cbs;
	}

}

void dSurface::DigitiseSurface (UntrimmedSurface &surf)
{
	m_tri = surf.Digitise (m_tol);
}

void dSurface::MakeLoops ( IgesTrimLoop &m_outerloop
				, std::vector<IgesTrimLoop *> &m_innerloop 
				, IgesEntitySet &allent , UntrimmedSurface &surf)
{	
	int size2 = static_cast<int>(m_outerloop.loopcurves.size());
	for (int i=0; i<size2; i++)
	{
		IgesEntity *ent =m_outerloop.loopcurves[i];
		//DigitiseLoop3d (ent,allent,false,m_outerLoopVec);
		m_outerLoopVec.push_back (ent);
	}
	// Inner Loops 3d
	int size5 = static_cast<int>(m_innerloop.size());
	for (int j=0; j<size5; j++)
	{
		IgesTrimLoop *tl = m_innerloop[j];
		m_innerLoopVec.push_back(std::vector<IgesEntity *>());
		int size6 = static_cast<int>(tl->loopcurves.size());
		for (int w=0; w<size6; w++)
		{
			IgesEntity *ent = tl->loopcurves[w];
			//DigitiseLoop3d (ent,allent,false,m_innerLoopVec);
			m_innerLoopVec.back().push_back (ent);
		}
	}
	
}

void dSurface::MakeLoops2d (IgesTrimLoop2d &outerloop 
							, std::vector<IgesTrimLoop2d *> &innerloop 
							, IgesEntitySet &allent , UntrimmedSurface &surf)
{
	//2D LOOPS
	//OrderSequentialLineSets<SequentialLineSet2d,VPoint2d>order;
	//OrderSequentialLineSets<SequentialLineSet,VPoint>order3d;
	p_outerLoop = new SequentialLineSet2d (100);
	int size1 = static_cast<int>(outerloop.loopcurves.size());
	//std::vector<SequentialLineSet2d> tmploopsouter (size1,100);
	if (size1 == 0)
	{
		p_outerLoop->InsertPoint(VPoint2d(surf.U0(),surf.V0()));
		p_outerLoop->InsertPoint(VPoint2d (surf.U1(),surf.V0()));
		p_outerLoop->InsertPoint(VPoint2d(surf.U1(),surf.V1()));
		p_outerLoop->InsertPoint(VPoint2d(surf.U0(),surf.V1()));
		p_outerLoop->InsertPoint(VPoint2d(surf.U0(),surf.V1()));
		//throw VException (("dSurface::MakeLoops2d exception,no outer loop"),1);
	}
	for (int i=0; i<size1; i++)
	{
		IgesEntity *ent =outerloop.loopcurves[i];
		//SequentialLineSet2d &currsl = tmploopsouter[i];
		SequentialLineSet2d otmp (20);
		DigitiseLoop (ent ,allent,surf,otmp,false);
		if (i==0)
		{
			p_outerLoop->AddBack (otmp);
		}
		else
		{
			// Sometimes curves need to be reversed to make loop
			VPoint2d dv1 = *p_outerLoop->End() - *otmp.Start();
			VPoint2d dv2 = *p_outerLoop->End() - *otmp.End();
			if (dv1.GetLength2() <= dv2.GetLength2()) 
			{
				p_outerLoop->AddBack(otmp);
			}
			else
			{
				otmp.Reverse();
				p_outerLoop->AddBack (otmp);
			}
		}
	}
	if (p_outerLoop->End()->Compare (*p_outerLoop->Start(),EPSILON)==false)
	{
		// Last point should be equal to first
		p_outerLoop->InsertPoint (*p_outerLoop->Start());
	}
	IgesEntity *surfIges = allent.GetEntityPointer (surf.GetSurfDe());
	if (surfIges == 0)
		throw VException("dSurface::MakeLoops2d exception , surface entity null",1);
	int surfEntityType = surfIges->entitytype;
	CalculateLoopTransformationIf(surfEntityType,p_outerLoop);
	TransformParametricLoopIf(surfEntityType,p_outerLoop);
	cubesOuterLoop = new PosCubes2d (*p_outerLoop);
	ReverseOuterLoopIf ();
	
	// Inner Loops 2d
	int size3 = static_cast<int>(innerloop.size());
	for (int j=0; j<size3; j++)
	{
		SequentialLineSet2d *loop = new SequentialLineSet2d (100);
		IgesTrimLoop2d *tl = innerloop[j];
		int size4 = static_cast<int>(tl->loopcurves.size());
		//std::vector<SequentialLineSet2d>tmpinnerloops (size4,100);
		
		for (int w=0; w<size4; w++)
		{
			IgesEntity *ent = tl->loopcurves[w];
			SequentialLineSet2d itmp(20);
			DigitiseLoop (ent,allent,surf,itmp,false);
			if (w == 0)
			{
				loop->AddBack(itmp);
			}
			else
			{
				// Sometimes curves need to be reversed for looping
				VPoint2d dv1 = *loop->End() - *itmp.Start();
				VPoint2d dv2 = *loop->End() - *itmp.End();
				if (dv1.GetLength2() <= dv2.GetLength2())
				{
					loop->AddBack(itmp);
				}
				else
				{
					itmp.Reverse();
					loop->AddBack(itmp);
				}
			}
		}
		//SequentialLineSet2d *loop = order.MakeOrderedUnion(tmpinnerloops);
		if (loop->End()->Compare (*loop->Start(),EPSILON) == false)
		{
			// last point isnot equal to first point
			loop->InsertPoint (*loop->Start());
		}

		p_innerLoopVec.push_back (loop);
		TransformParametricLoopIf(surfEntityType,loop);
		PosCubes2d *currcubes = new PosCubes2d (*loop);
		cubesInnerLoopVec.push_back(currcubes);
		ReverseInnerLoopIf (j);
	}
}

void dSurface::DigitiseLoop (IgesEntity *ent 
							  , IgesEntitySet &allEnt
							  , UntrimmedSurface &surf
							  , SequentialLineSet2d &loop
							  ,bool isReversed)
{
	CurvePointGenerator *crv = CurvePointGeneratorFactory::MakeCurve (ent , allEnt);
	if (isReversed)
		crv->SetReversed (true);
	bool shouldIswap = ShouldIReverse (*crv,loop,surf);//needed for trimmedsurf
	if (shouldIswap)
		crv->SetReversed (!isReversed);
	DoubleVec params;
	//crv->Digitise (m_parSpaceTol , surf.U0() , surf.U1() , surf.V0() , surf.V1() , loop , params);
	crv->Digitise(m_parSpaceTol , surf , loop , params);
}

void dSurface::DigitiseLoop3d (IgesEntity *ent 
							  , IgesEntitySet &allEnt
							  , bool isReversed
							  , std::vector<dCurve *> &dcrvVec)
{
	dCurve *newdcrv = new dCurve (m_tol , /*VGlobalValues::viewpointdefaults.m_quickDisplayTol ,*/ ent , &allEnt , isReversed);
	dcrvVec.push_back (newdcrv);
}

bool dSurface::IsLoopCurveReversed (CurvePointGenerator &loopCurve , const VPoint &lastPoint)
{
	VPoint curveFirstP = loopCurve.GetPoint (0.0);
	VPoint curveLastP = loopCurve.GetPoint (1.0);

	if (lastPoint.Compare(curveFirstP , 1.e-8))
	{
		return false;
	}
	else if (lastPoint.Compare (curveLastP,1.e-8))
	{
		return true;
	}
	else
		throw VException (( IDS_TriLooCurAreSep) ,1);
}

VRefTriangleMesh * dSurface::GetTri ()
{
	return m_tri;
}

SequentialLineSet2d * dSurface::GetOuterLoop ()
{
	return p_outerLoop;
}

std::vector<IgesEntity *> &dSurface::GetOuterLoop3d ()
{
	return m_outerLoopVec;
}

std::vector<SequentialLineSet2d *> & dSurface::GetInnerLoopVec ()
{
	return p_innerLoopVec;
}

std::vector<std::vector<IgesEntity *>> & dSurface::GetInnerLoop3dVec ()
{
	return m_innerLoopVec;
}

const VRefTriangleMesh * dSurface::GetTri ()const
{
	return m_tri;
}

const SequentialLineSet2d * dSurface::GetOuterLoop ()const
{
	return p_outerLoop;
}

const std::vector<IgesEntity *> &  dSurface::GetOuterLoop3d ()const
{
	return m_outerLoopVec;
}

const std::vector<SequentialLineSet2d *> & dSurface::GetInnerLoopVec ()const
{
	return p_innerLoopVec;
}

const std::vector<std::vector<IgesEntity *>> & dSurface::GetInnerLoop3dVec ()const
{
	return m_innerLoopVec;
}

void dSurface::TrimTriangles (const UntrimmedSurface &surf)
{
	//TrimInParSpace trim;
	//std::list<VTriangle2dE> *myresult = trim.Trim (p_outerLoop , p_innerLoopVec);
	//std::list<VTriangle2dE>::const_iterator i = myresult->begin();
	//std::list<VTriangle2dE>::const_iterator e = myresult->end();
	//gmTriangleUtility ut;
	//for (i;i!=e;i++)
	//{
	//	const VPoint2d &p1 = (*i).GetCorner1();
	//	const VPoint2d &p2 = (*i).GetCorner2();
	//	const VPoint2d &p3 = (*i).GetCorner3();
	//	VPoint m1,m2,m3;
	//	VPoint n1,n2,n3;
	//	m1 = surf.GetPoint (p1.x(),p1.y());
	//	n1 = surf.GetNormal (p1.x(),p1.y(),m1);	
	//	m2 = surf.GetPoint (p2.x(),p2.y());
	//	n2 = surf.GetNormal (p2.x(),p2.y(),m2);
	//	m3 = surf.GetPoint (p3.x(),p3.y());
	//	n3 = surf.GetNormal (p3.x(),p3.y(),m3);
	//	m_trimmedTri->surfpack.InsertPoint (p1,m1,n1);
	//	m_trimmedTri->surfpack.InsertPoint (p2,m2,n2);
	//	m_trimmedTri->surfpack.InsertPoint (p3,m3,n3);
	//	int lastI = m_trimmedTri->surfpack.parampoints.Size() - 1;
	//	RefTriangle newref (lastI,lastI-1,lastI-2);
	//	ut.AddTriangle (m_trimmedTri->surfpack , newref , *m_trimmedTri);
	//}
	//return;
		
#if BSP_TRIM
	geo::BspTree *outerTrimLoop = MakeBspTree (*p_outerLoop);
#else
	geo::BspTree *outerTrimLoop =0; //= MakeBspTree (*p_outerLoop);
#endif

	std::vector<geo::BspTree *> innerTrimLoopVec;
	int size1 = static_cast<int>(p_innerLoopVec.size());

#if BSP_TRIM
	for (int i=0; i<size1; i++)
	{
		SequentialLineSet2d *innerloop = p_innerLoopVec.at(i);
		geo::BspTree * innerTrimLoop = MakeBspTree (*innerloop);
		innerTrimLoopVec.push_back(innerTrimLoop);
	}

	SetTrimLoopInsideSigns (outerTrimLoop , innerTrimLoopVec);
#endif

	TrimPointsAndDelaunayTriangulate (surf , outerTrimLoop , innerTrimLoopVec);

	delete outerTrimLoop;
	int mysize = (int)innerTrimLoopVec.size();
	for (int i=0;i<mysize;i++)
	{
		geo::BspTree *bt = innerTrimLoopVec[i];
		delete bt;
	}
}

geo::BspTree * dSurface::GetTriangleBsp (int untrimmedTriangleIndex)
{
	const VPoint2d &c1 = m_tri->GetParamTriCor1(untrimmedTriangleIndex);
	const VPoint2d &c2 = m_tri->GetParamTriCor2(untrimmedTriangleIndex);
	const VPoint2d &c3 = m_tri->GetParamTriCor3(untrimmedTriangleIndex);

	VLine2d l1 (c1,c2);
	VLine2d l2 (c2,c3);
	VLine2d l3 (c3,c1);

	LineSet2d tmp(3);
	tmp.AddLine (l1);
	tmp.AddLine (l2);
	tmp.AddLine (l3);

	geo::BspTreeConstructor calc;
	geo::BspTree *triTree = calc.ConstructTree (tmp);
	return triTree;
}

geo::BspTree * dSurface::MakeBspTree (const SequentialLineSet2d &loop)const
{
	int lsize = loop.Size();
	LineSet2d tmp (lsize);
	for (int i=0;i<lsize;i++)
	{
		VLine2d lc = loop.operator [](i);
		//if (lc.GetV().GetLength2() > 1.e-16)
		tmp.AddLine (lc);
	}
	geo::BspTreeConstructor calc;
	geo::BspTree *result = calc.ConstructTree (tmp);
	return result;
}

void dSurface::AddToTrimmedTriangles (int i, const TriangleMesh2d &tri)
{
	if (tri.Size() == 0)
		return;

	const RefTriangle r = m_tri->GetRefTri (i);
	const VPoint2d &p1 = m_tri->GetParamTriCor1(i);
	const VPoint2d &p2 = m_tri->GetParamTriCor2(i);
	const VPoint2d &p3 = m_tri->GetParamTriCor3(i);
	const VPoint &c1 = m_tri->GetModelTriCor1(i);
	const VPoint &c2 = m_tri->GetModelTriCor2(i);
	const VPoint &c3 = m_tri->GetModelTriCor3(i);
	const VPoint &n1 = m_tri->GetCor1Normal(i);
	const VPoint &n2 = m_tri->GetCor2Normal(i);
	const VPoint &n3 = m_tri->GetCor3Normal(i);
	
	VTriangle2dE p (p1,p2,p3);
	VTriangleE m (c1,c2,c3);
	VTriangleE n (n1,n2,n3);

	const VTriangle2dE *t = tri[0];
	if (t->GetCorner1() == p1 &&
		t->GetCorner2() == p2 &&
		t->GetCorner3() == p3)
	{
		//do nothing triangle is same
	}
	else
	{
		RefTriangle newref = MakeTriangle (*t,r,p,m,n);	
		m_tri->SetRefTri(i,newref);
	}

	for (int i=1;i<tri.Size();i++)
	{
		const VTriangle2dE *t = tri[i];
		RefTriangle newtri = MakeTriangle (*t,r,p,m,n);
		m_tri->AddTriangle (newtri);
	}
}

RefTriangle dSurface::MakeTriangle (  const VTriangle2dE &t 
							   , const RefTriangle &r
							   , const VTriangle2dE &p
							   , const VTriangleE &m 
							   , const VTriangleE &n)
{
	int r1=-1,r2=-1,r3=-1;
	if (t.GetCorner1() == p.GetCorner1())
	{
		r1 = r.ref0;
	}
	if (t.GetCorner2() == p.GetCorner2())
	{
		r2 = r.ref1;
	}
	if (t.GetCorner3() == p.GetCorner3())
	{
		r3 = r.ref2;
	}

	if (r1 == -1)
	{
		r1 = AddNewPointToTrianglesSurfPack (t.GetCorner1(),p,m,n);
	}
	if (r2 == -1)
	{
		r2 = AddNewPointToTrianglesSurfPack (t.GetCorner2(),p,m,n);
	}
	if (r3 == -1)
	{
		r3 = AddNewPointToTrianglesSurfPack (t.GetCorner3(),p,m,n);
	}
	RefTriangle newtri (r1,r2,r3);
	return newtri;
}

int dSurface::AddNewPointToTrianglesSurfPack (const VPoint2d &newp 
											  , const VTriangle2dE &p 
											  , const VTriangleE &m 
											  , const VTriangleE &n)
{
	vreal u,v;
	p.GetUV(newp,u,v);
	VPoint newm = m.GetP(u,v);
	VVector newn = n.GetP(u,v);
	newn.NormThis();
	m_tri->surfpack.InsertPoint (newp , newm , newn);
	return (m_tri->surfpack.parampoints.Size() - 1);
}

const vreal DSURFACE_BOUNDARY_FREQ = 3.0;
void dSurface::AddTrimLoopToSurfPack (const SequentialLineSet2d &sl , const UntrimmedSurface &surf)
{
	VPoint mymn = m_tri->surfpack.modelpoints.GetMin();//m_trimmedTri
	VPoint mymx = m_tri->surfpack.modelpoints.GetMax();//m_trimmedTri
	VVector diagVec = mymx - mymn;
	vreal approxMaxLen = diagVec.GetLength();
	vreal parThreshhold = DSURFACE_BOUNDARY_FREQ / approxMaxLen;
	int mysize = sl.Size();
	for (int i=0;i<mysize;i++)
	{
		VLine2d currl = sl[i];
		vreal currlen = currl.GetV().GetLength();
		int currstepno = static_cast<int>(currlen / parThreshhold) + 1;
		vreal currpar = 0.0;
		vreal parstep = 1.0 / currstepno;
		for (int j=0; j<currstepno; j++)
		{
			VPoint2d p = currl.GetP (currpar);
			VPoint m,n;
			surf.GetPointAndNormal (p.x() , p.y() ,m , n);
			m_trimmedTri->surfpack.InsertPoint(p,m,n);
			currpar += parstep;
		}
	}
	
	const VPoint2d * lastp = sl.End();
	VPoint lastm,lastn;
	surf.GetPointAndNormal (lastp->x() , lastp->y() ,lastm , lastn);
	m_trimmedTri->surfpack.InsertPoint(*lastp,lastm,lastn);

}
void dSurface::AddTrimLoopToPointVector (const SequentialLineSet2d &sl , const UntrimmedSurface &surf , std::vector<VPoint> &vec,const VPoint &mymn,const VPoint &mymx)
{
	//VPoint mymn = m_tri->surfpack.modelpoints.GetMin();//m_trimmedTri
	//VPoint mymx = m_tri->surfpack.modelpoints.GetMax();//m_trimmedTri
	VVector diagVec = mymx - mymn;
	vreal approxMaxLen = diagVec.GetLength();
	vreal parThreshhold = DSURFACE_BOUNDARY_FREQ / approxMaxLen;
	int mysize = sl.Size();
	for (int i=0;i<mysize;i++)
	{
		VLine2d currl = sl[i];
		vreal currlen = currl.GetV().GetLength();
		int currstepno = static_cast<int>(currlen / parThreshhold) + 1;
		vreal currpar = 0.0;
		vreal parstep = 1.0 / currstepno;
		for (int j=0; j<currstepno; j++)
		{
			VPoint2d p = currl.GetP (currpar);
			VPoint m,n;
			VPoint pppp = surf.GetPoint (p.x() , p.y());
			vec.push_back (pppp);
			currpar += parstep;
		}
	}
	const VPoint2d * lastp = sl.End();
	VPoint lastpppp = surf.GetPoint (lastp->x() , lastp->y());
	vec.push_back (lastpppp);
}
void dSurface::MakeTrimmedTri (int mysize)
{
	m_trimmedTri = new VRefTriangleMesh (mysize);
}
	
void dSurface::FillTrimmedTriSurfPack (const SurfPointsPack &surfpack 
									   , const geo::BspTree *outerLoop 
									   , const std::vector<geo::BspTree *> &innerLoopVec)
{
	int mysize = surfpack.parampoints.Size();
	for (int i=0;i<mysize;i++)
	{
		const VPoint2d *p=surfpack.parampoints[i];
#if BSP_TRIM		
	if (IsPointTrimmed (*p ,outerLoop,innerLoopVec) == false)
		{
			const VPoint *m = surfpack.modelpoints[i];
			const VPoint *n = surfpack.normals[i];
			m_trimmedTri->surfpack.InsertPoint(*p,*m,*n);
		}	
#else
		if (IsPointTrimmed (*p/*,outerLoop,innerLoopVec*/) == false)
		{
			const VPoint *m = surfpack.modelpoints[i];
			const VPoint *n = surfpack.normals[i];
			m_trimmedTri->surfpack.InsertPoint(*p,*m,*n);
		}
#endif
	}
}

//void dSurface::FillTrimmedTriSurfPack (int uno , int vno
//									   , const geo::BspTree *outerLoop 
//									   , const std::vector<geo::BspTree *> &innerLoopVec)
//{
//	int mysize = surfpack.parampoints.Size();
//	for (int i=0;i<mysize;i++)
//	{
//		const VPoint2d *p=surfpack.parampoints[i];
//		if (IsPointTrimmed (*p/*,outerLoop,innerLoopVec*/) == false)
//		{
//			const VPoint *m = surfpack.modelpoints[i];
//			const VPoint *n = surfpack.normals[i];
//			m_trimmedTri->surfpack.InsertPoint(*p,*m,*n);
//		}
//	}
//}

bool dSurface::IsPointTrimmed (const VPoint2d &p 
							   , const geo::BspTree *outerLoop 
							   , const std::vector<geo::BspTree *> &innerLoopVec)
{
	std::vector<geo::BspTree *>::const_iterator i=innerLoopVec.begin();
	std::vector<geo::BspTree *>::const_iterator e=innerLoopVec.end();
	std::vector<int>::iterator j=innerLoopsInsideSign.begin();

	for (i;i!=e;i++)
	{
		int currSign = *j;
		if ((*i)->PointLocation(p)==currSign)
			return true;
		j++;
	}
	if (outerLoop->PointLocation(p)!=outerLoopInsideSign)
		return true;//point is trimmed

	return false;
}

void dSurface::AddAllTrimLoopsToSurfPack (const UntrimmedSurface &surf)
{
	AddTrimLoopToSurfPack (*p_outerLoop,surf);
	int mysize = (int)(p_innerLoopVec.size());
	for (int i=0; i < mysize; i++)
	{
		SequentialLineSet2d * loop = p_innerLoopVec[i];
		AddTrimLoopToSurfPack(*loop,surf);
	}
}

const VRefTriangleMesh * dSurface::GetTrimmedTri ()const
{
	return m_trimmedTri;
}

VRefTriangleMesh * dSurface::GetTrimmedTri ()
{
	return m_trimmedTri;
}

void dSurface::SetTrimLoopInsideSigns(const geo::BspTree *outerLoop 
										, const std::vector<geo::BspTree *> &innerLoopVec)
{
	VPoint2d p (-5.,-2.);
	if (outerLoop->PointLocation (p) == -1)
		outerLoopInsideSign = 1;
	else
		outerLoopInsideSign = -1;

	std::vector<geo::BspTree *>::const_iterator i = innerLoopVec.begin();
	std::vector<geo::BspTree *>::const_iterator e = innerLoopVec.end();
	for (i;i!=e;i++)
	{
		int currSign;
		if ((*i)->PointLocation (p) == -1)
			currSign = 1;
		else
			currSign = -1;
		innerLoopsInsideSign.push_back (currSign);
	}
}
//void dSurface::PointSetToTriangleMesh (const UntrimmedSurface &surf
//									   ,geo::BspTree *outerLoop
//									   ,std::vector<geo::BspTree *> &innerLoopVec)
//{
//	int no = 10;
//	std::vector<std::vector<VPoint2d *>> allp (no*no);
//	std::vector<std::vector<int>> alli (no*no);
//
//	//dividing points to cells
//	PointSet2d &p = m_trimmedTri->surfpack.parampoints;
//	int mysize = p.Size();
//	for (int i=0; i<mysize; i++)
//	{
//		VPoint2d *currP = p[i];
//		int index1 = (int)(currP->x() * no);
//		if (currP->x() > (1. - EPSILON) )
//			index1 = no -1;
//		int index2 = (int)(currP->y() * no);
//		if (currP->y() > (1. -EPSILON))
//			index2 = no-1;
//		
//		int index = index1 + index2 *no;
//		allp[index].push_back (currP);
//		alli[index].push_back (i);
//	}
//
//	//inserting quad corners to surfpack and to temp lists
//	vreal s = 1./no;
//	SurfPointsPack &sp = m_trimmedTri->surfpack;
//	for (int v=0;v<no;v++)
//	{
//		vreal vp = v * s;
//		for (int u=0;u<no;u++)
//		{
//			vreal up = u * s; 
//			VPoint2d p1(up,vp);
//			VPoint2d p2 (up+s,vp);
//			VPoint2d p3 (up+s,vp+s);
//			VPoint2d p4 (up,vp+s);
//			VPoint m1,m2,m3,m4;
//			VPoint n1,n2,n3,n4;
//			int index = u + v *no;
//
//			surf.GetPointAndNormal (p1.x(),p1.y(),m1,n1);
//			sp.InsertPoint (p1,m1,n1);
//			allp[index].push_back(sp.parampoints[sp.parampoints.Size()-1]);
//			alli[index].push_back(sp.parampoints.Size()-1);
//
//			surf.GetPointAndNormal (p2.x(),p2.y(),m2,n2);
//			sp.InsertPoint (p2,m2,n2);
//			allp[index].push_back(sp.parampoints[sp.parampoints.Size()-1]);
//			alli[index].push_back(sp.parampoints.Size()-1);
//		
//			surf.GetPointAndNormal (p3.x(),p3.y(),m3,n3);
//			sp.InsertPoint (p3,m3,n3);
//			allp[index].push_back(sp.parampoints[sp.parampoints.Size()-1]);
//			alli[index].push_back(sp.parampoints.Size()-1);
//
//			surf.GetPointAndNormal (p4.x(),p4.y(),m4,n4);
//			sp.InsertPoint (p4,m4,n4);
//			allp[index].push_back(sp.parampoints[sp.parampoints.Size()-1]);
//			alli[index].push_back(sp.parampoints.Size()-1);
//		}	
//	}
//
//	std::vector<std::vector<VPoint2d *>>::iterator ii =allp.begin();
//	std::vector<std::vector<VPoint2d *>>::iterator ee=allp.end();
//	std::vector<std::vector<int>>::iterator jj = alli.begin();
//
//	for (ii;ii!=ee;ii++)
//	{
//		RefTriangleMesh result((*ii).size());
//		VDelaunay2d triangulator((*ii),result);;
//		//triangulator.Calculate (*ii,result);		
//		int size2 = result.Size();
//		//inserting new triangles
//		for (int w=0;w<size2;w++)
//		{
//			RefTriangle *r = result[w];
//			int r1 = (*jj).at(r->ref0);
//			int r2 = (*jj).at(r->ref1);
//			int r3 = (*jj).at(r->ref2);
//		
//			VPoint2d *pp1 = sp.parampoints[r1];
//			VPoint2d *pp2 = sp.parampoints[r2];
//			VPoint2d *pp3 = sp.parampoints[r3];
//			VTriangle2dE t(*pp1,*pp2,*pp3);
//			VPoint2d cg = t.GetCenterOfMass();
//			if (IsPointTrimmed(cg,outerLoop,innerLoopVec) == false)
//			{
//				RefTriangle newref (r1,r2,r3);
//				m_trimmedTri->AddTriangle (newref);
//			}
//		}
//		jj++;
//	}
//}

void dSurface::TrimPointsAndDelaunayTriangulate (const UntrimmedSurface & surf , geo::BspTree *outerTrimLoop , std::vector<geo::BspTree *> &innerTrimLoopVec)
{
	//trimming points with trimloops & adding to trimmed triangles
	FillTrimmedTriSurfPack (m_tri->surfpack , outerTrimLoop , innerTrimLoopVec); 
	//FillTrimmedTriSurfPack (uno,vno,outerTrimLoop,innerTrimLoopVec);	
	AddAllTrimLoopsToSurfPack(surf);
	//PointSetToTriangleMesh (surf,outerTrimLoop,innerTrimLoopVec);

	m_trimmedTri->surfpack.RemoveDuplicate();
	VDelaunay2d triangulator;
	triangulator.Calculate (m_trimmedTri->surfpack.parampoints,m_trimmedTri->GetRefTriangleMesh());

	int size2 = m_trimmedTri->Size();
	for (int j=0;j<size2;j++)
	{
		VPoint2d p1 = m_trimmedTri->GetParamTriCor1(j);
		VPoint2d p2 = m_trimmedTri->GetParamTriCor2(j);
		VPoint2d p3 = m_trimmedTri->GetParamTriCor3(j);
		VTriangle2dE t(p1,p2,p3);
		VPoint2d cg = t.GetCenterOfMass();
	
#if BSP_TRIM
	bool ck1 = IsPointTrimmed (cg , outerTrimLoop , innerTrimLoopVec);
#else
	bool ck1 = IsPointTrimmed (cg /*, outerTrimLoop , innerTrimLoopVec*/);
#endif
		//bool ck2 = IsPointTrimmed (cg /*, outerTrimLoop , innerTrimLoopVec*/);
		//if ( ck1 != ck2)
		//	int stp2 = 1;

		if (ck1)
		{
			RefTriangle newref (-1,-1,-1);
			m_trimmedTri->SetRefTri(j,newref);
		}
	}
	m_trimmedTri->CleanTrimmedTriangles();
	//SurfaceDigitiser digi;
	//digi.SubDivide (m_tol , *m_trimmedTri , surf);
}

bool dSurface::IsPointTrimmed (const VPoint2d &p)
{
	if (p_outerLoop == 0)
		return false;
	if (LoopLineInt::IsPointInLoop (p , p_outerLoop , cubesOuterLoop ) == false)
		return true;
	int mysize = (int)(p_innerLoopVec.size());
	for (int i=0;i<mysize;i++)
	{
		SequentialLineSet2d *currL = p_innerLoopVec[i];
		PosCubes2d *currC = cubesInnerLoopVec[i];
		if (LoopLineInt::IsPointInLoop (p , currL , currC) == true)
			return true;
	}
	return false;
}

//bool dSurface::IsPointInLoop ( const VPoint2d &p
//							  , const SequentialLineSet2d *sl 
//							  , PosCubes2d *cubes)
//{
//	VLine2d fake (VPoint2d(-0.73 , p.y()) , p);
//	LongVec relind,nii;
//	cubes->SetRelatedIndiceListForLine (fake , relind , nii);
//	//relind.RemoveDuplicate();
//	PointSet2d rec;
//	int intno = 0;
//	for ( int i =0; i < relind.Size(); i++ )
//	{
//		long currlineind = relind[i];
//		VLine2d currline = sl->operator [](currlineind);	
//		LineIntersection2d::LineIntersectionData2d info 
//			= calc.CalcInt ( fake , currline );	
//		if ( info.intinfo == LineIntersection2d::LINESINTERSECT )
//		{
//			if(info.intno == 1)
//			{
//				int ind;
//				bool ck = rec.GetIndex (info.intpoint[0] , ind);
//				if (ck==false)
//				{
//					intno++;	
//					rec.InsertPoint(info.intpoint[0]);
//				}
//			}
//		}
//	}
//	if (intno % 2 == 1)
//		return true;
//	else
//		return false;
//}

const UntrimmedSurface & dSurface::GetSurface ()const
{
	return m_surf;
}
	
UntrimmedSurface & dSurface::GetSurface ()
{
	return m_surf;
}

void dSurface::MakeLoopsForBoundedSurf (ProcessedBoundedSurface &pbs)
{
	std::vector <SequentialLineSet2d *>allloops;
	//std::vector <dCurve *>allloops3d;
	std::vector <PosCubes2d *>cubVec;

	int mysize = static_cast<int>(pbs.m_loop3d.size());
	for (int i=0;i<mysize;i++)
	{
		IgesTrimLoop2d *currloop2d = pbs.m_loop2d[i];
		IgesTrimLoop *currloop = pbs.m_loop3d[i];
		
		SequentialLineSet2d *currsl = new SequentialLineSet2d (100);
		int size2 = static_cast<int>(currloop2d->loopcurves.size());
		
		for (int j=0;j<size2;j++)
		{
			IgesEntity *current = currloop2d->loopcurves[j];
			bool isReversed = currloop2d->m_isReversed[j];
			DigitiseLoop (current,pbs.allent,m_surf,*currsl,isReversed);
		}
		allloops.push_back (currsl);
		PosCubes2d *cub = new PosCubes2d (*currsl);
		cubVec.push_back (cub);

		int size3 = static_cast<int>(currloop->loopcurves.size());
		for (int w=0;w<size3;w++)
		{
			IgesEntity *current = currloop->loopcurves[w];
			bool isReversed = currloop->m_isReversed[w];
			//DigitiseLoop3d (current,pbs.allent,isReversed,m_outerLoopVec);
			m_outerLoopVec.push_back (current);
		}
	}

	int size4 = static_cast<int>(allloops.size());
	
	for (int i=0;i<size4;i++)
	{
		SequentialLineSet2d *currloop = allloops[i];
		PosCubes2d *currcub = cubVec[i];
		bool includingAll = true;

		for (int j=0;j<size4;j++)
		{
			if (j==i)
				continue;
			SequentialLineSet2d *otherloop = allloops[j];
			const VPoint2d *otherpoint = otherloop->Start();
			if (LoopLineInt::IsPointInLoop (*otherpoint,currloop,currcub)==false)
			{
				includingAll=false;
				break;
			}
		}
		if (includingAll)
		{
			p_outerLoop = currloop;
			//m_outerLoop = currloop3d;
			cubesOuterLoop = currcub;
		}
		else
		{
			p_innerLoopVec.push_back (currloop);
			//m_innerLoopVec.push_back (currloop3d);
			cubesInnerLoopVec.push_back (currcub);
		}
	}
	if (p_outerLoop == 0)
	{
		std::vector<SequentialLineSet2d *>::iterator i=p_innerLoopVec.begin();
		//std::vector<SequentialLineSet *>::iterator ii=m_innerLoopVec.begin();
		std::vector<SequentialLineSet2d *>::iterator e=p_innerLoopVec.end();
		//std::vector<SequentialLineSet *>::iterator ee=m_innerLoopVec.end();
		std::vector<PosCubes2d *>::iterator c = cubesInnerLoopVec.begin();
		std::vector<PosCubes2d *>::iterator lc = cubesInnerLoopVec.begin();

		std::vector<SequentialLineSet2d *>::iterator l;
		//std::vector<SequentialLineSet *>::iterator ll;
		vreal maxlen = MIN_NUMBER_LIMIT;
		
		for(i;i!=e;i++)
		{
			vreal currlen = (*i)->GetLength();
			if ( currlen > maxlen)
			{
				maxlen = currlen;
				l = i;
				//ll = ii;
				lc = c;
			}
			//ii++;
			c++;
		}
		p_outerLoop = *l;
		//m_outerLoop = *ll;
		cubesOuterLoop = *lc;
		p_innerLoopVec.erase(l);
		//m_innerLoopVec.erase(ll);
		cubesInnerLoopVec.erase (lc);
	}
}

//void dSurface::MakePlaneSurface (const vreal &tol , const PlaneEntity &plnent , IgesEntitySet &entset)
//{
//	VPoint n(plnent.m_A,plnent.m_B,plnent.m_C);
//	n.NormThis();
//	int boude = plnent.m_PTR;
//	if (boude == 0)
//		throw VException (("Open plane surface"),1);
//
//	IgesEntity * be = entset.GetEntityPointer (boude);
//	dCurve *newdcurve = new dCurve (tol , be , &entset , false);
//	m_outerLoopVec.push_back (newdcurve);
//	FormMeshFromOuterLoop ();
//}
//
//void dSurface::FormMeshFromOuterLoop (const VPoint &n)
//{
//	int mysize = m_outerLoop->GetPointSize();
//	MakeTrimmedTri (m_outerLoop->GetPointSize());
//	for (int i=0;i<mysize;i++)
//	{
//		VPoint *currp = m_outerLoop->GetPoint (i);
//		VPoint2d currp2d (currp->x(),currp->y());
//		m_trimmedTri->surfpack.InsertPoint(currp2d,*currp,n);
//	}
//	m_trimmedTri->surfpack.RemoveDuplicate();
//	VDelaunay2d triangulator;
//	triangulator.Calculate (m_trimmedTri->surfpack.parampoints,m_trimmedTri->GetRefTriangleMesh());
//}

//void dSurface::ScaleToFit01 (SequentialLineSet2d &sl)
//{
//	VPoint2d mn = sl.GetMin();
//	VPoint2d mx = sl.GetMax();
//	VPoint2d diff = mx - mn;
//	vreal xScale = 1./diff.x();
//	vreal yScale = 1./diff.y();
//	if (fabs(xScale - 1.0) > EPSILON || fabs(yScale - 1.0) > EPSILON)
//	{
//		int mysize = sl.Size();
//		for (int i=0;i<mysize;i++)
//		{
//			VPoint2d *myp = sl.GetPoint(i);
//			vreal newX = myp->x() * xScale;
//			myp->x(newX);
//			vreal newY = myp->y() * yScale;
//			myp->y(newY);
//		}
//	}
//}

bool dSurface::ShouldIReverse (const CurvePointGenerator &crv , const SequentialLineSet2d &sl , const UntrimmedSurface &surf)
{
	if (sl.Size()==0)
		return false;
	const VPoint2d *toCompare = sl.End();
	VPoint p0 = crv.GetPoint(0.0);
	VPoint p1 = crv.GetPoint(1.0);
	VPoint2d ps0 ( (p0.x() - surf.U0()) / (surf.U1()-surf.U0()) 
					,(p0.y() - surf.V0()) / (surf.V1() - surf.V0()) );
	VPoint2d ps1 ( (p1.x() - surf.U0()) / (surf.U1() - surf.U0())
					,(p1.y() - surf.V0()) / (surf.V1() - surf.V0()) );

	if (toCompare->Compare (ps0,1.e-6))
		return false;
	if (toCompare->Compare (ps1,1.e-6))
		return true;
	return false;
	//throw VException (("trimloop order exception"),1);
}

void dSurface::MakeLoopsForPlaneSurface ( IgesTrimLoop &m_outerloop
										, std::vector<IgesTrimLoop *> &m_innerloop 
										, IgesEntitySet &allent , UntrimmedSurface &surf)
{	
	//PlaneEntityPointGenerator &plnsurf = (PlaneEntityPointGenerator &)surf.GetDigitiser();
	//int size2 = static_cast<int>(m_outerloop.loopcurves.size());
	////int size1 = 0;
	////VPoint mn (MAX_NUMBER_LIMIT);
	////VPoint mx (MIN_NUMBER_LIMIT);	
	//for (int i=0; i<size2; i++)
	//{
	//	IgesEntity *ent =m_outerloop.loopcurves[i];
	//	//DigitiseLoop3d (ent,allent,false,m_outerLoopVec);
	//	m_outerLoopVec.push_back (ent);
	//	//const SequentialLineSet &mysl = m_outerLoopVec.back()->GetLines();
	//	//mn.ReplaceMin (mysl.GetMin());
	//	//mx.ReplaceMax (mysl.GetMax());
	//	//size1 += mysl.GetPointSize();
	//}
	//
	////const SequentialLineSet &mysl = m_outerLoopVec.front()->GetLines();
	//vreal planeRadius = 1000.;//(mx - mn).GetLength();
	//CoordinateSystem &cs = plnsurf.GetPlaneCS();
	//VPoint anypoint = cs.AbsoluteCoordToRelativeCoord(VPoint(0,0,0));
	//VPoint tmp(planeRadius,planeRadius,0.0);
	//VPoint relmx = anypoint + tmp;
	//VPoint relmn = anypoint - tmp;
	//plnsurf.ResetRectangle (relmn.x(),relmx.x(),relmn.y(),relmx.y());
	//p_outerLoop = new SequentialLineSet2d (100);
	//
	//int mysize = static_cast<int>(m_outerLoopVec.size());
	//for (int i=0; i<mysize; i++)
	//{
	//	SequentialLineSet &currsl = m_outerLoopVec[i]->GetLines();
	//	int mysize2 = currsl.Size();
	//	for (int j=0;j<mysize2;j++)
	//	{
	//		const VPoint *currpoint = currsl.GetPoint(j);
	//		VPoint2d convp = plnsurf.GetNormalizedPlaneCoord (*currpoint);
	//		p_outerLoop->InsertPoint (convp);
	//	}
	//}
	//cubesOuterLoop = new PosCubes2d (*p_outerLoop);
	//// Inner Loops 3d
	//int size5 = static_cast<int>(m_innerloop.size());
	//for (int j=0; j<size5; j++)
	//{
	//	IgesTrimLoop *tl = m_innerloop[j];
	//	int size6 = static_cast<int>(tl->loopcurves.size());
	//	for (int w=0; w<size6; w++)
	//	{
	//		IgesEntity *ent = tl->loopcurves[w];
	//		DigitiseLoop3d (ent,allent,false,m_innerLoopVec);
	//	}
	//}	

	//// Inner Loops 2d
	//int size3 = static_cast<int>(m_innerLoopVec.size());
	//for (int j=0; j<size3; j++)
	//{
	//	const SequentialLineSet & currloop3d = m_innerLoopVec[j]->GetLines();
	//	int size4 = static_cast<int>(currloop3d.GetPointSize());
	//	SequentialLineSet2d *loop = new SequentialLineSet2d (size4);
	//	for (int w=0; w<size4; w++)
	//	{	
	//		const VPoint *spaceCoord = currloop3d.GetPoint (w);
	//		VPoint2d convp = plnsurf.GetNormalizedPlaneCoord (*spaceCoord);
	//		loop->InsertPoint(convp);
	//	}
	//	p_innerLoopVec.push_back (loop);
	//	PosCubes2d *currcubes = new PosCubes2d (*loop);
	//	cubesInnerLoopVec.push_back(currcubes);
	//}
}

void dSurface::One3dLoopToSl(std::vector<IgesEntity *> loopVec, SequentialLineSet &merged , IgesEntitySet &allent)
{
	std::vector<IgesEntity *>::iterator i = loopVec.begin();
	std::vector<IgesEntity *>::iterator e = loopVec.end();

	std::vector<dCurve *>dcrvVec;
	for (i; i != e; ++i)
	{
		DigitiseLoop3d(*i, allent, false, dcrvVec);
	}
	VPoint startPoint(0.0, 0.0, 0.0);
	vreal tol = GetTolerance();
	std::vector<std::vector<dCurve *>> dcrvContours;
	std::vector<bool> isClosed;
	LoopSort::MakeContoursFromCurvesNGT<dCurve>(startPoint, dcrvVec, dcrvContours, isClosed, tol, tol);

	if (dcrvContours.size() == 0)
		throw VException("dSurface::One3dLoopToSl exception , there is not even one loop", 1);
	if (isClosed.at(0) != true)
		throw VException("dSurface::One3dLoopToSl exception , loop is not closed", 1);
	std::vector<dCurve *>::iterator ii = dcrvContours.at(0).begin();
	std::vector<dCurve *>::iterator ee = dcrvContours.at(0).end();
	for (ii; ii != ee; ++ii)
	{
		SequentialLineSet &sl = (*ii)->GetLines();
		merged.AddBack(sl);
	}
}

void dSurface::One3dLoopToSl(std::vector<dCurve*> &dcrvVec, SequentialLineSet &merged)
{
	VPoint startPoint(0.0, 0.0, 0.0);
	vreal tol = GetTolerance();
	std::vector<std::vector<dCurve *>> dcrvContours;
	std::vector<bool> isClosed;
	LoopSort::MakeContoursFromCurvesNGT<dCurve>(startPoint, dcrvVec, dcrvContours, isClosed, tol, tol);

	//if (dcrvContours.size() != 1)
	//	throw VException("dSurface::One3dLoopToSl exception , there is not one loop", 1);
	//if (isClosed.at(0) != true)
	//	throw VException("dSurface::One3dLoopToSl exception , loop is not closed", 1);
	std::vector<dCurve *>::iterator ii = dcrvContours.at(0).begin();
	std::vector<dCurve *>::iterator ee = dcrvContours.at(0).end();
	for (ii; ii != ee; ++ii)
	{
		SequentialLineSet &sl = (*ii)->GetLines();
		merged.AddBack(sl);
	}
}

void dSurface::Make2dLoopsFrom3d (ProcessedTrimmedSurface *pts , IgesEntitySet &allent)
{
	p_outerLoop = new SequentialLineSet2d (50);
	SequentialLineSet merged(100);
	One3dLoopToSl(m_outerLoopVec, merged, allent);

	if (pts->GetSurfaceIgesEntity()->entitytype == 108)
	{
		PlaneEntityPointGenerator *pepg = static_cast<PlaneEntityPointGenerator *>(&m_surf.GetDigitiser());
		pepg->SetMinMax(merged);
	}
	if (pts->GetSurfaceIgesEntity()->entitytype == 190)
	{
		PlaneSurfacePointGenerator *pspg = static_cast<PlaneSurfacePointGenerator *>(&m_surf.GetDigitiser());
		pspg->SetMinMax(merged);
	}
	if (pts->GetSurfaceIgesEntity()->entitytype == 192)
	{
		//VpAfxMessageBox("Right circular cylinder surface");
		//throw VException("Right circular cylinder surface not implemented",1);
		RightCylindricalSurfacePointGenerator *rcpg = static_cast<RightCylindricalSurfacePointGenerator *>(&m_surf.GetDigitiser());
		rcpg->DecideH(merged);
	}
	if (pts->GetSurfaceIgesEntity()->entitytype == 194)
	{
		RightConicalSurfacePointGenerator *rcs = static_cast<RightConicalSurfacePointGenerator *>(&m_surf.GetDigitiser());
		rcs->DecideH(merged);
	}
	SequentialLineSet2d tmp(merged.GetPointSize());
	std::vector<VPoint> witnessPoints;
	ProjectModelSpaceTrimLoops(merged, tmp, witnessPoints);
	//vreal U0 = m_surf.U0();
	//vreal U1 = m_surf.U1();
	//vreal V0 = m_surf.V0();
	//vreal V1 = m_surf.V1();
	//tmp.TransformToSquare(U0, U1, V0, V1);
	p_outerLoop->AddBack(tmp);


	if (p_outerLoop->End()->Compare(*p_outerLoop->Start(), EPSILON) == false)
	{
		// Last point should be equal to first
		p_outerLoop->InsertPoint(*p_outerLoop->Start());
	}
	cubesOuterLoop = new PosCubes2d(*p_outerLoop);
	ReverseOuterLoopIf();

	int innerLoopSize = (int)m_innerLoopVec.size();
	for (int ii = 0; ii < innerLoopSize; ++ii)
	{
		std::vector<IgesEntity *> &curInnerLoop = m_innerLoopVec.at(ii);
		SequentialLineSet mergedinner(100);
		One3dLoopToSl(curInnerLoop , mergedinner, allent);

	
		SequentialLineSet2d tmp2(mergedinner.GetPointSize());
		std::vector<VPoint> witnessPoints2;
		ProjectModelSpaceTrimLoops(mergedinner , tmp2, witnessPoints2);
		//tmp2.TransformToSquare(U0, U1, V0, V1);
		SequentialLineSet2d *curInnerLoopSl = new SequentialLineSet2d(tmp2.Size());
		curInnerLoopSl->AddBack(tmp2);

		if (curInnerLoopSl->End()->Compare(*curInnerLoopSl->Start(), EPSILON) == false)
		{
			// Last point should be equal to first
			curInnerLoopSl->InsertPoint(*curInnerLoopSl->Start());
		}
		PosCubes2d * curCubes = new PosCubes2d(*curInnerLoopSl);
		p_innerLoopVec.push_back(curInnerLoopSl);
		cubesInnerLoopVec.push_back(curCubes);
		ReverseInnerLoopIf(ii);
	}

	//SequentialLineSet outerLoop(49);
	//p_outerLoop = new SequentialLineSet2d (outerLoop.GetPointSize());
	//ProjectModelSpaceTrimLoops (outerLoop , *p_outerLoop);
	//GetParamLoop ( outerLoop , *p_outerLoop); 
	//cubesOuterLoop = new PosCubes2d (*p_outerLoop);
	//int mysize = 1;//static_cast<int>(innerLoopVec.size());
	//for(int i=0;i<mysize;i++)
	//{
	//	SequentialLineSet currloop(49);
	//	SequentialLineSet2d *currloop2d = new SequentialLineSet2d (currloop.Size());
	//	ProjectModelSpaceTrimLoops (currloop,*currloop2d);
	//	p_innerLoopVec.push_back (currloop2d);
	//	PosCubes2d *currcubes = new PosCubes2d (*currloop2d);
	//	cubesInnerLoopVec.push_back(currcubes);
	//}
}

void dSurface::Make2dLoopsFrom3d(ProcessedFace &pf)
{
	p_outerLoop = new SequentialLineSet2d(50);
	SequentialLineSet merged(100);
	One3dLoopToSl(pf.m_outerLoop, merged);

	if (pf.GetSurfaceIgesEntity()->entitytype == 108)
	{
		PlaneEntityPointGenerator *pepg = static_cast<PlaneEntityPointGenerator *>(&m_surf.GetDigitiser());
		pepg->SetMinMax(merged);
	}
	if (pf.GetSurfaceIgesEntity()->entitytype == 190)
	{
		PlaneSurfacePointGenerator *pspg = static_cast<PlaneSurfacePointGenerator *>(&m_surf.GetDigitiser());
		pspg->SetMinMax(merged);
	}
	if (pf.GetSurfaceIgesEntity()->entitytype == 192)
	{
		//VpAfxMessageBox("Right circular cylinder surface");
		//throw VException("Right circular cylinder surface not implemented",1);
		RightCylindricalSurfacePointGenerator *rcpg = static_cast<RightCylindricalSurfacePointGenerator *>(&m_surf.GetDigitiser());
		rcpg->DecideH(merged);
	}
	if (pf.GetSurfaceIgesEntity()->entitytype == 194)
	{
		RightConicalSurfacePointGenerator *rcs = static_cast<RightConicalSurfacePointGenerator *>(&m_surf.GetDigitiser());
		rcs->DecideH(merged);
	}
	SequentialLineSet2d tmp(merged.GetPointSize());
	std::vector<VPoint> witnessPoints;
	ProjectModelSpaceTrimLoops(merged, tmp, witnessPoints);
	//vreal U0 = m_surf.U0();
	//vreal U1 = m_surf.U1();
	//vreal V0 = m_surf.V0();
	//vreal V1 = m_surf.V1();
	//tmp.TransformToSquare(U0, U1, V0, V1);
	p_outerLoop->AddBack(tmp);


	if (p_outerLoop->End()->Compare(*p_outerLoop->Start(), EPSILON) == false)
	{
		// Last point should be equal to first
		p_outerLoop->InsertPoint(*p_outerLoop->Start());
	}
	cubesOuterLoop = new PosCubes2d(*p_outerLoop);
	ReverseOuterLoopIf();

	int innerLoopSize = (int)pf.m_innerLoops.size();
	for (int ii = 0; ii < innerLoopSize; ++ii)
	{
		std::vector<dCurve*> &curInnerLoop = pf.m_innerLoops.at(ii);
		SequentialLineSet mergedinner(100);
		One3dLoopToSl(curInnerLoop, mergedinner);


		SequentialLineSet2d tmp2(mergedinner.GetPointSize());
		std::vector<VPoint> witnessPoints2;
		ProjectModelSpaceTrimLoops(mergedinner, tmp2, witnessPoints2);
		//tmp2.TransformToSquare(U0, U1, V0, V1);
		SequentialLineSet2d *curInnerLoopSl = new SequentialLineSet2d(tmp2.Size());
		curInnerLoopSl->AddBack(tmp2);

		if (curInnerLoopSl->End()->Compare(*curInnerLoopSl->Start(), EPSILON) == false)
		{
			// Last point should be equal to first
			curInnerLoopSl->InsertPoint(*curInnerLoopSl->Start());
		}
		PosCubes2d * curCubes = new PosCubes2d(*curInnerLoopSl);
		p_innerLoopVec.push_back(curInnerLoopSl);
		cubesInnerLoopVec.push_back(curCubes);
		ReverseInnerLoopIf(ii);
	}
}

void dSurface::ProjectModelSpaceTrimLoops (const SequentialLineSet &modelLoop 
										   , SequentialLineSet2d &paramLoop
										   , std::vector <VPoint> &witnessPoints)
{
	//SurfaceSpace sp (&m_surf,m_tol);
	//sp.ProjectModelSpaceTrimLoops(modelLoop,paramLoop,witnessPoints,&m_surf);
	
	int mpsize = modelLoop.GetPointSize();
	vreal u, v;
	vreal tol = this->GetTolerance();
	for (int mi = 0; mi < mpsize; ++mi)
	{
		const VPoint &mp = *modelLoop.GetPoint(mi);
		bool ck = m_surf.GenericUnproject(mp, u, v, tol);
		if (ck == false)
			throw VException("dSurface::ProjectModelSpaceTrimLoops exception , unproject failed",1);
		paramLoop.InsertPoint(VPoint2d(u, v));
	}
	vreal toltol = 1.e-6;
	if (this->GetSurface().GetDigitiser().GetIsPeriodicInU() == true)
		paramLoop.FixDiscontinuetyInXForTrimLoop01(toltol);
	if (this->GetSurface().GetDigitiser().GetIsPeriodicInV() == true)
		paramLoop.FixDiscontinuetyInYForTrimLoop01(toltol);

	//std::vector <const Triangles *> mytvec;
	//std::vector <const Points *>mypvec;
	//sPoints myspoints (modelLoop.GetPointSet());
	//VRefTriangleMesh * tmpTri = m_surf.Digitise(m_tol);
	//mytvec.push_back (tmpTri);
	//mypvec.push_back (&myspoints);
	//MultiTriangles multiTri(mytvec);
	//MultiPoints multiPo(mypvec);
	//TriangleMeshPointSetMinDist mycalc(multiTri,multiPo);	
	//std::vector<vreal> allmindist;
	//std::vector<int> triangleind;
	////std::vector<VPoint>witnessPoints;
	//mycalc.MinDistAllTrianglesAndAllPoints (allmindist,triangleind,witnessPoints);
	//int mysize = modelLoop.GetPointSize();
	//for (int i=0;i<mysize;i++)
	//{
	//	const VPoint currpoint = witnessPoints.at(i);
	//	int currtriindex = triangleind[i];
	//	VTriangleE currmtri = tmpTri->operator [](currtriindex);	
	//	VTriangle2dE currptri = tmpTri->GetParTri (currtriindex);	
	//	vreal u,v;
	//	currmtri.GetUV (currpoint , u , v);
	//	bool ck = currmtri.IsInTriangle (currpoint);
	//	VPoint2d parpoint = currptri.GetP (u,v);
	//	paramLoop.InsertPoint (parpoint);
	//}
	//delete tmpTri;
}

int dSurface::GetDeNo ()const
{
	return m_deno;
}

void dSurface::SetDeNo(int deno)
{
	m_deno = deno;
}

void dSurface::Transform (const TransformationMatrix &tm)
{
	if (m_tri)
		m_tri->Transform (tm);
	if (m_trimmedTri != m_tri && m_trimmedTri != 0)
		m_trimmedTri->Transform (tm);
	if (m_roughTri)
		m_roughTri->Transform(tm);
	m_surf.IncrementTransform (tm);
	if (m_wireframe)
		m_wireframe->Transform (tm);
}

void dSurface::UseNewTrim ()
{
		SurfaceTrimmer newtrimmer (m_surf,p_outerLoop,p_innerLoopVec);
		VPoint2d tlmn = p_outerLoop->GetMin();
		VPoint2d tlmx = p_outerLoop->GetMax();
		int ustep = 20;
		int vstep = 20;

		newtrimmer.DoTrimInParSpace (ustep
									,vstep
									,40
									,tlmn.x()
									,tlmx.x()
									,tlmn.y()
									,tlmx.y()
									,m_trimmedTri);
}

//void
//{
//	SurfaceTrimmerDelaunay d (m_tol);
//	m_tri = d.TriangulateSurf (m_surf,pts.outerloop , pts.innerloop , pts.allent);
//	m_trimmedTri = m_tri;
//	MakeLoops (pts.m_outerloop , pts.m_innerloop , pts.allent , m_surf);
//	MakeSnapLinesAndPoints();	
//	return;
//}

VPoint2d dSurface::GetMinPar()const
{
	if (p_outerLoop)
	{
		VPoint2d res (p_outerLoop->GetMin());
		return res;
	}
	else
	{
		VPoint2d res (0.0,0.0);
		return res;
	}
}	

VPoint2d dSurface::GetMaxPar()const
{
	if (p_outerLoop)
	{
		VPoint2d res (p_outerLoop->GetMax());
		return res;
	}
	else
	{
		VPoint2d res (1.0,1.0);
		return res;
	}
}

bool myvsort (VPoint2d &p1,VPoint2d &p2)
{
	if (p1.y() < p2.y())
		return true;
	else
		return false;
}

void dSurface::MakeUSection (const vreal &u , int vno , LineSet &lines)
{
	std::vector<VPoint2d> sortedVec;
	AllLoopsConstULineInt (u,sortedVec);
	int mysize2 = (int)sortedVec.size();
	for (int j=0;j<mysize2-1;j+=2)
	{
		VPoint2d &p1 = sortedVec[j];
		VPoint2d &p2 = sortedVec[j+1];
		MakeSect (p1,p2,vno,lines);
	}
	
}

void dSurface::MakeSect (const VPoint2d &p1,const VPoint2d &p2,int stepno,LineSet &lines)
{
	VPoint2d p = p1;
	VPoint2d dp = (p2 - p1) * ( 1. / stepno);
	VPoint mp = m_surf.GetPoint (p.x(),p.y());
	p+=dp;
	for (int i=1;i<stepno+1;i++)
	{
		VPoint m = m_surf.GetPoint (p.x() , p.y());
		VLine l(mp,m);
		lines.AddLine (l);
		mp = m;
		p+=dp;
	}
}
bool myusort (VPoint2d &p1,VPoint2d &p2)
{
	if (p1.x() < p2.x())
		return true;
	else
		return false;
}
void dSurface::MakeVSection (const vreal &v , int uno , LineSet &lines)
{
	std::vector<VPoint2d> vec;
	AllLoopsConstVLineInt (v , vec);

	int mysize3 = (int)vec.size();
	for (int j=0;j<mysize3-1;j+=2)
	{
		VPoint2d &p1 = vec[j];
		VPoint2d &p2 = vec[j+1];
		MakeSect (p1,p2,uno,lines);
	}
}

const LineSet * dSurface::GetWireFrame()const
{
	return m_wireframe;
}

//void dSurface::LoopLineInt (const SequentialLineSet2d *loop 
//							, PosCubes2d *cubes 
//							, const VLine2d &l1 , std::vector<VPoint2d> &intpvec)
//{
//	LongVec relind,nii;
//	cubes->SetRelatedIndiceListForLine (l1,relind,nii);
//	int mysize = relind.Size();
//	//int mysize = loop->Size();
//	//VPoint2d intp;
//	for (int i=0;i<mysize;i++)
//	{
//		int curri = relind[i];
//		VLine2d l2 = loop->operator [] (curri);
//		
//		//if (SurfaceQuadTrimmer::RegularLineInt (l1,l2,intp))
//		LineIntersection2d::LineIntersectionData2d info 
//			= calc.CalcInt ( l1 , l2 );	
//		if ( info.intinfo == LineIntersection2d::LINESINTERSECT )
//		{
//			if(info.intno == 1)
//			{		
//				intpvec.push_back (info.intpoint[0]);
//			}
//		}
//	}
//}
int SURF_MAX_GRID_LEN = 10000000;

int dSurface::GetDivideCount (const vreal tol)
{
	//if (tol < 0.01)
	//	return 30;
	//else if (tol < 0.04 && tol >= 0.01)
	//	return 20;
	//else if (tol >= 0.04 && tol < 0.1)
	//	return 10;
	//else if (tol < 0.5 && tol >= 0.1)
	//	return 6;
	
	return 4;
}
const vreal DSURF_GRID_START = -0.0123;
const vreal DSURF_GRID_END = 1.0123;

void dSurface::FormTrimmedTri (bool surfQuadTrimmerMakeBoundary)
{
	if (p_outerLoop == 0)
	{
		p_outerLoop = new SequentialLineSet2d (4);
		p_outerLoop->InsertPoint (VPoint2d (0,0));
		p_outerLoop->InsertPoint (VPoint2d (1,0));
		p_outerLoop->InsertPoint (VPoint2d (1,1));
		p_outerLoop->InsertPoint (VPoint2d (0,1));
		p_outerLoop->InsertPoint (VPoint2d (0,0));
	}

	vreal u0,u1,v0,v1;
	VPoint2d bmn = GetMinPar();
	VPoint2d bmx = GetMaxPar();
	u0 = bmn.x();
	u1 = bmx.x();
	v0 = bmn.y();
	v1 = bmx.y();

	AnalyzeSurface a;
	vreal tmpRad;
	m_surftypedata.m_ck = a.IsPlanarOrCylinder 
												(GetSurface()
													,u0
													,u1
													,v0
													,v1
													,m_surftypedata.m_rad
													,tmpRad
													,m_surftypedata.m_h
													,m_surftypedata.m_center
													,m_surftypedata.m_ax
													,m_surftypedata.m_isClosedCylinder 
													, m_tol);
		
	m_trimmedTriBackUp = m_trimmedTri;
	std::map<double,VRefTriangleMesh *>::iterator searchTrii = m_tolTrimmedTriMap.find (m_tol);
	if (searchTrii == m_tolTrimmedTriMap.end())
	{
		m_trimmedTri = CalculateTriangles (m_tol, surfQuadTrimmerMakeBoundary);
		m_tolTrimmedTriMap.insert (std::pair<double,VRefTriangleMesh*>(m_tol,m_trimmedTri));	
	}
	else
	{
		m_trimmedTri = searchTrii->second;
	}
	
	//if ( (m_trimmedTri->GetMax()-m_trimmedTri->GetMin()).GetLength () > 20000.0)
	//	throw VException (("Too big surface,this seems like a trimming problem"),1);
}

VRefTriangleMesh * dSurface::CalculateTriangles (const vreal tol,bool surfQuadTrimmerMakeBoundary)
{
	try
	{
		//bool ck = ShiftSurfaceParIf(p_outerLoop , m_surf);
		//if (ck)
		//	ShiftAllLoops(m_surf);

		std::vector<vreal> ugrid;
		std::vector<vreal> vgrid;
		
		int divideCount = GetDivideCount (tol);
		vreal dp= (DSURF_GRID_END - DSURF_GRID_START) /divideCount;
		for (int i=0;i<divideCount;++i)
		{
			vreal mypar= DSURF_GRID_START + i*dp + 0.0000000102702;
			vreal nextpar = DSURF_GRID_START + (i+1)*dp +0.0000000302801;
			int uadd =
				m_surf.GetDigitiser().CalculateDivideCountForIntervalAlongU (mypar>0.?mypar:0.
																	,nextpar<1.?nextpar:1.
																	,0.5
																	,tol);
			m_surf.GetDigitiser().InsertSegments (uadd,mypar,nextpar,ugrid);
			int vadd =
				m_surf.GetDigitiser().CalculateDivideCountForIntervalAlongV (mypar>0.?mypar:0.
																	,nextpar<1.?nextpar:1.
																	,0.5
																	,tol);
			m_surf.GetDigitiser().InsertSegments (vadd,mypar,nextpar,vgrid);
		}
		ugrid.push_back (DSURF_GRID_END);
		vgrid.push_back(DSURF_GRID_END);
		if ((int)ugrid.size()*vgrid.size()>SURF_MAX_GRID_LEN)//((int)ugrid.size()>SURF_MAX_GRID_LEN || (int)vgrid.size()>SURF_MAX_GRID_LEN)
			throw VException (( IDS_SurMaxGriSizExc),1);

		// Add param to ugrid if loop margins do not cross any quad
		bool cknocross = InsertParIfNoCrossToQuads (p_outerLoop,ugrid,vgrid);
		int innerloopcount = (int)p_innerLoopVec.size();
		for (int w=0;w<innerloopcount;++w)
		{
			cknocross = InsertParIfNoCrossToQuads (p_innerLoopVec[w],ugrid,vgrid);
		}
		
		SurfaceQuadTrimmer mytrimmer (ugrid
									,vgrid
									,m_surf
									,p_outerLoop
									,p_innerLoopVec
									,cubesOuterLoop
									,cubesInnerLoopVec
									,m_useposcubestrim
									,surfQuadTrimmerMakeBoundary);
		VRefTriangleMesh *res = mytrimmer.TrimSurface();
		if (surfQuadTrimmerMakeBoundary)
			MakeBoundaryCurvesFromSQT (mytrimmer);

		return res;
	}
	catch (VException &e)
	{
		throw e;
	}
	catch (exception &exc)
	{
		throw exc;
	}
	catch (...)
	{
		throw UntypedException(( IDS_UnkExcInDSuCal),999);
	}
}

//void dSurface::ExtendGridsIf (std::vector<vreal> &ugrid , std::vector<vreal> &vgrid , UntrimmedSurface &surf , SequentialLineSet2d *outerLoop)
//{
//	// The conditions for extending
//	// 1 check overflow in outer loop if no overflow there is no need to extend
//	// 2 if there is overflow in u lets say check if the surface is closed along u
//	// if it is closed in overflow axis then add one more set in the overflow direction ([0,1] to [0,2])
//	VPoint2d lmin = outerLoop->GetMin();
//	VPoint2d lmax = outerLoop->GetMax();
//	
//	std::vector<vreal> newugrid , newvgrid;
//	bool isSurfClosedInU = true; // surf.IsClosedInU();
//	bool isSurfClosedInV = true; // surf.IsClosedInV();
//	if (lmin.x() < -EPSILON)
//	{
//		// there is overflow along u min
//		if (isSurfClosedInU)
//		{
//			size_t usize = ugrid.size();
//			for (size_t i=0;i<usize;++i)
//			{
//				vreal curu = ugrid.at(i);
//				newugrid.push_back(curu-1.0);
//			}
//		}
//	}
//	newugrid.insert(newugrid.end(),ugrid.begin(),ugrid.end());
//	if (lmin.y() < -EPSILON)
//	{
//		// there is overflow along v min
//		if (isSurfClosedInV)
//		{
//			size_t vsize = vgrid.size();
//			for (size_t i=0;i<vsize;++i)
//			{
//				vreal curv = vgrid.at(i);
//				newvgrid.push_back(curv-1.0);
//			}
//		}
//	}
//	newvgrid.insert(newvgrid.end(),vgrid.begin(),vgrid.end());
//
//	if (lmax.x() > 1.0 + EPSILON)
//	{
//		//  there is overflow along u max
//		if (isSurfClosedInU)
//		{
//			size_t usize = ugrid.size();
//			for (size_t i=0;i<usize;++i)
//			{
//				vreal curu = ugrid.at(i);
//				newugrid.push_back(curu+1.0);
//			}
//		}
//	}
//	if (lmax.y() > 1.0 + EPSILON)
//	{
//		// there is overflow along v max
//		if (isSurfClosedInV)
//		{
//			size_t vsize = vgrid.size();
//			for (size_t i=0;i<vsize;++i)
//			{
//				vreal curv = vgrid.at(i);
//				newvgrid.push_back(curv+1.0);
//			}
//		}
//	}
//	ugrid = newugrid;
//	vgrid = newvgrid;
//}

//bool dSurface::ShiftSurfaceParIf(const SequentialLineSet2d *outerLoop , UntrimmedSurface &surf)
//{
//	//return false;
//	// !!! !!!
//
//	VPoint2d lmin = outerLoop->GetMin();
//	VPoint2d lmax = outerLoop->GetMax();
//	bool shifted = false;
//	vreal u0 = lmin.x();
//	vreal u1 = lmax.x();
//	//if (u1-u0 > 1.01)
//	//{
//	//	throw VException (("dSurface::loop u parameter overflow!"),1);
//	//}
//	vreal v0 = lmin.y();
//	vreal v1 = lmax.y();
//	//if (v1-v0 > 1.01)
//	//{
//	//	throw VException (("dSurface::loop v parameter overflow!"),1);
//	//}
//
//	bool isSurfClosedInU = true; // surf.IsClosedInU();
//	bool isSurfClosedInV = true; // surf.IsClosedInV();
//	if (lmin.x() < -EPSILON)
//	{
//		// there is overflow along u min
//		if (isSurfClosedInU)
//		{
//			// shift par in u
//			surf.SetUParShift(u0);
//			shifted = true;
//		}
//		else 
//			throw VException (( IDS_DSuLooParOveSur),1);
//	}
//	
//	if (lmin.y() < -EPSILON)
//	{
//		// there is overflow along v min
//		if (isSurfClosedInV)
//		{
//			// shift par in v
//			surf.SetVParShift(v0);
//			shifted = true;
//		}
//		else 
//			throw VException (( IDS_DSuLooParOveSur1),1);
//	}
//
//	if (lmax.x() > 1.0 + EPSILON)
//	{
//		//  there is overflow along u max
//		if (isSurfClosedInU)
//		{
//			surf.SetUParShift (u0);
//			shifted = true;
//		}
//		else
//			throw VException (( IDS_DSuLooParOveSur),1);
//	}
//	if (lmax.y() > 1.0 + EPSILON)
//	{
//		// there is overflow along v max
//		if (isSurfClosedInV)
//		{
//			surf.SetVParShift(v0);
//			shifted = true;
//		}
//		else 
//			throw VException (( IDS_DSuLooParOveSur1),1);
//	}
//	return shifted;
//}
void dSurface::MakeBoundaryCurvesFromSQT (SurfaceQuadTrimmer &sqt)
{
	size_t mysize1 = sqt.m_boundaryCurves.size();
	for (size_t i=0;i<mysize1;++i)
	{
		std::vector<VPoint2d> &curvec = sqt.m_boundaryCurves.at(i);
		std::vector<VPoint2d>::iterator ii = curvec.begin();
		std::vector<VPoint2d>::iterator ee = curvec.end();
		SequentialLineSet sl ((int)curvec.size());
		for (ii;ii!=ee;++ii)
		{
			VPoint p = m_surf.GetPoint (ii->x() , ii->y());
			sl.InsertPoint (p);
		}
		CopiousDataFactory f;
		CopiousData * cp = f.Make (sl);
		m_outerLoopVec.push_back (cp);
	}
}

SurfaceQuadTrimmer * dSurface::FormTrimmedTriTest ()
{
	std::vector<vreal> ugrid;
	std::vector<vreal> vgrid;
	
	int divideCount = GetDivideCount (m_tol);
	vreal dp= (DSURF_GRID_END - DSURF_GRID_START) /divideCount;
	for (int i=0;i<divideCount;++i)
	{
		vreal mypar= DSURF_GRID_START + i*dp + 0.0000000102702;
		vreal nextpar = DSURF_GRID_START + (i+1)*dp +0.0000000302801;
		int uadd =
			m_surf.GetDigitiser().CalculateDivideCountForIntervalAlongU (mypar>0.?mypar:0.
																,nextpar<1.?nextpar:1.
																,0.5
																,m_tol);
		m_surf.GetDigitiser().InsertSegments (uadd,mypar,nextpar,ugrid);
		int vadd =
			m_surf.GetDigitiser().CalculateDivideCountForIntervalAlongV (mypar>0.?mypar:0.
																,nextpar<1.?nextpar:1.
																,0.5
																,m_tol);
		m_surf.GetDigitiser().InsertSegments (vadd,mypar,nextpar,vgrid);
	}
	ugrid.push_back (DSURF_GRID_END);
	vgrid.push_back(DSURF_GRID_END);
	if ((int) ugrid.size()*vgrid.size() > SURF_MAX_GRID_LEN)//((int)ugrid.size()>SURF_MAX_GRID_LEN || (int)vgrid.size()>SURF_MAX_GRID_LEN)
		throw VException (( IDS_SurMaxGriSizExc),1);

	// Add param to ugrid if loop margins donot cross any quad
	bool cknocross = InsertParIfNoCrossToQuads (p_outerLoop,ugrid,vgrid);
	int innerloopcount = (int)p_innerLoopVec.size();
	for (int w=0;w<innerloopcount;++w)
	{
		cknocross = InsertParIfNoCrossToQuads (p_innerLoopVec[w],ugrid,vgrid);
	}

		
		SurfaceQuadTrimmer *mytrimmer=new SurfaceQuadTrimmer 
									(ugrid
									,vgrid
									,m_surf
									,p_outerLoop
									,p_innerLoopVec
									,cubesOuterLoop
									,cubesInnerLoopVec
									,m_useposcubestrim);
		return mytrimmer;
}
void dSurface::AllLoopsConstULineInt (const vreal &u , std::vector <VPoint2d> &sortedVec)
{
	VPoint2d p1 (u,-0.1);
	VPoint2d p2 (u,1.1);
	VLine2d l1 (p1,p2);
	std::vector<VPoint2d> vec1;
	if (p_outerLoop)
	{
		LoopLineInt::LineLoopInt (p_outerLoop,cubesOuterLoop,l1,vec1);
		int mysize = (int)p_innerLoopVec.size();
		for (int i=0;i<mysize;i++)
		{
			SequentialLineSet2d *currl = p_innerLoopVec.operator [](i);
			LoopLineInt::LineLoopInt (currl,cubesInnerLoopVec[i],l1,vec1);
		}
	}
	else
	{
		VPoint2d s (u,0.0);
		VPoint2d e (u,1.0);
		sortedVec.push_back (s);
		sortedVec.push_back (e);
	}
	std::sort (vec1.begin(),vec1.end(),myvsort);
	
	int mysize2 = (int)vec1.size();
	//removing duplicate point
	if (mysize2>0)
		sortedVec.push_back (vec1[0]);
	for (int i=1;i<mysize2;i++)
	{
		if (vec1[i-1].Compare (vec1[i],EPSILON) == false)
		{
			sortedVec.push_back (vec1[i]);
		}
	}
}

void dSurface::AllLoopsConstVLineInt (const vreal &v , std::vector <VPoint2d> &sortedVec)
{
	VPoint2d p1 (-0.1,v);
	VPoint2d p2 (1.1,v);
	VLine2d l1 (p1,p2);

	std::vector <VPoint2d> vec1;
	if (p_outerLoop)
	{
		LoopLineInt::LineLoopInt (p_outerLoop,cubesOuterLoop,l1,vec1);
		int mysize = (int)p_innerLoopVec.size();
		for (int i=0;i<mysize;i++)
		{
			SequentialLineSet2d *currl = p_innerLoopVec.operator [](i);
			LoopLineInt::LineLoopInt (currl,cubesInnerLoopVec[i],l1,vec1);
		}
	}
	else
	{
		VPoint2d s (0.0,v);
		VPoint2d e (1.0,v);
		sortedVec.push_back (s);
		sortedVec.push_back (e);
	}
	
	std::sort (vec1.begin(),vec1.end(),myusort);
	int mysize2 = (int)vec1.size();
	
	//removing duplicate point
	if (mysize2>0)
		sortedVec.push_back (vec1[0]);
	for (int i=1;i<mysize2;i++)
	{
		if (vec1[i-1].Compare (vec1[i],EPSILON) == false)
		{
			sortedVec.push_back (vec1[i]);
		}
	}
}

void dSurface::AllLoopsLineInt (const VLine2d &l , std::vector<VPoint2d> &sortedVec)
{
	std::vector <VPoint2d> vec1;
	if (p_outerLoop)
	{
		LoopLineInt::LineLoopInt (p_outerLoop,cubesOuterLoop,l,vec1);
		int mysize = (int)p_innerLoopVec.size();
		for (int i=0;i<mysize;i++)
		{
			SequentialLineSet2d *currl = p_innerLoopVec.operator [](i);
			LoopLineInt::LineLoopInt (currl,cubesInnerLoopVec[i],l,vec1);
		}
	}
	else
	{
		SequentialLineSet2d tmpsl (4);
		tmpsl.InsertPoint (VPoint2d (0,0));
		tmpsl.InsertPoint (VPoint2d (1,0));
		tmpsl.InsertPoint (VPoint2d (1,1));
		tmpsl.InsertPoint (VPoint2d (0,1));
		tmpsl.InsertPoint (VPoint2d (0,0));
		PosCubes2d tmpcub (tmpsl);
		LoopLineInt::LineLoopInt (&tmpsl,&tmpcub,l,vec1);
	}
	
	VPoint2d linevec = l.GetV();
	
	if (fabs(linevec.x()) > fabs(linevec.y()) )
		std::sort (vec1.begin(),vec1.end(),myusort);
	else
		std::sort (vec1.begin(),vec1.end(),myvsort);

	int mysize2 = (int)vec1.size();
	
	//removing duplicate point
	if (mysize2>0)
		sortedVec.push_back (vec1[0]);
	for (int i=1;i<mysize2;i++)
	{
		if (vec1[i-1].Compare (vec1[i],EPSILON) == false)
		{
			sortedVec.push_back (vec1[i]);
		}
	}
}

const dSurface::SurfTypeData & dSurface::GetSurfTypeData() const
{
	return m_surftypedata;
}

void dSurface::DoPolygonTriangulation()
{
	int mysize = p_outerLoop->GetPointSize();
	m_trimmedTri = new VRefTriangleMesh(mysize);
	std::vector<int> polyref;
	for (int i=0;i<mysize-1;++i)
	{
		const VPoint2d *p = p_outerLoop->GetPoint(i);
		VPoint m = m_surf.GetPoint (p->x(),p->y());
		int r = m_trimmedTri->surfpack.InsertPoint (*p,m,m_surftypedata.m_ax);
		polyref.push_back (r);
	}
	VPolygon poly(m_trimmedTri->surfpack.parampoints.GetVec(),polyref);
	std::vector<VPolygon> tripolyvec;
	poly.Triangulate (tripolyvec);
	std::vector<VPolygon>::iterator ii = tripolyvec.begin();
	std::vector<VPolygon>::iterator ee = tripolyvec.end();
	for (ii;ii!=ee;++ii)
	{
		RefTriangle rt;
		rt.ref0 = ii->m_polygonPoints[0];
		rt.ref1 = ii->m_polygonPoints[1];
		rt.ref2 = ii->m_polygonPoints[2];
		m_trimmedTri->AddTriangle(rt);
	}
}

void dSurface::CalcParSpaceTol()
{
	vreal mystp = this->GetTolerance()*0.05;
	VPoint p1 = m_surf.GetPoint (0. , 0.);
	VPoint p2 = m_surf.GetPoint (mystp , 0.);
	VPoint p3 = m_surf.GetPoint (0.,mystp);
	vreal modelSpaceLen1 = (p2 - p1).GetLength();
	vreal modelSpaceLen2 = (p3 - p1).GetLength();
	// parSpaceLen = sqrt(0.1^2 + 0.1^2) = 0,14142135623730950488016887242097
	vreal parSpaceTol1 = mystp * (m_tol / modelSpaceLen1);
	vreal parSpaceTol2 = mystp * (m_tol / modelSpaceLen2);
	m_parSpaceTol = parSpaceTol1 < parSpaceTol2 ? parSpaceTol1 : parSpaceTol2;
	if (m_parSpaceTol < EPSILON)
		throw VException ("dSurface::CalcParSpaceTol exception,tolerance is zero!",1);
}
void dSurface::ChangeToleranceForVeryBigSurfaces()
{
	if (m_changeBigSurfaceTol)
	{
		bool ck = IsBigSurface ();
		if (ck)
			m_tol = m_bigSurfaceTol;//modelSpaceLen / 500;
	}
}
bool dSurface::IsBigSurface ()const
{
	VPoint p1 = m_surf.GetPoint (0.1 , 0.1);
	VPoint p2 = m_surf.GetPoint (0.2 , 0.2);
	vreal modelSpaceLen = (p2 - p1).GetLength();
	if ( modelSpaceLen > m_bigSurfaceLength*0.1)
		return true;
	else
		return false;
}

bool dSurface::IsParLoopCCW (SequentialLineSet2d *sl,PosCubes2d *cubes)
{
	VLine2dE l1 (sl->operator [] ((int)(sl->Size()*0.5)));
	VLine2dE l2 (sl->operator [] (0));
	VLine2dE l3 (sl->operator [] (sl->Size()-1));
	vreal d1 = l1.GetV().GetLength();
	vreal d2 = l2.GetV().GetLength();
	vreal d3 = l3.GetV().GetLength();
	VLine2dE l;
	if (d1 >= d2 && d1 >= d3)
		l = l1;
	if (d2 >= d1 && d2 >= d3)
		l = l2;
	if (d3 >= d1 && d3 >= d2)
		l = l3;
	if ( LoopLineInt::IsPointInLoop ( l.GetP(0.5)+l.N()*0.0001 , sl , cubes))
	{
		// line normal is pointing out of loop so CCW
		return true;
	}	
	else
		return false;
}
void dSurface::ReverseOuterLoopIf ()
{
	if ( IsParLoopCCW (p_outerLoop,cubesOuterLoop) == false)
	{
		// clockwise rotating so reverse it
		p_outerLoop->Reverse();
		delete cubesOuterLoop;
		cubesOuterLoop = new PosCubes2d (*p_outerLoop);
	}
}

void dSurface::ReverseInnerLoopIf (int seqref)
{
	SequentialLineSet2d *sl = p_innerLoopVec[seqref];
	if (IsParLoopCCW (sl,cubesInnerLoopVec[seqref]) == true)
	{
		// inner loop is ccw rotating so reverse it
		sl->Reverse();
		delete cubesInnerLoopVec[seqref];
		cubesInnerLoopVec[seqref] = new PosCubes2d (*sl);
	}
}

const VPoint dSurface::GetMin() const
{
	if (GetTrimmedTri())
		return GetTrimmedTri()->GetMin();
	else
	{
		//VPoint temp (MAX_NUMBER_LIMIT);
		//int mysize=(int)m_outerLoopVec.size();
		//for (int i=0;i<mysize;++i)
		//{
		//	const dCurve *crv = m_outerLoopVec[i];
		//	temp.ReplaceMin (crv->GetLines().GetMin());
		//}
		//return temp;
		return VPoint (-1000,-1000,1000);
	}
}
const VPoint dSurface::GetMax() const
{
	if (GetTrimmedTri())
		return GetTrimmedTri()->GetMax();
	else
	{
		//VPoint temp (MIN_NUMBER_LIMIT);
		//int mysize=(int)m_outerLoopVec.size();
		//for (int i=0;i<mysize;++i)
		//{
		//	const dCurve *crv=m_outerLoopVec[i];
		//	temp.ReplaceMax (crv->GetLines().GetMax());
		//}
		//return temp;
		return VPoint (1000,1000,1000);
	}
}

void dSurface::GetParamLoop(const SequentialLineSet &loop, SequentialLineSet2d &paramLoop)
{
	if (!m_tri)
		m_tri = m_surf.Digitise (m_tol);
	PointSet pset (200);
	ThreeDLoopToSurfParSpace::Convert (*m_tri,loop,0.05,pset);		

}

void dSurface::MyTwoDLoopTriangulate()
{
	std::vector<vreal> ugrid;
	std::vector<vreal> vgrid;
	ugrid.push_back (0.);
	ugrid.push_back (0.25);
	ugrid.push_back (0.5);
	ugrid.push_back (0.75);
	ugrid.push_back (1.);
	vgrid.push_back (0.);
	vgrid.push_back (0.25);
	vgrid.push_back (0.5);
	vgrid.push_back (0.75);
	vgrid.push_back (1.);
	TwoDLoopTriangulator t (p_outerLoop,p_innerLoopVec,ugrid,vgrid);	
	
	AnalyzeSurface an;
	vreal mystep1 = an.GetParStepForU (m_surf,0.5,0.5,m_tol);
	vreal mystep2 = an.GetParStepForV (m_surf,0.5,0.5,m_tol);
	vreal mystep (mystep1);
	if (mystep2 < mystep1)
		mystep = mystep2;
	m_trimmedTri = t.TriangulateOuterLoop (mystep);
	VPoint m,n;
	int mysize = m_trimmedTri->surfpack.parampoints.Size();
	for (int i=0;i<mysize;++i)
	{
		VPoint2d *p = m_trimmedTri->surfpack.parampoints[i];
		this->m_surf.GetPointAndNormal (p->x(),p->y(),m,n);
		(*m_trimmedTri->surfpack.modelpoints[i]) = m;
		(*m_trimmedTri->surfpack.normals[i]) = n;
	}
	
}
bool dSurface::InsertParIfNoCrossToQuads (const SequentialLineSet2d *loop,std::vector<vreal> &ugrids,const std::vector<vreal> &vgrids)
{
	VPoint2d lmin=loop->GetMin();
	VPoint2d lmax=loop->GetMax();
	bool uck = DoesIntervalCrossGrids (lmin.x(),lmax.x(),ugrids);
	if (uck==false)
	{
		bool vck = DoesIntervalCrossGrids (lmin.y(),lmax.y(),vgrids);
		if (vck == false)
		{
			// Add param to ugrid
			ugrids.push_back ((lmin.x()+lmax.x()) *0.5 +0.00000000102702);
			std::sort (ugrids.begin(),ugrids.end());
			return true;
		}
	}
	return false;
}

bool dSurface::DoesIntervalCrossGrids(const vreal i0, const vreal i1, const std::vector<vreal> &grids)
{
	int mysize = (int)grids.size()-1;
	for (int i=0;i<mysize;++i)
	{
		vreal g0 = grids[i];
		vreal g1 = grids[i+1];
		if (i0<g0 && i1>g0)
			return true;
		if (i0<g1 && i1>g1)
			return true;
		if (i0>=g0 && i0<=g1 && i1>=g0 && i1<=g1)
			return false;
	}
	return false;
}

VRefTriangleMesh * dSurface::MakeTriangleMesh(const vreal tol)
{
	double tmptol = tol;
	VRefTriangleMesh *mymesh = 0;
	if (m_changeBigSurfaceTol && IsBigSurface())
	{
		tmptol = m_bigSurfaceTol > tol ? m_bigSurfaceTol : tol;
		std::map<double,VRefTriangleMesh*>::iterator searchTrii = m_tolTrimmedTriMap.find (tmptol);
		if (searchTrii != m_tolTrimmedTriMap.end())
		{
			mymesh = searchTrii->second;
		}
	}
	std::map<double,VRefTriangleMesh*>::iterator searchTrii = m_tolTrimmedTriMap.find (tol);
	if (searchTrii != m_tolTrimmedTriMap.end())
	{
		mymesh = searchTrii->second;
	}
	if (mymesh!=0)
	{
		VRefTriangleMesh * result = new VRefTriangleMesh (*mymesh);
		return result;
	}
	else
	{
		return CalculateTriangles (tmptol,false);		
	}
}

const double dSurface::GetTolerance() const
{
	return m_tol;
}

//void dSurface::ShiftAllLoops(UntrimmedSurface &surf)
//{
//	ShiftSingleLoop(p_outerLoop , surf);
//	size_t mysize = p_innerLoopVec.size();
//	for (size_t i=0;i<mysize;++i)
//	{
//		SequentialLineSet2d *curinnerloop = p_innerLoopVec.at(i);
//		ShiftSingleLoop (curinnerloop , surf);
//	}
//}

//void dSurface::ShiftSingleLoop (SequentialLineSet2d *curloop , UntrimmedSurface &surf)
//{
//	vreal ushift = surf.GetUParShift();
//	vreal vshift = surf.GetVParShift();
//	int mysize = curloop->GetPointSize();
//	for (int i=0;i<mysize;++i)
//	{
//		VPoint2d *curp = curloop->GetPoint(i);
//		vreal newu,newv;
//		newu = curp->x() - ushift;
//		newv = curp->y() - vshift;
//		curp->x(newu);
//		curp->y(newv);
//	}
//	curloop->GetPointSet().UpdateMinMax();
//}

void dSurface::Retesselate (double tol)
{
	try
	{
		m_tol = tol;
		//m_isTransformedBackUp = m_surf.IsTransformed();
		//if (m_isTransformedBackUp)
		//{
		//	m_tmBackUp = m_surf.GetTransformation();
		//	m_surf.ClearTransformation();
		//}
		m_trimmedTriBackUp = m_trimmedTri;
		FormTrimmedTri (false);
		//if (m_trimmedTriBackUp)
		//	delete m_trimmedTriBackUp;
		BuildWireFrame();
	}	
	catch (VException &e)
	{
		m_trimmedTri = m_trimmedTriBackUp;
		//if (m_isTransformedBackUp)
		// 	m_surf.ResetTransform (m_tmBackUp);
		throw e;
	}
	catch (std::exception &e)
	{
		m_trimmedTri = m_trimmedTriBackUp;
		//if (m_isTransformedBackUp)
		//	m_surf.ResetTransform (m_tmBackUp);
		throw e;
	}
	catch (...)
	{
		m_trimmedTri = m_trimmedTriBackUp;
		//if (m_isTransformedBackUp)
		//	m_surf.ResetTransform (m_tmBackUp);
		throw VException (( IDS_UnkExcInDSuRet),1);
	}
}

void dSurface::RollbackPreviousTransform()
{
	if (m_isTransformedBackUp)
	{
		m_surf.ResetTransform (m_tmBackUp);
		m_trimmedTri->Transform (m_tmBackUp);
	}
}

std::map<double,VRefTriangleMesh *> & dSurface::GetTolTrimmedTriMap()
{
	return m_tolTrimmedTriMap;
}

void dSurface::BuildWireFrame()
{
	int stepno = 30;
	m_wireframe = new LineSet (5 * stepno);
	//MakeUSection (0.5,stepno,*m_wireframe);
	//int lineCount = m_wireframe->Size();
	//if (lineCount != 0)
	//{
	//	m_snapPoints.PushBack(m_wireframe->At(0).GetP1());
	//	m_snapPoints.PushBack(m_wireframe->At(lineCount - 1).GetP2());
	//	m_snapPoints.PushBack(m_wireframe->At((int)(lineCount*0.5) ).GetP2());
	//}
	//MakeVSection (0.5,stepno,*m_wireframe);	
	//int lineCount2 = m_wireframe->Size();
	//if (lineCount < lineCount2 && lineCount2 != 0)
	//{
	//	m_snapPoints.PushBack(m_wireframe->At(lineCount).GetP1());
	//	m_snapPoints.PushBack(m_wireframe->At(lineCount2-1).GetP2());
	//}
	
	if (m_trimmedTri)
	{
		m_trimmedTri->FindEdges (*m_wireframe);
	}

	//MakeUSection (0.0,stepno,*m_wireframe);
	//int lineCount = m_wireframe->Size();
	//if (lineCount != 0)
	//{
	//	m_snapPoints.PushBack(m_wireframe->At(0).GetP1());
	//	m_snapPoints.PushBack(m_wireframe->At(lineCount - 1).GetP2());
	//	m_snapPoints.PushBack(m_wireframe->At((int)(lineCount*0.5) ).GetP2());
	//}

	//MakeVSection (0.0,stepno,*m_wireframe);	
	//int lineCount2 = m_wireframe->Size();
	//if (lineCount < lineCount2 && lineCount2 != 0)
	//{
	//	m_snapPoints.PushBack(m_wireframe->At(lineCount).GetP1());
	//	m_snapPoints.PushBack(m_wireframe->At(lineCount2-1).GetP2());
	//	m_snapPoints.PushBack(m_wireframe->At((int)((lineCount + lineCount2)*0.5)).GetP2());
	//}

	//MakeUSection(1.0, stepno, *m_wireframe);
	//int lineCount3 = m_wireframe->Size();
	//if (lineCount3 > lineCount2 && lineCount3 != 0)
	//{
	//	m_snapPoints.PushBack(m_wireframe->At(lineCount2).GetP1());
	//	m_snapPoints.PushBack(m_wireframe->At(lineCount3 - 1).GetP2());
	//	m_snapPoints.PushBack(m_wireframe->At((int)((lineCount3+lineCount2)*0.5)).GetP2());
	//}

	//MakeVSection(1.0, stepno, *m_wireframe);
	//int lineCount4 = m_wireframe->Size();
	//if (lineCount4 > lineCount3 && lineCount4 != 0)
	//{
	//	m_snapPoints.PushBack(m_wireframe->At(lineCount3).GetP1());
	//	m_snapPoints.PushBack(m_wireframe->At(lineCount4 - 1).GetP2());
	//	m_snapPoints.PushBack(m_wireframe->At((int)((lineCount4 + lineCount3)*0.5)).GetP2());
	//}

	VPoint midPoint, midNormal;
	m_surf.GetPointAndNormal(0.5, 0.5, midPoint, midNormal);
	m_snapLines.AddLine(VLine(midPoint - midNormal, midPoint + midNormal));
	m_snapPoints.PushBack(midPoint);
	int stepi = (int)((m_wireframe->Size() / 10))+1;
	if (stepi < 1)
		throw VException("dSurface::BuildWireFrame exception. stepi is zero.",1);
	for (int i = 0; i < m_wireframe->Size(); i += stepi)
	{
		m_snapPoints.PushBack(m_wireframe->At(i).GetP1());
	}
}
	
VRefTriangleMesh * dSurface::GetRoughTri()
{
	return m_roughTri;
}

void dSurface::SetRoughTri (VRefTriangleMesh *roughTri)
{
	m_roughTri = roughTri;
}

dSurface::SurfaceSubType dSurface::GetSurfaceSubType()const
{
	int etype = GetSurface().m_surf->entitytype;
	switch (etype)
	{
	case 128:
		return bspline;
	case 108:
		return plane;
	case 118:
		return ruled;
	case 120:
		return revolved;
	case 122:
		return tabulatedcylinder;
	case 114:
		return parametricspline;
	case 140:
		return offset;
	};
	throw VException ("dSurface::GetSurfaceSubType exception , iges entity type not expected",1);
}

void dSurface::UpdateBSplineControlPointsIf()
{
	if (GetSurfaceSubType() == bspline)
	{
		BSplineSurfacePointGenerator &bspgen = static_cast<BSplineSurfacePointGenerator &> (m_surf.GetDigitiser());
		bspgen.GetNE().Update();
	}
}

void dSurface::TransformParametricLoopIf(int surfEntityType , SequentialLineSet2d *loop)
{
	if (m_transformParametricLoop == true)
	{
		loop->Transform(*m_MParametricLoop);
		loop->Transform(*m_SParametricLoop);
	}
}
void dSurface::CalculateLoopTransformationIf(int surfEntityType , SequentialLineSet2d *outerLoop)
{
	if (m_transformParametricLoop)
	{
		VPoint2d olmn = outerLoop->GetMin();
		VPoint2d olmx = outerLoop->GetMax();
		VPoint2d deltal = olmx - olmn;

		if (surfEntityType == 190)
		{
			// plane entity
			m_MParametricLoop = new MoveMatrix(-VPoint(olmn.x(),olmn.y(),0.0));
			vreal sx = 1. / deltal.x();
			vreal sy = 1. / deltal.y();
			m_SParametricLoop = new ScaleMatrix(sx,sy,1.0);
		}
		else if (surfEntityType == 192 || surfEntityType == 194)
		{
			// cylindrical entity or conical entity
			m_MParametricLoop = new MoveMatrix(-VPoint(0.0,olmn.y(),0.0));
			vreal sx = 1. / 360.0;
			vreal sy = 1. / deltal.y();
			m_SParametricLoop = new ScaleMatrix(sx, sy, 1.0);
		}
		else if (surfEntityType == 196)
		{
			// spherical entity
			m_MParametricLoop = new MoveMatrix(VPoint(0.0, 90.0, 0.0));
			vreal sx = 1. / 360.0;
			vreal sy = 1. / 180.0;
			m_SParametricLoop = new ScaleMatrix(sx, sy, 1.0);
		}
		else if (surfEntityType == 198)
		{
			// toroidal surface entity
			m_MParametricLoop = new MoveMatrix(VPoint(0.0,0.0, 0.0));
			vreal sx = 1. / 360.;
			vreal sy = 1. / 360.;
			m_SParametricLoop = new ScaleMatrix(sx, sy, 1.0);
		}
		else
		{
			VPoint2d curmn = outerLoop->GetMin();
			VPoint2d curmx = outerLoop->GetMax();
			//bool doRepar = false;
			//if (curmn.x() < m_surf.U0()+1.e-7 || curmx.x() > m_surf.U1()-1.e-7)
			//	doRepar = true;
			//if (curmn.y() < m_surf.V0()+1.e-7 || curmx.y() > m_surf.V1()-1.e-7)
			//	doRepar = true;

			//if (doRepar)
			//{
				m_MParametricLoop = new MoveMatrix(VPoint(m_surf.U0(), m_surf.V0(), 0.0));
				vreal sx = (m_surf.U1() - m_surf.U0()) / (curmx.x() - curmn.x());
				vreal sy = (m_surf.V1() - m_surf.V0()) / (curmx.y() - curmn.y());
				m_SParametricLoop = new ScaleMatrix(sx, sy, 1.0);
			//}
			//else
			//	m_transformParametricLoop = false;
		}
	}
}

LineSet & dSurface::GetSnapLines()
{
	return m_snapLines;
}
const LineSet & dSurface::GetSnapLines()const
{
	return m_snapLines;
}
PointSet & dSurface::GetSnapPoints()
{
	return m_snapPoints;
}
const PointSet & dSurface::GetSnapPoints()const
{
	return m_snapPoints;
}