// oglNurbsSurface.cpp

#include "stdafx.h"
#include "oglNurbsSurface.h"
#include "BSplineSurface.h"

oglNurbsSurface::oglNurbsSurface( const BSplineSurface &mybspline )
{
	FillIntParameters ( mybspline );
	FillKnots ( mybspline );
	FillControlPoints ( mybspline );
}

oglNurbsSurface::~oglNurbsSurface()
{
	delete [] uknots;
	delete [] vknots;
	delete [] ctlpoints;
}

void oglNurbsSurface::FillIntParameters ( const BSplineSurface &mybspline )
{
	ucount = (mybspline.m_K1 + 1);
	vcount = (mybspline.m_K2 + 1);
	uorder = (GLint)(mybspline.m_A - mybspline.m_K1);
	vorder = (GLint)(mybspline.m_B - mybspline.m_K2);
	uknotno = (GLint)(mybspline.m_A + 1);
	vknotno = (GLint)(mybspline.m_B + 1);	
	ustride = 3;//vcount * 3;
	vstride = ucount * 3;
	
}

void oglNurbsSurface::FillKnots ( const BSplineSurface &mybspline )
{
	uknots = new GLfloat[uknotno];
	vknots = new GLfloat[vknotno];
	
	for ( int i = 0; i < uknotno; i++ )
	{
		uknots[i] = (GLfloat)(mybspline.m_S[i]);
	}

	for ( int j = 0; j < vknotno; j++ )
	{
		vknots[j] = (GLfloat)(mybspline.m_T[j]);
	}

}

void oglNurbsSurface::FillControlPoints ( const BSplineSurface &mybspline )
{
/*	int u,v;
	int pointno;
	int indexs;
	
	ctlpoints = new GLfloat**[ucount];
	for (int i = 0; i < ucount; i++)
	{
		ctlpoints[i] = new GLfloat*[vcount];
		
		for (int j = 0; j < vcount; j++)
		{
			ctlpoints[i][j] = new GLfloat[3];

		}
	}

	for ( u = 0; u<ucount; u++ )
	{
		for ( v=0; v < vcount; v++ )
		{
			pointno = v * ucount + u; 
			indexs = pointno * 3;
			ctlpoints[u][v][0] = (GLfloat)mybspline.controlpoints[indexs];
			ctlpoints[u][v][1] = (GLfloat)mybspline.controlpoints[indexs+1];
			ctlpoints[u][v][2] = (GLfloat)mybspline.controlpoints[indexs+2];
		}
	}
*/
	//ctlpoints = &(mybspline.controlpoints[0]);	
	int pointno = ucount * vcount;
	
	ctlpoints = new GLfloat [pointno];
	for ( int i = 0; i < pointno; i++ )
	{
		int indexs = i * 3;
		ctlpoints[indexs] = mybspline.controlpoints[indexs];//*mybspline.W[indexs];
		ctlpoints[indexs+1] = mybspline.controlpoints[indexs+1];//*mybspline.W[indexs];
		ctlpoints[indexs+2] = mybspline.controlpoints[indexs+2];//*mybspline.W[indexs];
	}

}
