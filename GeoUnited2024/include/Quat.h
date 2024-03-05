// Quat.h: interface for the Quat class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_QUAT_H__E26D7CE8_45EA_4CA8_8883_F892A3957DE2__INCLUDED_)
#define AFX_QUAT_H__E26D7CE8_45EA_4CA8_8883_F892A3957DE2__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#ifndef M_PI
#define M_PI 3.14159265358979323846264338327950288419716939937510f
#endif

#define DELTA 1e-6     // error tolerance

#include "gl/gl.h"
struct CoordinateSystem;
class Bofstream;
class Bifstream;

class Quat  
{
public:
	GLdouble m[4][4];
	GLdouble w, x, y, z;
	GLdouble* QuatToMat();
	void EulerToQuat(GLdouble roll, GLdouble pitch, GLdouble yaw);
	void Multip(Quat q2); 

	Quat();
	virtual ~Quat();
	void Serialize(Bofstream &ar)const;
	void Serialize(Bifstream &ar);
	void GetLookDir(vreal lookDir[]);
	CoordinateSystem GetLookCoordinateSystem();
	
private:

};

#endif // !defined(AFX_QUAT_H__E26D7CE8_45EA_4CA8_8883_F892A3957DE2__INCLUDED_)
