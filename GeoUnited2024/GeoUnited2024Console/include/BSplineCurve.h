// BSplineCurve.h
// Entity type 126
// K:upper index of sum (int)
// M:degree of basis functions (int)
// PROP1 (int) 0 = nonplanar 1 = planar
// PROP2 (int) 0 = opencurve 1 = closed
// PROP3 (int) 0 = rational 1 = polynomial
// PROP4 (int) 0 = non periodic 1 = periodic
// rem: N = 1 + K - M    A = N + 2 * M
// knots ( number of knots is A+1)
// weights ( number of weights is K+1 )
// controlpoints ( number of control points is (K+1) points (K+1)*3 vreals values )
//* last 5 values in the parameter data of entity
//12+A+4*K   V(0)        Real      Starting parameter value
//13+A+4*K   V(1)        Real      Ending parameter value
//14+A+4*K   XNORM       Real      Unit normal (if curve is planar)
//15+A+4*K   YNORM       Real
//16+A+4*K   ZNORM       Real

/*
Parameter Data
Index__      Name____     Type___   Description___
1            K            Integer   Upper index of sum. See Appendix B
2            M            Integer   Degree of basis functions
3            PROP1       Integer   0 = nonplanar, 1 = planar
4            PROP2       Integer   0 = open curve, 1 = closed curve
5            PROP3       Integer   0 = rational, 1 = polynomial
6            PROP4       Integer   0 = nonperiodic, 1 = periodic


Let N = 1+K-M and A = N+2*M


7            T(-M)       Real      First value of knot sequence
..           .            .
.            ..           ..
7+A         T(N+M)      Real      Last value of knot sequence
8+A         W(0)        Real      First weight
..           .            .
.            ..           ..
8+A+K       W(K)        Real      Last weight
9+A+K       X0           Real      First control point
10+A+K      Y0           Real
11+A+K      Z0           Real
..           .            .
.            ..           ..
9+A+4*K    XK           Real      Last control point
10+A+4*K   YK           Real
11+A+4*K   ZK           Real
12+A+4*K   V(0)        Real      Starting parameter value
13+A+4*K   V(1)        Real      Ending parameter value
14+A+4*K   XNORM       Real      Unit normal (if curve is planar)
15+A+4*K   YNORM       Real
16+A+4*K   ZNORM       Real

*/


#ifndef __BSplineCurve_h__
#define __BSplineCurve_h__


#include "IgesEntity.h"
#include "Test_IgesSerialize.h"
class SequentialLineSet;
class TransformationMatrix;

class BSplineCurve : public IgesEntity
{
public:
	BSplineCurve();
	BSplineCurve (int degreeOfBasisFunctions
					, int controlPointCount
					, bool isRational
					, bool isPeriodic
					, const std::vector<vreal> &knots
					, const std::vector<vreal> &weights
					, const std::vector<vreal> &controlp
					, const vreal &V0
					, const vreal &V1 );

	virtual ~BSplineCurve();
	virtual void GetEntityIn( FileLines & mylines , int poffs);
	virtual void WriteEntityOut (std::vector<VStr> &strvec)const;
	virtual void GetChildren (std::vector<int> &childVec)const;
	virtual void AcceptVisitor (IgesEntityVisitor &visitor)const;
	//virtual bool IsPrimitiveEntity()const;
	//virtual double GetSortNo ()const;
	virtual IgesEntity * MakeCopyAndAddToSet (const IgesEntitySet &mainSet , IgesEntitySet &newSet)const;

	bool IsClosed () const;
	bool IsPeriodic () const;
	bool IsPlanar () const;
	bool IsRational () const;
	int GetDegreeOfBasisFunctions () const;
	int GetControlPointNo () const;
//private:
	int m_K;
	int m_M;
	int m_PROP1;
	int m_PROP2;
	int m_PROP3;
	int m_PROP4;
	int m_N;
	int m_A;
	std::vector<vreal>  m_T;// knots;
	std::vector<vreal>  m_W;//weights;
	std::vector<vreal> controlpoints;
	//vreal * wcp; // weighted control points.
	vreal m_V0 , m_V1;
	vreal m_XNORM , m_YNORM , m_ZNORM;
	bool IsCircularArc()const;
	bool IsLine()const;
	bool IsPolyline()const;
	bool IsEllipticalArc()const;
	bool IsParabolicArc()const;
	bool IsHyperbolicArc()const;
	SequentialLineSet * MakeControlPointsSl ()const;
	void Transform(const TransformationMatrix &tm);
};

#endif // BSplineCurve