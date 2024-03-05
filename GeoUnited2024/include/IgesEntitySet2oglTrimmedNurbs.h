/******************************************************************************
*               File: IgesEntitySet2oglTrimmedNurbsVec.h											  *
*******************************************************************************
*               Description:                                                  *
*                                                                             *
*******************************************************************************
*               History:                                                      *
*  20.12.2003 17:16:53 Created by:    Ata Hurdogan Demiray                    *
*******************************************************************************
*               (C) 2003 by Pozitif GmbH	                                 *
******************************************************************************/

#ifndef __IgesEntitySet2oglTrimmedNurbsVec_h__
#define __IgesEntitySet2oglTrimmedNurbsVec_h__

#include "oglTrimmedNurbs.h"

class IgesEntitySet2oglTrimmedNurbsVec
{
public:
	IgesEntitySet2oglTrimmedNurbsVec( const IgesEntitySet &allentities );
	~IgesEntitySet2oglTrimmedNurbsVec();
	oglTrimmedNurbsVec mysurfvec;	
};


#endif //__IgesEntitySet2oglTrimmedNurbsVec_h__
