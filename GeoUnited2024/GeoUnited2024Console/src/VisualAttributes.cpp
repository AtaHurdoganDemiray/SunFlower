/******************************************************************************
*               File: VisualAttributes.h											  *
*******************************************************************************
*               Description:                                                  *
*                                                                             *
*******************************************************************************
*               History:                                                      *
*  02.07.2003 18:23:08 Created by:    Ata Hurdogan Demiray                    *
*******************************************************************************
*               (C) 2003 by Pozitif GmbH                                     *
******************************************************************************/

#include "stdafx.h"
#include "VisualAttributes.h"
#include "Stipple.h"
#include "FacetVisualAtt.h"
#include "LineVisualAtt.h"
#include "PointVisualAtt.h"

VisualAttributes::VisualAttributes()
{
	shadecolor[0] = 0.0/255.0;
	shadecolor[1] = 128.0/255.0;
	shadecolor[2] = 0.0/255.0;

	wirecolor[0] = 0.0;
	wirecolor[1] = 0.0;
	wirecolor[2] = 0.0;

	pointcolor[0] = 1.0;
	pointcolor[1] = 0.0;
	pointcolor[2] = 0.0;
	
	selcolor[0] = 0.5;
	selcolor[1] = 0.5;
	selcolor[2] = 0.5;
	
	linewidth = 2;
	pointsize = 7.0;
	translucent = false;

	linefont = LINEFONT_NORMAL;
	draw = DRAW_BOTH;

	for ( int i=0; i < 16; i++ )
		for ( int j = 0; j < 8; j++ )
		{
			if ( j<4 )
				halftone[i*8 + j] = 170;
			else
				halftone[i*8 + j] = 85;
		}
	
	tempcolor[0] = -1.0;
	tempcolor[1] = -1.0;
	tempcolor[2] = -1.0;

}

//#############################################################################

VisualAttributes::~VisualAttributes()
{

}

//#############################################################################

void VisualAttributes::SetShadeColor( bool modelselected )
{//return;
	//glEnable ( GL_LIGHTING );
	if ( translucent )
	{
		//glDisable ( GL_LIGHTING );
		glEnable ( GL_POLYGON_STIPPLE );
		glPolygonStipple (halftone);	 
	
	}
	else
		glDisable ( GL_POLYGON_STIPPLE );

//	if ( modelselected == false 
//		&& tempcolor[0]==shadecolor[0] 
//		&& tempcolor[1]==shadecolor[1] 
//		&& tempcolor[2]==shadecolor[2] ) 
//		return;

	if ( modelselected )
	{
		tempcolor[0] = selcolor[0];
		tempcolor[1] = selcolor[1];
		tempcolor[2] = selcolor[2];
	}
	else
	{
		tempcolor[0] = shadecolor[0];
		tempcolor[1] = shadecolor[1];
		tempcolor[2] = shadecolor[2];
	}

	// opengl commands for setting shade color
	//...
	GLfloat mat_red = static_cast<GLfloat>(tempcolor[0]);
	GLfloat mat_green = static_cast<GLfloat>(tempcolor[1]);
	GLfloat mat_blue = static_cast<GLfloat>(tempcolor[2]);
	 

	GLfloat no_mat [] = { 0.0f , 0.0f , 0.0f , 1.0f };
	GLfloat mat_ambient [] = { 0.5f , 0.5f , 0.5f , 1.0f };
	GLfloat mat_ambient_color [] = { 0.8f , 0.8f , 0.2f , 1.0f };
    GLfloat mat_diffuse[] = { mat_red , mat_green , mat_blue , 1.0f };
    GLfloat mat_specular[] = { 1.0f , 1.0f , 1.0f , 1.0f };
	GLfloat mat_shininess[] = { 80.0f };
	GLfloat mat_emission [] = { 0.1f , 0.1f , 0.1f , 1.0f };

	glMaterialfv(GL_FRONT_AND_BACK , GL_AMBIENT , mat_ambient );
	glMaterialfv(GL_FRONT_AND_BACK , GL_DIFFUSE , mat_diffuse );
	glMaterialfv(GL_FRONT_AND_BACK , GL_SPECULAR , mat_specular );
	glMaterialfv(GL_FRONT_AND_BACK , GL_SHININESS , mat_shininess );
	glMaterialfv(GL_FRONT_AND_BACK , GL_EMISSION , mat_emission );

	GLfloat mat_amb_diff[] = { mat_red , mat_green , mat_blue , 1.0 };
	glMaterialfv ( GL_FRONT_AND_BACK , GL_AMBIENT_AND_DIFFUSE , mat_amb_diff );
	
}

//#############################################################################

vreal * VisualAttributes::GetShadeColor ()
{
	return shadecolor;
}

//#############################################################################

void VisualAttributes::ChangeShadeColor ( vreal newcolor[])
{
	shadecolor[0] = newcolor[0];
	shadecolor[1] = newcolor[1];
	shadecolor[2] = newcolor[2];
}

//#############################################################################

void VisualAttributes::SetWireAttributes( bool modelselected )
{
	// opengl commands for setting wire color
	//...
	
	if ( modelselected )
		glColor3d ( selcolor[0] , selcolor[1] , selcolor[2] );
	else
		glColor3d ( wirecolor[0] , wirecolor[1] , wirecolor[2] );
	
	//glEnable ( GL_LINE_SMOOTH  );
	glLineWidth ( static_cast<GLfloat>(linewidth) );

	// line font 
	if ( linefont == LINEFONT_NORMAL )
		glDisable ( GL_LINE_STIPPLE );
	else if ( linefont == LINEFONT_DASHDOT )
	{
		glEnable ( GL_LINE_STIPPLE );
		glLineStipple ( 5 , 0x1c47 );
	}
	else if ( linefont == LINEFONT_DOTTED )
	{
		glEnable ( GL_LINE_STIPPLE );
		glLineStipple ( 5 , 0xAAAA );
	}

}

//#############################################################################

vreal * VisualAttributes::GetWireColor ()
{
	return wirecolor;
}

//#############################################################################

void VisualAttributes::ChangeWireColor ( vreal newcolor[] )
{
	wirecolor[0] = newcolor[0];
	wirecolor[1] = newcolor[1];
	wirecolor[2] = newcolor[2];
}

//#############################################################################

void VisualAttributes::SetPointAttributes( bool modelselected )
{
	// opengl commands for setting point attributes
	glPointSize ( static_cast<GLfloat>(pointsize) );
	
	//glDisable (GL_LIGHTING);
	
	//Antialising, so that the point will look circular.
	//glEnable ( GL_POINT_SMOOTH );
	//glEnable ( GL_BLEND );
	//glBlendFunc ( GL_SRC_ALPHA , GL_ONE_MINUS_SRC_ALPHA );

	if ( modelselected )
		glColor3d ( selcolor[0] , selcolor[1] , selcolor[2] );
	else
		glColor3d ( shadecolor[0] , shadecolor[1] , shadecolor[2] );
	
}

//#############################################################################

vreal * VisualAttributes::GetPointColor ()
{
	return pointcolor;
}

//#############################################################################

void VisualAttributes::ChangePointColor ( vreal newcolor [])
{
	pointcolor[0] = newcolor[0];
	pointcolor[1] = newcolor[1];
	pointcolor[2] = newcolor[2];
}

//#############################################################################

void VisualAttributes::SetPointSize ( vreal newsize )
{
	pointsize = newsize;
}

//#############################################################################

void VisualAttributes::SetLineWidth ( vreal newwidth )
{
	linewidth = newwidth;
}

//#############################################################################

void VisualAttributes::SetTranslucent( bool newval )
{
	translucent = newval;
}

//#############################################################################

bool VisualAttributes::GetTranslucent()
{
	return translucent;
}

//#############################################################################

int VisualAttributes::GetLineFont()
{
	return linefont;
}

//#############################################################################

void VisualAttributes::SetLineFont ( int newfont )
{
	linefont = newfont;
}

//#############################################################################

int VisualAttributes::GetDraw()
{
	return draw;
}

//#############################################################################

void VisualAttributes::SetDraw ( int newdraw )
{
	draw = newdraw;
}

//#############################################################################

vreal VisualAttributes::GetLineWidth()
{
	return linewidth;
}

//#############################################################################

vreal VisualAttributes::GetPointSize()
{
	return pointsize;
}

//#############################################################################

void VisualAttributes::FillDefaults(PointVisualAtt &pvis, LineVisualAtt &lvis, FacetVisualAtt &fvis)
{
	fvis.ChangeShadeColor(VGlobalValues::viewpointdefaults.m_defaultSurfaceColor);
	fvis.SetTranslucent(VGlobalValues::viewpointdefaults.m_defaultIsTranslucent == TRUE);
	lvis.ChangeWireColor(VGlobalValues::viewpointdefaults.m_defaultSurfaceWireColor);
	lvis.SetLineWidth(VGlobalValues::viewpointdefaults.m_defaultLineWidth);
	lvis.SetLineFont((LineVisualAtt::LineFont)VGlobalValues::viewpointdefaults.m_defaultLineFont);
	pvis.ChangePointColor(VGlobalValues::viewpointdefaults.m_defaultPointColor);
	pvis.SetPointSize(VGlobalValues::viewpointdefaults.m_defaultPointSize);
}
void VisualAttributes::FillDefaults2(PointVisualAtt &pvis, LineVisualAtt &lvis, FacetVisualAtt &fvis)
{
	fvis.ChangeShadeColor(VGlobalValues::viewpointdefaults.m_surfaceColor2);
	fvis.SetTranslucent(VGlobalValues::viewpointdefaults.m_defaultIsTranslucent == TRUE);
	lvis.ChangeWireColor(VGlobalValues::viewpointdefaults.m_surfaceWireColor2);
	lvis.SetLineWidth(VGlobalValues::viewpointdefaults.m_defaultLineWidth);
	lvis.SetLineFont((LineVisualAtt::LineFont)VGlobalValues::viewpointdefaults.m_defaultLineFont);
	pvis.ChangePointColor(VGlobalValues::viewpointdefaults.m_defaultPointColor);
	pvis.SetPointSize(VGlobalValues::viewpointdefaults.m_defaultPointSize);
}
void VisualAttributes::FillDefaults3(PointVisualAtt &pvis, LineVisualAtt &lvis, FacetVisualAtt &fvis)
{
	fvis.ChangeShadeColor(VGlobalValues::viewpointdefaults.m_surfaceColor3);
	fvis.SetTranslucent(VGlobalValues::viewpointdefaults.m_defaultIsTranslucent == TRUE);
	lvis.ChangeWireColor(VGlobalValues::viewpointdefaults.m_surfaceWireColor3);
	lvis.SetLineWidth(VGlobalValues::viewpointdefaults.m_defaultLineWidth);
	lvis.SetLineFont((LineVisualAtt::LineFont)VGlobalValues::viewpointdefaults.m_defaultLineFont);
	pvis.ChangePointColor(VGlobalValues::viewpointdefaults.m_defaultPointColor);
	pvis.SetPointSize(VGlobalValues::viewpointdefaults.m_defaultPointSize);
}




