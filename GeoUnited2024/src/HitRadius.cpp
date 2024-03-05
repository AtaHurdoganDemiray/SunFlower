/******************************************************************************
*               File: HitRadius.cpp											  *
*******************************************************************************
*               Description:                                                  *
*                                                                             *
*******************************************************************************
*               History:                                                      *
*  01.08.2003 15:40:32 Created by:    Ata Hurdogan Demiray                    *
*******************************************************************************
*               (C) 2003 by ModuleWorks GmbH                                  *
******************************************************************************/

#include "stdafx.h"
#include "HitRadius.h"

HitRadius::HitRadius ( int corner_x , int corner_y )
{
	HitRadius::CalcDiagonal ( corner_x , corner_y );

	HitRadius::SetDivisor( 80.0 );

	HitRadius::CalcR();

	hitr = 1.0;
}

//#############################################################################

HitRadius::~HitRadius ()
{

}

//#############################################################################

void HitRadius::CalcDiagonal ( int corner_x , int corner_y )
{
	vreal tmp = static_cast<vreal> ( (corner_x * corner_x + corner_y * corner_y) );
	diagonal = sqrt (tmp);
}

//#############################################################################

vreal HitRadius::GetR ()
{
	return hitr;
}

//#############################################################################

void HitRadius::CalcR ()
{
	hitr = diagonal / divisor;
}

//#############################################################################

void HitRadius::SetDivisor ( vreal newdiv )
{
	divisor = newdiv;
}

//#############################################################################

