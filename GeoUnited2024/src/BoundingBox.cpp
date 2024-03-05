// BoundingBox.cpp

#include "stdafx.h"
#include "BoundingBox.h"


BoundingBox::BoundingBox ( const VPoint &min , const VPoint &max )
: m_min ( min ) , m_max ( max )
{

}

//#############################################################################

BoundingBox::BoundingBox ( const BoundingBox &Foo )
: m_min ( Foo.m_min ) , m_max ( Foo.m_max )
{

}

//#############################################################################

const BoundingBox & BoundingBox::operator = ( const BoundingBox &Foo )
{
	m_min = Foo.m_min;
	m_max = Foo.m_max;
	return (*this);
}

//#############################################################################

bool BoundingBox::operator == ( const BoundingBox &comp )const
{
	if ( m_min == comp.m_min && m_max == comp.m_max )
		return true;

	return false;
}

//#############################################################################

bool BoundingBox::operator != ( const BoundingBox &comp )const
{
	if ( m_min != comp.m_min && m_max != comp.m_max )
		return true;

	return false;
}

//#############################################################################

const VPoint& BoundingBox::GetMin()const
{
	return m_min;
}

//#############################################################################

const VPoint& BoundingBox::GetMax()const
{
	return m_max;
}

//#############################################################################
