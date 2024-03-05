// PrsNurbsCurveConvert.h

#ifndef __PrsNurbsCurveConvert_h__
#define __PrsNurbsCurveConvert_h__

class PrsNurbsCurveConvert
{
public:
	// input: prs::NurbsCurve , prs::BSplineVertices , prs::KnotSet  , prs::KnotMult
	PrsNurbsCurveConvert (prs::NurbsCurve &crv
						,prs::BSplineVertices &vert
						,prs::KnotSet &knots
						,prs::KnotMult &knotmult);
private:

};

#endif