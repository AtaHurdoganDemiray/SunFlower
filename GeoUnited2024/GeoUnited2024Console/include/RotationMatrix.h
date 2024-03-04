// RotationMatrix.h

#ifndef __RotationMatrix_h__
#define __RotationMatrix_h__

#include "TransformationMatrix.h"
#include "VPoint.h"

class RotationMatrix : public TransformationMatrix
{
public:
	RotationMatrix();
	RotationMatrix (const VPoint &rotationcenter,const UnitVector &rotationaxis , const vreal &thetaInDegrees);
	RotationMatrix (const RotationMatrix &toCopy);
	RotationMatrix &operator=(const RotationMatrix &toCopy);
	virtual ~RotationMatrix();
	void Serialize (Bofstream &outfile)const;
	void Serialize (Bifstream &infile);
	bool operator == (const RotationMatrix &toCompare);
	bool operator != (const RotationMatrix &toCompare);
	virtual VStr GetInformationText()const;
protected:
	VPoint m_rotationcenter;
	UnitVector m_rotationaxis;
	vreal m_theta;
};

#endif //__RotationMatrix_h__
