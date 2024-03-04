/******************************************************************************
*               File: gmVectorUtility.h										  *
*******************************************************************************
*               Description:                                                  *
*                                                                             *
*******************************************************************************
*               History:                                                      *
*  02.07.2003 18:16:47 Created by:    Ata Hurdogan Demiray                    *
*******************************************************************************
*               (C) 2003 by Pozitif GmbH                                      *
******************************************************************************/

#ifndef __gmVectorUtility_h__
#define __gmVectorUtility_h__

#include "VPoint.h"
#include "VPoint2d.h"

struct VLine;
struct VLine2d;

void v_add_3d ( const vreal v1[3] , const vreal v2[3] , vreal result[3] );

class gmVectorUtility
{
public:


	gmVectorUtility();
	~gmVectorUtility();
	static vreal GetAngleBetweenVectors ( const VVector &v1 , const VVector &v2 );
	static void RotatePointAroundLine ( const vreal point[3] , const vreal line[3] 
								, const vreal origin[3] 
								, vreal pointrotated[3] , vreal angle );//radians 

private:

};


#endif //__gmVectorUtility_h__