// LinePointGenerator.cpp

#include "stdafx.h"
#include "LinePointGenerator.h"
#include "Line.h"
#include "DoubleVec.h"
#include "SequentialLineSet.h"

LinePointGenerator::LinePointGenerator (Line & _line , IgesEntitySet &allent)
:CurvePointGenerator (&_line,&allent) 
,line ( VPoint ( _line.X1 , _line.Y1 , _line.Z1 ) , VPoint ( _line.X2 , _line.Y2 , _line.Z2 ) )
{

}
VPoint LinePointGenerator::GetPoint ( const vreal & v ) const
{
	vreal vv = ReverseIf (v);
	vv = TransformPar(vv);
	VPoint res = line.GetP (static_cast<vreal>(vv));
	TransformIf (res);
	return res;
}

void LinePointGenerator::GetPointAndTangent (const vreal &v , VPoint &p , VVector &tng)const
{
	vreal vv = ReverseIf (v);
	vv = TransformPar(vv);
	p = line.GetP (vv);
	tng = line.GetV().Unit();
	int recursiveCounter(0);
	ReverseTangentIf (tng);
	TransformIf (p,tng);
}

void LinePointGenerator::GetDefIntervals ( DoubleVec & defint ) const
{
	defint.Addvreal ( 0.0 );
	defint.Addvreal ( 1.0 );
}

void LinePointGenerator::Digitise(const vreal &tol, SequentialLineSet &result , DoubleVec &params) const
{
	//vreal deltap = 1. / 15.;
	vreal deltap = 1.0;
	for (int i=0;i</*16*/2;i++)
	{
		vreal currp = i * deltap;
		VPoint p1 = GetPoint (currp);
		bool ck = result.InsertPoint (p1);
		if (ck)
			params.Addvreal (currp);
	}
}

void LinePointGenerator::Digitise(const vreal &tol
								  , SequentialLineSet &result 
								  , PointSet &tangents
								  , DoubleVec &params) const
{
	//vreal deltap = 1. / 15.;
	vreal deltap = 1.0;
	for (int i=0;i</*16*/2;i++)
	{
		vreal currp = i * deltap;
		VPoint p1,t1;
		GetPointAndTangent (currp,p1,t1);
		bool ck = result.InsertPoint (p1);
		if (ck)
		{
			tangents.InsertPoint (t1);
			params.Addvreal (currp);
		}
	}
}

void LinePointGenerator::GetSnapPoints (PointSet &snapp)const
{
	snapp.InsertPoint (GetPoint (0.0));
	snapp.InsertPoint (GetPoint (0.5));
	snapp.InsertPoint (GetPoint (1.0));		
}
void LinePointGenerator::GetSnapLines (LineSet &snapl)const
{

}

VPoint LinePointGenerator::GetTangentDerivative (const vreal &v)const
{
	VPoint p(0.0,0.0,0.0);
	return p;
}

vreal LinePointGenerator::GetRadiusOfCurvature (const vreal &v)const
{
	return MAX_NUMBER_LIMIT;
}
