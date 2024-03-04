// ParametricSpline.h

#ifndef __ParametricSpline_h__
#define __ParametricSpline_h__

// Entitytype : 112
/*Parameter Data

Index__      Name____     Type___   Description___
1            CTYPE       Integer   Spline Type:
                                       1=Linear
                                       2=Quadratic
                                       3=Cubic
                                       4=Wilson-Fowler
                                       5=Modified Wilson-Fowler
                                       6=B Spline
2            H            Integer   Degree of continuity with respect to arc length
3            NDIM        Integer   Number of dimensions:
                                       2=planar
                                       3=nonplanar
4            N            Integer   Number of segments
5            T(1)        Real      First break point of piecewise polynomial
..           .            .
.            ..           ..
5+N         T(N+1)      Real      Last break point of piecewise polynomial
6+N         AX(1)       Real      X coordinate polynomial
7+N         BX(1)       Real
8+N         CX(1)       Real
9+N         DX(1)       Real
10+N        AY(1)       Real      Y coordinate polynomial
11+N        BY(1)       Real
12+N        CY(1)       Real
13+N        DY(1)       Real
14+N        AZ(1)       Real      Z coordinate polynomial
15+N        BZ(1)       Real
16+N        CZ(1)       Real
17+N        DZ(1)       Real
..           .            .
.            ..           ..        Subsequent X, Y, Z polynomials concluding with the twelve
                                    coefficients of the Nth polynomial segment.


The parameters that follow comprise the evaluations of the polynomials of the N -th segment and
their derivatives at the parameter value u = T (N + 1) corresponding to the terminate point. Sub-
sequently, these evaluations are divided by appropriate factorials.
6+13*N      TPX0        Real      X value
7+13*N      TPX1        Real      X first derivative
8+13*N      TPX2        Real      X second derivative/2!
9+13*N      TPX3        Real      X third derivative/3!
10+13*N    TPY0        Real      Y value
11+13*N    TPY1        Real      Y first derivative
12+13*N    TPY2        Real      Y second derivative/2!
13+13*N    TPY3        Real      Y third derivative/3!
14+13*N    TPZ0        Real      Z value
15+13*N    TPZ1        Real      Z first derivative
16+13*N    TPZ2        Real      Z second derivative/2!
17+13*N    TPZ3        Real      Z third derivative/3!*/                                               
#include "IgesEntity.h"

class ParametricSpline:public IgesEntity
{
public:
	ParametricSpline();
	virtual void GetEntityIn( FileLines & mylines , int poffs);
	virtual void WriteEntityOut (std::vector <VStr> &strvec)const;
	virtual void GetChildren (std::vector<int> &childVec)const;
	virtual void AcceptVisitor (IgesEntityVisitor &visitor)const;
	//virtual bool IsPrimitiveEntity()const;
	//virtual double GetSortNo ()const;
	virtual IgesEntity * MakeCopyAndAddToSet (const IgesEntitySet &mainSet , IgesEntitySet &newSet)const;

	int m_CTYPE;
	int m_H;
	int m_NDIM;
	int m_N;

	std::vector<vreal> m_T;
	
	std::vector<vreal> m_AX;
	std::vector<vreal> m_BX;
	std::vector<vreal> m_CX;
	std::vector<vreal> m_DX;

	std::vector<vreal> m_AY;
	std::vector<vreal> m_BY;
	std::vector<vreal> m_CY;
	std::vector<vreal> m_DY;

	std::vector<vreal> m_AZ;
	std::vector<vreal> m_BZ;
	std::vector<vreal> m_CZ;
	std::vector<vreal> m_DZ;

	vreal m_TPX0 , m_TPX1 , m_TPX2 , m_TPX3;
	vreal m_TPY0 , m_TPY1 , m_TPY2 , m_TPY3;
	vreal m_TPZ0 , m_TPZ1 , m_TPZ2 , m_TPZ3;
};

#endif