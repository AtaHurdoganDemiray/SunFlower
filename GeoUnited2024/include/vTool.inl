// vTool.inl

#ifndef __vTool_inl__
#define __vTool_inl__

#include "vTool.h"
#include "ToolLine.h"
#include "ToolVerticalLine.h"
#include "ToolCircle.h"
#include "ToolPrimMaker.h"

template <class T>
vTool<T>::vTool ()
{

}

template<class T>
vTool<T>::vTool (const vTool<T> &foo)
{
	ToolPrimMaker<T> maker;
	std::vector<ToolPrim<T> *>::const_iterator i = foo.m_prim.begin();
	std::vector<ToolPrim<T> *>::const_iterator e = foo.m_prim.end();
	for (i;i!=e;++i)
	{
		ToolPrim<T> *newpr = maker.MakeCopy (*(*i));
		m_prim.push_back (newpr);
	}
}

template <class T>
vTool<T> & vTool<T>::operator = (const vTool<T> &foo)
{
	ToolPrimMaker<T> maker;
	std::vector<ToolPrim<T> *>::const_iterator i = foo.m_prim.begin();
	std::vector<ToolPrim<T> *>::const_iterator e = foo.m_prim.end();
	for (i;i!=e;++i)
	{
		ToolPrim<T> *newpr = maker.MakeCopy  (*(*i));
		m_prim.push_back (newpr);
	}
	return *this;
}

template<class T> 
vTool<T>::~vTool()
{
	std::vector <ToolPrim<T> *>::iterator i = m_prim.begin();
	std::vector <ToolPrim<T> *>::iterator e = m_prim.end();
	for (i;i!=e;i++)
	{
		delete (*i);
	}
};

template <class T>
void vTool<T>::AddCircle(const T &rad, const sVector<T,2> &cen)
{
	ToolCircle<T> *newprim = new ToolCircle<T> (rad,cen);
	m_prim.push_back (newprim);
};

template <class T>
void vTool<T>::AddLine(const sVector<T,2> &lowerp, const sVector<T,2> &upperp)
{
	ToolLine<T> *newprim = new ToolLine<T> (lowerp,upperp);
	m_prim.push_back(newprim);
};

template <class T>
void vTool<T>::AddVerticalLine (const sVector<T,2> &lowerp , const T &topy)
{
	ToolVerticalLine<T> *newprim = new ToolVerticalLine <T> (lowerp , topy);
	m_prim.push_back (newprim);
};

template <class T>
bool vTool<T>::IsIncluding(const sVector<T,2> &p) const
{
	std::vector <ToolPrim<T> *>::const_iterator i = m_prim.begin();
	std::vector <ToolPrim<T> *>::const_iterator e = m_prim.end();
	for (i;i!=e;i++)
	{
		bool ck = (*i)->IsIncluding(p);
		if (!ck)
			return false;
	}
	return true;
};

template<class T>
int vTool<T>::Size()const
{
	return (int)(m_prim.size());
}

template <class T>
const ToolPrim<T> * vTool<T>::operator[](int i)const
{
	return m_prim[i];
}

template <class T>
void vTool<T>::Clear()
{
	m_prim.clear();
}

#endif