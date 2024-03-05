/******************************************************************************
*               File: OGLNurbsSupport.cpp									  *
*******************************************************************************
*               Description:                                                  *
*                                                                             *
*******************************************************************************
*               History:                                                      *
*  01.08.2003 15:15:25 Created by:    Ata Hurdogan Demiray                    *
*******************************************************************************
*               (C) 2003 by Pozitif GmbH                                      *
******************************************************************************/

#include "stdafx.h"
#include "OGLNurbsSupport.h"
#include "GL\SGI\glu.h"
#include <stdlib.h>
#include <stdio.h>
#include "TriangleMesh.h"
#include "LineSet.h"
#include "gmE128.h"
#include "gmE126.h"
#include "IgesEntitySet.h"
#include "BSplineSurface.h"
#include "BSplineCurve.h"

#ifndef CALLBACK
#define CALLBACK
#endif

TriangleMesh * trimesh;
LineSet * lineset;
VTriangle * newtri;
bool error_occured;
int cornercount;
int quadno;
int s;
double qnormal[4][3];
double qcorner[4][3];
double lp1[3];
double lp2[3];
bool linestart;

void RecorQuadToTriMesh ()
{
	// make 2 triangles out of quad
	//1st tri
	newtri = trimesh->AddTriangle();
	newtri->SetCorner1 ( &qcorner[0][0] );
	newtri->SetCorner2 ( &qcorner[1][0] );
	newtri->SetCorner3 ( &qcorner[3][0] );
	newtri->SetEdges();
	newtri->SetNormal();
	// The normals
	trimesh->GetNormalSet().InsertPoint( &qnormal[0][0] );
	trimesh->GetNormalSet().InsertPoint( &qnormal[1][0] );
	trimesh->GetNormalSet().InsertPoint( &qnormal[3][0] );

	//2nd tri
	newtri = trimesh->AddTriangle();
	newtri->SetCorner1 ( &qcorner[0][0] );
	newtri->SetCorner2 ( &qcorner[3][0] );
	newtri->SetCorner3 ( &qcorner[2][0] );
	newtri->SetEdges();
	newtri->SetNormal();
	// The normals
	trimesh->GetNormalSet().InsertPoint( &qnormal[0][0] );
	trimesh->GetNormalSet().InsertPoint( &qnormal[3][0] );
	trimesh->GetNormalSet().InsertPoint( &qnormal[2][0] );

}

//#############################################################################

void CALLBACK nurbsError ( GLenum errorCode )//, GLvoid* nurbssupport  )
{
	error_occured = true;
	return;
}

//#############################################################################

void CALLBACK beginCallback (GLenum whichType )//, GLvoid* nurbssupport ) 
{
	glBegin ( whichType );
	switch ( whichType )
	{
		case GL_LINES:
			s=1;
		break;

		case GL_LINE_LOOP:
			s=2;
		break;
					
		case GL_LINE_STRIP:
			s=3;
			linestart = true;
		break;

		case GL_TRIANGLE_STRIP:
			s=4;	
		break;

		case GL_TRIANGLE_FAN:
			s=5;
			newtri = trimesh->AddTriangle();
			cornercount = 1;
		break;
		
		case GL_QUADS:
			s=6;
		break;
		
		case GL_QUAD_STRIP:
			quadno = 0;
			s=7;
		break;

		case GL_POLYGON:
			s=8;
		break;
	}
};

//#############################################################################

void CALLBACK dispbeginCallback (GLenum whichType ) 
{
	glBegin ( whichType );
};

//#############################################################################

void CALLBACK vertexCallback ( GLfloat * vertex )
{
	double temp[3];
	
	if ( s == 5 )
	{
		// triangle fan
		temp[0] = (double)vertex[0];
		temp[1] = (double)vertex[1];
		temp[2] = (double)vertex[2];

		if ( cornercount == 1 )
			newtri->SetCorner1 ( temp );
		else if ( cornercount == 2 )
			newtri->SetCorner2 ( temp );
		else if ( cornercount == 3 )
		{
			newtri->SetCorner3 ( temp );
			newtri->SetEdges();
			newtri->SetNormal();
		}
		else if ( cornercount > 3 )
		{
			VTriangle *tmp = trimesh->AddTriangle();
			tmp->SetCorner1 ( newtri->GetCorner1().GetArray() );
			tmp->SetCorner2 ( newtri->GetCorner3().GetArray() );
			tmp->SetCorner3 ( temp );
			tmp->SetEdges();
			tmp->SetNormal();
			newtri = tmp;
		}


		cornercount++;
	}
	else if ( s == 7 )
	{
		// quad strip
		qcorner[quadno][0] = (double)vertex[0];
		qcorner[quadno][1] = (double)vertex[1];
		qcorner[quadno][2] = (double)vertex[2];

		quadno++;

		if ( quadno == 4 )
		{
			RecorQuadToTriMesh();
			quadno = 2;
			qcorner[0][0] = qcorner[2][0];
			qcorner[0][1] = qcorner[2][1];
			qcorner[0][2] = qcorner[2][2];

			qcorner[1][0] = qcorner[3][0];
			qcorner[1][1] = qcorner[3][1];
			qcorner[1][2] = qcorner[3][2];

			qnormal[0][0] = qnormal[2][0];
			qnormal[0][1] = qnormal[2][1];
			qnormal[0][2] = qnormal[2][2];

			qnormal[1][0] = qnormal[3][0];
			qnormal[1][1] = qnormal[3][1];
			qnormal[1][2] = qnormal[3][2];

		}
	}
	else if ( s == 3 )
	{
		if ( linestart )
		{
			lp1[0] = (double)vertex[0];
			lp1[1] = (double)vertex[1];
			lp1[2] = (double)vertex[2];
			linestart = false;
		}
		else
		{
			// line end
			lp2[0] = (double)vertex[0];
			lp2[1] = (double)vertex[1];
			lp2[2] = (double)vertex[2];

			lineset->AddLine ( lp1 , lp2 );

			lp1[0] = (double)vertex[0];
			lp1[1] = (double)vertex[1];
			lp1[2] = (double)vertex[2];
		}
	}
	
	return;
}

//#############################################################################

void CALLBACK dispvertexCallback ( GLfloat * vertex )
{
	glVertex3fv ( vertex );	
}	

//#############################################################################

void CALLBACK normalCallback ( GLfloat * vertex )//, GLvoid* nurbssupport )
{
	double temp[3];

	if ( s == 5 )
	{
		// triangle fan
		temp[0] = (double)vertex[0];
		temp[1] = (double)vertex[1];
		temp[2] = (double)vertex[2];

		if ( cornercount <= 3 )
		{
			trimesh->GetNormalSet().InsertPoint( temp );
		}
		else
		{
			int lastpno = trimesh->GetNormalSet().Size() - 1;
			trimesh->GetNormalSet().InsertPoint( 
										trimesh->GetNormalSet()[lastpno-2]->GetArray() );
			trimesh->GetNormalSet().InsertPoint( 
										trimesh->GetNormalSet()[lastpno]->GetArray() );

			trimesh->GetNormalSet().InsertPoint( temp );
		}
	}
	else if ( s == 7 )
	{
		//quad strip
		qnormal[quadno][0] = (double)vertex[0];
		qnormal[quadno][1] = (double)vertex[1];
		qnormal[quadno][2] = (double)vertex[2];
	}

	return;
}

//#############################################################################

void CALLBACK dispnormalCallback ( GLfloat * vertex )
{
	glNormal3fv ( vertex );
	
	return;
}

//#############################################################################

void CALLBACK endCallback ( )//GLvoid* nurbssupport )
{
	if ( s == 5 )
	{
	
	}
	else if ( s == 7 )
	{
		quadno = 0;
	}

}

//#############################################################################

void CALLBACK dispendCallback ( )//GLvoid* nurbssupport )
{
	glEnd();
}

//#############################################################################

OGLNurbsSupport::OGLNurbsSupport()
{
	theNurb = gluNewNurbsRenderer();
}

OGLNurbsSupport::~OGLNurbsSupport()
{
	gluDeleteNurbsRenderer ( theNurb );
}

void OGLNurbsSupport::SetProperties( double tolerance , bool outline , bool displaylist )
{
	const GLubyte * str = gluGetString ( GLU_VERSION );
	gluNurbsProperty ( theNurb , GLU_NURBS_MODE_EXT , GLU_NURBS_TESSELLATOR_EXT );
	gluNurbsProperty ( theNurb , GLU_SAMPLING_METHOD , GLU_OBJECT_PATH_LENGTH_EXT );
	gluNurbsProperty ( theNurb , GLU_SAMPLING_TOLERANCE , tolerance );
	
	if ( !outline )
		gluNurbsProperty ( theNurb , GLU_DISPLAY_MODE , GLU_FILL );
	else
		gluNurbsProperty ( theNurb , GLU_DISPLAY_MODE , GLU_OUTLINE_PATCH );
	
	gluNurbsCallback ( theNurb , GLU_ERROR , (GLvoid(CALLBACK*)())nurbsError );
	
	if ( !displaylist )
	{
		gluNurbsCallback ( theNurb , GLU_NURBS_BEGIN_EXT , (GLvoid(CALLBACK*)())beginCallback );
		gluNurbsCallback ( theNurb , GLU_NURBS_VERTEX_EXT , (GLvoid(CALLBACK*)())vertexCallback );
		gluNurbsCallback ( theNurb , GLU_NURBS_NORMAL_EXT , (GLvoid(CALLBACK*)())normalCallback );
		gluNurbsCallback ( theNurb , GLU_NURBS_END_EXT , (GLvoid(CALLBACK*)())endCallback );
	}
	else
	{
		gluNurbsCallback ( theNurb , GLU_NURBS_BEGIN_EXT , (GLvoid(CALLBACK*)())dispbeginCallback );
		gluNurbsCallback ( theNurb , GLU_NURBS_VERTEX_EXT , (GLvoid(CALLBACK*)())dispvertexCallback );
		gluNurbsCallback ( theNurb , GLU_NURBS_NORMAL_EXT , (GLvoid(CALLBACK*)())dispnormalCallback );
		gluNurbsCallback ( theNurb , GLU_NURBS_END_EXT , (GLvoid(CALLBACK*)())dispendCallback );
	}

}

//#############################################################################

TriangleMesh * OGLNurbsSupport::TriangulateNurbs 
							( gmE128 & nurbs , gmE126List & trimmingloops , double tolerance )
{
	
	SetProperties ( tolerance , false , false );
	
	trimesh = new TriangleMesh;

	// sample trimming boundaries and nurbs data
	GLfloat knots[8] = { 8.0 , 8.0 , 8.0 , 8.0 , 1.0 , 1.0 , 1.0 , 1.0 };
	
	GLfloat edgePt [5][2] = 
	{ {0.0 , 0.0 } , {1.0 , 0.0 } , { 1.0 , 1.0 } , {0.0 , 1.0 } , {0.0 , 0.0 } };
	
	GLfloat curvePt[4][2] = 
	{ {0.25 , 0.5} , {0.25,0.75} , {0.75,0.75} , {0.75,0.5} };

	GLfloat curveKnots[8]
		= { 0.0 , 0.0 , 0.0 , 0.0 , 1.0 , 1.0 , 1.0 , 1.0 };

	GLfloat pwlPt[3][2] =
	{ {0.75,0.5} , {0.5,0.25} , {0.25,0.5} };

	GLfloat ctlpoints[4][4][3];
	int u,v;

	for ( u = 0; u<4; u++ )
	{
		for ( v=0; v < 4; v++ )
		{
			ctlpoints[u][v][0] = 2.0 * ((GLfloat)u - 1.5 );
			ctlpoints[u][v][1] = 2.0 * ((GLfloat)v - 1.5 );

			if ( (u == 1 || u == 2) && ( v == 1 || v == 2 ) )
				ctlpoints[u][v][2] = 3.0;
			else
				ctlpoints[u][v][2] = -3.0;
		}
	}



/*
	GLfloat ***ctlpoints = new GLfloat**[nurbs.M1];
	for (int i = 0; i < nurbs.M1; i++)
	{
		ctlpoints[i] = new GLfloat*[nurbs.M2];
		
		for (int j = 0; j < nurbs.M2; j++)
		{
			ctlpoints[i][j] = new GLfloat[3];

		}
	}

	for (i = 0; i < nurbs.M1; i++)
	{
		for (int j = 0; j < nurbs.M2; j++)
		{
			ctlpoints[i][j][0] = nurbs.Xc[i][j];
			ctlpoints[i][j][1] = nurbs.Yc[i][j];
			ctlpoints[i][j][2] = nurbs.Zc[i][j];
		}
	}



//	ctlpoints = new GLFloat [3][nurbs.M2][nurbs.M1];

	int uknotno = nurbs.K1;
	int vknotno = nurbs.K2;
	
	int ustride = 3 * nurbs.M1;
	int vstride = 3;

	int uorder = uknotno - nurbs.M1;
	int vorder = vknotno - nurbs.M2;
*/
	// Realising nurb and trimming it...

	double *cntlpoints=new double[50];

	gluBeginSurface ( theNurb );

	gluNurbsSurface ( theNurb , 8 , knots , 8 , knots , 4*3 , 3 , &ctlpoints[0][0][0]
						, 4 , 4 , GL_MAP2_VERTEX_3 );

	gluBeginTrim ( theNurb );

	gluPwlCurve ( theNurb , 5 , &edgePt[0][0] , 2 , GLU_MAP1_TRIM_2 );

	gluEndTrim ( theNurb );

	gluBeginTrim ( theNurb );

	gluNurbsCurve ( theNurb , 8 , curveKnots , 2 , &curvePt[0][0] 
					, 4 , GLU_MAP1_TRIM_2 );

	gluPwlCurve ( theNurb , 3 , &pwlPt[0][0] , 2 , GLU_MAP1_TRIM_2 );

	gluEndTrim ( theNurb );

	gluEndSurface ( theNurb );

	trimesh->UpdateMinMax();

	return trimesh;

}

//#############################################################################

unsigned int OGLNurbsSupport::CreateDispListForNurbs ( double tolerance )
{
	SetProperties ( tolerance , false , true );

	// sample trimming boundaries and nurbs data
	GLfloat knots[8] = { 0.0 , 0.0 , 0.0 , 0.0 , 1.0 , 1.0 , 1.0 , 1.0 };
	
	GLfloat edgePt [5][2] = 
	{ {0.0 , 0.0 } , {1.0 , 0.0 } , { 1.0 , 1.0 } , {0.0 , 1.0 } , {0.0 , 0.0 } };
	
	GLfloat curvePt[4][2] = 
	{ {0.25 , 0.5} , {0.25,0.75} , {0.75,0.75} , {0.75,0.5} };

	GLfloat curveKnots[8]
		= { 0.0 , 0.0 , 0.0 , 0.0 , 1.0 , 1.0 , 1.0 , 1.0 };

	GLfloat pwlPt[3][2] =
	{ {0.75,0.5} , {0.5,0.25} , {0.25,0.5} };

	GLfloat ctlpoints[4][4][3];
	int u,v;

	for ( u = 0; u<4; u++ )
	{
		for ( v=0; v < 4; v++ )
		{
			ctlpoints[u][v][0] = 2.0 * ((GLfloat)u - 1.5 );
			ctlpoints[u][v][1] = 2.0 * ((GLfloat)v - 1.5 );

			if ( (u == 1 || u == 2) && ( v == 1 || v == 2 ) )
				ctlpoints[u][v][2] = 3.0;
			else
				ctlpoints[u][v][2] = -3.0;
		}
	}
	
	glEnd();
	GLuint disp_list;
	disp_list = glGenLists(1);

	glNewList(disp_list, GL_COMPILE);

	glEnable ( GL_LIGHTING );
	glShadeModel (GL_SMOOTH);
	
	// Realising nurb and trimming it...
	gluBeginSurface ( theNurb );

	gluNurbsSurface ( theNurb , 8 , knots , 8 , knots , 4*3 , 3 , &ctlpoints[0][0][0]
						, 4 , 4 , GL_MAP2_VERTEX_3 );

	gluBeginTrim ( theNurb );

	gluPwlCurve ( theNurb , 5 , &edgePt[0][0] , 2 , GLU_MAP1_TRIM_2 );

	gluEndTrim ( theNurb );

	gluBeginTrim ( theNurb );

	gluNurbsCurve ( theNurb , 8 , curveKnots , 2 , &curvePt[0][0] 
					, 4 , GLU_MAP1_TRIM_2 );

	gluPwlCurve ( theNurb , 3 , &pwlPt[0][0] , 2 , GLU_MAP1_TRIM_2 );

	gluEndTrim ( theNurb );

	gluEndSurface ( theNurb );

	glDisable (GL_LIGHTING);
	glDisable ( GL_POLYGON_STIPPLE );

	glEndList();

	return (unsigned int)disp_list;

}

unsigned int OGLNurbsSupport::CreateWireDispListForNurbs ( double tolerance )
{
	SetProperties ( tolerance , true , true );

	// sample trimming boundaries and nurbs data
	GLfloat knots[8] = { 0.0 , 0.0 , 0.0 , 0.0 , 1.0 , 1.0 , 1.0 , 1.0 };
	
	GLfloat edgePt [5][2] = 
	{ {0.0 , 0.0 } , {1.0 , 0.0 } , { 1.0 , 1.0 } , {0.0 , 1.0 } , {0.0 , 0.0 } };
	
	GLfloat curvePt[4][2] = 
	{ {0.25 , 0.5} , {0.25,0.75} , {0.75,0.75} , {0.75,0.5} };

	GLfloat curveKnots[8]
		= { 0.0 , 0.0 , 0.0 , 0.0 , 1.0 , 1.0 , 1.0 , 1.0 };

	GLfloat pwlPt[3][2] =
	{ {0.75,0.5} , {0.5,0.25} , {0.25,0.5} };

	GLfloat ctlpoints[4][4][3];
	int u,v;

	for ( u = 0; u<4; u++ )
	{
		for ( v=0; v < 4; v++ )
		{
			ctlpoints[u][v][0] = 2.0 * ((GLfloat)u - 1.5 );
			ctlpoints[u][v][1] = 2.0 * ((GLfloat)v - 1.5 );

			if ( (u == 1 || u == 2) && ( v == 1 || v == 2 ) )
				ctlpoints[u][v][2] = 3.0;
			else
				ctlpoints[u][v][2] = -3.0;
		}
	}
	
	glEnd();
	GLuint disp_list;
	disp_list = glGenLists(1);

	glNewList(disp_list, GL_COMPILE);
	
	// Realising nurb and trimming it...
	gluBeginSurface ( theNurb );

	gluNurbsSurface ( theNurb , 8 , knots , 8 , knots , 4*3 , 3 , &ctlpoints[0][0][0]
						, 4 , 4 , GL_MAP2_VERTEX_3 );

	gluBeginTrim ( theNurb );

	gluPwlCurve ( theNurb , 5 , &edgePt[0][0] , 2 , GLU_MAP1_TRIM_2 );

	gluEndTrim ( theNurb );

	gluBeginTrim ( theNurb );

	gluNurbsCurve ( theNurb , 8 , curveKnots , 2 , &curvePt[0][0] 
					, 4 , GLU_MAP1_TRIM_2 );

	gluPwlCurve ( theNurb , 3 , &pwlPt[0][0] , 2 , GLU_MAP1_TRIM_2 );

	gluEndTrim ( theNurb );

	gluEndSurface ( theNurb );

	glDisable ( GL_POLYGON_STIPPLE );

	glEndList();

	return (unsigned int)disp_list;

}

LineSet * OGLNurbsSupport::TessellateOutlineOfNurbs ( double tolerance )
{
	SetProperties ( tolerance , true , false );
	
	lineset = new LineSet;

	// sample trimming boundaries and nurbs data
	GLfloat knots[8] = { 0.0 , 0.0 , 0.0 , 0.0 , 1.0 , 1.0 , 1.0 , 1.0 };
	
	GLfloat edgePt [5][2] = 
	{ {0.0 , 0.0 } , {1.0 , 0.0 } , { 1.0 , 1.0 } , {0.0 , 1.0 } , {0.0 , 0.0 } };
	
	GLfloat curvePt[4][2] = 
	{ {0.25 , 0.5} , {0.25,0.75} , {0.75,0.75} , {0.75,0.5} };

	GLfloat curveKnots[8]
		= { 0.0 , 0.0 , 0.0 , 0.0 , 1.0 , 1.0 , 1.0 , 1.0 };

	GLfloat pwlPt[3][2] =
	{ {0.75,0.5} , {0.5,0.25} , {0.25,0.5} };

	GLfloat ctlpoints[4][4][3];
	int u,v;

	for ( u = 0; u<4; u++ )
	{
		for ( v=0; v < 4; v++ )
		{
			ctlpoints[u][v][0] = 2.0 * ((GLfloat)u - 1.5 );
			ctlpoints[u][v][1] = 2.0 * ((GLfloat)v - 1.5 );

			if ( (u == 1 || u == 2) && ( v == 1 || v == 2 ) )
				ctlpoints[u][v][2] = 3.0;
			else
				ctlpoints[u][v][2] = -3.0;
		}
	}

	// Realising nurb and trimming it...
	gluBeginSurface ( theNurb );

	gluNurbsSurface ( theNurb , 8 , knots , 8 , knots , 4*3 , 3 , &ctlpoints[0][0][0]
						, 4 , 4 , GL_MAP2_VERTEX_3 );

	gluBeginTrim ( theNurb );

	gluPwlCurve ( theNurb , 5 , &edgePt[0][0] , 2 , GLU_MAP1_TRIM_2 );

	gluEndTrim ( theNurb );

	gluBeginTrim ( theNurb );

	gluNurbsCurve ( theNurb , 8 , curveKnots , 2 , &curvePt[0][0] 
					, 4 , GLU_MAP1_TRIM_2 );

	gluPwlCurve ( theNurb , 3 , &pwlPt[0][0] , 2 , GLU_MAP1_TRIM_2 );

	gluEndTrim ( theNurb );

	gluEndSurface ( theNurb );

	lineset->UpdateMinMax();

	return lineset;	

}

//#############################################################################

TriangleMeshSet * OGLNurbsSupport::TriangulateNurbs 
							( IgesEntitySet &allentities , double tolerance )
{
	BSplineSurface * mybspline = NULL;
	// BSplineCurve * mycurve;

	TriangleMeshSet * newset = new TriangleMeshSet;

	for ( int i = 0; i < allentities.Size(); i++ )
	{
		if ( allentities[i]->entitytype == 128 )
		{
			mybspline = (BSplineSurface *)allentities[i];
			
			oglNurbsSurface mysurface ( *mybspline );
			
			oglNurbsCurveVec trimvec;	

			if ( i != allentities.Size() - 2 )
				while ( allentities[i+1]->entitytype == 126 )
				{	
					i++;
					BSplineCurve * absplinetrimcurve = static_cast<BSplineCurve *>(allentities[i]);
					oglNurbsCurve * atrimcurve = new oglNurbsCurve( *absplinetrimcurve );
					trimvec.push_back ( atrimcurve );
				}
					
			//TriangleMesh * mytri = TriangulateNurbs ( mybspline , tolerance );
			TriangleMesh * mytri = DrawNurbsSurface ( mysurface , trimvec , tolerance );
			mytri->UpdateMinMax();
			newset->push_back ( mytri );
		}
		//else if (  allentities[i]->entitytype == 126 )
		//{
		//	mycurve = (BSplineCurve *)allentities[i];
		//	break;
		//}
	}

	return newset;

}

//#############################################################################
/*
TriangleMesh * OGLNurbsSupport::TriangulateNurbs 
							( BSplineSurface * mybspline , double tolerance )
{

	if ( mybspline == NULL )
		return NULL;

	SetProperties ( tolerance , false , false );
	
	trimesh = new TriangleMesh;

	int u,v;

	int pointno;
	int ucount = (mybspline->K1 + 1);
	int vcount = (mybspline->K2 + 1);
	GLint uorder = (GLint)(mybspline->A - mybspline->K1);
	GLint vorder = (GLint)(mybspline->B - mybspline->K2);
	GLint knot1no = (GLint)(mybspline->A + 1);
	GLint knot2no = (GLint)(mybspline->B + 1);	
	int indexs;

	GLfloat * knots1 = new GLfloat[knot1no];
	GLfloat * knots2 = new GLfloat[knot2no];
	
	for ( int i = 0; i < knot1no; i++ )
	{
		knots1[i] = (GLfloat)(mybspline->S[i]);
	}

	for ( int j = 0; j < knot2no; j++ )
	{
		knots2[j] = (GLfloat)(mybspline->T[j]);
	}

	//GLfloat ***ctlpoints;//[4][4][3];
//	GLfloat ***ctlpoints = new GLfloat**[ucount];
//	for (i = 0; i < ucount; i++)
//	{
//		ctlpoints[i] = new GLfloat*[vcount];
//		
//		for (j = 0; j < vcount; j++)
//		{
//			ctlpoints[i][j] = new GLfloat[3];
//
//		}
//	}
	
	GLfloat ctlpoints[4][4][3];

	for ( u = 0; u<ucount; u++ )
	{
		for ( v=0; v < vcount; v++ )
		{
			pointno = v * ucount + u; 
			indexs = pointno * 3;
			ctlpoints[u][v][0] = (GLfloat)mybspline->controlpoints[indexs];
			ctlpoints[u][v][1] = (GLfloat)mybspline->controlpoints[indexs+1];
			ctlpoints[u][v][2] = (GLfloat)mybspline->controlpoints[indexs+2];
		}
	}

	// see mapping
	GLfloat * mytry = &(ctlpoints[0][0][0]);
	for ( u=0; u<ucount; u++ )
		for( v=0; v < vcount; v++ )
		{
			GLfloat mynum1 = ctlpoints[u][v][0];
			GLfloat singlearr = *(mytry);
			
			mynum1 = ctlpoints[u][v][1];
			mytry++;
			singlearr = *(mytry);

			mynum1 = ctlpoints[u][v][2];
			mytry++;
			singlearr = *(mytry);
			mytry++;
		}


	GLfloat knots[8] = { 0.0 , 0.0 , 0.0 , 0.0 , 1.0 , 1.0 , 1.0 , 1.0 };

	// Realising nurb and trimming it...
	gluBeginSurface ( theNurb );

	gluNurbsSurface ( theNurb , knot1no , knots1 , knot2no , knots2 
	//, vcount*3 , 3 , &ctlpoints[0][0][0]
	, 3 , 3*ucount , &(mybspline->controlpoints[0])	
	, uorder , vorder	
	, GL_MAP2_VERTEX_3 );
	
	gluEndSurface ( theNurb );

	trimesh->UpdateMinMax();

	return trimesh;

}

*/

TriangleMesh * OGLNurbsSupport::DrawNurbsSurface ( oglNurbsSurface &mysurface , oglNurbsCurveVec &trimvec , double tolerance)
{
	error_occured = false;

	SetProperties ( tolerance , false , false );
	
	trimesh = new TriangleMesh;
	
	gluBeginSurface ( theNurb );

	gluNurbsSurface ( theNurb , mysurface.uknotno , &(mysurface.uknots[0]) 
					, mysurface.vknotno , &(mysurface.vknots[0]) 
					, mysurface.ustride , mysurface.vstride , mysurface.ctlpoints
					, mysurface.uorder , mysurface.vorder , GL_MAP2_VERTEX_3 );
		
	gluBeginTrim ( theNurb );

	for ( int i = 0; i < trimvec.size() ; i++ )
	{
		oglNurbsCurve * mytrimcurve = trimvec[i];
		ApplyATrimLoop ( *mytrimcurve );
	}
		
	gluEndTrim (theNurb);

	gluEndSurface ( theNurb );

	return trimesh;
}

void OGLNurbsSupport::ReNewTheNurb ()
{
	gluDeleteNurbsRenderer ( theNurb );
	theNurb = gluNewNurbsRenderer();

}

void OGLNurbsSupport::ApplyATrimLoop ( oglNurbsCurve &mytrimcurve )
{

	//gluBeginTrim ( theNurb );
	
	gluNurbsCurve ( theNurb , mytrimcurve.uknotno , &mytrimcurve.uknots[0] 
					, mytrimcurve.ustride , &mytrimcurve.ctlpoints[0]
					, mytrimcurve.uorder , GLU_MAP1_TRIM_2 );
	
	//gluEndTrim (theNurb);

}

