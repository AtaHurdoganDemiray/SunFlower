// PolarCalc2d.h
#ifndef __PolarCalc2d_h__
#define __PolarCalc2d_h__


struct VPoint;

struct VPoint2d;
class VInterval;

class PolarCalc2d
{
public:
	enum SideFactor { CCW , CW };
	enum BetweenAnglesInfo { OUTSIDE , BETWEEN , ONSTART , ONEND };
	static vreal GetPolarAngle ( const VPoint2d &originp , const VPoint2d &secondp );
	static vreal GetPolarDistance ( const VPoint2d &originp , const VPoint2d &secondp );
	static vreal GetPolarDistance2 ( const VPoint2d &originp , const VPoint2d &secondp );
	static vreal GetCCWAngle ( const vreal &startpolarangle , const vreal &endpolarangle );
	static vreal GetCWAngle ( const vreal &startpolarangle , const vreal &endpolarangle );
	static vreal GetPolarAngle ( const vreal &xval , const vreal &yval );
	static vreal Inverse ( const vreal &polarangle );
	static BetweenAnglesInfo IsAngleBetween2Angles ( const vreal &startangle , const vreal &endangle , const vreal &checkangle );
	static double GetAbsShortestAngleBtw (double angle1 , double angle2);
	static void CastAngleTo360presc6 (double &angle);
	static int GetQuadrantFor (vreal angle);
	static VInterval GetQuadrantAngles (int qno);
	static void UnwrapArcToLineInterval (vreal positiveStartAngle , vreal sweepAngle , std::list <VSignedInterval> &unwrappedArcs);
};

double vp_asin (double val);
double vp_acos (double val);

#endif //__PolarCalc2d_h__