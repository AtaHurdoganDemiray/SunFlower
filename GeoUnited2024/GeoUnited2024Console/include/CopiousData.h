// CopiousData.h

// entityType = 106
//Index__      Name____     Type___   Description___
//1            IP           Integer   Interpretation Flag
//                                      1 = x,y pairs, common z
//                                      2 = x,y,z coordinates
//                                      3 = x,y,z coordinates and i,j,k vectors
//2            N            Integer   Number of n-tuples
//
//
//For IP=1 (x,y pairs, common z), i.e., for Form 1:
//
//
//3            ZT           Real      Common z displacement
//4            X1           Real      First data point abscissa
//5            Y1           Real      First data point ordinate
//..           .            .
//.            ..           ..
//3+2*N       YN           Real      Last data point ordinate
//
//
//For IP=2 (x,y,z triples), i.e., for Form 2:
//
//
//3            X1           Real      First data point x value
//4            Y1           Real      First data point y value
//5            Z1           Real      First data point z value
//..           .            .
//.            ..           ..
//2+3*N       ZN           Real      Last data point z value
//
//
//For IP=3 (x,y,z,i,j,k sextuples), i.e., for Form 3:
//
//
//3            X1           Real      First data point x value
//4            Y1           Real      First data point y value
//5            Z1           Real      First data point z value
//6            I1           Real      First data point i value
//7            J1           Real      First data point j value
//8            K1           Real      First data point k value
//..           .            .
//.            ..           ..
//2+6*N       KN           Real      Last data point k value

#ifndef __CopiousData_h__
#define __CopiousData_h__

#include "IgesEntity.h"

class CopiousData : public IgesEntity
{
public:
	CopiousData();
	void GetEntityIn( FileLines & mylines , int poffs);
	void WriteEntityOut (std::vector <VStr> &strvec)const;
	virtual void GetChildren (std::vector<int> &childVec)const;
	virtual void AcceptVisitor (IgesEntityVisitor &visitor)const;
	//virtual bool IsPrimitiveEntity()const;
	//virtual double GetSortNo ()const;
	virtual IgesEntity * MakeCopyAndAddToSet (const IgesEntitySet &mainSet , IgesEntitySet &newSet)const;

	int m_IP;
	int m_N;
    std::vector<std::vector<vreal> > tuples;
	vreal m_zoffset2d;
};

#endif
