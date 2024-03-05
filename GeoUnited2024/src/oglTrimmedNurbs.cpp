/******************************************************************************
*               File: oglTrimmedNurbs.cpp											  *
*******************************************************************************
*               Description:                                                  *
*                                                                             *
*******************************************************************************
*               History:                                                      *
*  20.12.2003 17:28:18 Created by:    Ata Hurdogan Demiray                    *
*******************************************************************************
*               (C) 2003 by Pozitif GmbH	                                 *
******************************************************************************/

#include "stdafx.h"
#include "oglTrimmedNurbs.h"
#include "BSplineSurface.h"

oglTrimmedNurbs::oglTrimmedNurbs()
{
	alltrimloops = NULL;
	currtrimloop = NULL;
	thebody = NULL;
}

//#############################################################################

oglTrimmedNurbs::~oglTrimmedNurbs()
{
	
	if ( thebody != NULL )
		delete thebody;

	// deleting all trimming loops and the curves in these loops.
	if ( alltrimloops != NULL )
	{
		for ( int i = 0; i < alltrimloops->size(); i++ )
		{
			TrimLoop *temp = (*alltrimloops)[i];

			if ( temp != NULL )
			{
				for ( int j = 0; j < temp->size(); i++ )
				{
					oglNurbsCurve * tempcurve = (*temp)[j];
					delete tempcurve;
				}// for all curves in a trim loop
				delete temp;
			}// if a trim loop != NULL
		}// for all trim loops
		delete alltrimloops;
	}// if alltrimloops!-NULL


}
				
//#############################################################################

void oglTrimmedNurbs::FormTheBody ( const BSplineSurface &newnurbsbody )
{
	thebody = new oglNurbsSurface ( newnurbsbody );
}

//#############################################################################

void oglTrimmedNurbs::AddATrimLoop ()
{
	// news a TrimLoop as currtrimloop and adds that to all trim loops.
	currtrimloop = new TrimLoop;
	alltrimloops->push_back ( currtrimloop );

}

//#############################################################################

void oglTrimmedNurbs::AddoglNurbsCurveToCurrTrimLoop ( const BSplineCurve &newtrimcurve )
{
	// news an oglNurbsCurve by using BSplineCurve newtrimcurve and adds it to currtrimloop

	if ( currtrimloop == NULL )
		return; // exception.

	oglNurbsCurve * atrimcurve = new oglNurbsCurve ( newtrimcurve );
	currtrimloop->push_back ( atrimcurve );

}

//#############################################################################
