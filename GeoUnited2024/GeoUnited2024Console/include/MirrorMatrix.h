// MirrorMatrix.h

#ifndef __MirrorMatrix_h__
#define __MirrorMatrix_h__

#include "TransformationMatrix.h"
#include "CoordinateSystem.h"

class MirrorMatrix : public TransformationMatrix
{
public:
	// mirror axis no: 0 X , 1 Y , 2 Z
	MirrorMatrix (const CoordinateSystem &cs , int mirrorAxisNo);
private:
	CoordinateSystem m_cs;
	int m_mirrorAxisNo;
public:
	const CoordinateSystem & GetCs()const;
	int GetMirrorAxisNo()const;
	virtual void Serialize (Bofstream &f)const;
	virtual void Serialize (Bifstream &f);

};

#endif