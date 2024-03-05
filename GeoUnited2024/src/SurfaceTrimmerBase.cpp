// SurfaceTrimmerBase.cpp

#include "stdafx.h"
#include "SurfaceTrimmerBase.h"
#include"IgesTrimLoop2d.h"
#include "IgesEntitySet.h"
#include "UntrimmedSurface.h"
#include "SequentialLineSet2d.h"
#include "IgesEntity.h"
#include "PosCubes2d.h"
#include "CurvePointGenerator.h"
#include "CurvePointGeneratorFactory.h"
#include "DoubleVec.h"

SurfaceTrimmerBase::SurfaceTrimmerBase (double tol)
:m_tol (tol),p_outerLoop(0)
{
	
}

SurfaceTrimmerBase::~SurfaceTrimmerBase()
{
	delete p_outerLoop;
	int mysize = (int)p_innerLoopVec.size();
	for (int i=0;i<mysize;i++)
	{
		delete p_innerLoopVec[i];
	}
}

void SurfaceTrimmerBase::MakeLoops2d (const IgesTrimLoop2d &outerloop 
							, const std::vector<IgesTrimLoop2d *> &innerloop 
							, const IgesEntitySet &allent ,const UntrimmedSurface &surf)
{
	//2D LOOPS
	//OrderSequentialLineSets<SequentialLineSet2d,VPoint2d>order;
	//OrderSequentialLineSets<SequentialLineSet,VPoint>order3d;
	p_outerLoop = new SequentialLineSet2d (100);
	int size1 = static_cast<int>(outerloop.loopcurves.size());
	//std::vector<SequentialLineSet2d> tmploopsouter (size1,100);
	for (int i=0; i<size1; i++)
	{
		const IgesEntity *ent =outerloop.loopcurves[i];
		//SequentialLineSet2d &currsl = tmploopsouter[i];
		DigitiseLoop (ent ,allent,surf,*p_outerLoop,false);
	}
	//p_outerLoop = order.MakeOrderedUnion (tmploopsouter);
	// Inner Loops 2d
	int size3 = static_cast<int>(innerloop.size());
	for (int j=0; j<size3; j++)
	{
		SequentialLineSet2d *loop = new SequentialLineSet2d (100);
		const IgesTrimLoop2d *tl = innerloop[j];
		int size4 = static_cast<int>(tl->loopcurves.size());
		//std::vector<SequentialLineSet2d>tmpinnerloops (size4,100);
		
		for (int w=0; w<size4; w++)
		{
			const IgesEntity *ent = tl->loopcurves[w];
			//SequentialLineSet2d &currsl = tmpinnerloops[w];
			DigitiseLoop (ent,allent,surf,*loop,false);
		}
		//SequentialLineSet2d *loop = order.MakeOrderedUnion(tmpinnerloops);
		p_innerLoopVec.push_back (loop);
	}
}

void SurfaceTrimmerBase::DigitiseLoop (const IgesEntity *ent 
							  , const IgesEntitySet &allEnt
							  , const UntrimmedSurface &surf
							  , SequentialLineSet2d &loop
							  ,bool isReversed)
{
	IgesEntity *copent = ent->MakeCopyAndAddToSet (allEnt,m_eSet);
	CurvePointGenerator *crv = CurvePointGeneratorFactory::MakeCurve (copent , m_eSet);
	if (isReversed)
		crv->SetReversed (true);
	bool shouldIswap = ShouldIReverse (*crv,loop,surf);//needed for trimmedsurf
	if (shouldIswap)
		crv->SetReversed (!isReversed);
	DoubleVec params;
	//crv->Digitise (m_tol , surf.U0() , surf.U1() , surf.V0() , surf.V1() , loop , params);
	crv->Digitise (m_tol , surf , loop , params);
}

bool SurfaceTrimmerBase::ShouldIReverse (const CurvePointGenerator &crv , const SequentialLineSet2d &sl , const UntrimmedSurface &surf)
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

//void SurfaceTrimmerBase::MakeLoopsForBoundedSurf (const ProcessedBoundedSurface &pbs)
//{
//	std::vector <SequentialLineSet2d *>allloops;
//	//std::vector <dCurve *>allloops3d;
//	std::vector <PosCubes2d *>cubVec;
//
//	int mysize = static_cast<int>(pbs.m_loop3d.size());
//	for (int i=0;i<mysize;i++)
//	{
//		IgesTrimLoop2d *currloop2d = pbs.m_loop2d[i];
//		IgesTrimLoop *currloop = pbs.m_loop3d[i];
//		
//		SequentialLineSet2d *currsl = new SequentialLineSet2d (100);
//		int size2 = static_cast<int>(currloop2d->loopcurves.size());
//		
//		for (int j=0;j<size2;j++)
//		{
//			IgesEntity *current = currloop2d->loopcurves[j];
//			bool isReversed = currloop2d->m_isReversed[j];
//			DigitiseLoop (current,pbs.allent,m_surf,*currsl,isReversed);
//		}
//		allloops.push_back (currsl);
//		PosCubes2d *cub = new PosCubes2d (*currsl);
//		cubVec.push_back (cub);
//
//		int size3 = static_cast<int>(currloop->loopcurves.size());
//		for (int w=0;w<size3;w++)
//		{
//			IgesEntity *current = currloop->loopcurves[w];
//			bool isReversed = currloop->m_isReversed[w];
//			DigitiseLoop3d (current,pbs.allent,isReversed,m_outerLoopVec);
//		}
//	}
//
//	int size4 = static_cast<int>(allloops.size());
//	
//	for (int i=0;i<size4;i++)
//	{
//		SequentialLineSet2d *currloop = allloops[i];
//		PosCubes2d *currcub = cubVec[i];
//		bool includingAll = true;
//
//		for (int j=0;j<size4;j++)
//		{
//			if (j==i)
//				continue;
//			SequentialLineSet2d *otherloop = allloops[j];
//			const VPoint2d *otherpoint = otherloop->Start();
//			if (IsPointInLoop (*otherpoint,currloop,currcub)==false)
//			{
//				includingAll=false;
//				break;
//			}
//		}
//		if (includingAll)
//		{
//			p_outerLoop = currloop;
//			//m_outerLoop = currloop3d;
//			cubesOuterLoop = currcub;
//		}
//		else
//		{
//			p_innerLoopVec.push_back (currloop);
//			//m_innerLoopVec.push_back (currloop3d);
//			cubesInnerLoopVec.push_back (currcub);
//		}
//	}
//	if (p_outerLoop == 0)
//	{
//		std::vector<SequentialLineSet2d *>::iterator i=p_innerLoopVec.begin();
//		//std::vector<SequentialLineSet *>::iterator ii=m_innerLoopVec.begin();
//		std::vector<SequentialLineSet2d *>::iterator e=p_innerLoopVec.end();
//		//std::vector<SequentialLineSet *>::iterator ee=m_innerLoopVec.end();
//		std::vector<PosCubes2d *>::iterator c = cubesInnerLoopVec.begin();
//		std::vector<PosCubes2d *>::iterator lc = cubesInnerLoopVec.begin();
//
//		std::vector<SequentialLineSet2d *>::iterator l;
//		//std::vector<SequentialLineSet *>::iterator ll;
//		vreal maxlen = MIN_NUMBER_LIMIT;
//		
//		for(i;i!=e;i++)
//		{
//			vreal currlen = (*i)->GetLength();
//			if ( currlen > maxlen)
//			{
//				maxlen = currlen;
//				l = i;
//				//ll = ii;
//				lc = c;
//			}
//			//ii++;
//			c++;
//		}
//		p_outerLoop = *l;
//		//m_outerLoop = *ll;
//		cubesOuterLoop = *lc;
//		p_innerLoopVec.erase(l);
//		//m_innerLoopVec.erase(ll);
//		cubesInnerLoopVec.erase (lc);
//	}
//}
