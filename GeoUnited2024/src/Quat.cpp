// Quat.cpp: implementation of the Quat class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "Quat.h"
#include <math.h>
#include "Matrix.h"
#include "CoordinateSystem.h"
#include "TransformationMatrix.h"
#include "Bifstream.h"
#include "Bofstream.h"

#ifdef _DEBUG
#undef THIS_FILE
static VChr THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

Quat::Quat()
{
	w = 0;
	x = 0;
	y = 0;
	z = 0;
	for (int i = 0; i < 4; i++){
		for (int j = 0; j < 4; j++){
			m[i][j] = 0;
		}
	}
	EulerToQuat(45.0 * DEGREE_TO_RAD , 0, 45.0 * DEGREE_TO_RAD);
	
}

Quat::~Quat()
{

}

void Quat::Serialize(Bofstream &ar)const 
{
	ar.WriteDouble(w);//ar << w;
	ar.WriteDouble(x);//ar << x;
	ar.WriteDouble(y);//ar << y;
	ar.WriteDouble(z);//ar << z;
	for (int i = 0; i < 4; i++)
	{
		for (int j = 0; j < 4; j++)
		{
			ar.WriteDouble (m[i][j]);
		}
	}
}
void Quat::Serialize(Bifstream &ar)
{
	ar.ReadDouble(w);//ar >> w;
	ar.ReadDouble(x);//ar >> x;
	ar.ReadDouble(y);//ar >> y;
	ar.ReadDouble(z);//ar >> z;
	for (int i = 0; i < 4; i++)
	{
		for (int j = 0; j < 4; j++)
		{
			ar.ReadDouble (m[i][j]);
		}
	}
}

/*SDOC***********************************************************************

  Name:		gluQuatToMat_EXT

  Action:   Converts quaternion representation of a rotation to a matrix
			representation

  Params:   GL_QUAT* (our quaternion), GLfloat (4x4 matrix)

  Returns:  nothing

  Comments: remember matrix (in OGL) is represented in COLUMN major form

***********************************************************************EDOC*/
GLdouble* Quat::QuatToMat()
{

  GLdouble wx, wy, wz, xx, yy, yz, xy, xz, zz, x2, y2, z2;

  x2 = x + x; y2 = y + y; z2 = z + z;
  xx = x * x2;   xy = x * y2;   xz = x * z2;
  yy = y * y2;   yz = y * z2;   zz = z * z2;
  wx = w * x2;   wy = w * y2;   wz = w * z2;
/* 
  GLdouble (*m)[16];

  *m[0] = 1.0 - (yy + zz);
  *m[1] = xy - wz;
  *m[2] = xz + wy;
  *m[3] = 0.0;
 
  *m[4] = xy + wz;
  *m[5] = 1.0 - (xx + zz);
  *m[6] = yz - wx;
  *m[7] = 0.0;

  *m[8] = xz - wy;
  *m[9] = yz + wx;
  *m[10] = 1.0 - (xx + yy);
  *m[11] = 0.0;

  *m[12] = 0;
  *m[13] = 0;
  *m[14] = 0;
  *m[15] = 1;
*/
/*
  GLdouble(* m)[4][4] = new GLdouble m[4][4];
*/
  m[0][0] = 1.0 - (yy + zz);
  m[0][1] = xy - wz;
  m[0][2] = xz + wy;
  m[0][3] = 0.0;
 
  m[1][0] = xy + wz;
  m[1][1] = 1.0 - (xx + zz);
  m[1][2] = yz - wx;
  m[1][3] = 0.0;

  m[2][0] = xz - wy;
  m[2][1] = yz + wx;
  m[2][2] = 1.0 - (xx + yy);
  m[2][3] = 0.0;

  m[3][0] = 0.;
  m[3][1] = 0.;
  m[3][2] = 0.;
  m[3][3] = 1.;

  return &m[0][0];

}

/*SDOC***********************************************************************

  Name:		gluEulerToQuat_EXT

  Action:   Converts representation of a rotation from Euler angles to
			quaternion representation

  Params:   GLfloat (roll), GLfloat (pitch), GLfloat (yaw), GL_QUAT* (quat)

  Returns:  nothing

  Comments: remember:	roll  - rotation around X axis
						pitch - rotation around Y axis
						yaw   - rotation around Z axis
			
			rotations are performed in the following order:
					yaw -> pitch -> roll

			Qfinal = Qyaw Qpitch Qroll

***********************************************************************EDOC*/
void Quat::EulerToQuat(GLdouble roll, GLdouble pitch, GLdouble yaw)
{
	
	GLdouble cr, cp, cy, sr, sp, sy, cpcy, spsy;

	cr = /*(GLfloat)*/(cos(roll/2));
	cp = /*(GLfloat)*/(cos(pitch/2));
	cy = /*(GLfloat)*/(cos(yaw/2));

	sr = /*(GLfloat)*/(sin(roll/2));
	sp = /*(GLfloat)*/(sin(pitch/2));
	sy = /*(GLfloat)*/(sin(yaw/2));
	
	cpcy = cp * cy;
	spsy = sp * sy;

	w = cr * cpcy + sr * spsy;
	x = sr * cpcy - cr * spsy;
	y = cr * sp * cy + sr * cp * sy;
	z = cr * cp * sy - sr * sp * cy;

}

void Quat::Multip(Quat q2)  
{
    GLdouble x1, y1, z1, w1;

	x1 =  x * q2.w + y * q2.z - z * q2.y + w * q2.x;
    y1 = -x * q2.z + y * q2.w + z * q2.x + w * q2.y;
    z1 =  x * q2.y - y * q2.x + z * q2.w + w * q2.z;
    w1 = -x * q2.x - y * q2.y - z * q2.z + w * q2.w;
	/*
	x1 =  q2.x * w + q2.y * z - q2.z * y + q2.w * x;
    y1 = -q2.x * z + q2.y * w + q2.z * x + q2.w * y;
    z1 =  q2.x * y - q2.y * x + q2.z * w + q2.w * z;
    w1 = -q2.x * x - q2.y * y - q2.z * z + q2.w * w;
*/
	x = x1;
	y = y1;
	z = z1;
	w = w1;
}

void Quat::GetLookDir(vreal lookDir[])
{
	vreal * mat1val;// = new vreal [16];
	mat1val = QuatToMat(); 
	int rowno1 = 4; int colno1 = 4;
	Matrix mat1 ( rowno1 , colno1 , mat1val );
	int rowno2 = 4; int colno2 = 1;
	vreal mat2val[4] = { 0.0 , 0.0 , 1.0 , 1.0 };
	Matrix mat2 ( rowno2 , colno2 , mat2val );
	Matrix mat3 = mat1 * mat2;
	vreal xval = mat3.GetElement ( 1 , 1 );
	vreal yval = mat3.GetElement ( 2 , 1 );
	vreal zval = mat3.GetElement ( 3 , 1 );
	//vreal tmp = mat3.GetElement ( 4 , 1 );
	lookDir[0]=xval;
	lookDir[1]=yval;
	lookDir[2]=zval;	
}

CoordinateSystem Quat::GetLookCoordinateSystem()
{
	CoordinateSystem res;
	vreal * mat1val;// = new vreal [16];
	mat1val = QuatToMat(); 
	Matrix temp(4,4,mat1val);
	TransformationMatrix tm(temp);
	//tm.TransposeThis();
	//TransformationMatrix inverse=tm.GetInverse();
	res.Transform(tm);
	return res;
}
