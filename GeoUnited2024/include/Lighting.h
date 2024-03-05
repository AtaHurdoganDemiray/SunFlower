// Lighting.h: interface for the Lighting class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_LIGHTING_H__58675C4B_BA11_4EA0_A5E1_27F02810F206__INCLUDED_)
#define AFX_LIGHTING_H__58675C4B_BA11_4EA0_A5E1_27F02810F206__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "Light.h"
#include "MaterialLight.h"


class Lighting  
{
public:

	Light light;
	MaterialLight mat_light;

	Lighting();
	virtual ~Lighting();

};

#endif // !defined(AFX_LIGHTING_H__58675C4B_BA11_4EA0_A5E1_27F02810F206__INCLUDED_)
