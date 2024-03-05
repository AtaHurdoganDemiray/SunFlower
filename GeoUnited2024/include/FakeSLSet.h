// FakeSLSet.h

#ifndef __FakeSLSet_h__
#define __FakeSLSet_h__
#include "VPoint.h"

class IgesEntity;
class TransformationMatrix;

class FakeSLSet
{
public:
	FakeSLSet (int i);
	int Size()const;
	int GetPointSize()const;
	const VPoint * Start()const;
	const VPoint * End()const;
	void AddBack (const FakeSLSet &toAdd);
	void Initialise (IgesEntity *ent,const TransformationMatrix &tm);
	void Reverse();
	bool IsReversed()const;
	std::list<const FakeSLSet *> & GetChild();
	IgesEntity * GetEntity()const;
private:
	mutable IgesEntity *m_ent;
	VPoint m_start;
	VPoint m_end;
	std::list <const FakeSLSet *>m_child;
	bool m_reversed;
};

#endif