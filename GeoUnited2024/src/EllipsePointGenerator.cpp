// EllipsePointGenerator.cpp

#include "stdafx.h"
#include "EllipsePointGenerator.h"
#include "DoubleVec.h"
#include "VEllipse.h"
#include "IgesEntitySet.h"
#include "IgesEntity.h"
#include "SequentialLineSet.h"
#include "LineSet.h"
#include "ArcFit.h"
const int ELLIPSE_DIGIT_NO=400;

EllipsePointGenerator::EllipsePointGenerator (IgesEntity *ent , IgesEntitySet &allent , const VEllipse&elps)
:CurvePointGenerator (ent,&allent)
{
	Construct(elps);
}

//EllipsePointGenerator::EllipsePointGenerator(const VEllipse&elps)
//:CurvePointGenerator (0,0)
//{
//	Construct(elps);	
//	//initialising base class members
//}

EllipsePointGenerator::EllipsePointGenerator(const EllipsePointGenerator &toCopy)
:CurvePointGenerator(toCopy)	
{
	m_ellipse=toCopy.m_ellipse;
	m_cs=toCopy.m_cs;
	m_sweepangle1=toCopy.m_sweepangle1;
	m_sweepangle2=toCopy.m_sweepangle2;
	m_xr2=toCopy.m_xr2;
	m_yr2=toCopy.m_yr2;
}
EllipsePointGenerator & EllipsePointGenerator::operator =(const EllipsePointGenerator &toCopy)
{
	CurvePointGenerator::operator=(toCopy);
	m_ellipse=toCopy.m_ellipse;
	m_cs=toCopy.m_cs;
	m_sweepangle1=toCopy.m_sweepangle1;
	m_sweepangle2=toCopy.m_sweepangle2;
	m_xr2=toCopy.m_xr2;
	m_yr2=toCopy.m_yr2;
	return *this;
}
void EllipsePointGenerator::GetDefIntervals(DoubleVec &defint)const
{
	vreal deltat=1./ELLIPSE_DIGIT_NO;
	for (int i=0;i<ELLIPSE_DIGIT_NO+1;i++)
	{
		vreal currt=deltat*i;
		defint.Addvreal(currt);
	}
}
EllipsePointGenerator::~EllipsePointGenerator()
{
}

//tanQ=y/x
//x^2/r^2+y^2/b^2 = 1
//x^2/r^2  + (x^2 tan2Q)/b^2 = 1
//x^2 (1/r^2 + tan2Q/b^2) = 1
//x = sqrt (1 / (1/r^2 + tan2Q/b^2)) ( special case:if Q=0 x=0)
//y = b * sqrt (1 - x^2/r^2)

VPoint EllipsePointGenerator::GetPoint(const vreal &v)const
{
	vreal vv = ReverseIf (v);
	vv = TransformPar (vv);
	//vreal currAngle=m_sweepangle*v;
	vreal deltaA=m_sweepangle2-m_sweepangle1;
	vreal currAngle=m_sweepangle1 + deltaA*vv;

	vreal localX=GetXForAngle(currAngle);
	vreal localY=GetY(localX);
	vreal region=floor (currAngle/(PI/2.));
	
	if (region==1 || region==2 || region == -2 || region == -3)
		localX*=-1.0;
	if (region==2 || region==3 || region == -1 || region == -2)
		localY*=-1.0;

	VPoint relp(localX,localY,0.);
	VPoint newp=m_cs.RelativeCoordToAbsoluteCoord(relp);
	TransformIf (newp);
	return newp;		
}
		
vreal EllipsePointGenerator::GetY(const vreal &x)const
{
	vreal y=m_ellipse.GetYRadius()*sqrt(1.-x*x/m_xr2);
	return y;
}

vreal EllipsePointGenerator::GetX(const vreal &y)const
{
	vreal x=m_ellipse.GetXRadius() * sqrt(1.-y*y/m_yr2);
	return x;
}
	
vreal EllipsePointGenerator::GetXForAngle (const vreal &angleInRadian)const
{	
	//tan PI/2
	if (fabs(angleInRadian-PI/2.)<1.e-8 || fabs(angleInRadian-PI*3./2.)<1.e-8)
		return 0.0;
	if (fabs(angleInRadian+PI/2.)<1.e-8 || fabs(angleInRadian+PI*3./2.)<1.e-8)
		return 0.0;
	//x = sqrt (1 / (1/r^2 + tan2Q/b^2)) ( special case:if Q=0 x=0)
	vreal t2Q=tan(angleInRadian)*tan(angleInRadian);			
	vreal x=sqrt(1./(1./m_xr2 + t2Q/m_yr2));
	return x;
}	
void EllipsePointGenerator::Construct(const VEllipse &ellipse)
{
	m_ellipse=ellipse;
	m_cs=ellipse.GetCoordinateSystem();
	m_sweepangle1=m_ellipse.GetSweepAngle1()*DEGREE_TO_RAD;
	m_sweepangle2=m_ellipse.GetSweepAngle2()*DEGREE_TO_RAD;
	if (fabs(m_sweepangle1-m_sweepangle2) < 1.e-6)
		m_sweepangle2 += (PI*2.0);

	m_xr2=m_ellipse.GetXRadius()*m_ellipse.GetXRadius();
	m_yr2=m_ellipse.GetYRadius()*m_ellipse.GetYRadius();		
	//_Curve::SetStart_End_Closed();
}		
//void EllipsePointGenerator::Transform (const TransformationMatrix &tm)
//{
//	VEllipse newellipse=m_ellipse;
//	newellipse.Transform(tm);
//	Construct(newellipse);
//}
//void EllipsePointGenerator::Serialize (Bofstream &outfile)const
//{
//	_Curve::Serialize(outfile);
//	int version=1;
//	outfile.write((VChr*)&version,sizeof(int));
//	if(version==1)
//	{
//		outfile.write((VChr*)&m_xr2,sizeof(vreal));
//		outfile.write((VChr*)&m_yr2,sizeof(vreal));
//		outfile.write((VChr*)&m_sweepangle1,sizeof(vreal));
//		outfile.write((VChr*)&m_sweepangle2,sizeof(vreal));
//		m_cs.Serialize(outfile);
//		m_ellipse.Serialize(outfile);
//	}
//	else
//		throw VException(("EllipsePointGenerator serialization exception"),1);
//}
//void EllipsePointGenerator::Serialize (Bifstream &infile)
//{
//	_Curve::Serialize(infile);
//	int version;
//	infile.read((VChr*)&version,sizeof(int));
//	if(version==1)
//	{
//		infile.read((VChr*)&m_xr2,sizeof(vreal));
//		infile.read((VChr*)&m_yr2,sizeof(vreal));
//		infile.read((VChr*)&m_sweepangle1,sizeof(vreal));
//		infile.read((VChr*)&m_sweepangle2,sizeof(vreal));
//		m_cs.Serialize(infile);
//		m_ellipse.Serialize(infile);
//	}
//	else
//		throw VException(("EllipsePointGenerator serialization exception"),2);
//}
//void EllipsePointGenerator::AcceptVisitor (GeometryVisitor &visitor)
//{
//	visitor.ComeBack(*this);
//}
//void EllipsePointGenerator::AcceptVisitor (ConstGeometryVisitor &visitor)const
//{
//	visitor.ComeBack(*this);
//}

//
// dy / dx = -(b/r^2) * (x/sqrt(1-x^2/r^2))
//

void EllipsePointGenerator::GetPointAndTangent (const vreal &v , VPoint &p , VVector &tng)const
{
	vreal vv = ReverseIf (v);
	vv = TransformPar(vv);
	//vreal currAngle=m_sweepangle*v;
	vreal deltaA=m_sweepangle2-m_sweepangle1;
	vreal currAngle=m_sweepangle1 + deltaA*vv;

	vreal localX=GetXForAngle(currAngle);
	vreal localY=GetY(localX);
	vreal region=floor (currAngle/(PI/2.));

	if (region==1 || region==2 || region == -2 || region == -3)
		localX*=-1.0;
	if (region==2 || region==3 || region == -1 || region == -2)
		localY*=-1.0;

	vreal dy_dx = (-m_ellipse.GetYRadius() / m_xr2) * localX / sqrt (1.0 - localX * localX / m_xr2);

	VPoint relp(localX,localY,0.);
	VVector relv (1. , dy_dx , 0.0);
	p=m_cs.RelativeCoordToAbsoluteCoord(relp);
	tng = m_cs.RelativeVectorToAbsoluteVector (relv);		
	tng.NormThis();
	TransformIf (p,tng);
	ReverseTangentIf (tng);
}

void EllipsePointGenerator::Digitise (const vreal &tol , SequentialLineSet &result , DoubleVec &params)const
{
	return GenericDigitise (tol , result , params);
	//DoubleVec myvec;
	//GetDefIntervals (myvec);
	//int mysize = myvec.Size();
	//for (int i=0;i<mysize;i++)
	//{
	//	vreal currpar = myvec[i];
	//	VPoint currpoint = GetPoint (currpar);
	//	bool ck = result.InsertPoint (currpoint);
	//	if (ck)
	//		params.Addvreal (currpar);
	//}
}

void EllipsePointGenerator::Digitise (const vreal &tol , SequentialLineSet &result , PointSet &tangents , DoubleVec &params)const
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

void EllipsePointGenerator::GetSnapPoints (PointSet &snapp)const
{
	VPoint tmp(m_ellipse.GetCoordinateSystem().GetOrigin());
	TransformIf (tmp);
	snapp.InsertPoint (tmp);
	snapp.InsertPoint (GetPoint (0.0));
	if (m_ellipse.IsFull())
		snapp.InsertPoint (GetPoint (0.25));
	snapp.InsertPoint (GetPoint (0.5));
	if (m_ellipse.IsFull())
		snapp.InsertPoint (GetPoint (0.75));
	snapp.InsertPoint (GetPoint (1.0));
}
void EllipsePointGenerator::GetSnapLines (LineSet &snapl)const
{
	vreal myr = (m_ellipse.GetXRadius()+m_ellipse.GetYRadius()) * 0.5;
	VPoint tmp (m_ellipse.GetCoordinateSystem().GetOrigin());
	VLine myl;
	myl.SetP1 (tmp);
	myl.SetP2 (tmp+m_ellipse.GetCoordinateSystem().GetDirz()*myr);
	TransformIf (myl);
	snapl.AddLine (myl);
}

VPoint EllipsePointGenerator::GetTangentDerivative (const vreal &v)const
{
	VPoint p = GetPoint (v);
	VPoint derv = this->GetDervApr (v , p);
	return GetDervDervApr(v,p,derv);
}
	
vreal EllipsePointGenerator::GetRadiusOfCurvature (const vreal &v)const
{
	VPoint derv,dervderv;
	return 1. / GetCurvatureApr(v,derv,dervderv);
}

void EllipsePointGenerator::EllipseToLineOrArcs (const VEllipse &e , std::vector<LineOrArc> &res , int stepno)
{
	std::vector<VPoint> vec;
	DivideEllipse (e , stepno , vec);
	ArcFit af;
	af.Fit(vec,0.02,res);
}

void EllipsePointGenerator::DivideEllipse (const VEllipse &ellipse,int stepno,std::vector<VPoint> &pvec)
{
	CoordinateSystem cs = ellipse.GetCoordinateSystem();
	vreal xr = ellipse.GetXRadius();
	vreal xr2 = xr*xr;
	vreal yr = ellipse.GetYRadius();
	vreal yr2 = yr * yr;
	vreal t0 = ellipse.GetSweepAngle1()*DEGREE_TO_RAD;
	vreal dt = (ellipse.GetSweepAngle2()*DEGREE_TO_RAD - t0 ) / stepno;	

	for (int i=0;i<stepno+1;++i)
	{
		vreal t = t0 +dt*i;
		VPoint p (GetEllipsePoint (t,xr2,yr2,yr));
		pvec.push_back (cs.RelativeCoordToAbsoluteCoord (p));
	}
}

VPoint EllipsePointGenerator::GetEllipsePoint (const vreal t,const vreal xr2 , const vreal yr2,const vreal yr)
{
	vreal tQ = tan (t);
	vreal t2Q = tQ*tQ;
	vreal x = sqrt ( 1.0 / (1.0/xr2 + t2Q/yr2) );
	if ((t > HALF_PI && t < PI+HALF_PI) || (t<-HALF_PI && t>-(PI+HALF_PI)))
		x*=-1.;
	vreal y = yr * sqrt (1.0 - x*x / xr2);
	if ( (t>PI && t<TWO_PI) || (t<0 && t>-PI))
		y*=-1.;
	VPoint p (x,y,0.0);
	return p;
}

