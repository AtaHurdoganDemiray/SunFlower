// DigitisedCurve.h
#ifndef __DigitisedCurve_h__
#define __DigitisedCurve_h__

#include "SequentialLineSet.h"

class Curve;

class DigitisedCurve
{
public:
	DigitisedCurve ( const vreal & tolerance , const Curve & curve );
	
	SequentialLineSet lines;

};

#endif