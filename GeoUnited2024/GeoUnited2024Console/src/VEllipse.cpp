 //VEllipse.cpp
#include "stdafx.h"
#include "VEllipse.h"
#include "ConicArc.h"
#include "igesTransformationMatrix.h"
#include "ReorientMatrix.h"

VEllipse::VEllipse()
:m_xRadius(1.),m_yRadius(0.5),m_sweepAngle1(0.0),m_sweepAngle2(360.)
{
	//m_cs:defaullt coordinate system
}
VEllipse::VEllipse (const vreal &xRadius
					,const vreal &yRadius)
:m_xRadius(xRadius)
,m_yRadius(yRadius)
,m_sweepAngle1(0.0)
,m_sweepAngle2(360.)
{

}
VEllipse::VEllipse (const vreal &xRadius 
				, const vreal &yRadius
				, const vreal &sweepAngle1
				, const vreal &sweepAngle2)
				:m_xRadius(xRadius)
				,m_yRadius(yRadius)
				,m_sweepAngle1(sweepAngle1)
				,m_sweepAngle2(sweepAngle2)
{

}
VEllipse::VEllipse(const vreal &xRadius
			, const vreal &yRadius 
			, const CoordinateSystem &cs 
			, const vreal &sweepAngle1
			, const vreal &sweepAngle2)
:m_xRadius(xRadius)
,m_yRadius(yRadius)
,m_cs(cs)
,m_sweepAngle1(sweepAngle1)
,m_sweepAngle2(sweepAngle2)
{

}
VEllipse::VEllipse(const VEllipse &toCopy)
{
	m_xRadius=toCopy.m_xRadius;
	m_yRadius=toCopy.m_yRadius;
	m_cs=toCopy.m_cs;
	m_sweepAngle1=toCopy.m_sweepAngle1;
	m_sweepAngle2=toCopy.m_sweepAngle2;
}
VEllipse & VEllipse::operator = (const VEllipse &toCopy)
{
	m_xRadius=toCopy.m_xRadius;
	m_yRadius=toCopy.m_yRadius;
	m_cs=toCopy.m_cs;
	m_sweepAngle1=toCopy.m_sweepAngle1;
	m_sweepAngle2=toCopy.m_sweepAngle2;
	return *this;
}
VEllipse:: ~VEllipse()
{

}
const vreal & VEllipse::GetXRadius()const
{
	return m_xRadius;
}
const vreal & VEllipse::GetYRadius()const
{
	return m_yRadius;
}
const vreal & VEllipse::GetSweepAngle1()const
{
	return m_sweepAngle1;
}
const vreal & VEllipse::GetSweepAngle2() const
{
	return m_sweepAngle2;
}
const CoordinateSystem & VEllipse::GetCoordinateSystem()const
{
	return m_cs;
}
void VEllipse::Serialize (Bifstream &infile)
{
	int currversion;
	infile.read((VChr*)&currversion,sizeof(int));
	if(currversion==1)
	{
		infile.read((VChr*)&m_xRadius,sizeof(vreal));
		infile.read((VChr*)&m_yRadius,sizeof(vreal));
		m_cs.Serialize(infile);
		infile.read((VChr*)&m_sweepAngle1,sizeof(vreal));
		infile.read((VChr*)&m_sweepAngle2,sizeof(vreal));
	}
	else
		throw VException(( IDS_VElSerExc),1);
}
void VEllipse::Serialize (Bofstream &outfile)const
{
	int currversion=1;
	outfile.write((VChr*)&currversion,sizeof(int));
	if(currversion==1)
	{
		outfile.write((VChr*)&m_xRadius,sizeof(vreal));
		outfile.write((VChr*)&m_yRadius,sizeof(vreal));
		m_cs.Serialize(outfile);
		outfile.write((VChr*)&m_sweepAngle1,sizeof(vreal));
		outfile.write((VChr*)&m_sweepAngle2,sizeof(vreal));
	}
	else
		throw VException(( IDS_VElSerExc),1);
}
void VEllipse::Transform (const TransformationMatrix &tm)
{
	m_cs.Transform(tm);
}
bool VEllipse::operator == (const VEllipse &toCompare)const
{
	if(this==&toCompare)
		return true;

	if(m_xRadius!=toCompare.m_xRadius)
		return false;
	if(m_yRadius!=toCompare.m_yRadius)
		return false;
	if(m_sweepAngle1!=toCompare.m_sweepAngle1)
		return false;
	if(m_sweepAngle2!=toCompare.m_sweepAngle2)
		return false;
	if(m_cs!=toCompare.m_cs)
		return false;
	return true;
}
bool VEllipse::operator != (const VEllipse &toCompare)const
{
	return !(operator==(toCompare));
}
bool VEllipse::IsFull()const
{
	if (fabs(m_sweepAngle2 - m_sweepAngle1) < EPSILON ||
		fabs (fabs (m_sweepAngle2 - m_sweepAngle1)-360.0) < EPSILON)
		return true;
	else
		return false;
}

void VEllipse::GetIgesEllipse (ConicArc &myarc , igesTransformationMatrix &tm , bool & isTransformed)const
{
	myarc.m_A = (1 / (GetXRadius()*GetXRadius()));
	myarc.m_B = 0.0;
	myarc.m_C = (1 / (GetYRadius()*GetYRadius()));
	myarc.m_D = 0.0;
	myarc.m_E = 0.0;
	myarc.m_F = -1.0;
	double a = sqrt (-myarc.m_F / myarc.m_A);
	double b = sqrt (-myarc.m_F / myarc.m_C);
	double t1 = GetSweepAngle1();
	double t2 = GetSweepAngle2();
	
	myarc.m_X1 = a*cos (t1*DEGREE_TO_RAD);
	myarc.m_Y1 = b*sin (t1*DEGREE_TO_RAD);
	myarc.m_X2 = a*cos (t2*DEGREE_TO_RAD);
	myarc.m_Y2 = b*sin (t2*DEGREE_TO_RAD);
	myarc.m_ZT = 0.0;	

	CoordinateSystem absolutecs(VPoint(0.0));
	CoordinateSystem cs = this->GetCoordinateSystem();
	if (cs != absolutecs)
	{
		ReorientMatrix M (absolutecs , cs);
		tm = M.GetIgesTransformation ();
		isTransformed = true;
	}
	else
	{
		isTransformed = false;
	}
}
