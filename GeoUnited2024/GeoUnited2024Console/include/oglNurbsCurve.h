// oglNurbsCurve.h

#ifndef __oglNurbsCurve_h__
#define __oglNurbsCurve_h__

#include <vector>

class BSplineCurve;

struct oglNurbsCurve
{
public:
	// temp
	oglNurbsCurve () {};
	oglNurbsCurve( const BSplineCurve &mycurve );
	~oglNurbsCurve();
	int uknotno;
	float * uknots;
	int ustride;
	float * ctlpoints;
	int uorder;
	void FillIntParameters(const BSplineCurve &mycurve);
	void FillKnots(const BSplineCurve &mycurve);
	void FillControlPoints(const BSplineCurve &mycurve);

private:
	int ucount;
};

typedef std::vector<oglNurbsCurve*> oglNurbsCurveVec;
typedef std::vector<oglNurbsCurve*> TrimLoop;
typedef std::vector<TrimLoop*> glAllTrimLoops;

#endif //__oglNurbsCurve_h__
