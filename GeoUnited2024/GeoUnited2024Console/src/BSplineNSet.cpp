// BSplineNSet.cpp

#include "stdafx.h"
#include "BSplineNSet.h"
#include "BSplineN.h"

void BSplineNSet::AddItem ( const BSplineN & newitem )
{
	set.push_back ( newitem );
}

const BSplineN & BSplineNSet::operator [] ( int index ) const
{
	return (set[index]);
}

int BSplineNSet::Size () const
{
	return (set.size());
}



