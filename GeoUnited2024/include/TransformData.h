// TransformData.h

#ifndef __TransformData_h__
#define __TransformData_h__

#include "TransformationMatrix.h"
#include "OglMatrix.h"
class Bofstream;
class Bifstream;

struct TransformData
{
public:
	TransformData ();
	TransformData (const TransformationMatrix &tm);
	TransformationMatrix m_tm;
	OglMatrix m_om;
	bool isactive;
	void Serialize (Bofstream &f)const;
	void Serialize (Bifstream &f);
};

#endif