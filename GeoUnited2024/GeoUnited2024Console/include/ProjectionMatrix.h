// ProjectionMatrix.h

#ifndef __ProjectionMatrix_h__
#define __ProjectionMatrix_h__

#include "TransformationMatrix.h"

class ProjectionMatrix : public TransformationMatrix
{
public:
	ProjectionMatrix (int principalAxisNo,vreal planeOfset); // Principal axis no: 0 project along X axis , 1 project along ,2project along Z axis

protected:
	int m_principalAxisNo;
	vreal m_planeOfset;
public:
	int GetPrincipalAxisNo()const;
	vreal GetPlaneOfset()const;
	virtual void Serialize (Bofstream &f)const;
	virtual void Serialize (Bifstream &f);

};

#endif