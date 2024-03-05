// OglScreenPixelData.h

#ifndef __OglScreenPixelData_h__
#define __OglScreenPixelData_h__

#include "PixelData.h"

struct VColor;
//class Vbmp;
#include"Vbmp.h" 

class OglScreenPixelData:public PixelData<GLfloat>
{
public:
	OglScreenPixelData (int screenx,int screeny)
	:PixelData<GLfloat>(screenx,screeny)
	{

	};
	OglScreenPixelData ( const Vbmp &mybmp )
	:PixelData<GLfloat>(mybmp)
	{

	};
	OglScreenPixelData (const dPixelData &pixdat)
	:PixelData<GLfloat>(pixdat.m_screenx,pixdat.m_screeny)
	{
		for (int i=0;i<pixelcount*3;i++)
		{
			colorvalues[i]=(GLfloat)(pixdat[i]);
		}
	};
	virtual ~OglScreenPixelData()
	{

	};
	void Capture()
	{
		glReadPixels ( 0 , 0 , m_screenx , m_screeny , GL_RGB , GL_FLOAT , colorvalues );
	};
	void Draw ()
	{
		glDrawPixels ( m_screenx , m_screeny , GL_RGB , GL_FLOAT , colorvalues );
	};
};

#endif
