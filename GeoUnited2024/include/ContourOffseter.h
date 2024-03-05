#ifndef __ContourOffseter_h__
#define __ContourOffseter_h__

#include "ContourCCWCheck.h"

class ContourOffseter
{
private:
	LineOrArcVecPtr m_originalGeoVec; // arcs are added at every non-tangent corner
	LineOrArcVecPtr m_originalExtendedGeoVec;
	vreal m_tol;
	vreal m_initialOffsetValue;
	LineOrArcVecPtr InitialOffset(vreal offsVal)const;
public:
	ContourOffseter(const std::vector<LineOrArc>* originalGeoVec, vreal initialOffsetValue, vreal tol); // adds arcs with 0 diameter on non-tangent meeting vertex and fills m_originalExtendedGeoVec
	const LineOrArcVecPtr GetOriginalExtendedGeoVec()const;
	LineOrArcVecPtr GetOriginalExtendedGeoVec();
	LineOrArcVecPtr Offset(vreal offsVal)const;
	const LineOrArcVecPtr GetOriginalGeoVec()const;
	LineOrArcVecPtr GetOriginalGeoVec();
};

#endif