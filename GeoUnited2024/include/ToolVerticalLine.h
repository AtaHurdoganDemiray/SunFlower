// ToolVerticalLine.h

#ifndef __ToolVerticalLine_h__
#define __ToolVerticalLine_h__
#include "ToolPrim.h"
#include "LineCast.h"

template <class T> class ToolVerticalLine: public ToolPrim<T>
{
public:
	ToolVerticalLine (const sVector<T,2> &lowerp , const T &ytop);
	virtual bool IsIncluding (const sVector<T,2> &p)const;
	virtual T GetMaxRadius()const;
	virtual T GetStartH()const;
	virtual void AcceptVisitor (ToolPrimVisitor<T> &visitor)const;
	const sVector<T,2> & GetLowerPoint()const;
	const T  GetHeight ()const;
	bool IsColl (const LineCast<T> &l)const;
	virtual T GetRadiusAt (T height)const;
	virtual void GetMinMax (T &minx , T &maxx , T &miny , T &maxy)const;
	virtual T GetHeightAtRadius (T rad)const;
private:
	sVector <T,2> m_lowerp;
	T m_ytop;
	T m_r2;
};
#include "ToolVerticalLine.inl"
#endif