// ToolPrim.h

#ifndef __ToolPrim_h__
#define __ToolPrim_h__
#include "sVector.h"

template <class T> class ToolPrimVisitor;

template <class T> class ToolPrim
{
public:
	enum PrimType {circle , line , verticalline};
	//virtual const PrimType GetType()const=0;
	virtual bool IsIncluding (const sVector<T,2> &p)const =0;	
	virtual T GetMaxRadius()const =0;
	virtual T GetStartH()const=0;
	virtual void AcceptVisitor (ToolPrimVisitor<T> &visitor)const=0;
	PrimType m_type;
	virtual T GetRadiusAt (T height)const = 0;
	virtual void GetMinMax (T &minx , T &maxx , T &miny , T &maxy)const = 0;
	virtual T GetHeightAtRadius (T rad)const = 0;
};

template <class T> class ToolLine;
template <class T> class ToolVerticalLine;
template <class T> class ToolCircle;

template<class T> class ToolPrimVisitor
{
public:
	virtual void Visit (const ToolPrim<T> &visitor)=0;
	virtual void Visit (const ToolLine<T> &visitor)=0;
	virtual void Visit (const ToolVerticalLine<T> &visitor)=0;
	virtual void Visit (const ToolCircle<T> &visitor)=0;
};

#endif