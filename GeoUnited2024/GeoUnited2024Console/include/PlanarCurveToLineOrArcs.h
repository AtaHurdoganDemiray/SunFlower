// PlanarCurveToLineOrArcs.h

#ifndef __PlanarCurveToLineOrArcs_h__
#define __PlanarCurveToLineOrArcs_h__

#include "LineOrArc.h"
class VCurve;

class PlanarCurveToLineOrArcs
{
public:
	static bool Convert (VCurve *crv , double tol , std::vector<LineOrArc> &afit);
};

#endif