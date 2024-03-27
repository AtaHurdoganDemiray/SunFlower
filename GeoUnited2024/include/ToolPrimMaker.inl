// ToolPrimMaker.inl

#ifndef __ToolPrimMaker_inl__
#define __ToolPrimMaker_inl__

#include "ToolPrimMaker.h"

#include "ToolLine.h"
#include "ToolVerticalLine.h"
#include "ToolCircle.h"

template <class T>
ToolPrim<T> * ToolPrimMaker<T>::MakeCopy(const ToolPrim<T> &org)
{
	org.AcceptVisitor (*this);
	return m_p;
}

template <class T>
void ToolPrimMaker<T>::Visit (const ToolPrim<T> &visitor)
{
	throw VException ("ToolPrimMaker don't copy base type pointer",1);
}

template <class T>
void ToolPrimMaker<T>::Visit (const ToolLine<T> &visitor)
{
	m_p = new ToolLine<T> (visitor);
}

template <class T>
void ToolPrimMaker<T>::Visit (const ToolVerticalLine<T> &visitor)
{
	m_p=new ToolVerticalLine<T>(visitor);
}
template<class T>	
void ToolPrimMaker<T>::Visit (const ToolCircle<T> &visitor)
{
	m_p=new ToolCircle<T>(visitor);
}

#endif