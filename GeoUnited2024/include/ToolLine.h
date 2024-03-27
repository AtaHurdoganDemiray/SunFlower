// ToolLine.h

#ifndef __ToolLine_h__
#define __ToolLine_h__
#include "ToolPrim.h"
#include "sVector.h"
#include "LineCast.h"

template <class T> class ToolLine: public ToolPrim<T>
{
public:
	ToolLine (const sVector<T,2> &lowerp,const sVector<T,2> &upperp);
	//virtual const ToolPrim<T>::PrimType GetType()const;
	virtual bool IsIncluding (const sVector<T,2> &p)const;
	virtual T GetMaxRadius()const;
	virtual T GetStartH()const;
	virtual void AcceptVisitor (ToolPrimVisitor<T> &visitor)const;
	const sVector<T,2> & GetLowerPoint()const;
	const sVector<T,2> & GetUpperPoint()const;
	const T GetHeight()const;
	bool IsColl (const LineCast<T> &l)const;
	virtual T GetRadiusAt(T height)const;
	virtual void GetMinMax (T &minx , T &maxx , T &miny , T &maxy)const;
	virtual T GetHeightAtRadius (T rad)const;
private:
	sVector<T,2> m_lowerp;
	sVector<T,2> m_upperp;
	sVector<T,2> m_n;
	T m_c;

	T m_kt , m_ct , m_kt2 ,m_ct2 , m_2ktct;
};

#include "ToolLine.inl"

#endif