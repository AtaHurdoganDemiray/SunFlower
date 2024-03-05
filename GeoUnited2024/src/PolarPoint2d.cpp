/******************************************************************************
*               File: PolarPoint2d.cpp											  *
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
#include "PolarPoint2d.h"
#include "PolarCalc2d.h"
#include "VPoint2d.h"
#include "BasicDefinitions.h"

PolarPoint2d::PolarPoint2d ( const vreal &newr , const vreal &newq )
{
	pr = newr;
	pq = newq;
}

//#############################################################################

PolarPoint2d::PolarPoint2d ( const VPoint2d &cartesian )
{
	PolarCalc2d mycalc;

	pr = sqrt ( cartesian.x() * cartesian.x() + cartesian.y() * cartesian.y() );
	pq = mycalc.GetPolarAngle ( cartesian.x() , cartesian.y() );

}

//#############################################################################

PolarPoint2d::~PolarPoint2d()
{

}

//#############################################################################

void PolarPoint2d::operator = ( const PolarPoint2d &Foo )
{
	pr = Foo.pr;
	pq = Foo.pq;
}

//#############################################################################

bool PolarPoint2d::operator == ( const PolarPoint2d &comp ) const
{
	if ( pq == comp.pq && pr == comp.pr )
		return true;

	return false;

}

//#############################################################################

vreal PolarPoint2d::q() const
{
	return pq;
}

//#############################################################################

vreal PolarPoint2d::Q() const
{
	return pq;
}

//#############################################################################

vreal PolarPoint2d::r() const
{
	return pr;
}

//#############################################################################

vreal PolarPoint2d::R() const 
{
	return pr;
}

//#############################################################################

void PolarPoint2d::q( const vreal &newq )
{
	pq = newq;
}

//#############################################################################

void PolarPoint2d::Q( const vreal &newq )
{
	pq = newq;
}

//#############################################################################

void PolarPoint2d::r( const vreal &newr )
{
	pr = newr;
}

//#############################################################################

void PolarPoint2d::R( const vreal &newr )
{
	pr = newr;
}

//#############################################################################




