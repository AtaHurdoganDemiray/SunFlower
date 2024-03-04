#if !defined(BASIC_DEF_INCLUDED)
#define BASIC_DEF_INCLUDED
#include <cmath>

const double OGL_SEL_ZCONST = pow (2.0,32)-1;

/*
inline GLfloat* GetColorArray (COLORREF color)
{
	GLfloat* color_array = new GLfloat[3];

	color_array[0] = (GLfloat) ( GetRValue(color) / 255.0 );
	color_array[1] = (GLfloat) ( GetGValue(color) / 255.0 );
	color_array[2] = (GLfloat) ( GetBValue(color) / 255.0 );

	return color_array;
}
*/
#endif // !defined(BASIC_DEF_INCLUDED)
