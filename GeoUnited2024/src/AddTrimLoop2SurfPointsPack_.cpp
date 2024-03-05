/******************************************************************************
*               File: AddTrimLoop2SurfPointsPack.cpp											  *
*******************************************************************************
*               Description:                                                  *
*                                                                             *
*******************************************************************************
*               History:                                                      *
*  11.03.2004 14:47:13 Created by:    Ata Hurdogan Demiray                    *
*******************************************************************************
*               (C) 2003 by ModuleWorks GmbH                                  *
******************************************************************************/

#include "stdafx.h"
#include "AddTrimLoop2SurfPointsPack.h"
#include "SurfPointsPack.h"
#include "gmTrimLoop.h"
#include "SetMerger.h"

AddTrimLoop2SurfPointsPack::AddTrimLoop2SurfPointsPack ( SurfPointsPack &surfpointspack )
:surfpack ( surfpointspack )
{

}

//#############################################################################

void AddTrimLoop2SurfPointsPack::AddTrimLoop ( const gmTrimLoop &trimloop )
{
	SetMerger merger;

	merger.Merge ( trimloop.looppoints.parampoints , surfpack.parampoints );
	merger.Merge ( trimloop.looppoints.modelpoints , surfpack.modelpoints );
	merger.Merge ( trimloop.looppoints.normals , surfpack.normals );

}

//#############################################################################
