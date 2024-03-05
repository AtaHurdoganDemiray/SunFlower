// PeriodicOpenBSplineCurve.h

#ifndef __PeriodicOpenBSplineCurve_h__
#define __PeriodicOpenBSplineCurve_h__

class BSplineKnots;

struct VPoint;

class BSplineCurve;
class BSplineM;

class PeriodicOpenBSplineCurve
{
public:
	PeriodicOpenBSplineCurve ( const BSplineCurve & basecurve );		
	virtual ~PeriodicOpenBSplineCurve ();
	VPoint PeriodicOpenBSplineCurve::GetPoint ( const float & f_v ) const;
	const BSplineKnots & GetKnots () const;

private:
	const BSplineCurve & _basecurve;
	int degree;
	BSplineM * m;
	float delta_v;
	BSplineKnots * knots;
};

#endif //__PeriodicOpenBSplineCurve_h__
