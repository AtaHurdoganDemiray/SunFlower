// BSplineSurface.h

// K1: (int) upper index of first sum
// K2: (int) upper index of second sum
// M1: (int) degree of first set of basis functions.
// M2: (int) degree of second set of basis functions.
// PROP1: (int) 1 = closed , 0 = not closed in first parametric variable direction.
// PROP2: (int) 1 = closed , 0 = not closed in second parametric variable direction.
// PROP3: (int) 0 = rational , 1 = polynomial
// PROP4: (int) 0 = nonperiodic 1 = periodic in first parametric variable direction.
// PROP5: (int) 0 = nonperiodic 1 = periodic in second parametric variable direction.

// N1 = 1 + K1 - M1
// N2 = 2 + K2 - M2
// A = N1 + 2 * M1
// B = N2 + 2 * M2
// C = ( 1 + K1 )*( 1 + K2 )
// S :knot values for first set ( its count = A+1 )
// T :knot values for second set ( its count = B+1 )
// W :weights for the control points ( its count = C )
// controlpoints : X Y Z  ( its count = C ) ( 3 * C vreal values )

// last 4 values in the parameter data of entity
// 12+A+B+4*C  U(0)       Real      Starting value for first parametric direction
// 13+A+B+4*C  U(1)       Real      Ending value for first parametric direction
// 14+A+B+4*C  V(0)       Real      Starting value for second parametric direction
// 15+A+B+4*C  V(1)       Real      Ending value for second parametric direction

/*
Parameter Data
Index__      Name____     Type___   Description___
1             K1         Integer   Upper index of first sum. See Appendix B
2             K2         Integer   Upper index of second sum. See Appendix B
3             M1         Integer   Degree of first set of basis functions
4             M2         Integer   Degree of second set of basis functions
5             PROP1      Integer   1 = Closed in first parametric variable direction
                                    0 = Not closed
6             PROP2      Integer   1 = Closed in second parametric variable direction
                                    0 = Not closed
7             PROP3      Integer   0 = Rational
                                    1 = Polynomial
8             PROP4      Integer   0 = Nonperiodic in first parametric variable direction
                                    1 = Periodic in first parametric variable direction
9             PROP5      Integer   0 = Nonperiodic in second parametric variable direction
                                    1 = Periodic in second parametric variable direction


  Let   N1    =   1+K1-M1,
        N2    =   1+K2-M2,
        A     =   N1+2*M1,
        B     =   N2+2*M2,
        C     =   (1+K1)*(1+K2)
10            S(-M1)    Real      First value of first knot sequence
..            .           .
.             ..          ..
10+A         S(N1+M1)  Real      Last value of first knot sequence
11+A         T(-M2)    Real      First value of second knot sequence
..            .           .
.             ..          ..
11+A+B       T(N2+M2)  Real      Last value of second knot sequence
12+A+B       W(0,0)    Real      First Weight
13+A+B       W(1,0)    Real
..            .           .
.             ..          ..
11+A+B+C    W(K1,K2)  Real      Last Weight
12+A+B+C    X(0,0)    Real      First Control Point
13+A+B+C    Y(0,0)    Real
14+A+B+C    Z(0,0)    Real
15+A+B+C    X(1,0)    Real
16+A+B+C    Y(1,0)    Real
17+A+B+C    Z(1,0)    Real
..            .           .
.             ..          ..
 9+A+B+4*C  X(K1,K2)  Real      Last Control Point
10+A+B+4*C  Y(K1,K2)  Real
11+A+B+4*C  Z(K1,K2)  Real
12+A+B+4*C  U(0)       Real      Starting value for first parametric direction
13+A+B+4*C  U(1)       Real      Ending value for first parametric direction
14+A+B+4*C  V(0)       Real      Starting value for second parametric direction
15+A+B+4*C  V(1)       Real      Ending value for second parametric direction

*/

#ifndef __BSplineSurface_h__
#define __BSplineSurface_h__

#include "IgesEntity.h"

class BSplineSurface : public IgesEntity
{
public:
	BSplineSurface();
	~BSplineSurface();
	virtual void GetEntityIn( FileLines & mylines , int poffs);
	void WriteEntityOut (std::vector<VStr> &strvec)const;
	virtual void GetChildren (std::vector<int> &childVec)const;
	virtual void AcceptVisitor (IgesEntityVisitor &visitor)const;
	//virtual bool IsPrimitiveEntity()const;
	//virtual double GetSortNo ()const;
	virtual IgesEntity * MakeCopyAndAddToSet (const IgesEntitySet &mainSet , IgesEntitySet &newSet)const;

	bool IsClosedInU () const;
	bool IsClosedInV () const;
	bool IsPeriodicInU () const;
	bool IsPeriodicInV () const;
	bool IsRational () const;
	int GetDegreeOfBasisFunctionsU ()const;
	int GetDegreeOfBasisFunctionsV ()const;
	int GetControlPointNoU ()const;
	int GetControlPointNoV ()const;
//private:
	int m_K1;
	int m_K2;
	int m_M1;
	int m_M2;
	int m_PROP1;
	int m_PROP2;
	int m_PROP3;
	int m_PROP4;
	int m_PROP5;

	int m_N1;
	int m_N2;
	int m_A;
	int m_B;
	int m_C;
	int CalculateA (const int , const int)const;
	int CalculateB (const int , const int)const;
	int CalculateN1 (const int ,const int)const;
	int CalculateN2 (const int ,const int)const;
	int CalculateC (const int K1,const int K2)const;
	std::vector<vreal> m_S;// first knot sequence
	std::vector<vreal> m_T;// second knot sequence

	std::vector<vreal> m_W;// weights

	std::vector<vreal> controlpoints;

	//vreal *wcp; // weighed control points.

	vreal m_U0 , m_U1;// start and end values for first parametric direction
	vreal m_V0 , m_V1;// start and end values for second parametric direction

	void ConstructAsCubicUniformPolynomial ( int upointno , int vpointno
											, const std::vector<vreal> &uval
											, const std::vector<vreal> &knotsu
											, const std::vector<vreal> &vval
											, const std::vector<vreal> &knotsv
											, const std::vector<vreal> &controlPoints 
											, bool isClosedU 
											, bool isClosedV );
		
};

#endif // BSplineSurface