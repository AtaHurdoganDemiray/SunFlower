// OglWindowData.cpp

#include "stdafx.h"
#include "OglWindowData.h"
#include "gl/gl.h"

OglWindowData::OglWindowData ( const vreal &cornerx , const vreal &cornery 
							  , const vreal & newnear , const vreal &newfar )
{
	mcornerx = cornerx;
	mcornery = cornery;
	mnear = newnear;
	mfar = newfar;
}

//#############################################################################

OglWindowData::OglWindowData ( const OglWindowData &Foo )
{
	mcornerx = Foo.mcornerx;
	mcornery = Foo.mcornery;
	mnear = Foo.mnear;
	mfar = Foo.mfar;
}

//#############################################################################

OglWindowData & OglWindowData::operator = ( const OglWindowData & Foo )
{
	mcornerx = Foo.mcornerx;
	mcornery = Foo.mcornery;
	mnear = Foo.mnear;
	mfar = Foo.mfar;
	return *this;
}
				
bool OglWindowData::GetScreenCoordOfPoint ( GLfloat mypoint[4] , GLfloat inform [4] )
{
	//Getting the screen coord of point
	glRasterPos3f( mypoint[0] , mypoint[1] , mypoint[2] );

	GLboolean chk ;
	glGetBooleanv( GL_CURRENT_RASTER_POSITION_VALID , &chk );

	if (!chk )
		return false;

	glGetFloatv( GL_CURRENT_RASTER_POSITION , inform );
	
	return true;
}

//#############################################################################
