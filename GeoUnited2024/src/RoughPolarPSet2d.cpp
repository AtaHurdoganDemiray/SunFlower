// RoughPolarPSet2d.cpp

#include "stdafx.h"
#include "RoughPolarPSet2d.h"

RoughPolarPSet2d::~RoughPolarPSet2d()
{
	Clear();
}

long RoughPolarPSet2d::Size () const
{
	return ( points.size() );
}

RoughPolarP2d * RoughPolarPSet2d::operator [] ( const int &index ) const
{
	if ( index < 0 || index >= Size() )
		return NULL;

	return ( points[index] );
}

void RoughPolarPSet2d::Clear()
{
	points.clear();
}

void RoughPolarPSet2d::Destruct()
{
	for ( long i = 0; i < Size(); i++ )
	{
		delete points[i];
	}
}

void RoughPolarPSet2d::InsertPoint ( const vreal &polar_r2 , const vreal &polar_q )
{
	RoughPolarP2d * p_polar = new RoughPolarP2d ( polar_r2 , polar_q );
	points.push_back ( p_polar );
}

void RoughPolarPSet2d::InsertPoint ( const RoughPolarP2d &newpoint )
{
	RoughPolarP2d * p_polar = new RoughPolarP2d (newpoint);
	points.push_back ( p_polar );
}

