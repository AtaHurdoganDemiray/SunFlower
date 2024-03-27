// ToolPrimMaker.h

#ifndef __ToolPrimMaker_h__
#define __ToolPrimMaker_h__

#include "ToolPrim.h"

template <class T> class ToolPrimMaker:public ToolPrimVisitor<T>
{
public:
	ToolPrim<T> * MakeCopy (const ToolPrim<T> &org);
	virtual void Visit (const ToolPrim<T> &visitor);
	virtual void Visit (const ToolLine<T> &visitor);
	virtual void Visit (const ToolVerticalLine<T> &visitor);
	virtual void Visit (const ToolCircle<T> &visitor);
	ToolPrim<T> *m_p;
};

#include "ToolPrimMaker.inl"
#endif

