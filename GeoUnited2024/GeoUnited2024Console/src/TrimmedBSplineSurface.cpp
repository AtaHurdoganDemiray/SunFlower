// TrimmedBSplineSurface.cpp

#include "stdafx.h"
#include "TrimmedBSplineSurface.h"
#include "BSplineSurface.h"
#include "BSplineCurve.h"

TrimmedBSplineSurface::TrimmedBSplineSurface( BSplineSurface* mybody)
{

}

TrimmedBSplineSurface::~TrimmedBSplineSurface()
{

}
/*
long TrimmedBSplineSurface::Size()
{
	return (trimcurves.size());
}

BSplineCurve * TrimmedBSplineSurface::operator [] ( long index )
{
	if ( index < 0 || index >= Size() )
		return NULL;

	return trimcurves[index];

}

void TrimmedBSplineSurface::operator + ( BSplineCurve * newtrimcurve )
{
	if ( newtrimcurve != NULL )
		trimcurves.push_back ( newtrimcurve );

}
*/
