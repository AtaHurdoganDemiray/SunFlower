// ProjectionMatrix.cpp

#include "stdafx.h"
#include "ProjectionMatrix.h"

ProjectionMatrix::ProjectionMatrix (int principalAxisNo , vreal planeOfset)
	:m_principalAxisNo(principalAxisNo) , m_planeOfset (planeOfset)
{
	vreal xofset (0.0);
	vreal yofset (0.0);
	vreal zofset (0.0);
	vreal ival (1.0);
	vreal jval (1.0);
	vreal kval (1.0);
	switch (principalAxisNo)
	{
	case 0:
		ival = 0.0;
		xofset = planeOfset;
		break;
	case 1:
		jval = 0.0;
		yofset = planeOfset;
		break;
	case 2:
		kval = 0.0;
		zofset = planeOfset;
		break;
	};
	Matrix::SetElement (ival,1,1);
	Matrix::SetElement (0.0,1,2);
	Matrix::SetElement (0.0,1,3);
	Matrix::SetElement (xofset,1,4);

	Matrix::SetElement (0.0,2,1);
	Matrix::SetElement (jval,2,2);
	Matrix::SetElement (0.0,2,3);
	Matrix::SetElement (yofset,2,4);

	Matrix::SetElement (0.0,3,1);
	Matrix::SetElement (0.0,3,2);
	Matrix::SetElement (kval,3,3);
	Matrix::SetElement (zofset,3,4);

	Matrix::SetElement (0.0,4,1);
	Matrix::SetElement (0.0,4,2);
	Matrix::SetElement (0.0,4,3);
	Matrix::SetElement (1.0,4,4);

}

int ProjectionMatrix::GetPrincipalAxisNo()const
{
	return m_principalAxisNo;
}

vreal ProjectionMatrix::GetPlaneOfset()const
{
	return m_planeOfset;
}

void ProjectionMatrix::Serialize (Bofstream &f)const
{
	int version = 1;
	f.WriteInt (version);
	TransformationMatrix::Serialize (f);
	f.WriteInt (m_principalAxisNo);
	f.WriteDouble (m_planeOfset);
}

void ProjectionMatrix::Serialize (Bifstream &f)
{
	int version;
	f.ReadInt (version);
	if (version >= 1)
	{
		TransformationMatrix::Serialize (f);
		f.ReadInt (m_principalAxisNo);
		f.ReadDouble (m_planeOfset);
	}
}