// HelicalRevolvedSection.cpp

#include "stdafx.h"
#include "HelicalRevolvedSection.h"
#include "VPoint.h"
#include "CurvePointGenerator.h"
#include "CurvePointGeneratorFactory.h"
#include "HelisEval.h"
#include "SurfaceOfRevolution.h"
#include "Line.h"
#include "VLine.h"
#include "PolarCalc2d.h"
#include "RotationMatrix.h"
#include "DoubleVec.h"

HelicalRevolvedSection::HelicalRevolvedSection (const SurfaceOfRevolution *surfaceOfRev , const IgesEntitySet &allent)
:SurfacePointGenerator (0.,1.,0.,1.) , m_surfaceOfRev (surfaceOfRev)
{
	m_inv360 = 1. / 360.;
	int sectionCurveDe = surfaceOfRev->m_C;
	IgesEntity * sectionCurve = allent.GetEntityPointer(sectionCurveDe);
	bool iscurve = allent.IsSupportedCurveType(sectionCurve);
	if (iscurve == false)
		throw VException (( IDS_HelConExcSecIsN),1);

	allent.MakeAllSubsetIndex0();
	m_sectionCurve = sectionCurve->MakeCopyAndAddToSet (allent , m_eSet);
	m_section = CurvePointGeneratorFactory::MakeCurve( m_sectionCurve , m_eSet);
	//m_section->SetTransformationMatrixIf ();
	int lineDe = surfaceOfRev->m_L;
	IgesEntity *myline = allent.GetEntityPointer(lineDe);
	if (myline->entitytype != 110)
		throw VException (( IDS_HelConExcAxiLin),1);
	Line *iline = (Line *)myline;
	VLine l (VPoint(iline->X1 , iline->Y1 , iline->Z1) , VPoint(iline->X2 , iline->Y2 , iline->Z2));
	// This line may contain 6 double parameters for future use

	// revolution is done along absolute coordinate system z axis

	m_pitch = surfaceOfRev->SA;
	m_deltac = surfaceOfRev->TA;
	
	vreal deltaDiameterPerRound = l.GetP1().x();
	vreal deltaPitchPerRound = l.GetP1().y();
	vreal turnAroundNumber = m_deltac * m_inv360;
	m_totalDeltaDiameter = deltaDiameterPerRound * turnAroundNumber;
	m_totalDeltaPitch = deltaPitchPerRound * turnAroundNumber;

	SetU0 (m_section->GetV0());
	SetU1 (m_section->GetV1());
	SetV0 (0.0);
	SetV1 (1.0);
	
}
HelicalRevolvedSection::~HelicalRevolvedSection()
{
	delete m_section;
}
VPoint HelicalRevolvedSection::GetPoint ( const vreal & u , const vreal & v ) const
{
	vreal su,sv;
	ShiftParIf(u,v,su,sv);
	VPoint secp = m_section->GetPoint(su);
	double rotangle = m_deltac * sv;
	secp.x(secp.x() + m_totalDeltaDiameter*sv*fabs(rotangle)*m_inv360 * 0.5);	
	RotationMatrix R (VPoint(0.0),UZ,rotangle);
	secp = R.Transform(secp);
	//secp.y(secp.y() + secp.y()*0.2*v);
	//double deltaz = fabs(rotangle) * (m_pitch + m_totalDeltaPitch*sv) / 360.0;
	double deltaz = fabs(rotangle) * m_pitch * m_inv360 + (m_totalDeltaPitch * sv * fabs(rotangle) * m_inv360 * 0.5);
	secp.z (secp.z() + deltaz);
	return secp;
}

VPoint HelicalRevolvedSection::GetNormal (const vreal & u , const vreal & v , const VPoint & surfpoint) const
{
	return GetNormalAppr(u,v,surfpoint);
}

void HelicalRevolvedSection::GetPointAndNormal (const vreal &u,const vreal &v,VPoint &m,VPoint &n)const
{
	m = GetPoint(u,v);
	n = GetNormal(u,v,m);
}
void HelicalRevolvedSection::GetUDefIntervals ( DoubleVec & defint ) const
{
	m_section->GetDefIntervals(defint);
}
void HelicalRevolvedSection::GetVDefIntervals ( DoubleVec & defint ) const
{
	if (fabs(m_deltac) < EPSILON)
		throw VException (( IDS_HelGetExcMDelIs),1);
	int divideCount = (int) (m_deltac * 0.05); // Each 10 degrees (approximately) rotation is an interval
	double deltat = 1. / divideCount;
	defint.Addvreal(0.0);
	for (int i=1; i<divideCount; ++i)
	{
		defint.Addvreal(deltat*i);
	}
	defint.Addvreal(1.0);
}
VRefTriangleMesh * HelicalRevolvedSection::Digitise (const vreal &tol) const
{
	return GenericDigitise(tol);
}

//bool HelicalRevolvedSection::Unproject(const VPoint &mp, vreal &u, vreal &v, vreal tol)
//{
//	return GenericUnproject(mp, u, v, tol);
//}

IgesEntity * HelicalRevolvedSection::GetIgesEntity()
{
	return NULL;
}