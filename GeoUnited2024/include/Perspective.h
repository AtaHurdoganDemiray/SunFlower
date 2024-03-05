/******************************************************************************
*               File: Perspective.h											  *
*******************************************************************************
*               Description:                                                  *
*                                                                             *
*******************************************************************************
*               History:                                                      *
*  23.11.2003 05:35:50 Created by:    Ata Hurdogan Demiray                    *
*******************************************************************************
*               (C) 2003 by Pozitif GmbH		                               *
******************************************************************************/

#ifndef __Perspective_h__
#define __Perspective_h__

class Perspective
{
public:
	Perspective();
	~Perspective();
	vreal GetFovy();
	vreal GetAspect();
	vreal GetNear();
	vreal GetFar();
	void SetFovy ( vreal newval );
	void SetAspect ( vreal newval );
	void SetNear ( vreal newval );
	void SetFar ( vreal newval );
	void SetPerspective ();

private:
	vreal m_fovy;
	vreal m_aspect;
	vreal m_near;
	vreal m_far;

};

#endif //__Perspective_h__

