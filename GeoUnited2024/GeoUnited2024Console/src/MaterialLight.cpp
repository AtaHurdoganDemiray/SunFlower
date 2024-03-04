// MaterialLight.cpp: implementation of the MaterialLight class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "MaterialLight.h"

#ifdef _DEBUG
#undef THIS_FILE
static VChr THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

MaterialLight::MaterialLight()
{
	front_ambient[0]		= 0.2f;
	front_ambient[1]		= 0.2f;
	front_ambient[2]		= 0.2f;
	front_ambient[3]		= 1.0f;
	front_diffuse[0]		= 0.8f;
	front_diffuse[1]		= 0.8f;
	front_diffuse[2]		= 0.8f;
	front_diffuse[3]		= 1.0f;
	front_specular[0]		= 0.0f;
	front_specular[1]		= 0.0f;
	front_specular[2]		= 0.0f;
	front_specular[3]		= 1.0f;
	front_emission[0]		= 0.0f;
	front_emission[1]		= 0.0f;
	front_emission[2]		= 0.0f;
	front_emission[3]		= 1.0f;
	front_shininess			= 0;

	back_ambient[3]			= 1.0f;
	back_diffuse[0]			= 0.8f;
	back_diffuse[1]			= 0.8f;
	back_diffuse[2]			= 0.8f;
	back_diffuse[3]			= 1.0f;
	back_specular[0]		= 0.0f;
	back_specular[1]		= 0.0f;
	back_specular[2]		= 0.0f;
	back_specular[3]		= 1.0f;
	back_emission[0]		= 0.0f;
	back_emission[1]		= 0.0f;
	back_emission[2]		= 0.0f;
	back_emission[3]		= 1.0f;
	back_shininess			= 0;

}

MaterialLight::~MaterialLight()
{

}

void MaterialLight::SetMaterialLight()
{
	glMaterialfv(GL_FRONT	, GL_AMBIENT	, front_ambient		);
	glMaterialfv(GL_FRONT	, GL_DIFFUSE	, front_diffuse		);
	glMaterialfv(GL_FRONT	, GL_SPECULAR	, front_specular	);
	glMaterialfv(GL_FRONT	, GL_EMISSION	, front_emission	);
	glMaterialf(GL_FRONT	, GL_SHININESS	, front_shininess	);

	glMaterialfv(GL_BACK	, GL_AMBIENT	, front_ambient		);
	glMaterialfv(GL_BACK	, GL_DIFFUSE	, front_diffuse		);
	glMaterialfv(GL_BACK	, GL_SPECULAR	, front_specular	);
	glMaterialfv(GL_BACK	, GL_EMISSION	, front_emission	);
	glMaterialf(GL_BACK	, GL_SHININESS	, front_shininess	);

}
