/******************************************************************************
*               File: IgesEntitySet2oglTrimmedNurbsVec.cpp											  *
*******************************************************************************
*               Description:                                                  *
*                                                                             *
*******************************************************************************
*               History:                                                      *
*  20.12.2003 19:05:04 Created by:    Ata Hurdogan Demiray                    *
*******************************************************************************
*               (C) 2003 by Pozitif GmbH		                               *
******************************************************************************/

#include "stdafx.h"
#include "IgesEntitySet2oglTrimmedNurbsVec.h"
#include "IgesEntitySet.h"
#include "TrimmedSurface.h"

IgesEntitySet2oglTrimmedNurbsVec::IgesEntitySet2oglTrimmedNurbsVec( const IgesEntitySet &allentities )
{
	FilloglTrimmedNurbsVec ( allentities );
}

//#############################################################################

IgesEntitySet2oglTrimmedNurbsVec::~IgesEntitySet2oglTrimmedNurbsVec()
{

}

//#############################################################################

void IgesEntitySet2oglTrimmedNurbsVec
::FilloglTrimmedNurbsVec ( const IgesEntitySet &allentities )
{
	IgesEntity * temp;

	// search for TrimmedSurface entity in all entities
	for ( long i = 0; i < allentities.Size(); i++ )
	{
		temp = allentities[i];
		if ( temp->entitytype == 144 )
		{
			// yes this is a TrimmedSurface entity
			TrimmedSurface * igestrimmedsurf = ( TrimmedSurface *)temp;
			AddoglTrimmedNurbs ( *(igestrimmedsurf) , allentities );
		}
		// in the future add other entity types (eg bounded surface ) here...

	}
		
}

//#############################################################################

void IgesEntitySet2oglTrimmedNurbsVec::AddoglTrimmedNurbs 
( const TrimmedSurface &igestrimmedsurf , const IgesEntitySet &allentities )
{
	// first check the outer boundary 
	if ( igestrimmedsurf.N1 == 1 )
	{
		// there is an outer boundary
		// let s check in which format it is given.
	}
}

//#############################################################################
