// Light.cpp: implementation of the Light class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "Light.h"

#ifdef _DEBUG
#undef THIS_FILE
static VChr THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

Light::Light()
{
	ambient[0]				= 0.5;
	ambient[1]				= 0.5;
	ambient[2]				= 0.5;
	ambient[3]				= 1.0;//1.0;
	diffuse[0]				= 1.0;
	diffuse[1]				= 1.0;
	diffuse[2]				= 1.0;
	diffuse[3]				= 1.0;//1.0;
	specular[0]				= 1.0;
	specular[1]				= 1.0;
	specular[2]				= 1.0;
	specular[3]				= 1.0;
	position[0]				= 0.0;//-0.5145f;//0
	position[1]				= 0.0;
	position[2]				= 1.0;//0.857493f;//1
	position[3]				= 0.0;
	spot_direction[0]		= 0;
	spot_direction[1]		= 0;
	spot_direction[2]		= 1;
	spot_extent				= 0;
	spot_cutoff				= 180;
	constant_attenuation	= 1;
	linear_attenuation		= 0;
	quadratic_attenuation	= 0;

}

Light::~Light()
{

}

void Light::SetLight(unsigned int light)
{
	glLightfv(light,	GL_AMBIENT				,	ambient					);
	glLightfv(light,	GL_DIFFUSE				,	diffuse					);
	glLightfv(light,	GL_SPECULAR				,	specular				);
	glLightfv(light,	GL_POSITION				,	position				);
	glLightfv(light,	GL_SPOT_DIRECTION		,	spot_direction			);

	glLightf( light,	GL_SPOT_EXPONENT		,	spot_extent				);
	glLightf( light,	GL_SPOT_CUTOFF			,	spot_cutoff				);
	glLightf( light,	GL_CONSTANT_ATTENUATION	,	constant_attenuation	);
	glLightf( light,	GL_LINEAR_ATTENUATION	,	linear_attenuation		);
	glLightf( light,	GL_QUADRATIC_ATTENUATION,	quadratic_attenuation	);

	glEnable(light);

}

void Light::InitLightAsRedBookLitSphereExample ()
{
	GLfloat mat_specular[] = {1.0f,1.0f,1.0f,1.0f};
	GLfloat mat_shininess[] = {50.0};
	GLfloat light_position[] = { 1.0f, 1.0f, 1.0f, 0.0f }; //{0.0f, 0.0f, 1000.0f, 0.0f};// {1.0f, 1.0f, 1.0f, 0.0f};
	GLfloat white_light[] = {1.0f,1.0f,1.0f,1.0f};
	GLfloat lmodel_ambient[] = {0.50f,0.50f,0.50f,1.0f};
	//GLfloat lmodel_ambient[] = { 0.80f, 0.80f, 0.80f, 1.0f };

	glClearColor(0.0,0.0,0.0,0.0);
	glShadeModel(GL_SMOOTH);
	glMaterialfv (GL_FRONT_AND_BACK , GL_SPECULAR , mat_specular);
	glMaterialfv (GL_FRONT_AND_BACK , GL_SHININESS , mat_shininess);
	glLightfv (GL_LIGHT0 , GL_POSITION , light_position);
	glLightfv (GL_LIGHT0 , GL_DIFFUSE , white_light);
	glLightfv (GL_LIGHT0 , GL_SPECULAR , white_light);
	glLightfv (GL_LIGHT0 , GL_AMBIENT , lmodel_ambient);
	//glLightModelfv (GL_LIGHT_MODEL_AMBIENT , lmodel_ambient);
	
	glEnable(GL_LIGHTING);
	glEnable(GL_LIGHT0);
	glEnable(GL_DEPTH_TEST);
	glEnable (GL_NORMALIZE);

}
