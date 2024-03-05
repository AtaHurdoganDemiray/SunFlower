// OglServ.cpp

#include "stdafx.h"
#include "OglServ.h"
#include "VPoint.h"

bool OglServ::GetScreenCoordForPoint ( const VPoint & mypoint , VPoint &screenpoint ) const
{
	glRasterPos3dv ( mypoint.GetArray() );
	GLboolean ispointinviewvolume[1];
	glGetBooleanv ( GL_CURRENT_RASTER_POSITION_VALID , ispointinviewvolume );

	if ( ispointinviewvolume[0] )
	{
		GLfloat info[5];
		glGetFloatv ( GL_CURRENT_RASTER_POSITION , info );
		screenpoint.x ( static_cast<vreal>(info[0]));
		screenpoint.y ( static_cast<vreal>(info[1]));
		screenpoint.z ( static_cast<vreal>(info[2]));
		return true;
	}
	else
		return false;
}
