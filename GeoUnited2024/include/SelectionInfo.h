/******************************************************************************
*               File: SelectionInfo.h											  *
*******************************************************************************
*               Description:                                                  *
*                                                                             *
*******************************************************************************
*               History:                                                      *
*  05.07.2003 18:27:19 Created by:    Ata Hurdogan Demiray                    *
*******************************************************************************
*               (C) 2003 by ModuleWorks GmbH                                  *
******************************************************************************/

#ifndef __SelectionInfo_h__
#define __SelectionInfo_h__

#include "VPoint.h"
#include "VLine.h"
#include "HandleSet.h"

//#include "VolumeBox.h"
//#include "CoordinateSystem.h"
class SelectionInfo
{
public:
	SelectionInfo();
	virtual ~SelectionInfo();
	void SetSelectedScaler ( const vreal &myscaler );
	void SetSelectedPoint ( const vreal apoint[3] );
	void SetSelectedVector ( const vreal avect1[3] , const vreal avect2[3] );	
	void SetSelectedLine ( const vreal point1[3] , const vreal point2[3] );
	const vreal & GetSelectedScaler()const;
	VPoint GetSelectedPoint()const;
	VLine GetSelectedVector()const;
	VLine GetSelectedLine();
	HandleSet & GetHandleSet();
	//void SetVolumeBox(const VolumeBox &newbox);
	//void SetSelectedCoordinateSystem (const CoordinateSystem &newcoord);
	//VolumeBox GetSelectedVolumeBox()const;
	//CoordinateSystem GetSelectedCoordinateSystem()const;
	void Clear();
	vreal selscaler;
	VPoint selpoint;
	VLine selvector;
	VLine selline;
	//VolumeBox volbox;
	//CoordinateSystem coord;
	HandleSet selmodels;
};

#endif //__SelectionInfo_h__