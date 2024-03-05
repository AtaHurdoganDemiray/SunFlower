// ProcessedTrimmedSurface.cpp

#include "stdafx.h"
#include "ProcessedTrimmedSurface.h"

#include "IgesEntitySet.h"
#include "IgesEntity.h"
#include "CurveOnParametricSurface.h"
#include "TrimmedSurface.h"
#include "CompositeCurve.h"
#include "IgesTrimLoop2d.h"

ProcessedTrimmedSurface::ProcessedTrimmedSurface 
( TrimmedSurface & _tsurf , IgesEntitySet & _allent )
:tsurf ( _tsurf ) , allent ( _allent ) , m_is2dLoopsGiven (true)
{
	surf = allent.GetEntityPointer ( tsurf.PTS );
	if (surf == 0 || allent.IsSupportedSurfaceType (surf) == false)
		throw (VException ( ( IDS_SurNotSup) , 0 ) );

	allent.MakePartOfTrimmedSurface ( surf );
	FormAllTrimLoops();
}

//#############################################################################

ProcessedTrimmedSurface::~ProcessedTrimmedSurface ()
{
	unsigned int size1 = (unsigned int)(innerloop.size());
	for (size_t i=0;i<size1;i++)
	{
		IgesTrimLoop2d *tl = innerloop[i];
		delete tl;
	}

	unsigned int size2 = (unsigned int)(m_innerloop.size());
	for (size_t i=0;i<size2;i++)
	{
		IgesTrimLoop *tl = m_innerloop[i];
		delete tl;
	}
}

//#############################################################################

void ProcessedTrimmedSurface::FormAllTrimLoops()
{
	// form the outer trimloop	if exists
	FormOuterLoop();
	
	// form the inner loops
	FormInnerLoops();
	
}

//#############################################################################

void ProcessedTrimmedSurface::FormOuterLoop()
{
	// form the outer trimloop	if exists
	// is there an outer boundary?
	if ( tsurf.N1 == 1 )
	{
		// there is outer boundary
		long outerloopref = tsurf.PTO;
		bool triminside = false;
		
		IgesEntity * p_igesent = 
		allent.GetEntityPointer ( outerloopref );
		
		if ( p_igesent == 0 )
			throw ( VException ( ( IDS_UnsIgeEnt) , 0 ) );

		if ( p_igesent->entitytype == 142 )
		{
			// this is curve on a parametric surface
			CurveOnParametricSurface * pcops = 
				(CurveOnParametricSurface*) p_igesent;
			allent.MakePartOfTrimmedSurface ( p_igesent );

			long modelSpace = pcops->CPTR;
			IgesEntity * pmodelCurve = allent.GetEntityPointer ( modelSpace );
			//allent.MakePartOfTrimmedSurface ( pmodelCurve );
			FormLoop3d (pmodelCurve , triminside);

			long pccref = pcops->BPTR;
			IgesEntity * ptmp = 0;
			if (pccref)
				ptmp = allent.GetEntityPointer ( pccref );

			if ( ptmp == 0 )
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
				FormLoop ( ptmp , triminside );
		}
		else
			throw ( VException ( ( IDS_ProTriSurExc) , 0 ) );

	}
	else // N1==0
		return;
	
}

//#############################################################################

void ProcessedTrimmedSurface::FormInnerLoops()
{
	// fill the inner trimloops	if exists
	int loopcount = tsurf.N2;

	if ( loopcount > 0 )
	{
		bool triminside = true;
		
		// there is inner boundaries
		for ( int i=0; i < loopcount; i++ )
		{
			long loopref = tsurf.PTI[i];
			
			IgesEntity * p_igesent = 
				allent.GetEntityPointer ( loopref );
			
			if ( p_igesent == 0 )
				throw ( VException ( ( IDS_ProTriSurExcUns) , 0 ));

			if ( p_igesent->entitytype == 142 )
			{
				// this is curve on a parametric surface
				CurveOnParametricSurface * pcops = 
					(CurveOnParametricSurface*) p_igesent;
				allent.MakePartOfTrimmedSurface ( p_igesent );

				long modelSpace = pcops->CPTR;
				IgesEntity * pmodelCurve = allent.GetEntityPointer ( modelSpace );
				//allent.MakePartOfTrimmedSurface ( pmodelCurve );
				FormLoop3d (pmodelCurve ,triminside);

				long pccref = pcops->BPTR;
				IgesEntity * ptmp = 0;
				
				if (pccref)
					ptmp = allent.GetEntityPointer ( pccref );
				
				if ( ptmp == 0 )
				{
					m_is2dLoopsGiven =false;
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
					FormLoop ( ptmp , triminside );
			}
			else
				throw ( VException ( ( IDS_ProTriSurExc1) , 0 ) );
		}
	}
	else // N2==0
		return;
	
}

//#############################################################################

void ProcessedTrimmedSurface::FormLoop( IgesEntity * ptmp , bool triminside )
{
	IgesTrimLoop2d * loop;
	if ( triminside == false )	
		loop = &outerloop;
	else
		loop = new IgesTrimLoop2d;

	if ( ptmp->entitytype == 102 )
	{
		// this is composite curve
		CompositeCurve * pcc = (CompositeCurve*)ptmp;
		allent.MakePartOfTrimmedSurface ( ptmp );
		for ( int i = 0; i < pcc->N; i++ )
		{
			long currDE = pcc->mDE[i];
			IgesEntity *pcrv = allent.GetEntityPointer(currDE);	
			if ( pcrv == 0 )
			{
				VOStrStream mystream;
				mystream<<( IDS_ProTriSurExcUns1)<<currDE;
				throw ( VException (  mystream.str(), 0 ));
			}
			if (pcrv->entitytype == 116)
				continue; //this is a point		

			if ( allent.IsSupportedCurveType( pcrv ) )
			{
				loop->loopcurves.push_back ( pcrv );
				allent.MakePartOfTrimmedSurface ( pcrv );
			}
			else
			{
				VOStrStream mystream;
				mystream<<( IDS_ProTriSurExcUns1)<<pcrv->entitytype;
				throw ( VException (  mystream.str(), 0 ));
			}
		}
	}
	else if ( allent.IsSupportedCurveType( ptmp ) )
	{
		// this is a single bspline curve
		loop->loopcurves.push_back ( ptmp );
		allent.MakePartOfTrimmedSurface ( ptmp );
	}
	else
	{
		VOStrStream mystream;
		mystream<<( IDS_ProTriSurExcUns1)<<ptmp->entitytype;
		throw ( VException (  mystream.str(), 0 ));
	}

	loop->Order(&allent);
	if ( triminside == true )
	{
		innerloop.push_back ( loop );
	}

}

//#############################################################################

void ProcessedTrimmedSurface::FormLoop3d( IgesEntity * ptmp , bool triminside )
{
	IgesTrimLoop * loop;
	if ( triminside == false )	
		loop = &m_outerloop;
	else
		loop = new IgesTrimLoop;

	if ( ptmp->entitytype == 102 )
	{
		// this is composite curve
		CompositeCurve * pcc = (CompositeCurve*)ptmp;
		allent.MakePartOfTrimmedSurface ( ptmp );
		for ( int i = 0; i < pcc->N; i++ )
		{
			long currDE = pcc->mDE[i];
			IgesEntity *pcrv = allent.GetEntityPointer(currDE);
			if ( pcrv == 0 )
			{
				VOStrStream mystream;
				mystream<<( IDS_ProTriSurExcUns1)<<currDE;
				throw ( VException (  mystream.str(), 0 ));
			}
			if (pcrv->entitytype == 116)
				continue; //this is a point

			if ( allent.IsSupportedCurveType( pcrv ) )
			{
				loop->loopcurves.push_back ( pcrv );
				allent.MakePartOfTrimmedSurface ( pcrv );
			}
			else
			{
				VOStrStream mystream;
				mystream<<( IDS_ProTriSurExcUns1)<<pcrv->entitytype;
				throw ( VException (  mystream.str(), 0 ));
			}
		}
	}
	else if ( allent.IsSupportedCurveType( ptmp ) )
	{
		// this is a single bspline curve
		loop->loopcurves.push_back ( ptmp );
		allent.MakePartOfTrimmedSurface ( ptmp );
	}
	else
	{
		VOStrStream mystream;
		mystream<<( IDS_ProTriSurExcUns1)<<ptmp->entitytype;
		throw ( VException (  mystream.str(), 0 ));
	}
	
	if ( triminside == true )
		m_innerloop.push_back ( loop );

}

//#############################################################################

const IgesEntity * ProcessedTrimmedSurface::GetSurfaceIgesEntity () const
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

bool ProcessedTrimmedSurface::Is2dLoopsGiven() const
{
	return m_is2dLoopsGiven;
}

const TrimmedSurface & ProcessedTrimmedSurface::GetTrimmedSurface()const
{
	return tsurf;
}
TrimmedSurface & ProcessedTrimmedSurface::GetTrimmedSurface()
{
	return tsurf;
}

IgesEntity * ProcessedTrimmedSurface::GetSurfaceIgesEntity ()
{
	return surf;
}
