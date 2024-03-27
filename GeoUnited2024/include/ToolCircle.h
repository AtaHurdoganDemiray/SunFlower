// ToolCircle.h

#ifndef __ToolCircle_h__
#define __ToolCircle_h__
#include "ToolPrim.h"

template <class T> class ToolCircle : public ToolPrim<T>
{
public:
	ToolCircle (const T &rad , const sVector<T,2> cen);
	virtual bool IsIncluding (const sVector<T,2> &p)const;
	virtual T GetMaxRadius()const;
	virtual T GetStartH()const;
	virtual void AcceptVisitor (ToolPrimVisitor<T> &visitor)const;
	const sVector<T,2> & GetCenter()const;
	const T & GetRadius()const;
	virtual T GetRadiusAt (T height)const;
	virtual void GetMinMax (T &minx , T &maxx , T &miny , T &maxy)const;
	virtual T GetHeightAtRadius (T rad)const;
private:
	T m_rad;
	sVector<T,2> m_cen;
	T m_rad2;
};

#include "ToolCircle.inl"

#endif