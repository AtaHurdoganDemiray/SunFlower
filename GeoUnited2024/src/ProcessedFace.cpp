// ProcessedFace.cpp

// ProcessedTrimmedSurface.cpp

#include "stdafx.h"
#include "ProcessedFace.h"
#include "IgesEntitySet.h"
#include "IgesEntity.h"
#include "CurveOnParametricSurface.h"
#include "Face.h"
#include "CompositeCurve.h"
#include "IgesTrimLoop2d.h"
#include "Loop.h"

ProcessedFace::ProcessedFace
(Face & _tsurf, IgesEntitySet & _allent , std::map<int,std::map<int,dCurve *>> &edgeLines)
:tsurf(_tsurf), allent(_allent), m_is2dLoopsGiven(false), m_edgeLines(edgeLines)
{
	surf = allent.GetEntityPointer(tsurf.m_SURF);
	if (surf == 0 || allent.IsSupportedSurfaceType(surf) == false)
		throw (VException((IDS_SurNotSup), 0));

	allent.MakePartOfTrimmedSurface(surf);
	FormAllTrimLoops();
}

//#############################################################################

ProcessedFace::~ProcessedFace()
{
	unsigned int size1 = (unsigned int)(innerloop.size());
	for (size_t i = 0; i<size1; i++)
	{
		IgesTrimLoop2d *tl = innerloop[i];
		delete tl;
	}

	unsigned int size2 = (unsigned int)(m_innerloop.size());
	for (size_t i = 0; i<size2; i++)
	{
		IgesTrimLoop *tl = m_innerloop[i];
		delete tl;
	}
}

//#############################################################################

void ProcessedFace::FormAllTrimLoops()
{
	int lsize = tsurf.m_N;
	if (tsurf.m_OF == 0)
	{
		// outer loop not defined 
		throw VException("ProcessedFace::FormAllTrimLoops , outer loop not defined",1);
	}
	else
	{
		int outerloopde = tsurf.m_LOOP.at(0);
		Loop *outerloop11 = static_cast<Loop *> (allent.GetEntityPointer(outerloopde));
		outerloop11->AddLoopCurves(allent, m_edgeLines, m_outerLoop);
		std::vector<IgesTrimLoop2d *> tempVec;
		outerloop11->AddLoopCurves2d(allent, tempVec);
		if (tempVec.size() != 1)
			throw VException("ProcessedFace::FormAllTrimLoops exception , outer loop count not 1",1);
		this->outerloop = *tempVec.at(0);
		for (int li = 1; li < lsize; ++li)
		{
			int innerloopde = tsurf.m_LOOP.at(li);
			Loop *curinnerloop = static_cast<Loop *>(allent.GetEntityPointer(innerloopde));
			std::vector<dCurve*> loopv;
			curinnerloop->AddLoopCurves(allent, m_edgeLines,loopv);
			m_innerLoops.push_back(loopv);
			curinnerloop->AddLoopCurves2d(allent, this->innerloop);
		}
		if (outerloop.loopcurves.size() > 0)
			m_is2dLoopsGiven = true;
	}

}

//#############################################################################

void ProcessedFace::FormOuterLoop()
{
	// form the outer trimloop	if exists
	// is there an outer boundary?
	if (tsurf.m_N == 1 && tsurf.m_OF != 0)
	{
		// there is outer boundary
		long outerloopref = tsurf.m_LOOP.at(0);
		bool triminside = false;

		IgesEntity * p_igesent =
			allent.GetEntityPointer(outerloopref);

		if (p_igesent == 0)
			throw (VException((IDS_UnsIgeEnt), 0));

		if (p_igesent->entitytype == 142)
		{
			// this is curve on a parametric surface
			CurveOnParametricSurface * pcops =
				(CurveOnParametricSurface*)p_igesent;
			allent.MakePartOfTrimmedSurface(p_igesent);

			long modelSpace = pcops->CPTR;
			IgesEntity * pmodelCurve = allent.GetEntityPointer(modelSpace);
			//allent.MakePartOfTrimmedSurface ( pmodelCurve );
			FormLoop3d(pmodelCurve, triminside);

			long pccref = pcops->BPTR;
			IgesEntity * ptmp = 0;
			if (pccref)
				ptmp = allent.GetEntityPointer(pccref);

			if (ptmp == 0)
			{
				m_is2dLoopsGiven = false;
				if (surf->entitytype == 108)
				{
					//this is a plane surface
				}
				else
				{
					//allent.TakeOutOfTrimmedSurface (surf);				
					//throw ( VException ( ("processed trimmed surface exception 4,unsupported iges entity") , 0 ));
				}
			}
			else
				FormLoop(ptmp, triminside);
		}
		else
			throw (VException((IDS_ProTriSurExc), 0));

	}
	else // N1==0
		return;

}

//#############################################################################

void ProcessedFace::FormInnerLoops()
{
	// fill the inner trimloops	if exists
	int loopcount = tsurf.m_N;

	if (loopcount > 0)
	{
		bool triminside = true;

		// there is inner boundaries
		for (int i = 0; i < loopcount; i++)
		{
			long loopref = 1;//tsurf.PTI[i];

			IgesEntity * p_igesent =
				allent.GetEntityPointer(loopref);

			if (p_igesent == 0)
				throw (VException((IDS_ProTriSurExcUns), 0));

			if (p_igesent->entitytype == 142)
			{
				// this is curve on a parametric surface
				CurveOnParametricSurface * pcops =
					(CurveOnParametricSurface*)p_igesent;
				allent.MakePartOfTrimmedSurface(p_igesent);

				long modelSpace = pcops->CPTR;
				IgesEntity * pmodelCurve = allent.GetEntityPointer(modelSpace);
				//allent.MakePartOfTrimmedSurface ( pmodelCurve );
				FormLoop3d(pmodelCurve, triminside);

				long pccref = pcops->BPTR;
				IgesEntity * ptmp = 0;

				if (pccref)
					ptmp = allent.GetEntityPointer(pccref);

				if (ptmp == 0)
				{
					m_is2dLoopsGiven = false;
					if (surf->entitytype == 108)
					{
						//this is plane surface
					}
					else
					{
						//allent.TakeOutOfTrimmedSurface (surf);
						//throw ( VException ( ("processed trimmed surface exception 4,unsupported iges entity") , 0 ));
					}
				}
				else
					FormLoop(ptmp, triminside);
			}
			else
				throw (VException((IDS_ProTriSurExc1), 0));
		}
	}
	else // N2==0
		return;

}

//#############################################################################

void ProcessedFace::FormLoop(IgesEntity * ptmp, bool triminside)
{
	IgesTrimLoop2d * loop;
	if (triminside == false)
		loop = &outerloop;
	else
		loop = new IgesTrimLoop2d;

	if (ptmp->entitytype == 102)
	{
		// this is composite curve
		CompositeCurve * pcc = (CompositeCurve*)ptmp;
		allent.MakePartOfTrimmedSurface(ptmp);
		for (int i = 0; i < pcc->N; i++)
		{
			long currDE = pcc->mDE[i];
			IgesEntity *pcrv = allent.GetEntityPointer(currDE);
			if (pcrv == 0)
			{
				VOStrStream mystream;
				mystream << (IDS_ProTriSurExcUns1) << currDE;
				throw (VException(mystream.str(), 0));
			}
			if (pcrv->entitytype == 116)
				continue; //this is a point		

			if (allent.IsSupportedCurveType(pcrv))
			{
				loop->loopcurves.push_back(pcrv);
				allent.MakePartOfTrimmedSurface(pcrv);
			}
			else
			{
				VOStrStream mystream;
				mystream << (IDS_ProTriSurExcUns1) << pcrv->entitytype;
				throw (VException(mystream.str(), 0));
			}
		}
	}
	else if (allent.IsSupportedCurveType(ptmp))
	{
		// this is a single bspline curve
		loop->loopcurves.push_back(ptmp);
		allent.MakePartOfTrimmedSurface(ptmp);
	}
	else
	{
		VOStrStream mystream;
		mystream << (IDS_ProTriSurExcUns1) << ptmp->entitytype;
		throw (VException(mystream.str(), 0));
	}

	loop->Order(&allent);
	if (triminside == true)
	{
		innerloop.push_back(loop);
	}

}

//#############################################################################

void ProcessedFace::FormLoop3d(IgesEntity * ptmp, bool triminside)
{
	IgesTrimLoop * loop;
	if (triminside == false)
		loop = &m_outerloop;
	else
		loop = new IgesTrimLoop;

	if (ptmp->entitytype == 102)
	{
		// this is composite curve
		CompositeCurve * pcc = (CompositeCurve*)ptmp;
		allent.MakePartOfTrimmedSurface(ptmp);
		for (int i = 0; i < pcc->N; i++)
		{
			long currDE = pcc->mDE[i];
			IgesEntity *pcrv = allent.GetEntityPointer(currDE);
			if (pcrv == 0)
			{
				VOStrStream mystream;
				mystream << (IDS_ProTriSurExcUns1) << currDE;
				throw (VException(mystream.str(), 0));
			}
			if (pcrv->entitytype == 116)
				continue; //this is a point

			if (allent.IsSupportedCurveType(pcrv))
			{
				loop->loopcurves.push_back(pcrv);
				allent.MakePartOfTrimmedSurface(pcrv);
			}
			else
			{
				VOStrStream mystream;
				mystream << (IDS_ProTriSurExcUns1) << pcrv->entitytype;
				throw (VException(mystream.str(), 0));
			}
		}
	}
	else if (allent.IsSupportedCurveType(ptmp))
	{
		// this is a single bspline curve
		loop->loopcurves.push_back(ptmp);
		allent.MakePartOfTrimmedSurface(ptmp);
	}
	else
	{
		VOStrStream mystream;
		mystream << (IDS_ProTriSurExcUns1) << ptmp->entitytype;
		throw (VException(mystream.str(), 0));
	}

	if (triminside == true)
		m_innerloop.push_back(loop);

}

//#############################################################################

const IgesEntity * ProcessedFace::GetSurfaceIgesEntity() const
{
	return surf;
}

/*
bool ProcessedTrimmedSurface::IsCurveSupported ( IgesEntity * pigs ) const
{
// BSplineCurve
if ( pigs->entitytype == 126 )
return true;

// Line
if ( pigs->entitytype == 110 )
return true;

// Not supported
return false;
}
*/

bool ProcessedFace::Is2dLoopsGiven() const
{
	return m_is2dLoopsGiven;
}

const Face & ProcessedFace::GetFace()const
{
	return tsurf;
}
Face & ProcessedFace::GetFace()
{
	return tsurf;
}

IgesEntity * ProcessedFace::GetSurfaceIgesEntity()
{
	return surf;
}

