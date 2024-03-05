// oglNurbsCurve.cpp

#include "stdafx.h"
#include "oglNurbsCurve.h"
#include "BSplineCurve.h"

oglNurbsCurve::oglNurbsCurve( const BSplineCurve &mycurve )
{
	FillIntParameters (mycurve);
	FillKnots(mycurve);
	FillControlPoints(mycurve);
}

oglNurbsCurve::~oglNurbsCurve ()
{
	delete [] uknots;
	delete [] ctlpoints;
}

void oglNurbsCurve::FillIntParameters ( const BSplineCurve &mycurve )
{
	uknotno = mycurve.m_A + 1;
	ucount = mycurve.m_K + 1;
	uorder = (mycurve.m_A - mycurve.m_K);
	ustride = 2;
}

void oglNurbsCurve::FillKnots ( const BSplineCurve &mycurve )
{
	uknots = new float[uknotno];
	
	for ( int i = 0; i < uknotno; i++ )
	{
		uknots[i] = (mycurve.m_T[i]);
	}
}

void oglNurbsCurve::FillControlPoints ( const BSplineCurve &mycurve )
{
	ctlpoints = new float[2*ucount];

	for ( int i = 0; i < ucount; i++ )
	{
		int indexs = i * 3;
		ctlpoints[i*2] = (mycurve.controlpoints[indexs]);//mycurve.weights[indexs]);
		ctlpoints[i*2+1] = (mycurve.controlpoints[indexs+1]);//*mycurve.weights[indexs]);
	}

//	ctlpoints = &(mycurve.controlpoints[0]);

}


