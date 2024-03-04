// HelisEval.cpp

#include "stdafx.h"
#include "HelisEval.h"

#include "RotationMatrix.h"

HelisEval::HelisEval(const CoordinateSystem &cs , double pitch , double startangle , double startz , double endz , double startradius , double endradius,double tiltangle,int anglesign)
:m_cs(cs),m_pitch(pitch),m_startangle(startangle),m_startz(startz),m_endz(endz),m_startradius(startradius),m_endradius(endradius),m_tiltangle(tiltangle),m_tiltangle2(0),m_anglesign(anglesign)
{
	m_deltaZ = m_endz - m_startz;
	if (fabs (m_pitch)<EPSILON)
		throw VException (( IDS_HelConExcPitIs),1);
	m_pitchInv = 1. / m_pitch;
	if (fabs (m_deltaZ) < EPSILON)
		throw VException (( IDS_HelConExcDelIs),1);
	m_deltaRadF = (m_endradius - m_startradius) / m_deltaZ;	
	m_inv360 = 1. / 360.;
	m_endAngle = GetAngle (m_endz);
	//m_graph.SetEval (this);
}

const double & HelisEval::GetPitch ()const
{
	return m_pitch;
}
void HelisEval::SetPitch (double pitch)
{
	m_pitch = pitch;
}
const double & HelisEval::GetStartAngle ()const
{
	return m_startangle;
}
void HelisEval::SetStartAngle (double startangle)
{
	m_startangle = startangle;
}
const double & HelisEval::GetStartZ ()const
{
	return m_startz;
}
void HelisEval::SetStartZ (double startz)
{
	m_startz = startz;
}
const double & HelisEval::GetEndZ ()const
{
	return m_endz;
}
void HelisEval::SetEndZ(double endz)
{
	m_endz = endz;
}
const double & HelisEval::GetStartRadius ()const
{
	return m_startradius;
}
void HelisEval::SetStartRadius (double startradius)
{
	m_startradius = startradius;
}
const double & HelisEval::GetEndRadius ()const
{
	return m_endradius;
}
void HelisEval::SetEndRadius (double endradius)
{
	m_endradius = endradius;
}
VPoint HelisEval::GetPoint1(double angle)const
{
	double myz = GetZ (angle);
	double myr = GetRadius(myz);
	double myangle = angle * DEGREE_TO_RAD;
	double myx = myr * cos (myangle);
	double myy = myr * sin (myangle);
	VPoint pr (myx , myy , myz);
	VPoint pa = m_cs.RelativeCoordToAbsoluteCoord(pr);
	return pa;
}
VPoint HelisEval::GetPoint2(double zcoord)const
{
	double mya = GetAngle (zcoord);
	double myr = GetRadius(zcoord);
	double myangle = mya * DEGREE_TO_RAD;
	double myx = myr * cos (myangle);
	double myy = myr * sin (myangle);
	VPoint rp (myx , myy , zcoord);
	VPoint ap = m_cs.RelativeCoordToAbsoluteCoord(rp);
	return ap;
}
double HelisEval::GetRadius(double z)const
{
	if (m_startz < m_endz)
	{
		if (z < m_startz-EPSILON || z > m_endz + EPSILON)
			throw VException (( IDS_HelGetExcZIsOut),1);
	}
	else 
	{
		if (z > m_startz + EPSILON || z < m_endz - EPSILON)
			throw VException (( IDS_HelGetExcZIsOut),1);
	}
	double r = m_startradius + (z-m_startz) * m_deltaRadF;
	return r;
}
double HelisEval::GetAngle(double z)const
{
	if (m_startz < m_endz)
	{
		if (z < m_startz-EPSILON || z > m_endz + EPSILON)
			throw VException (( IDS_HelGetExcZIsOut1),1);
	}
	else 
	{
		if (z > m_startz + EPSILON || z < m_endz - EPSILON)
			throw VException (( IDS_HelGetExcZIsOut1),1);
	}
	double a = m_startangle + 360.0 * m_anglesign * (z-m_startz) * m_pitchInv;
	return a;
}

double HelisEval::GetZ (double angle)const
{
	if (m_anglesign > 0)
	{
		if (angle < m_startangle - EPSILON || angle > m_endAngle + EPSILON)
			throw VException (( IDS_HelGetExcAngIsO),1);
	}
	else
	{
		if (angle > m_startangle + EPSILON || angle < m_endAngle - EPSILON)
			throw VException (( IDS_HelGetExcAngIsO),1);
	}
	double pitchno = (angle - m_startangle) * m_inv360 * m_anglesign;
	double myz = m_startz + pitchno * m_pitch;
	return myz;	
}

VPoint HelisEval::GetCylinderNormal1(double angle) const
{
	double myangle = angle * DEGREE_TO_RAD;
	double xi = cos (myangle);
	double yi = sin (myangle);
	VPoint n (xi,yi,0.0);
	VPoint nr = m_cs.RelativeVectorToAbsoluteVector(n);
	return nr;
}

VPoint HelisEval::GetCylinderNormal2(double z) const
{
	double angle = GetAngle(z);
	return GetCylinderNormal1(angle);
}

const double & HelisEval::GetEndAngle()const
{
	return m_endAngle;
}

VStr HelisEval::GetInfoTxt ()const
{
	//CString info;
	//info.Format (("hatve:%.2f a1:%.2f a2:%.2f z1:%.2f z2:%.2f r1:%.2f r2:%.2f"),m_pitch,m_startangle,m_endAngle,m_startz,m_endz,m_startradius,m_endradius);
	//return info;
	VOStrStream info;
	info << ( IDS_Hatve);
	info << m_pitch;
	info << ( IDS_A);
	info << m_startangle;
	info << ( IDS_A1);
	info << m_endAngle;
	info << ( IDS_Z1);
	info << m_startz;
	info << ( IDS_Z2);
	info << m_endz;
	info << ( IDS_R1);
	info << m_startradius;
	info << ( IDS_R2);
	info << m_endradius;
	info << ( IDS_TiltA);
	info << m_tiltangle;
	return info.str();
}

//HelisGraph & HelisEval::GetGraph ()
//{
//	return m_graph;
//}

//const HelisGraph & HelisEval::GetGraph ()const
//{
//	return m_graph;
//}

const VStr & HelisEval::GetName()const
{
	return m_name;
}
void HelisEval::SetName (const VStr &name)
{
	m_name = name;
}

//void HelisEval::MakeGraph (CommandToViewPoint &ap , double step)
//{
//	m_graph.AddCurve (ap , step);
//}

double HelisEval::GetTiltAngle() const
{
	return m_tiltangle;
}

void HelisEval::SetTiltAngle(double tiltangle)
{
	m_tiltangle = tiltangle;
}

VPoint HelisEval::GetToolAxis(double angle) const
{
	VPoint cn = GetCylinderNormal1(angle);
	if (fabs (m_tiltangle) < EPSILON && fabs (m_tiltangle2) < EPSILON)
		return cn;
	VPoint ydir = UZ ^ cn;// ^ UX;
	RotationMatrix R (VPoint(0.0) , ydir , m_tiltangle);
	return R.TransformVec (cn);
}

void HelisEval::SetTiltAngle2(double angle)
{
	m_tiltangle2 = angle;
}

double HelisEval::GetTiltAngle2()const
{
	return m_tiltangle2;
}

int HelisEval::GetAngleSign ()const
{
	return m_anglesign;
}

void HelisEval::SetAngleSign (int anglesign)
{
	m_anglesign = anglesign;
}

const CoordinateSystem & HelisEval::GetCs ()const
{
	return m_cs;
}

void HelisEval::SetCs (const CoordinateSystem &cs)
{
	m_cs = cs;
}

void HelisEval::ReverseThis()
{

}