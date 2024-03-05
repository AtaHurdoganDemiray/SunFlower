/******************************************************************************
*               File: AddTrimLoop2SurfPointsPack.h											  *
*******************************************************************************
*               Description:                                                  *
*                                                                             *
*******************************************************************************
*               History:                                                      *
*  11.03.2004 14:44:03 Created by:    Ata Hurdogan Demiray                    *
*******************************************************************************
*               (C) 2003 by POsitive GmbH                                  *
******************************************************************************/

#ifndef __AddTrimLoop2SurfPointsPack_h__
#define __AddTrimLoop2SurfPointsPack_h__

class SurfPointsPack;
class gmTrimLoop;

class AddTrimLoop2SurfPointsPack
{
public:
	AddTrimLoop2SurfPointsPack ( SurfPointsPack &surfpointspack );
	void AddTrimLoop ( const gmTrimLoop &trimloop );

private:
	SurfPointsPack &surfpack;
};

#endif //__AddTrimLoop2SurfPointsPack_h__
