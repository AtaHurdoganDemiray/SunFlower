// LineSetToBSplineCurve.h

#ifndef __LineSetToBSplineCurve_h__
#define __LineSetToBSplineCurve_h__

class SequentialLineSet2d;
class SequentialLineSet;
class LineSet2d;
class LineSet;
class BSplineCurve;

class LineSetToBSplineCurve
{
public:
	static void Convert (const SequentialLineSet2d &lset , BSplineCurve &res);
	static void Convert (const SequentialLineSet &lset , BSplineCurve &res);
	static void Convert (const vreal &U0,const vreal &U1
						,const vreal &V0,const vreal &V1
						,const SequentialLineSet2d &lset , BSplineCurve &res);
	static void Convert (const LineSet2d &lset , BSplineCurve &res);
	static void Convert (const LineSet &lset , BSplineCurve &res);
};

#endif