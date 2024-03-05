/******************************************************************************
*               File: Stipple.h											  *
*******************************************************************************
*               Description:                                                  *
*                                                                             *
*******************************************************************************
*               History:                                                      *
*  08.10.2003 15:40:52 Created by:    Ata Hurdogan Demiray                    *
*******************************************************************************
*               (C) 2003 by ModuleWorks GmbH                                  *
******************************************************************************/

#ifndef __Stipple_h__
#define __Stipple_h__
#include "gl/gl.h"

class Stipple
{
public:
	Stipple();
	~Stipple();
	double GetByteValue ( int tdegree );
	GLubyte * CreatePattern( int tdegree );
	GLubyte * GetPattern();

private:
	GLubyte * pat[8];
	int patno;
};

#endif //__Stipple_h__
