// sPoints.cpp

#include "stdafx.h"
#include "sPoints.h"
#include "PointSet.h"

sPoints::sPoints (const PointSet &ps)
:m_ps (ps)
{

}
const VPoint & sPoints::operator [] (int i)const
{
	return * m_ps[i];
}
int sPoints::Size()const
{
	return m_ps.Size();
}
const VPoint sPoints::GetMin()const
{
	return m_ps.GetMin();
}
const VPoint sPoints::GetMax()const
{
	return m_ps.GetMax();
}
