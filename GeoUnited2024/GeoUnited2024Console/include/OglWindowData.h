// OglWindowData.h

#ifndef __OglWindowData_h__
#define __OglWindowData_h__

struct OglWindowData
{
public:
	OglWindowData ( const vreal &cornerx , const vreal &cornery 
					, const vreal &newnear , const vreal &newfar );
	OglWindowData ( const OglWindowData &Foo );
	OglWindowData & operator = ( const OglWindowData & Foo );
	bool GetScreenCoordOfPoint ( GLfloat mypoint[4] , GLfloat inform [4] );

	vreal mcornerx , mcornery;
	vreal mnear , mfar;
};

#endif //__OglWindowData_h__
