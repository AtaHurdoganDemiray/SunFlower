/******************************************************************************
*               File: RoughPolarP2d.cpp											  *
*******************************************************************************
*               Description:                                                  *
*                                                                             *
*******************************************************************************
*               History:                                                      *
*  14.02.2004 21:09:10 Created by:    Ata Hurdogan Demiray                    *
*******************************************************************************
*               (C) 2003 by Positive GmbH		                               *
******************************************************************************/

#include "stdafx.h"
#include "RoughPolarP2d.h"
#include "PolarCalc2d.h"
#include "VPoint2d.h"
#include "BasicDefinitions.h"

RoughPolarP2d::RoughPolarP2d ( const vreal &newr2 , const vreal &newq )
{
	pr2 = newr2;
	pq = newq;
}

//#############################################################################

RoughPolarP2d::RoughPolarP2d ( const VPoint2d &cartesian )
{
	PolarCalc2d mycalc;
	pr2 = cartesian.x() * cartesian.x() + cartesian.y() * cartesian.y();
	pq = mycalc.GetPolarAngle ( cartesian.x() , cartesian.y() );
}

//#############################################################################

RoughPolarP2d::~RoughPolarP2d()
{

}

//#############################################################################

void RoughPolarP2d::operator = ( const RoughPolarP2d &Foo )
{
	pr2 = Foo.pr2;
	pq = Foo.pq;

}

//#############################################################################

bool RoughPolarP2d::operator == ( const RoughPolarP2d &comp ) const
{
	if ( pq == comp.pq && pr2 == comp.pr2 )
		return true;

	return false;

}

//#############################################################################

vreal RoughPolarP2d::q() const
{
	return pq;
}

//#############################################################################

vreal RoughPolarP2d::Q() const
{
	return pq;
}

//#############################################################################

vreal RoughPolarP2d::r2() const
{
	return pr2;
}

//#############################################################################

vreal RoughPolarP2d::R2() const 
{
	return pr2;
}

//#############################################################################

void RoughPolarP2d::q( const vreal &newq )
{
	pq = newq;
}

//#############################################################################

void RoughPolarP2d::Q( const vreal &newq )
{
	pq = newq;
}

//#############################################################################

void RoughPolarP2d::r2( const vreal &newr2 )
{
	pr2 = newr2;
}

//#############################################################################

void RoughPolarP2d::R2( const vreal &newr2 )
{
	pr2 = newr2;
}

//#############################################################################




