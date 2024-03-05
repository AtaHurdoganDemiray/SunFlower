// oglNurbsSurface.h

#ifndef __oglNurbsSurface_h__
#define __oglNurbsSurface_h__

class BSplineSurface;

struct oglNurbsSurface
{
public:
	oglNurbsSurface ( const BSplineSurface &mybspline );
	~oglNurbsSurface ();
	void FillIntParameters ( const BSplineSurface &mybspline );
	void FillKnots ( const BSplineSurface &mybspline );
	void FillControlPoints ( const BSplineSurface & mybspline );
	
	GLint uknotno , vknotno;
	GLfloat *uknots , *vknots;
	// controlpointno 
	GLint  ustride , vstride;
	GLfloat * ctlpoints;
	GLint uorder , vorder;
private:
	int ucount , vcount;

};

#endif // __oglNurbsSurface_h__