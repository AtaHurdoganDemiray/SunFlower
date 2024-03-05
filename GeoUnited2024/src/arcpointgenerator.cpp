// ArcPointGenerator.cpp

#include "stdafx.h"
#include "ArcPointGenerator.h"
#include "VPoint.h"
#include "CoordinateSystem.h"
#include "DoubleVec.h"
#include "Arc.h"
#include "IgesEntitySet.h"
#include "igesTransformationMatrix.h"
#include "TransformationMatrix.h"
#include "PointMatrix.h"
#include "SequentialLineSet.h"
#include "LineSet.h"
#include "igesTransformationMatrix.h"
#include "VArc2d.h"
#include "SequentialLineSet2d.h"
#include "PolarCalc2d.h"

const int ARC_FIRST_TESS_NO=10;

//ArcPointGenerator::ArcPointGenerator ( const VArc & _arc , IgesEntitySet &allent)
//:CurvePointGenerator (0,0),arc(_arc)
//{
//	ResetArc (_arc,allent);
//}
//void ArcPointGenerator::ResetArc(const VArc &_arc , IgesEntitySet &allent)
//{
//	arccoord = new CoordinateSystem ( arc.GetCoordinateSystem() );
//	bool isTransformed = false;
//	igesTransformationMatrix tm;
//	igesArc a;
//	arc.GetIgesArc (a,tm,isTransformed);
	
//}
ArcPointGenerator::ArcPointGenerator ( igesArc & igesarc , IgesEntitySet & allent )
:CurvePointGenerator (&igesarc , &allent)
{
	VPoint arccp ( igesarc.mX1 , igesarc.mY1 , igesarc.mZT );
	VPoint arcsp ( igesarc.mX2 , igesarc.mY2 , igesarc.mZT );
	VPoint arctp ( igesarc.mX3 , igesarc.mY3 , igesarc.mZT );
	VPoint arcmp;
	VPoint v1 = arcsp - arccp;
	VPoint v2 = arctp - arccp;
	vreal radius = sqrt ( (v1%v1) );

	if (arcsp==arctp)
	{
		VArc newarc(arccp , radius , UZ , arcsp , 360.0 );
		arc = newarc;
	}
	else
	{
		//VPoint d = arctp - arcsp;
		//VPoint crs = v1 ^ d;
		//if (crs % crs < EPSILON)
		//{
		//	VPoint rot(-v1.y() , v1.x() , v1.z());
		//	arcmp = arccp + rot;
		//}
		//else
		//{
		//	d *= 0.5;
		//	arcmp = arcsp + d;
		//	VPoint v = arcmp - arccp;
		//
		//	if (crs.z() < 0.0)
		//	{
		//		v *= -1.;
		//	}
		//	v.NormThis();
		//	v *= radius; 
		//	arcmp = arccp + v;
		//}
		//VArc f (arcsp , arcmp , arctp);
		//arc = f;

		VArc tmparc (arccp,radius,UZ,arcsp,arctp);
		arc = tmparc;
	}
	//v1.NormThis();
	//v2.NormThis();
	
	//vreal cosangle = v1%v2;
	//VPoint crossp = v1^v2;

	//vreal radangle = acos ( cosangle );
	//vreal angle = radangle * 180.0 / PI;
	//if ( fabs(cosangle-1.0) < EPSILON )
	//{
	//	angle = 360.0;
	//}
	//else
	//{
	//	if ( crossp.z() < 0.0 )
	//	{
	//		angle = 360.0 - angle;
	//	}
	//}

	//VPoint dir ( 0.0 , 0.0 , 1.0 );
	//if ( igesarc.DE.pXform != 0 )
	//{
	//	igesTransformationMatrix * tmatrix = (igesTransformationMatrix *)(allent.GetEntityPointer(igesarc.DE.pXform));
	//	TransformationMatrix vmatrix ( *tmatrix );
	//	//arccp = vmatrix.Transform(arccp);
	//	//arcsp = vmatrix.Transform(arcsp);
	//	//arctp = vmatrix.Transform(arctp);
	//	//dir = vmatrix.TransformVec(dir);
	//	arc.Transform (vmatrix);
	//}

	//VArc tmp ( arccp , radius , dir , arcsp , angle );
	//VArc tmp (arcsp , arcmp , arctp);
	//arc = tmp;
	
	//VPoint xdir = arc.GetStartPoint() - arc.GetCenter();
	//xdir.NormThis();
	//VPoint zdir = UZ;//arc.GetAx();
	//VPoint ydir = zdir ^ xdir;
	arccoord = new CoordinateSystem (arc.GetCoordinateSystem());
}

ArcPointGenerator::~ArcPointGenerator()
{
	delete arccoord;
}

VPoint ArcPointGenerator::GetPoint ( const vreal & v ) const
{
	vreal vv = ReverseIf(v);
	vv = TransformPar(vv);
	vreal angle = arc.GetSweepAngle() * vv;
	angle *= DEGREE_TO_RAD;
	VPoint lp ( arc.GetRadius() * cos(angle) , arc.GetRadius() * sin(angle) , 0.0 );
	VPoint result = arccoord->RelativeCoordToAbsoluteCoord ( lp );
	TransformIf (result);	
	return result;
}

void ArcPointGenerator::GetPointAndTangent(const vreal &v, VPoint &p, VVector &tng) const
{
	vreal vv = ReverseIf(v);
	vv = TransformPar(vv);
	vreal angle = arc.GetSweepAngle() * vv;
	angle *= DEGREE_TO_RAD;
	vreal costeta = cos(angle);
	vreal sinteta = sin(angle);
	VPoint lp ( arc.GetRadius() * costeta  , arc.GetRadius() * sinteta , 0.0 );
	VVector lv (-sinteta , costeta , 0.0);
	p = arccoord->RelativeCoordToAbsoluteCoord ( lp );
	tng = arccoord->RelativeVectorToAbsoluteVector ( lv );
	ReverseTangentIf (tng);
	TransformIf (p,tng);	
}

void ArcPointGenerator::GetDefIntervals ( DoubleVec & defint ) const
{
	vreal divideNof = ARC_FIRST_TESS_NO;//* (arc.GetSweepAngle() / 360.0);
	int divideNo = static_cast<int>(divideNof);
	vreal deltav = 1.0 / (divideNo-1);
	for ( int i=0; i < divideNo; i++ )
	{
		vreal currpar = i*deltav;
		defint.Addvreal(currpar);	
	}
	defint.Addvreal(1.0);
}

void ArcPointGenerator::Digitise(const vreal &tol, SequentialLineSet &result , DoubleVec &params) const
{
	return GenericDigitise (tol , result , params );
//	int segmentNo = GetDigitiseSegmentNo (tol);
//	int pointNo = segmentNo + 1;
//	vreal dv = 1. / segmentNo;
	//vreal v = 0.0;
	//for (int i=0; i<pointNo; i++)
	//{
	//	
	//	VPoint currp = GetPoint (v);
	//	bool ck = result.InsertPoint (currp);
	//	if (ck)
	//	{
	//		params.Addvreal(v);
	//	}
	//	v += dv;
	//}
}

void ArcPointGenerator::Digitise(const vreal &tol
								, SequentialLineSet &result 
								, PointSet &tangents
								, DoubleVec &params) const
{
	int segmentNo = GetDigitiseSegmentNo (tol);
	int pointNo = segmentNo + 1;
	vreal dv = 1. / segmentNo;
	vreal v = 0.0;
	for (int i=0; i<pointNo; i++)
	{
		VPoint currp,currtangent;
		GetPointAndTangent (v,currp,currtangent);
		bool ck = result.InsertPoint (currp);
		if (ck)
		{	
			tangents.InsertPoint (currtangent);
			params.Addvreal(v);
		}
		v += dv;
	}
}

int ArcPointGenerator::GetDigitiseSegmentNo (const vreal &tol)const
{
	//vreal divideNof = ARC_FIRST_TESS_NO * (arc.GetSweepAngle() / 360.0);
	//int divideNo = static_cast<int>(divideNof);
	//return divideNo;
	
	//vreal tol
	vreal r = arc.GetRadius();
	vreal teta = vp_acos ( (r-tol)/r )* 2.0;
	vreal sweepAngle = arc.GetSweepAngle() * DEGREE_TO_RAD;
	return  (static_cast<int>(sweepAngle / teta)) + 1;
}

void ArcPointGenerator::GetSnapPoints (PointSet &snapp)const
{
	VPoint tmp(arc.GetCenter());
	TransformIf (tmp);
	snapp.InsertPoint (tmp);
	snapp.InsertPoint (GetPoint (0.0));
	if (arc.IsFull())
	{
		snapp.InsertPoint (GetPoint(0.125));
		snapp.InsertPoint (GetPoint (0.25));
		snapp.InsertPoint (GetPoint(0.375));
	}
	snapp.InsertPoint (GetPoint (0.5));
	if (arc.IsFull())
	{
		snapp.InsertPoint (GetPoint(0.625));
		snapp.InsertPoint (GetPoint (0.75));
		snapp.InsertPoint (GetPoint (0.875));
	}
	snapp.InsertPoint (GetPoint (1.0));
}
void ArcPointGenerator::GetSnapLines (LineSet &snapl)const
{
	vreal myr = arc.GetRadius();
	VPoint tmp (arc.GetCenter());
	VLine myl;
	myl.SetP1 (tmp);
	myl.SetP2 (tmp+arc.GetCoordinateSystem().GetDirz()*myr);
	TransformIf (myl);
	snapl.AddLine (myl);
}

VPoint ArcPointGenerator::GetTangentDerivative (const vreal &v)const
{
	return GetTangentDerivativeGeneric(v);
	//// derivative is wrong !!!

	//vreal sweepangle = arc.GetSweepAngle() * DEGREE_TO_RAD;
	//VPoint p1 (-cos(sweepangle),-sin(sweepangle),0.0);
	//VPoint p2 = arccoord->RelativeCoordToAbsoluteCoord(p1);
	//p2 *= arc.GetRadius();
	////TransformIf (p1,p2);
	//return p2;
}
	
vreal ArcPointGenerator::GetRadiusOfCurvature (const vreal &v)const
{
	return arc.GetRadius();
}

void ArcPointGenerator::Digitise (const VArc2d &a , const vreal tol , SequentialLineSet2d &sl)
{
	vreal r = a.GetRadius();
	vreal r2 = r*r;
	const VPoint2d &c = a.GetCenter();
	vreal cx = c.x();
	vreal cy = c.y();
	vreal angstep = 2.0 * vp_acos ( (r - tol) / r );
	vreal ang = DEGREE_TO_RAD * a.GetStartAngle();
	vreal endang = DEGREE_TO_RAD * a.GetEndAngle();
	if (endang < ang)
	{
		while (endang < ang)
		{
			vreal x = r * cos (ang);
			vreal y = r * sin (ang);
			VPoint2d o (x+cx,y+cy);
			sl.InsertPoint (o);
			ang-=angstep;
		}
	}
	else
	{
		while (endang > ang)
		{
			vreal x = r * cos (ang);
			vreal y = r * sin (ang);
			VPoint2d o (x+cx,y+cy);
			sl.InsertPoint (o);
			ang+=angstep;
		}
	}
	vreal xe = r * cos(endang);
	vreal ye = r * sin(endang);
	VPoint2d oe (xe,ye);
	sl.InsertPoint(c+oe);
	
}


