// Face.h

#ifndef __Face_h__
#define __Face_h__

#include "IgesEntity.h"

class Face : public IgesEntity
{
public:
	Face();

	virtual void GetEntityIn( FileLines & mylines , int poffs);
	virtual void WriteEntityOut (std::vector <VStr> &strvec)const;
	virtual void GetChildren (std::vector<int> &childVec)const;
	virtual void AcceptVisitor (IgesEntityVisitor &visitor)const;
	virtual IgesEntity * MakeCopyAndAddToSet (const IgesEntitySet &mainSet , IgesEntitySet &newSet)const;

	int m_SURF;		// Pointer to the DE of the underlying surface
	int m_N;		// Number of loops (N>0)
	int m_OF;		// Outer loop flag (True implies that Loop[0] is outer loop,false implies that there is no outer loop
	std::vector<int> m_LOOP;	// Pointer to the DE of the loop(s)
};

#endif