// DigitisedCurve.cpp

#include "stdafx.h"
#include "DigitisedCurve.h"
#include "CurveDigitiserWithTol.h"
#include "DoubleVec.h"

DigitisedCurve::DigitisedCurve ( const vreal & tolerance , const Curve & curve )
:lines(50)
{
	CurveDigitiserWithTol dig ( curve );	
	DoubleVec curveparams;
	PointSet curvepoints(50);
	dig.DigitiseCurve ( tolerance , curvepoints , curveparams );
	for ( int i=0; i < curvepoints.Size(); i++ )
	{
		VPoint * currpoint = curvepoints[i];
		lines.InsertPoint ( *currpoint );
	}
}
