// OglScreenPixelData.cpp

#include "stdafx.h"
#include "OglScreenPixelData.h"
#include "VColor.h"
#include "Vbmp.h"

OglScreenPixelData::OglScreenPixelData( int screenx , int screeny )
{
	m_screenx = screenx;
	m_screeny = screeny;

	pixelcount = m_screenx * m_screeny;
	colorvalues = new GLfloat [pixelcount * 3];	

}

OglScreenPixelData::OglScreenPixelData( const Vbmp &mybmp )
{
	m_screenx = mybmp.m_col;
	m_screeny = mybmp.m_row;
	
	pixelcount = m_screenx * m_screeny;
	colorvalues = new GLfloat [pixelcount * 3];	
	
	for ( int rows = 0; rows < m_screeny; rows++ )
	{
		for ( int i=0; i < m_screenx; i++ )
		{
			int curr_pixel_no = rows * m_screenx + i;
			int array_begin_no = curr_pixel_no * 3;			
			unsigned char bv = mybmp.aBitmapBits[array_begin_no];//(unsigned char) ( 255 * currcol.R() );
			unsigned char gv = mybmp.aBitmapBits[array_begin_no + 1];//(unsigned char) ( 255 * currcol.G() );
			unsigned char rv = mybmp.aBitmapBits[array_begin_no + 2];//(unsigned char) ( 255 * currcol.B() );
			double myRcomp = (double)(rv) / 255.0;
			double myGcomp = (double)(gv) / 255.0;
			double myBcomp = (double)(bv) / 255.0;
			VColor currcolor (myRcomp,myGcomp,myBcomp);
			SetPixelColor(curr_pixel_no,currcolor);	
		}
	}
}

OglScreenPixelData::~OglScreenPixelData()
{
	delete [] colorvalues;
}

void OglScreenPixelData::Capture ()
{
	glReadPixels ( 0 , 0 , m_screenx , m_screeny , GL_RGB , GL_FLOAT , colorvalues );
}

void OglScreenPixelData::Draw ()
{
	glDrawPixels ( m_screenx , m_screeny , GL_RGB , GL_FLOAT , colorvalues );
}

int OglScreenPixelData::GetPixelCount ()
{
	return pixelcount;
}

VColor OglScreenPixelData::GetPixelColor ( int pixelno )const
{
	int array_begin_i = pixelno * 3;
	double rval = (double) colorvalues[array_begin_i];
	double gval = (double) colorvalues[array_begin_i+1];
	double bval = (double) colorvalues[array_begin_i+2];

	VColor result ( rval , gval , bval );
	return result;
}

void OglScreenPixelData::SetPixelColor ( int pixelno,VColor &newcolor )
{
	int array_begin_i = pixelno * 3;
	colorvalues[array_begin_i]=(GLfloat)newcolor.R();
	colorvalues[array_begin_i+1]=(GLfloat)newcolor.G();
	colorvalues[array_begin_i+2]=(GLfloat)newcolor.B();
}
