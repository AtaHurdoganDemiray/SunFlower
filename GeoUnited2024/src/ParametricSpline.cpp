// ParametricSpline.cpp
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
#include "stdafx.h"
#include "ParametricSpline.h"
#include "IgesEntityVisitor.h"
#include "IgesEntitySet.h"


ParametricSpline::ParametricSpline()
{
	entitytype = 112;
}

void ParametricSpline::GetEntityIn(FileLines &mylines , int poffs)
{
	DoubleVector myvect;
	GetAllParam ( myvect , mylines );
	int vecsize = (int)myvect.size();
	if (vecsize <= 4)
		throw VException (( IDS_ParGetExcNotEno),1);
	m_CTYPE = (int)myvect[1];
	m_H = (int)myvect[2];
	m_NDIM = (int)myvect[3];
	m_N = (int)myvect[4];
	
	int offset = 5;
	m_T.resize(m_N+1);
	if (vecsize <= m_N+5)
		throw VException (( IDS_ParGetExcNotEno),1);

	for (int i=0;i<m_N+1;i++)
	{
		m_T[i] = myvect[offset];
		offset++;
	}
	m_AX.resize(m_N);
	m_BX.resize(m_N);
	m_CX.resize(m_N);
	m_DX.resize(m_N);
	m_AY.resize(m_N);
	m_BY.resize(m_N);
	m_CY.resize(m_N);
	m_DY.resize(m_N);
	m_AZ.resize(m_N);
	m_BZ.resize(m_N);
	m_CZ.resize(m_N);
	m_DZ.resize(m_N);

	if (vecsize <= 12*(m_N-1) + offset)
		throw VException (( IDS_ParGetExcNotEno),1);

	for (int i=0;i<m_N;i++)
	{
		m_AX[i] = myvect[offset];
		offset++;
		m_BX[i] = myvect[offset];
		offset++;
		m_CX[i] = myvect[offset];
		offset++;
		m_DX[i] = myvect[offset];
		offset++;
		
		m_AY[i] = myvect[offset];
		offset++;
		m_BY[i] = myvect[offset];
		offset++;
		m_CY[i] = myvect[offset];
		offset++;
		m_DY[i] = myvect[offset];
		offset++;

		m_AZ[i] = myvect[offset];
		offset++;
		m_BZ[i] = myvect[offset];
		offset++;
		m_CZ[i] = myvect[offset];
		offset++;
		m_DZ[i] = myvect[offset];
		offset++;
	}
	
	if (vecsize <= offset + 11)
		throw VException (( IDS_ParGetExcNotEno),1);

	m_TPX0 = myvect[offset];
	offset++;
	m_TPX1 = myvect[offset]; 
	offset++;
	m_TPX2 = myvect[offset];
	offset++;
	m_TPX3 = myvect[offset];
	offset++;
	
	m_TPY0 = myvect[offset];
	offset++;
	m_TPY1 = myvect[offset];
	offset++;
	m_TPY2 = myvect[offset];
	offset++;
	m_TPY3 = myvect[offset];
	offset++;
	
	m_TPZ0 = myvect[offset];
	offset++;
	m_TPZ1 = myvect[offset];
	offset++;
	m_TPZ2 = myvect[offset];
	offset++;
	m_TPZ3 = myvect[offset];
	offset++;
	ReadModelId(offset, myvect);
}

void ParametricSpline::WriteEntityOut (std::vector<VStr> &strvec)const
{
	WriteParam (entitytype , strvec);
	WriteParam (m_CTYPE , strvec);
	WriteParam (m_H , strvec);
	WriteParam (m_NDIM , strvec);
	WriteParam (m_N , strvec);
	
	for ( int i=0; i<m_N+1; i++ )
	{
		WriteParam (m_T[i],strvec);
	}

	for ( int i=0; i<m_N; i++ )
	{
		WriteParam (m_AX[i],strvec);
		WriteParam (m_BX[i],strvec);
		WriteParam (m_CX[i],strvec);
		WriteParam (m_DX[i],strvec);
	}
	WriteModelId(strvec);
}

void ParametricSpline::GetChildren (std::vector<int> &childVec)const
{
	GetDirectoryChild (childVec);
}

void ParametricSpline::AcceptVisitor (IgesEntityVisitor &visitor)const
{
	visitor.AcceptVirtual (*this);
}

//bool ParametricSpline::IsPrimitiveEntity()const
//{
//	return true;
//}

//double ParametricSpline::GetSortNo ()const
//{
//	return 7.0;
//}

IgesEntity * ParametricSpline::MakeCopyAndAddToSet (const IgesEntitySet &mainSet,IgesEntitySet &newSet)const
{
	if (m_subsetIndex !=0)
		return newSet.GetEntityPointer(m_subsetIndex);
	ParametricSpline *mycopy = new ParametricSpline (*this);
	CopyDE (*mycopy,mainSet,newSet);
	newSet.InsertEntity (mycopy);
	m_subsetIndex = mycopy->directorylineno;
	return mycopy;
}
