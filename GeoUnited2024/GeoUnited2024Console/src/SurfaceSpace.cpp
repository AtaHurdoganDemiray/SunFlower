// SurfaceSpace.cpp

#include "stdafx.h"
#include "SurfaceSpace.h"

#include "VPoint.h"
#include "PointSet2d.h"
#include "PointSet.h"
#include "UntrimmedSurface.h"
#include "PosCubes.h"
#include "TriangleMeshPointSetMinDist.h"
#include "MultiTriangles.h"
#include "MultiPoints.h"
#include "sPoints.h"
#include "VRefTriangleMesh.h"
#include "SequentialLineSet.h"
#include "SequentialLineSet2d.h"
#include "UntrimmedSurface.h"

SurfaceSpace::SurfaceSpace(UntrimmedSurface *surf,double tol,int pointCountInOneDirection , int refinePointCountInOneDirection)
:m_surf(surf),m_parVec (0),m_modVec(0),m_ownerOfParModVec(true),m_tol(tol),m_pointCountInOneDirection(pointCountInOneDirection),m_refinePointCountInOneDirection(refinePointCountInOneDirection)
{
	MakeParModVec(m_pointCountInOneDirection * m_pointCountInOneDirection);
	FillParModVec(m_pointCountInOneDirection);
	m_pc = MakePosCubes(m_modVec);
}
SurfaceSpace::SurfaceSpace (PointSet2d *parVec , PointSet *modVec,UntrimmedSurface *surf,double tol,int pointCountInOneDirection , int refinePointCountInOneDirection)
:m_surf(surf),m_parVec(parVec),m_modVec(modVec),m_ownerOfParModVec(false),m_tol(tol),m_pointCountInOneDirection(pointCountInOneDirection),m_refinePointCountInOneDirection(refinePointCountInOneDirection)
{
	m_pc = MakePosCubes (m_modVec);
}
SurfaceSpace::~SurfaceSpace()
{
	if (m_ownerOfParModVec)
	{
		delete m_parVec;
		delete m_modVec;
	}
}

int SurfaceSpace::m_maxRefineIterationCount = 30;

VPoint SurfaceSpace::Convert(const VPoint &p)
{
	// Find the nearest point for p in modvec
	LongVec relind;
	VPoint cubcenter;
	m_pc->GetRelatedIndicesForMinDist(p,relind,cubcenter);
	int mysize = relind.Size();
	vreal mindist (MAX_NUMBER_LIMIT);
	int mini = -1;
	for (int i=0;i<mysize;++i)
	{
		int curi = relind[i];
		VPoint *curp = m_modVec->operator [](curi);
		vreal curdist = (*curp -p).GetLength2();
		if (curdist < mindist)
		{
			mindist = curdist;
			mini = curi;
		}
	}

	if (mini == -1)
		throw VException (( IDS_SurConExcMinPoi),1);
	// We now know the nearest point , refine the solution around this point if not close enough
	VPoint2d refinedPar (*m_parVec->operator [](mini));
	VPoint refinedMod (*m_modVec->operator [](mini));
	int counter = 0;
	double dprev = 1./m_pointCountInOneDirection;
	while (sqrt(mindist) > m_tol)
	{
		RefineSolutionAround (p,refinedPar,refinedMod,dprev,mindist);
		++counter;
		if (counter > m_maxRefineIterationCount)
		{
			if (sqrt(mindist) > m_tol*10.0)
				throw VException((IDS_SurConExcMaxIte), 1);
			else
				LogFileWriter::WriteUnknownExceptionLog("SurfaceSpace::Convert error distance bigger than tolerance warning!");
		}
	}
	
	return VPoint(refinedPar.x(),refinedPar.y(),0.0);
}

VPoint SurfaceSpace::ConvertThrowIfOut(const VPoint &p , double tol)
{
	return VPoint(0.0);
}

void SurfaceSpace::SetParModVec(PointSet2d *parVec,PointSet *modVec)
{
	m_parVec = parVec;
	m_modVec = modVec;
	m_ownerOfParModVec = false;
}

void SurfaceSpace::MakeParModVec(int size)
{
	m_parVec = new PointSet2d (size);
	m_modVec = new PointSet (size);
	m_ownerOfParModVec = true;
}

void SurfaceSpace::FillParModVec(int pointNoInOneDirection)
{
	if (pointNoInOneDirection < 2)
		throw VException (( IDS_SurFilExcPoiNoI),1);
	double dp = 1. / pointNoInOneDirection;
	double pmax = 1.+1.e-8;
	for (double v=0;v<pmax;v+=dp)
	{
		for (double u=0;u<pmax;u+=dp)
		{
			VPoint2d pp (u,v);
			VPoint pm = m_surf->GetPoint(u,v);
			m_parVec->InsertPoint(pp);
			m_modVec->InsertPoint(pm);
		}
	}
}

PosCubes * SurfaceSpace::MakePosCubes(PointSet *pset)
{
	PosCubes *pc = new PosCubes (*pset);
	return pc;
}

void SurfaceSpace::SetPointCountInOneDirection(int pointCountInOneDirection)
{
	m_pointCountInOneDirection = pointCountInOneDirection;
}

int SurfaceSpace::GetPointCountInOneDirection()const
{
	return m_pointCountInOneDirection;
}

void SurfaceSpace::SetRefinePointCountInOneDirection(int refinePointCountInOneDirection)
{
	m_refinePointCountInOneDirection = refinePointCountInOneDirection;
}

int SurfaceSpace::GetRefinePointCountInOneDirection()const
{
	return m_refinePointCountInOneDirection;
}

void SurfaceSpace::RefineSolutionAround(const VPoint &p , VPoint2d &refinedPar , VPoint &refinedMod , double &dprev , double &mindist)
{
	double dnew = dprev;//(6*dprev) / m_refinePointCountInOneDirection;
	if (dnew < 1.e-14)
		return;
	double vstart = refinedPar.y()-dprev*10.0;
	double vend = refinedPar.y()+dprev*10.0;
	double ustart = refinedPar.x()-dprev*10.0;
	double uend = refinedPar.x()+dprev*10.0;
	if (vstart<0.0)
		vstart = 0.0;
	else if (vstart > 1.0)
		vstart = 1.0;
	if (ustart<0.0)
		ustart = 0.0;
	else if (ustart>1.0)
		ustart = 1.0;
	if(vend < 0.0)
		vend = 0.0;
	else if (vend > 1.0)
		vend = 1.0;
	if (uend < 0.0)
		uend = 0.0;
	else if (uend > 1.0)
		uend = 1.0;
	for (double v=vstart;v<vend;v+=dnew)
	{
		for (double u=ustart;u<uend;u+=dnew)
		{
			VPoint curp = m_surf->GetPoint(u,v);
			double curdist = (p-curp).GetLength2();
			if (curdist < mindist)
			{
				refinedPar = VPoint2d (u,v);
				refinedMod = curp;
				mindist = curdist;
			}
		}
	}
	dprev = dnew;
}

void SurfaceSpace::ProjectModelSpaceTrimLoops (const SequentialLineSet &modelLoop 
										   , SequentialLineSet2d &paramLoop
										   , std::vector <VPoint> &witnessPoints
										   , UntrimmedSurface *surf)
{
	std::vector <const Triangles *> mytvec;
	std::vector <const Points *>mypvec;
	SequentialLineSet slmore(modelLoop.Size());
	sPoints myspoints(modelLoop.GetPointSet());
	VRefTriangleMesh * tmpTri = surf->Digitise(m_tol);
	mytvec.push_back (tmpTri);
	mypvec.push_back (&myspoints);
	MultiTriangles multiTri(mytvec);
	MultiPoints multiPo(mypvec);
	TriangleMeshPointSetMinDist mycalc(multiTri,multiPo);	
	std::vector<vreal> allmindist;
	std::vector<int> triangleind;
	//std::vector<VPoint>witnessPoints;
	mycalc.MinDistAllTrianglesAndAllPoints (allmindist,triangleind,witnessPoints);
	int mysize = modelLoop.GetPointSize();
	for (int i=0;i<mysize;i++)
	{
		VPoint &currpoint = witnessPoints.at(i);
		int currtriindex = triangleind[i];
		VTriangleE currmtri = tmpTri->operator [](currtriindex);	
		VTriangle2dE currptri = tmpTri->GetParTri (currtriindex);	
		vreal u,v;
		currmtri.GetUV (currpoint , u , v);
		bool ck = currmtri.IsInTriangle (currpoint);
		if (!ck)
		{
			throw VException(( IDS_PointOutTri),1);
		}
		VPoint2d parpoint = currptri.GetP (u,v);
		VPoint calcSurfPoint = surf->GetPoint(parpoint.x(), parpoint.y());


		double dp = (currptri.GetMax() - currptri.GetMin()).GetLength() / 60.0;//GetDpAroundForTolerance (parpoint,surf);
		const VPoint *modpoint = &currpoint;//modelLoop.GetPoint(i);
		double dist = ((*modpoint) - /*currpoint*/calcSurfPoint).GetLength2();
		//while (dist > m_tol * m_tol)
		for (int j = 0; j < 2; ++j)
		{
			RefineSolutionAround(*modpoint, parpoint,/*currpoint*/calcSurfPoint, dp, dist);
			dp /= 8.0;
		}
		if (dist > m_tol*m_tol)
		{
			CString tmp;
			tmp.Format("Position error while trimming is : %.4f", sqrt(dist));
			LogFileWriter::WriteUnknownExceptionLog((LPCTSTR)tmp);
		}
		//RefineSolutionAround(*modpoint,parpoint,currpoint,dp,dist);
		//RefineSolutionAround(*modpoint,parpoint,currpoint,dp,dist);

		paramLoop.InsertPoint (parpoint);
	}
	delete tmpTri;
}

double SurfaceSpace::GetDpAroundForTolerance (const VPoint2d &par,UntrimmedSurface *surf)
{
	VPoint p0 = surf->GetPoint(par.x(),par.y());
	VPoint p1 = surf->GetPoint(par.x()+m_tol,par.y()+m_tol);
	double len = (p1-p0).GetLength();
	//double DLforoneparchange = len / m_tol;
	double dp = m_tol * m_tol * len;
	return dp;
}
