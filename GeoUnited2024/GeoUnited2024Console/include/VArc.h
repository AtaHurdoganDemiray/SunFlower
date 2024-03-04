// VArc.h

#ifndef __VArc_h__
#define __VArc_h__

#include "VPoint.h"
class TransformationMatrix;
struct CoordinateSystem;
class igesArc;
class igesTransformationMatrix;
class IgesEntitySet;
struct VLine;
class VSignedInterval;

//end angle is in degrees
//x axis of circle is startpoint-center
//end angle is relative to x axis
class VArc
{
public:
	friend class IgesEntityAdderGeo;
	VArc();
	VArc ( const VPoint & _center , const vreal & _radius , const VPoint & _ax , const VPoint & _startpoint , const vreal & _sweepangle );
	VArc ( const VArc & Foo );
	VArc & operator = ( const VArc & Foo );
	VArc ( const VPoint & p1 , const VPoint & p2 , const VPoint & p3 );
	VArc (const VPoint &_center , const vreal &_radius );
	// always CCW angled arc is formed from start point to end point
	VArc ( const VPoint & _center , const vreal & _radius , const VPoint & _ax , const VPoint & _startpoint , const VPoint & _endpoint );
	VArc (const VPoint &p1 , const VPoint &p2 , const VPoint &p3 , const VPoint &p4);
	VArc (const VPoint &p1, const VPoint &p2, const VPoint &unitArcAx, double bulge);
	VArc(const VPoint& _center , vreal _radius , const VPoint& tn0 , const VPoint& tn1);// for r = 0 case , ax is UZ
	~VArc();

	bool operator == ( const VArc & comp ) const;
	bool operator != ( const VArc & comp ) const;
	const vreal & GetRadius () const;
	const VPoint & GetStartPoint () const;	
	const vreal & GetSweepAngle () const;
	const VPoint & GetCenter () const;
	const VPoint & GetAx() const;

	bool CalcCircleFrom3Points ( const VPoint & p1 , const VPoint & p2 , const VPoint & p3 
								, VPoint & _center , VPoint & nor , vreal & _radius );
	void Serialize (Bofstream &outfile)const;
	void Serialize (Bifstream &infile);
	void Transform (const TransformationMatrix &tm);
	void MakeFullCircle();
	CoordinateSystem GetCoordinateSystem()const;
	bool IsFull()const;
	int MakeAndAddIgesArc (IgesEntitySet &entset);
	VPoint GetEndPoint ()const;
	double GetMinDist (const VPoint &p , VPoint &minponarc , double &angle)const;
	std::vector<VArc> Split (const double angle,const double lengthtreshold)const;
	VPoint ProjectPointOnArcPlane (const VPoint &p)const;
	std::vector<VPoint> GetInt (const VLine &l)const;
	std::vector<VPoint> GetInt (const VArc &a , vreal overlapTol)const;
	VArc GetReversed ()const;
	VPoint GetP (double angle)const;
	bool IsCCW ()const;
	bool IsAngleOnArc (double positiveAngle)const;
	double GetAngle (const VPoint &p)const;
	void ReverseAxis ();
	void ReplaceMinMax (VPoint &minpoint,VPoint &maxpoint , int dividecount)const;
	void ReplaceMinMaxInCs (const CoordinateSystem &cs , VPoint &minpoint,VPoint &maxpoint , int dividecount)const;
	double GetArcLength ()const;
	void GetIgesArc (igesArc &a , igesTransformationMatrix &tm , bool &isTransformed)const;
protected:
	vreal radius;
	VPoint center;
	//angle is in degrees
	vreal sweepangle;
	VPoint startpoint;
	VPoint ax;
	void MakeCircleFrom3Points (const VPoint &p1 , const VPoint &p2 , const VPoint &p3);
public:
	bool IsDegenerate (bool ckMinRadius
			, double minRadiusTolerance
			, bool ckCenterStartPointConformsRadius
			, double radiusConformanceTolerance
			, bool ckIsAxVectorUnit
			, double axVectorCkTolerance
			, bool ckIsAxPerpToStartVec
			, double axPerpToStartVecTolerance
			, bool ckSweepAngle
			, double sweepAngleTolerance
			, bool ckMinPerimeter
			, double minPerimeterAllowed 
			, bool ckMaxPerimeter
			, double maxPerimeterAllowed)const;
public:
	bool Compare (const VArc &a2,double tol)const;
	double GetBulge() const;
	std::vector<VPoint> GetIntForInfiniteLineAndCircleOfArc (const VLine &l)const;
	std::vector<VPoint> VArc::GetIntForFullArcs (const VArc &a , vreal overlapTol) const;
	bool IsOnArc(const VPoint &p, vreal tol) const;
	bool Split (int n, std::vector<VArc> &res)const;
	vreal GetChordWidthAtH (vreal h)const;// h is assumed to be between 0 and radius (0 means center chord width is diam, r means tip of circle chord width is 0)
	vreal GetChordalDistanceForChordWidth (vreal chordWidth)const; // chordal distance is distance between center of chord and circle tip
	vreal GetChordHForWidth (vreal chordWidth)const;
	vreal GetChordAngle (vreal chordWidth)const;
	void GetStartEndTangentUnitVectors (VPoint &uStartTangentVec , VPoint &uEndTangentVec)const;
	VPoint GetUnitStartTangentVec()const;
	VPoint GetUnitEndTangentVec()const;
	VPoint GetTangAt (const VPoint &pointOnArc)const;
	VPoint GetTan (vreal par)const;
	void SplitTo4Quadrants (std::vector<VSignedInterval> &fromStartToEnd , int quadi[4])const;// quadi[i] = -1 if no arc portion lies on that angle quadrant
	VArc GetArcOnXY()const;
	vreal GetArcStartAngleBetween0_360()const;
	vreal GetArcEndAngleBetween0_360()const;
	bool IsPointOnArc(const VPoint& p, vreal tol)const;
	static VArc MakeArcWith2PointsAndRadius(const VPoint& p1, const VPoint& p2, vreal radius , const VPoint& arcPlaneNormal);// negative radius allowed
	static bool VerifyCenterPointOn(const VPoint& c
		, const VPoint& p
		, const VVector& ax);
};

class OffsetHelperArc
{
private:
	VPoint m_corner,m_tn1,m_n1,m_n2,m_nMid;
public:
	OffsetHelperArc(const VPoint& tn1 , const VPoint& corner , const VPoint& tn2 , const VPoint& planeNormal); 
	VArc GetArc(vreal signedRadius)const;
};

#endif