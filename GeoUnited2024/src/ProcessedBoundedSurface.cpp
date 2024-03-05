// ProcessedBoundedSurface.cpp

#include "stdafx.h"
#include "ProcessedBoundedSurface.h"
#include "BoundedSurface.h"
#include "IgesEntitySet.h"
#include "IgesEntity.h"
#include "Boundary.h"

#include "CompositeCurve.h"
#include "IgesTrimLoop2d.h"

ProcessedBoundedSurface::ProcessedBoundedSurface 
( BoundedSurface & _bsurf , IgesEntitySet & _allent )
: bsurf ( _bsurf ) , allent ( _allent )
{
	surf = allent.GetEntityPointer ( bsurf.m_SPTR );

	if (surf == 0 || allent.IsSupportedSurfaceType (surf) ==false)
		throw (VException ( ( IDS_SurNotSup) , 0 ) );

	allent.MakePartOfTrimmedSurface ( surf );
	FormAllTrimLoops ();
}

ProcessedBoundedSurface::~ProcessedBoundedSurface ()
{
	int size1 = static_cast<int>(m_loop2d.size());
	for (int i=0;i<size1;i++)
	{
		IgesTrimLoop2d *tl = m_loop2d[i];
		delete tl;
	}

	int size2 = static_cast<int>(m_loop3d.size());
	for (int i=0;i<size2;i++)
	{
		IgesTrimLoop *tl = m_loop3d[i];
		delete tl;
	}
}

void ProcessedBoundedSurface::FormAllTrimLoops ()
{
	if ( bsurf.m_TYPE != 1 )
		throw ( VException ( ("processed bounded surface exception 1") , 0 )); // parameter curves are not given

	for ( int i=0; i< bsurf.m_N; i++ )
	{
		IgesEntity *piges = allent.GetEntityPointer (bsurf.m_BDPT[i]);	
		if ( piges == 0 )
			throw ( VException ( ( IDS_ProBouSurExc) , 0 ));
			
		Boundary * pB;
	
		if (piges->entitytype == 141 )
		{
			pB = static_cast<Boundary*> (piges);
			allent.MakePartOfTrimmedSurface ( piges );
		}
		else
			throw ( VException ( ( IDS_ProBouSurExc1) , 0 ));
		
		FormLoop ( pB );	
	}

}

//#############################################################################

void ProcessedBoundedSurface::FormLoop( const Boundary *pB )
{
	//gmTrimLoopUtility * newloopu = mdata.alltrimlooputilities.AddItem ( mdata.GetParentSurf() , triminside );

	IgesTrimLoop * loop = new IgesTrimLoop;
	IgesTrimLoop2d *loop2d = new IgesTrimLoop2d;

	for ( int j=0; j<pB->m_N; j++ )
	{
		IgesEntity * ptmp = allent.GetEntityPointer ( pB->m_CRVPT[j] );
		// this igesEntity can be a composite curve 
		//, its components should be recorded too 
		//allent.MakePartOfTrimmedSurface ( ptmp );
		FillLoopByCurve3d (ptmp ,loop);
		if (pB->m_SENSE[j] == 2)
		{
			loop->m_isReversed.push_back(true);
			loop2d->m_isReversed.push_back(true);
		}
		else
		{
			loop->m_isReversed.push_back(false);
			loop2d->m_isReversed.push_back(false);
		}
		for ( int i=0; i < pB->m_K[j]; i++ )
		{
			IgesEntity *piges = allent.GetEntityPointer ( pB->m_PSCPT[j][i] );	
			
			if ( piges == 0 )
				throw ( VException ( ( IDS_UnsIgeEnt) , 0 ) );

			FillLoopByCurve (piges ,loop2d);
		}
	}
	m_loop3d.push_back (loop);
	loop2d->Order(&allent);
	m_loop2d.push_back (loop2d);
}

//#############################################################################

void ProcessedBoundedSurface::FillLoopByCurve (IgesEntity * ptmp , IgesTrimLoop2d *loop)
{
	if ( ptmp->entitytype == 102 )
	{
		// this is composite curve
		// get the curves in composite curve entity and record to loop
		CompositeCurve * pcc = (CompositeCurve*)(ptmp);
		allent.MakePartOfTrimmedSurface ( ptmp );

		for ( int i = 0; i < pcc->N; i++ )
		{
			long currDE = pcc->mDE[i];
			IgesEntity *pcrv = allent.GetEntityPointer(currDE);
			if ( pcrv == 0 )
				throw ( VException ( ( IDS_BouTriConUnsCur) , 0 ));
			if (pcrv->entitytype == 116)
				continue; //this is a point

			allent.MakePartOfTrimmedSurface ( pcrv );
			if ( allent.IsSupportedCurveType ( pcrv ) )
				loop->loopcurves.push_back ( pcrv );
			else
			{
				// what else than bsplinecurve can be in composite curve?
				throw ( VException ( ( IDS_ProBouSur) , 0 ) );
			}
		}

	}
	else if ( allent.IsSupportedCurveType (ptmp) )
	{
		// this is a single bspline curve
		loop->loopcurves.push_back ( ptmp );
		allent.MakePartOfTrimmedSurface ( ptmp );
	}
	else
		throw ( VException ( ("processed bounded surface") , 0 ));// exception

}

//#############################################################################

void ProcessedBoundedSurface::FillLoopByCurve3d (IgesEntity * ptmp , IgesTrimLoop * loop)
{
	if ( ptmp->entitytype == 102 )
	{
		// this is composite curve
		// get the curves in composite curve entity and record to loop
		CompositeCurve * pcc = (CompositeCurve*)(ptmp);
		allent.MakePartOfTrimmedSurface ( ptmp );

		for ( int i = 0; i < pcc->N; i++ )
		{
			long currDE = pcc->mDE[i];
			IgesEntity *pcrv = allent.GetEntityPointer(currDE);
			if ( pcrv == 0 )
				throw ( VException ( ( IDS_BouTriConUnsCur1) , 0 ));
			if (pcrv->entitytype == 116)
				continue; //this is a point

			allent.MakePartOfTrimmedSurface ( pcrv );
			if ( allent.IsSupportedCurveType ( pcrv ) )
				loop->loopcurves.push_back ( pcrv );
			else
			{
				// what else than bsplinecurve can be in composite curve?
				throw ( VException ( ( IDS_ProBouSur) , 0 ) );
			}
		}

	}
	else if ( allent.IsSupportedCurveType (ptmp) )
	{
		// this is a single bspline curve
		loop->loopcurves.push_back ( ptmp );
		allent.MakePartOfTrimmedSurface ( ptmp );
	}
	else
		throw ( VException ( ("processed bounded surface") , 0 ));// exception
}

//#############################################################################

const IgesEntity * ProcessedBoundedSurface::GetSurfaceIgesEntity () const
{
	return surf;
}
/*
bool ProcessedBoundedSurface::IsCurveSupported ( IgesEntity * pigs ) const
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

const BoundedSurface & ProcessedBoundedSurface::GetBoundedSurface()const
{
	return bsurf;
}
BoundedSurface & ProcessedBoundedSurface::GetBoundedSurface()
{
	return bsurf;
}

IgesEntity * ProcessedBoundedSurface::GetSurfaceIgesEntity()
{
	return surf;
}
