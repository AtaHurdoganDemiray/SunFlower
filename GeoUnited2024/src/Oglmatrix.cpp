// OglMatrix.cpp

#include "stdafx.h"
#include "OglMatrix.h"
#include "TransformationMatrix.h"

OglMatrix::OglMatrix()
{
	xmat=new vreal[16];
	xmat[0] = 1.;
	xmat[1] = 0.;
	xmat[2] = 0.;
	xmat[3] = 0.;

	xmat[4] = 0.;
	xmat[5] = 1.;
	xmat[6] = 0.;
	xmat[7] = 0.;

	xmat[8] = 0.;
	xmat[9] = 0.;
	xmat[10] = 1.;
	xmat[11] = 0.;

	xmat[12] = 0.;
	xmat[13] = 0.;
	xmat[14] = 0.;
	xmat[15] = 1.;
}

OglMatrix::OglMatrix(const TransformationMatrix &tm)
{
	xmat=new vreal [16];
	const vreal * p = tm.GetMatrixArr();
	xmat[0] = p[0];
	xmat[1] = p[4];
	xmat[2] = p[8];
	xmat[3] = p[12];

	xmat[4] = p[1];
	xmat[5] = p[5];
	xmat[6] = p[9];
	xmat[7] = p[13];

	xmat[8] = p[2];
	xmat[9] = p[6];
	xmat[10] = p[10];
	xmat[11] = p[14];

	xmat[12] = p[3];
	xmat[13] = p[7];
	xmat[14] = p[11];
	xmat[15] = p[15];
}

OglMatrix::~OglMatrix()
{
	delete []xmat;
}

const vreal * OglMatrix::GetArray()const
{
	return xmat;
}

OglMatrix & OglMatrix::operator = (const OglMatrix &toCopy)
{
	for(int i=0;i<16;i++)
		xmat[i]=toCopy.xmat[i];
	return *this;
}

OglMatrix::OglMatrix(const OglMatrix &toCopy)
{
	xmat=new vreal[16];
	for(int i=0;i<16;i++)
		xmat[i]=toCopy.xmat[i];
}
