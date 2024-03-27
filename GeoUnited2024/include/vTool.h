// vTool.h

#ifndef __vTool_h__
#define __vTool_h__
#include "ToolPrim.h"

template <class T> class vTool
{
public:
	virtual ~vTool();
	vTool ();
	vTool (const vTool<T> &foo);
	vTool<T> & operator = (const vTool<T> &foo);
	void AddLine (const sVector<T,2> &lowerp , const sVector<T,2> &upperp);
	void AddVerticalLine (const sVector<T,2> &lowerp , const T &topy);
	void AddCircle (const T &rad , const sVector<T,2> &cen);
	bool IsIncluding (const sVector<T,2> &p)const;
	int Size()const;
	const ToolPrim<T> * operator[](int i)const;
	void Clear ();
private:
	std::vector<ToolPrim<T> *> m_prim;
};

#include "vTool.inl"
#endif