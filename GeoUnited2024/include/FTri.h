// FTri.h

#ifndef __FTri_h__
#define __FTri_h__

#include "sVector.h"
#include "VTriangleE.h"

class FTri
{
public:
	FTri ();
	FTri (const VTriangleE &tri);
	VTriangleE  GetVTri ()const;
	void Transform (const TransformationMatrix &tm);
	void Serialize (Bofstream &f)const;
	void Serialize (Bifstream &f);
	sVector<float,3> v1;
	sVector<float,3> v2;
	sVector<float,3> v3;
};

#endif