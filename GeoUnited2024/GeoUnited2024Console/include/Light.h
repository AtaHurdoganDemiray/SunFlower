// Light.h: interface for the Light class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_LIGHT_H__88B48334_123A_40D5_A844_1A48A5816B3B__INCLUDED_)
#define AFX_LIGHT_H__88B48334_123A_40D5_A844_1A48A5816B3B__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "gl/gl.h"

class Light  
{
public:

	GLfloat ambient[4];
	GLfloat diffuse[4];
	GLfloat specular[4];
	GLfloat position[4];
	GLfloat spot_direction[3];
	GLfloat spot_extent;
	GLfloat spot_cutoff;
	GLfloat constant_attenuation;
	GLfloat linear_attenuation;
	GLfloat quadratic_attenuation;

	Light();
	virtual ~Light();

	void SetLight(unsigned int light);
	void InitLightAsRedBookLitSphereExample ();
};

#endif // !defined(AFX_LIGHT_H__88B48334_123A_40D5_A844_1A48A5816B3B__INCLUDED_)
