/******************************************************************************
*               File: SelectionInfo.cpp										  *
*******************************************************************************
*               Description:                                                  *
*                                                                             *
*******************************************************************************
*               History:                                                      *
*  05.07.2003 18:28:54 Created by:    Ata Hurdogan Demiray                    *
*******************************************************************************
*               (C) 2003 by ModuleWorks GmbH                                  *
******************************************************************************/

#include "stdafx.h"
#include "SelectionInfo.h"
#include "VolumeBox.h"
#include "VolBoxModel.h"

SelectionInfo::SelectionInfo()
{
}

//#############################################################################

SelectionInfo::~SelectionInfo()
{

}

//#############################################################################

void SelectionInfo::SetSelectedPoint ( const vreal apoint[3] )
{
	selpoint.x ( apoint[0] );
	selpoint.y ( apoint[1] );	
	selpoint.z ( apoint[2] );
}

//#############################################################################

void SelectionInfo::SetSelectedVector ( const vreal avect1[3] , const vreal avect2[3] )
{
	VLine tmp ( avect1 , avect2 );
	selvector = tmp;
}

//#############################################################################

void SelectionInfo::SetSelectedLine ( const vreal point1[3] , const vreal point2[3] )
{
	VPoint p1 ( point1 );
	VPoint p2 ( point2 );

	selline.SetP1 ( p1 );
	selline.SetP2 ( p2 );

}

//#############################################################################

VPoint SelectionInfo::GetSelectedPoint () const
{
	return selpoint;
}

//#############################################################################

VLine SelectionInfo::GetSelectedVector () const
{
	return selvector;
}

//#############################################################################

VLine SelectionInfo::GetSelectedLine ()
{
	return selline;
}

//#############################################################################

HandleSet & SelectionInfo::GetHandleSet ()
{
	return selmodels;
}

//#############################################################################

//VolumeBox SelectionInfo::GetSelectedVolumeBox()const
//{
//	return volbox;
//}

//#############################################################################

//void SelectionInfo::SetVolumeBox(const VolumeBox &newbox)
//{
//	volbox=newbox;
//}

//#############################################################################

const vreal & SelectionInfo::GetSelectedScaler()const
{
	return selscaler;
}

//#############################################################################

void SelectionInfo::SetSelectedScaler (const vreal &myscaler)
{
	selscaler=myscaler;
}

//#############################################################################

//void SelectionInfo::SetSelectedCoordinateSystem (const CoordinateSystem &newcoord)
//{
//	coord = newcoord;
//}

//#############################################################################

//CoordinateSystem SelectionInfo::GetSelectedCoordinateSystem ()const
//{
//	return coord;
//}


void SelectionInfo::Clear()
{
	selmodels.Clear();
}

