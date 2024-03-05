/******************************************************************************
*               File: HitRadius.h											  *
*******************************************************************************
*               Description:                                                  *
*                                                                             *
*******************************************************************************
*               History:                                                      *
*  01.08.2003 15:15:25 Created by:    Ata Hurdogan Demiray                    *
*******************************************************************************
*               (C) 2003 by Pozitif GmbH                                      *
******************************************************************************/

#ifndef __HitRadius_h__
#define __HitRadius_h__

class HitRadius
{
public:
	HitRadius ( int corner_x , int corner_y );
	virtual ~HitRadius ();
	void CalcR();
	vreal GetR();
	void CalcDiagonal ( int corner_x , int corner_y );
	void SetDivisor( vreal newdiv );
private:
	vreal hitr;
	vreal diagonal;
	vreal divisor;
};



#endif //__HitRadius_h__
