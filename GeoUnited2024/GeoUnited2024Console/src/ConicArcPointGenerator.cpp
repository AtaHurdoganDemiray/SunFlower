/*********************************************************************************
*			File: ConicArcPointGenerator.cpp									 *
**********************************************************************************
*			Description:														 *
*																				 *
**********************************************************************************
*			History:															 *
*																				 *
**********************************************************************************
*			(C)...																 *
*********************************************************************************/

#include "stdafx.h"
#include "ConicArcPointGenerator.h"
#include "ConicArc.h"
#include "VPoint.h"
#include "Math.h"
#include "GaussElemination.h"

#include "CurvePointGenerator.h"
#include "LinePointGenerator.h"
#include "ArcPointGenerator.h"
#include "EllipsePointGenerator.h"
#include "IgesEntitySet.h"
#include "IgesEntity.h"
#include "Line.h"
#include "VEllipse.h"
#include "VArc.h"
#include "VPoint2d.h"
#include "Arc.h"
#include "GaussElemination.h"
#include "PolarCalc2d.h"
#include "DoubleVec.h"
#include "SequentialLineSet.h"


ConicArcPointGenerator::ConicArcPointGenerator(ConicArc &a ,  IgesEntitySet &allent)
:CurvePointGenerator (&a,&allent) , m_a(a) , m_allent(&allent)
, m_A(2,2)
, m_B(2,1)
, m_D(2,2)
, m_E(2,1)
{	
	A=m_a.m_A;
	B=m_a.m_B;
	C=m_a.m_C;
	D=m_a.m_D;
	E=m_a.m_E;
	F=m_a.m_F;
	X1 = m_a.m_X1;
	Y1 = m_a.m_Y1;
	X2 = m_a.m_X2;
	Y2 = m_a.m_Y2;
	ZT = m_a.m_ZT;
	DecideType();
	if (m_type == ellipse)
	{
		CalcEllipseV0V1();
	}
	else if (m_type == parabola)
	{
		CalcParabolaV0V1();
	}
	else if (m_type == hyperbola)
	{
		CalcHyperbolaV0V1();
	}
	else
		throw VException (( IDS_ConConExcTypIsN),1);


	//if (m_type == ellipse)
	//	CalcEllipseV0V1();
	//else if (m_type == parabola)
	//	CalcParabolaV0V1();
	//else if (m_type == hyperbola)
	//	CalcHyperbolaV0V1();

	//m_A.SetElement( m_a.m_A, 1, 1 );
	//m_A.SetElement( m_a.m_B*0.5, 1, 2 );
	//m_A.SetElement( m_a.m_B*0.5, 2, 1 );
	//m_A.SetElement( m_a.m_C, 2, 2 );

	//m_B.SetElement( m_a.m_D, 1, 1 );
	//m_B.SetElement( m_a.m_E, 2, 1 );
	//SetDR();

	//vreal d0, d1, e0, e1;
	//d0 = m_D.GetElement(1,1);
	//d1 = m_D.GetElement(2,2);
	//
	//e0 = m_E.GetElement(1,1);
	//e1 = m_E.GetElement(2,1);

	//if( (d0 != 0) && (d1 != 0) )
	//{
	//	vreal r;
	//	r = ( (e0*e0)/(4*d0) ) + ( (e1*e1)/(4*d1) ) - (m_a.m_F);
	//	
	//	if( ( (d0*d1) > 0 ) && ((d0*r) < 0) )
	//		throw( VException( ("ConicArc Exception"), 10 ) );	//No Solution
	//	
	//	else if( ((d0*d1) > 0) && ((d0*r) == 0) )
	//		m_type = point;
	//	
	//	else if( ((d0*d1) > 0) && ((d0*r) > 0) )
	//	{
	//		if( d0 == d1 )
	//		{
	//			m_type = circle;
	//		}
	//		else
	//		{
	//			m_type = ellipse;
	//		}
	//	}
	//	
	//	else if( (d0*d1) < 0 )
	//	{
	//		if( r == 0 )
	//		{
	//			m_type = cone;
	//		}
	//		else
	//		{
	//			m_type = hyperbola;
	//		}
	//	}		
	//}
	//else if( ( (d0 != 0) && (d1 == 0) ) || ( (d0 == 0) && (d1 != 0) ) )
	//{
	//	vreal r;
	//	r = ( (e0*e0)/(4*d0) ) - (m_a.m_F);
	//	
	//	if( e1 == 0 )
	//	{
	//		if ( (d0*r) < 0  )
	//			throw( VException( ("ConicArc Exception"), 11 ) );		//No Solution

	//		else if( r == 0 )
	//		{
	//			m_type = line;
	//		}
	//		else if( (d0*r) > 0 )
	//		{
	//			m_type = cylinder;
	//		}
	//	}
	//	else if( e1 != 0)
	//		m_type = parabola;
	//}
	//else if( (d0 == 0) && (d1 == 0) )
	//{
	//	if( (e0 == 0) && (e1 == 0) && ((m_a.m_F != 0) || m_a.m_F == 0) )
	//	{
	//		if( m_a.m_F == 0 )
	//			throw( VException( ("ConicArc Exception"), 12 ) );	//Tautology (0=0)

	//		throw( VException( ("ConicArc Exception"), 13 ) );	//No Solution
	//	}	
	//	else if( (e0 =! 0) && (e1 =! 0) )
	//	{
	//		m_type = line;
	//	}
	//}
}

//#################################################################################

void ConicArcPointGenerator::SetDR()
{
	vreal a11, a12, a21, a22, lamda1, lamda2;
	a11 = m_A.GetElement(1,1);
	a12 = m_A.GetElement(1,2);
	a21 = m_A.GetElement(2,1);
	a22 = m_A.GetElement(2,2);

	lamda1 = 0.5*( (a11 + a22) + sqrt( (4*a12*a21) + (a11 - a22)*(a11 - a22) ) );
	lamda2 = 0.5*( (a11 + a22) - sqrt( (4*a12*a21) + (a11 - a22)*(a11 - a22) ) );
	
	Matrix I1(2,2);
	I1.SetElement ( 1.0, 1, 1);
	I1.SetElement ( 1.0, 2, 2);
	I1.Multiply (lamda1);

	Matrix I2(2,2);
	I2.SetElement ( 1.0, 1, 1);
	I2.SetElement ( 1.0, 2, 2);
	I2.Multiply (lamda2);
	
	Matrix X1 = m_A-I1;
	Matrix X2 = m_A-I2;
	
	vreal a1, a2, a3, a4;
	a1 = X1.GetElement(1,1);
	a2 = X1.GetElement(1,2);
	a3 = X1.GetElement(2,1);
	a4 = X1.GetElement(2,2);

	vreal b1, b2, b3, b4;
	b1 = X2.GetElement(1,1);
	b2 = X2.GetElement(1,2);
	b3 = X2.GetElement(2,1);
	b4 = X2.GetElement(2,2);

	vreal x1 = (a1-a3)*lamda1;
	vreal x2 = (a4-a2)*lamda1;
	vreal lenx = sqrt (x1*x1+x2*x2);
	x1 /= lenx;
	x2 /= lenx;

	vreal y1 = (b1-b3)*lamda2;
	vreal y2 = (b4-b2)*lamda2;
	vreal leny = sqrt (y1*y1+y2*y2);
	y1 /= leny;
	y2 /= leny;

	Matrix R(2,2);
	R.SetElement(x1,1,1);
	R.SetElement(x2,1,2);
	R.SetElement(y1,2,1);
	R.SetElement(y2,2,2);

	m_D.SetElement(lamda1,1,1);
	m_D.SetElement(lamda2,2,2);

	vreal teta;//, l0, l1;
	teta = vp_asin(y1);
	m_l0 = sqrt(lamda1);
	m_l1 = sqrt(lamda2);

	m_E = R*m_B;
	
	Matrix B1(2,1);
	B1.SetElement (1.,1,1);
	B1.SetElement (0.,2,1);
	Matrix B2(2,1);
	B2.SetElement (0.,1,1);
	B2.SetElement (1.,2,1);
	GaussElemination ge;
	Matrix tmp1 = ge.Solve (m_A,B1);
	Matrix tmp2 = ge.Solve (m_A,B2);
	Matrix AI (2,2);
	AI.SetElement (tmp1.GetElement(1,1),1,1);
	AI.SetElement (tmp1.GetElement (2,1),2,1);
	AI.SetElement (tmp2.GetElement(1,1),1,2);
	AI.SetElement (tmp2.GetElement(2,1),2,2);
	Matrix ck = m_A * AI;

	Matrix CCC = AI * m_B;
	CCC.Multiply (0.5);
}	

//  A * XT 2 + B * XT * Y T  + C * Y T 2 + D * XT  + E * Y T  + F  = 0

CurvePointGenerator * ConicArcPointGenerator::MakeGenerator(const IgesEntitySet &allent)
{
	//Type mytype = GetType();
	CurvePointGenerator *newgen=0;
	switch (m_type)
	{
	case(line):
		newgen = MakeLine ();
	break;
	case(ellipse):
		newgen = MakeEllipse ();
	break;
	case (circle):
		newgen = MakeArc();	
	break;
		throw VException (( IDS_UnsConTyp),1);
	}
	return newgen;

	
}

//  A * XT 2 + B * XT * Y T  + C * Y T 2 + D * XT  + E * Y T  + F  = 0

vreal ConicArcPointGenerator::GetAngle (const VPoint2d &arcPoint)const
{
	vreal a = vp_acos (arcPoint.x() / arcPoint.GetLength());
	if (arcPoint.x() > 0.0 && arcPoint.y() > 0.0)
	{
		// 1st quadrant
	}
	else if (arcPoint.x() < 0.0 && arcPoint.y() > 0.0)
	{
		// 2nd quadrant
		a = PI - a;	
	}
	else if (arcPoint.x() < 0.0 && arcPoint.y() < 0.0)
	{
		// 3rd quadrant
		a = PI + a;
	}
	else if (arcPoint.x() > 0.0 && arcPoint.y() < 0.0)
	{
		// 4th quadrant
		a = TWO_PI - a;
	}
	return a;
}

//  A * XT 2 + B * XT * Y T  + C * Y T 2 + D * XT  + E * Y T  + F  = 0

EllipsePointGenerator * ConicArcPointGenerator::MakeEllipse()const
{
	VPoint2d p1 (m_a.m_X1 , m_a.m_Y1);
	VPoint2d p2 (m_a.m_X2 , m_a.m_Y2);
	vreal a1 = GetAngle (p1);
	vreal a2 = GetAngle (p2);
	VPoint corigin (0.0,0.0,m_a.m_ZT);
	CoordinateSystem cs(corigin);
	VEllipse myelp (m_l0 , m_l1 , cs , a1*RAD_TO_DEGREE , a2*RAD_TO_DEGREE);
	EllipsePointGenerator *res = new EllipsePointGenerator (&m_a , *m_allent , myelp);
	return res;
}

//  A * XT 2 + B * XT * Y T  + C * Y T 2 + D * XT  + E * Y T  + F  = 0	

//ConicArcPointGenerator::Type ConicArcPointGenerator::GetType()const
//{
//	Type result = unsupported;
//	if (fabs(m_a.m_B) < EPSILON && fabs (m_a.m_D) < EPSILON && fabs (m_a.m_E) < EPSILON)
//	{
//		if (fabs (m_a.m_A) > 0.0 && fabs (m_a.m_C) > 0.0 && fabs(m_a.m_F) > 0.0)
//		{
//			if (m_a.m_A * m_a.m_F < 0.0 && m_a.m_C * m_a.m_F < 0.0)
//			{
//				if (fabs (m_a.m_A - m_a.m_C) < EPSILON)
//					result = circle;
//				else 
//					result = ellipse;
//			}
//		}
//	}
//	else if (fabs (m_a.m_A) < EPSILON && fabs (m_a.m_C) < EPSILON && fabs(m_a.m_B) < EPSILON)
//	{
//		if (fabs (m_a.m_D) > 0.0 || fabs (m_a.m_E) > 0.0)
//			result = line;
//	}
//
//	return result;
//}

LinePointGenerator * ConicArcPointGenerator::MakeLine ()const
{
	Line tmpline;
	tmpline.X1 = m_a.m_X1;
	tmpline.Y1 = m_a.m_Y1;
	tmpline.Z1 = m_a.m_ZT;
	tmpline.X2 = m_a.m_X2;
	tmpline.Y2 = m_a.m_Y2;
	tmpline.Z2 = m_a.m_ZT;
	tmpline.DE.pXform = m_a.DE.pXform;
	LinePointGenerator * res = new LinePointGenerator (tmpline , *m_allent);
	return res;
}

ArcPointGenerator * ConicArcPointGenerator::MakeArc() const
{
	igesArc myarc;
	myarc.mX1 = 0.0;
	myarc.mY1 = 0.0;
	myarc.mX2 = m_a.m_X1;
	myarc.mY2 = m_a.m_Y1;
	myarc.mX3 = m_a.m_X2;
	myarc.mY3 = m_a.m_Y2;
	myarc.mZT = m_a.m_ZT;
	myarc.DE.pXform = m_a.DE.pXform;
	ArcPointGenerator * res = new ArcPointGenerator (myarc , *m_allent);	
	return res;
}

ConicArcPointGenerator::~ConicArcPointGenerator()
{

}

VPoint ConicArcPointGenerator::GetPoint ( const vreal & v ) const
{
	vreal vv = ReverseIf (v);
	vv = TransformPar(vv);
	VPoint pp;
	switch (m_type)
	{
	case ellipse:
		pp = GetPointEllipse(vv);
		break;
	case parabola:
		pp = GetPointParabola(vv);
		break;
	case hyperbola:
		pp = GetPointHyperbola(vv);
		break;
	}
	TransformIf (pp);
	return pp;
}

void ConicArcPointGenerator::GetPointAndTangent ( const vreal &v , VPoint &p , VVector &tng)const
{
	vreal vv = ReverseIf (v);
	p = GetPoint (vv);
	tng = CurvePointGenerator::GetDervApr (v , p);
	tng.NormThis();
	//point is already transformed in GetPoint //TransformIf (p,tng);
	// points are reversed so numeric derivation need not be reversed //ReverseTangentIf(tng);
	TransformTanVecIf(tng);
}

VPoint ConicArcPointGenerator::GetTangentDerivative (const vreal &v)const
{
	return CurvePointGenerator::GetTangentDerivativeGeneric (v);
}

vreal ConicArcPointGenerator::GetRadiusOfCurvature (const vreal &v)const
{
	VPoint p = GetPoint (v);
	VPoint derv = CurvePointGenerator::GetDervApr (v , p);
	VPoint dervderv = CurvePointGenerator::GetDervDervApr(v,p,derv);
	return CurvePointGenerator::GetRadiusOfCurvatureApr (v , derv , dervderv);
}

VPoint ConicArcPointGenerator::GetPointEllipse (vreal v)const
{
	VPoint p (m_ellipseA*cos(v),m_ellipseB*sin(v),ZT);
	return p;
}

VPoint ConicArcPointGenerator::GetPointParabola (vreal v)const
{
	return VPoint (m_parabolaXCoef*v,m_parabolaYCoef*v*v,ZT);
}

VPoint ConicArcPointGenerator::GetPointHyperbola (vreal v)const
{
	double taneval = tan(m_hyperbolaTCoef*v);
	if (m_hyperbolaType1)
		return VPoint (m_hyperbolaXCoef/taneval , m_hyperbolaYCoef*taneval,ZT);
	else
		return VPoint (m_hyperbolaXCoef*taneval , m_hyperbolaYCoef/taneval,ZT);
}

void ConicArcPointGenerator::CalcEllipseV0V1()
{
	m_ellipseA = sqrt (-F / A);
	m_ellipseB = sqrt (-F / C);

	double t1 = PolarCalc2d::GetPolarAngle (X1 / m_ellipseA,Y1 / m_ellipseB);
	double t2 = PolarCalc2d::GetPolarAngle (X2 / m_ellipseA,Y2 / m_ellipseB);
	double deltat = t2 - t1;
	if (deltat < 1.e-16)
		deltat += 360.0;
	m_v0 = t1 * DEGREE_TO_RAD;
	m_v1 = m_v0 + (deltat * DEGREE_TO_RAD);
	m_deltav = m_v1 - m_v0;
}

void ConicArcPointGenerator::CalcParabolaV0V1()
{
	if (fabs(A) > 1.e-16 && fabs (E) > 1.e-16)
	{
		if (X1 < X2)
		{
			m_v0 = X1;
			m_v1 = X2;
			m_deltav = m_v1 - m_v0;
			m_parabolaXCoef = 1.;
			m_parabolaYCoef = -A/E;
		}
		else
		{
			m_v0 = -X1;
			m_v1 = -X2;
			m_deltav = m_v1 - m_v0;
			m_parabolaXCoef = -1;
			m_parabolaYCoef = -A/E;
		}
	}
	else if (fabs(C) > 1.e-16 && fabs(D) > 1.e-16)
	{
		if (Y1 < Y2)
		{
			m_v0 = Y1;
			m_v1 = Y2;
			m_deltav = m_v1 - m_v0;
			m_parabolaXCoef = -C/D;
			m_parabolaYCoef = 1.;
		}
		else
		{
			m_v0 = -Y1;
			m_v1 = -Y2;
			m_deltav = m_v1 - m_v0;
			m_parabolaXCoef = -C/D;
			m_parabolaYCoef = -1.;
		}
	}
	else 
		throw VException (( IDS_ConCalExcParCoe),1);
}

void ConicArcPointGenerator::CalcHyperbolaV0V1()
{
	if (F*A < 0.0 && F*C > 0.0)
	{
		m_hyperbolaType1 = true;
		m_hyperbolaXCoef = sqrt (-F/A);
		m_hyperbolaYCoef = sqrt (F/C);
		m_hyperbolaTCoef = 1.;
		m_v0 = atan2 (Y1,m_hyperbolaYCoef);
		m_v1 = atan2 (Y2,m_hyperbolaYCoef);
		if (m_v0 > m_v1)
		{
			m_v0*=-1.;
			m_v1*=-1.;
			m_hyperbolaTCoef = -1.;
		}
		m_deltav = m_v1-m_v0;
	}
	else if ( F*A > 0.0 && F*C < 0.0)
	{
		m_hyperbolaType1 = false;
		m_hyperbolaXCoef = sqrt (F/A);
		m_hyperbolaYCoef = sqrt (-F/C);
		m_hyperbolaTCoef = 1.;
		m_v0 = atan2 (X1,m_hyperbolaXCoef);
		m_v1 = atan2 (X2,m_hyperbolaXCoef);
		if (m_v0 > m_v1)
		{
			m_v0*=-1.;
			m_v1*=-1.;
			m_hyperbolaTCoef = -1.;
		}
		m_deltav = m_v1-m_v0;
	}
	else
		throw VException (( IDS_ConCalExcHypCoe),1);
}

void ConicArcPointGenerator::DecideType ()
{
	Matrix Q1 (3,3);
	Q1.SetElement(A,1,1);
	Q1.SetElement(B*0.5,1,2);
	Q1.SetElement(D*0.5,1,3);
	Q1.SetElement(B*0.5,2,1);
	Q1.SetElement(C,2,2);
	Q1.SetElement(E*0.5,2,3);
	Q1.SetElement(D*0.5,3,1);
	Q1.SetElement(E*0.5,3,2);
	Q1.SetElement(F,3,3);
	double dQ1 = Q1.GetDeterminant();
	Matrix Q2(2,2);
	Q2.SetElement(A,1,1);
	Q2.SetElement(B*0.5,1,2);
	Q2.SetElement(B*0.5,2,1);
	Q2.SetElement(C,2,2);
	double dQ2 = Q2.GetDeterminant();
	double dQ3 = A+C;

	m_type = unsupported;
	if (dQ2 > 1.e-16)// && dQ2*dQ1 < 0.)?? for a real ellipse , otherwise imaginary
	{
		// This is an ellipse
		m_type = ellipse;
	}
	else if ( dQ2 < -1.e-16)// && fabs(dQ1) > EPSILON)//??
	{
		// This is an hyperbola
		m_type = hyperbola;
		LogFileWriter::WriteUnknownExceptionLog ( IDS_HyperbolaHurray);
	}
	else if ( fabs (dQ2) < 1.e-16) // && fabs (dQ1) > EPSILON)
	{
		// This is parabola
		m_type = parabola;
		LogFileWriter::WriteUnknownExceptionLog ( IDS_ParabolaHurray);
	}
}

void ConicArcPointGenerator::GetDefIntervals ( DoubleVec & defint ) const
{
	vreal deltat=(1./400.);
	for (int i=0;i<400;i++)
	{
		vreal currt=deltat*i;
		defint.Addvreal(currt);
	}
}

void ConicArcPointGenerator::Digitise (const vreal &tol , SequentialLineSet &result , DoubleVec &params)const
{
	return GenericDigitise (tol,result,params);
}
void ConicArcPointGenerator::Digitise (const vreal &tol , SequentialLineSet &result , PointSet &tangents , DoubleVec &params) const
{
	DoubleVec myvec;
	GetDefIntervals (myvec);
	int mysize = myvec.Size();
	for (int i=0;i<mysize;i++)
	{
		vreal currpar = myvec[i];
		VPoint currpoint,currtangent;
		GetPointAndTangent (currpar,currpoint,currtangent);
		bool ck = result.InsertPoint (currpoint);
		if (ck)
		{
			tangents.InsertPoint (currtangent);
			params.Addvreal (currpar);
		}
	}
}
void ConicArcPointGenerator::GetSnapPoints (PointSet &snapp)const
{
	snapp.InsertPoint (GetPoint (0.));
	snapp.InsertPoint (GetPoint (0.25));
	snapp.InsertPoint (GetPoint (0.5));
	snapp.InsertPoint (GetPoint(0.75));
	snapp.InsertPoint (GetPoint (1.));
}
void ConicArcPointGenerator::GetSnapLines (LineSet &snapl)const
{

}
