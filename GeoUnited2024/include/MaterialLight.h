// MaterialLight.h: interface for the MaterialLight class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_MATERIALLIGHT_H__B4122CA1_5290_48D7_8875_7139DEAD5039__INCLUDED_)
#define AFX_MATERIALLIGHT_H__B4122CA1_5290_48D7_8875_7139DEAD5039__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000


class MaterialLight  
{
public:
	GLfloat front_ambient[4];
	GLfloat front_diffuse[4];
	GLfloat front_specular[4];
	GLfloat front_emission[4];
	GLfloat front_shininess;

	GLfloat back_ambient[4];
	GLfloat back_diffuse[4];
	GLfloat back_specular[4];
	GLfloat back_emission[4];
	GLfloat back_shininess;

	MaterialLight();
	virtual ~MaterialLight();

	void SetMaterialLight();

};

#endif // !defined(AFX_MATERIALLIGHT_H__B4122CA1_5290_48D7_8875_7139DEAD5039__INCLUDED_)
