// SurfaceTrimmerDelaunay.cpp

#include "stdafx.h"
#include "SurfaceTrimmerDelaunay.h"
#include "PosCubes2d.h"
#include "VRefTriangleMesh.h"
#include "VDelaunay2d.h"
#include "SequentialLineSet2d.h"
#include "LineIntersection2d.h"
#include "UntrimmedSurface.h"
#include "PointSet2d.h"
#include "AnalyzeSurface.h"
#include "dSurface.h"

SurfaceTrimmerDelaunay::SurfaceTrimmerDelaunay (UntrimmedSurface &surf
											,const SequentialLineSet2d *outerLoop
											,const std::vector <SequentialLineSet2d *> &innerLoopVec
											,dSurface &dsurf)
:m_surf(surf)
,m_outerLoop(outerLoop)
,m_innerLoopVec(innerLoopVec)
,m_dsurf(dsurf)
{

}
const vreal DSURFACE_STEP_3D = 5.0;
void SurfaceTrimmerDelaunay::TrimPointsAndDelaunayTriangulate (const UntrimmedSurface & surf)
{
	//trimming points with trimloops & adding to trimmed triangles

	vreal uperi = AnalyzeSurface::GetAprModelSpacePerimeterAlongU (surf);
	vreal vperi = AnalyzeSurface::GetAprModelSpacePerimeterAlongV (surf);
	

	int uno = (int)(uperi / DSURFACE_STEP_3D  +1) ;
	int vno = (int)(vperi / DSURFACE_STEP_3D +1);
	
	PointSet2d parset ((uno+1)*(vno+1));
	PrepareParPoints (uno,vno,parset);
	MakeTri (parset,surf); 
	AddAllTrimLoopsToSurfPack(surf);

	m_tri->surfpack.RemoveDuplicate();
	VDelaunay2d triangulator;
	triangulator.Calculate (m_tri->surfpack.parampoints,m_tri->GetRefTriangleMesh());

	int size2 = m_tri->Size();
	for (int j=0;j<size2;j++)
	{
		VPoint2d p1 = m_tri->GetParamTriCor1(j);
		VPoint2d p2 = m_tri->GetParamTriCor2(j);
		VPoint2d p3 = m_tri->GetParamTriCor3(j);
		VTriangle2dE t(p1,p2,p3);
		VPoint2d cg = t.GetCenterOfMass();
		bool ck1 = m_dsurf.IsPointTrimmed (cg);
		if (ck1)
		{
			RefTriangle newref (-1,-1,-1);
			m_tri->SetRefTri(j,newref);
		}
	}
	m_tri->CleanTrimmedTriangles();
	//SurfaceDigitiser digi;
	//digi.SubDivide (m_tol , *m_trimmedTri , surf);
}
void SurfaceTrimmerDelaunay::MakeTri (const PointSet2d &parset,const UntrimmedSurface &surf)
{
	int mysize = parset.Size();
	m_tri = new VRefTriangleMesh (260);
	for (int i=0;i<mysize;i++)
	{
		const VPoint2d *p=parset[i];
		if (m_dsurf.IsPointTrimmed (*p) == false)
		{
			const VPoint m = surf.GetPoint(p->x(),p->y());
			const VPoint n (-1,-1,-1);//= surf.GetNormal(p->x(),p->y(),m);
			m_tri->surfpack.InsertPoint(*p,m,n);
		}
	}
}

void SurfaceTrimmerDelaunay::AddAllTrimLoopsToSurfPack (const UntrimmedSurface &surf)
{
	AddTrimLoopToSurfPackSimple (*m_outerLoop,surf);
	int mysize = (int)(m_innerLoopVec.size());
	for (int i=0; i < mysize; i++)
	{
		SequentialLineSet2d * loop = m_innerLoopVec[i];
		AddTrimLoopToSurfPackSimple(*loop,surf);
	}
}
void SurfaceTrimmerDelaunay::AddTrimLoopToSurfPackSimple (const SequentialLineSet2d &sl
														  ,const UntrimmedSurface &surf)
{
	int mysize = sl.Size();
	for (int i=0;i<mysize;i++)
	{
		const VPoint2d *currp = sl.GetPoint (i);
		VPoint m = surf.GetPoint (currp->x() , currp->y());
		VPoint n(-1,-1,-1);
		m_tri->surfpack.InsertPoint(*currp,m,n);
	}
}

void SurfaceTrimmerDelaunay::AddTrimLoopToSurfPack (const SequentialLineSet2d &sl 
													, const UntrimmedSurface &surf)
{
	VPoint mymn = m_tri->surfpack.modelpoints.GetMin();
	VPoint mymx = m_tri->surfpack.modelpoints.GetMax();
	VVector diagVec = mymx - mymn;
	vreal approxMaxLen = diagVec.GetLength();
	vreal parThreshhold = DSURFACE_STEP_3D / approxMaxLen;
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
			VPoint m = 	surf.GetPoint (p.x() , p.y());
			VPoint n (-1,-1,-1);
			m_tri->surfpack.InsertPoint(p,m,n);
			currpar += parstep;
		}
	}
	
	const VPoint2d * lastp = sl.End();
	VPoint lastm = surf.GetPoint (lastp->x() , lastp->y());
	VPoint lastn(-1,-1,-1);
	m_tri->surfpack.InsertPoint(*lastp,lastm,lastn);

}

VRefTriangleMesh * SurfaceTrimmerDelaunay::TriangulateSurf ()
{
	TrimPointsAndDelaunayTriangulate (m_surf);
	m_tri->CalculateNormals();
	return m_tri;
}

void SurfaceTrimmerDelaunay::PrepareParPoints (int uno,int vno,PointSet2d &parset)
{
	vreal du = 1. / uno;
	vreal dv = 1. / vno;
	vreal ub = 0.0;
	vreal vb = 0.0;
	for (int u=0;u<uno+1;u++)
	{
		for (int v=0;v<vno+1;v++)
		{
			VPoint2d tmp (ub,vb);
			parset.InsertPoint (tmp);
			vb+= dv;
		}
		vb = 0.0;
		ub += du;
	}
}
