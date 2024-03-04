// VisualAttLoader.cpp

#include "stdafx.h"
#include "VisualAttLoader.h"
#include "PointVisualAtt.h"
#include "LineVisualAtt.h"
#include "FacetVisualAtt.h"

VisualAttLoader::VisualAttLoader ()
:m_prevLoadedFVA(0),m_prevLoadedLVA(0),m_prevLoadedPVA(0),m_prevSelected(false)
{
	no_mat [0] =  0.0f; no_mat[1] = 0.0f; no_mat[2] = 0.0f; no_mat[3] = 1.0f;
	mat_ambient [0] = 0.5f; mat_ambient[1] = 0.5f; mat_ambient[2] = 0.5f; mat_ambient[3] = 1.0f;
	mat_ambient_color [0] = 0.8f; mat_ambient_color[1] = 0.8f; mat_ambient_color[2] = 0.2f; mat_ambient_color[3] =  1.0f;
	mat_shininess[0] = 80.0f;
	mat_emission [0] = 0.1f; mat_emission[1] = 0.1f; mat_emission[2] = 0.1f; mat_emission[3] = 1.0f;
    mat_specular[0] = 1.0f; mat_specular[1] = 1.0f; mat_specular[2] = 1.0f; mat_specular[3] = 1.0f;

	//for translucency
	for ( int i=0; i < 16; i++ )
		for ( int j = 0; j < 8; j++ )
		{
			if ( j<4 )
				halftone[i*8 + j] = 170;
			else
				halftone[i*8 + j] = 85;
		}	
}

void VisualAttLoader::Load ( const PointVisualAtt & att , bool modelselected , const VColor & selcolor ) const
{
	//if (m_prevLoadedPVA != 0)
	//{
	//	if (*m_prevLoadedPVA == att && m_prevSelected == modelselected)
	//		return;
	//}
	//m_prevLoadedPVA = &att;
	//m_prevSelected = modelselected;
	// opengl commands for setting point attributes
	glPointSize (static_cast<GLfloat> (att.GetPointSize()) );
	
	//glDisable (GL_LIGHTING);
	
	//Antialising, so that the point will look circular.
	//glEnable ( GL_POINT_SMOOTH );
	//glEnable ( GL_BLEND );
	//glBlendFunc ( GL_SRC_ALPHA , GL_ONE_MINUS_SRC_ALPHA );

	if ( modelselected )
	{
		glColor3d ( selcolor.R() , selcolor.G() , selcolor.B() );
	}
	else
	{
		glColor3d ( att.GetPointColor().R() , att.GetPointColor().G() , att.GetPointColor().B() );
	}
	
}

void VisualAttLoader::Load ( const LineVisualAtt & att , bool modelselected , const VColor & selcolor ) const
{
	//if (m_prevLoadedLVA != 0)
	//{
	//	if (*m_prevLoadedLVA == att && m_prevSelected == modelselected)
	//		return;
	//}
	//m_prevLoadedLVA = &att;
	//m_prevSelected = modelselected;
	// opengl commands for setting wire color
	//...
		
	//glEnable ( GL_LINE_SMOOTH  );
	//if (modelselected)
	//	glLineWidth ( 6.f );
	//else
	if ( modelselected )
	{
		glColor3d ( selcolor.R() , selcolor.G() , selcolor.B() );
		glEnable ( GL_LINE_STIPPLE );
		glLineStipple ( 5 , 0xAAAA );
		glLineWidth ( static_cast<GLfloat> (att.GetLineWidth()*5.0) );
	}
	else
	{
		glColor3d ( att.GetWireColor().R() , att.GetWireColor().G() , att.GetWireColor().B() );
		glLineWidth ( static_cast<GLfloat> (att.GetLineWidth()) );

		// line font 
		if ( att.GetLineFont() == LineVisualAtt::LINEFONT_NORMAL )
			glDisable ( GL_LINE_STIPPLE );
		else if ( att.GetLineFont() == LineVisualAtt::LINEFONT_DASHDOT )
		{
			glEnable ( GL_LINE_STIPPLE );
			glLineStipple ( 5 , 0x1c47 );
		}
		else if ( att.GetLineFont() == LineVisualAtt::LINEFONT_DOTTED)
		{
			glEnable ( GL_LINE_STIPPLE );
			glLineStipple ( 5 , 0xAAAA );
		}
	}
}

void VisualAttLoader::Load ( const FacetVisualAtt & att , bool modelselected , const VColor & selcolor ) const
{
	//if (m_prevLoadedFVA != 0)
	//{
	//	if (*m_prevLoadedFVA == att && m_prevSelected == modelselected)
	//		return;
	//}
	//m_prevLoadedFVA = &att;
	//m_prevSelected = modelselected;
	if ( att.GetTranslucent() || modelselected)
	{
		glEnable ( GL_POLYGON_STIPPLE );
		glPolygonStipple(halftone);	 	
	}
	else
		glDisable ( GL_POLYGON_STIPPLE );

	VColor tempcolor;

	if ( modelselected )
		tempcolor = selcolor;
	else
		tempcolor = att.GetShadeColor();

	// opengl commands for setting shade color
	//...
	//GLfloat mat_red = (GLfloat)(tempcolor.R());
	//GLfloat mat_green =  (GLfloat)(tempcolor.G());
	//GLfloat mat_blue = (GLfloat)(tempcolor.B());
	glColor3d ( tempcolor.R(),tempcolor.G(),tempcolor.B() );	 
    GLfloat mat_amb_diffuse[4] = {static_cast<GLfloat> (tempcolor.R()) 
								, static_cast<GLfloat> (tempcolor.G()) 
								, static_cast<GLfloat> (tempcolor.B())  
								, 1.0f };

	glMaterialfv(GL_FRONT_AND_BACK , GL_SPECULAR , mat_specular );
	glMaterialfv(GL_FRONT_AND_BACK , GL_SHININESS , mat_shininess );
	glMaterialfv(GL_FRONT_AND_BACK , GL_EMISSION , mat_emission );

	glMaterialfv ( GL_FRONT_AND_BACK , GL_AMBIENT_AND_DIFFUSE , mat_amb_diffuse );
	glLineWidth (LineVisualAtt::thin);
}

void VisualAttLoader::ResetPrev()
{
	m_prevLoadedFVA = 0;
	m_prevLoadedLVA = 0;
	m_prevLoadedPVA = 0;
	m_prevSelected = false;
}
