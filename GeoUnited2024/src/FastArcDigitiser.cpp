// FastArcDigitiser.cpp

#include "stdafx.h"
#include "FastArcDigitiser.h"
#include "PolarCalc2d.h"

void FastArcDigitiser::Digitise(const VArc &a, int pointnumber, SequentialLineSet &sl)
{
	// Digitise arcs and add to linear lists
	double sa = a.GetSweepAngle();
	if (fabs(sa) < EPSILON)
		throw VException("FastArcDigitiser::Digitise exception , arc sweep angle is 0",1);
	double dt = sa / pointnumber;
	int pointcountck = (int)fabs(sa / dt);
	if (pointcountck > m_maxSegmentNo)
		throw VException("FastArcDigitiser::Digitise exception , pointcountck is bigger than max point count",1);
	for (double t = 0.; fabs(t)<fabs(sa)+EPSILON;t+=dt)
	{
		sl.InsertPoint(a.GetP(t));
	}
}

void FastArcDigitiser::Digitise (const VArc &a , int pointnumber , std::vector<VPoint> &pv)
{
	double sa = a.GetSweepAngle ();
	double dt = sa / pointnumber;
	if (fabs(sa) < EPSILON)
		throw VException("FastArcDigitiser::Digitise exception , arc sweep angle is 0", 1);
	int pointcountck = (int)fabs(sa / dt);
	if (pointcountck > m_maxSegmentNo)
		throw VException("FastArcDigitiser::Digitise exception , pointcountck is bigger than max point count", 1);
	pv.reserve(pointnumber+1);
	for (double t=0.;fabs(t)<fabs(sa)+EPSILON;t+=dt)
	{
		pv.push_back (a.GetP(t));
	}
}

int FastArcDigitiser::m_maxSegmentNo = 200000;

int FastArcDigitiser::GetDigitiseSegmentNo (const VArc &a , const vreal tol)
{
	vreal r = a.GetRadius();
	vreal teta = vp_acos ( (r-tol)/r )* 2.0;
	vreal sweepAngle = a.GetSweepAngle() * DEGREE_TO_RAD;
	int segmentNo = (static_cast<int>(fabs(sweepAngle / teta))) + 1;
	if (segmentNo < m_maxSegmentNo)
		return segmentNo;
	else
		throw VException (( IDS_FasGetExcSegNoI),1);
}

int FastArcDigitiser::GetDigitiseSegmentNoByStepLength(const VArc &a, const vreal stepLength)
{
	vreal r = a.GetRadius();
	vreal sweepAngle = a.GetSweepAngle() * DEGREE_TO_RAD;
	vreal len = fabs(sweepAngle * r);
	int segmentNo = (static_cast<int>(len / stepLength)) + 2;
	if (segmentNo < m_maxSegmentNo)
		return segmentNo;
	else
		throw VException("FastArcDigitiser::GetDigitiseSegmentNoByStepLength exception , segment no is too big",1);
}

int FastArcDigitiser::m_maxDigitiseCount = 100000;

void FastArcDigitiser::Digitise(const VArc &a, const vreal tol, SequentialLineSet &sl, std::vector<double> &angles)
{
	CoordinateSystem cs = a.GetCoordinateSystem();

	vreal r = a.GetRadius();
	vreal r2 = r*r;

	vreal angstep = 2.0 * vp_acos((r - tol) / r);
	vreal ang = 0.0;
	vreal endang = DEGREE_TO_RAD * a.GetSweepAngle();
	if (endang < ang)
	{
		int counter = 0;
		while (endang < ang)
		{
			vreal x = r * cos(ang);
			vreal y = r * sin(ang);
			VPoint o(x, y, 0.0);
			sl.InsertPoint(cs.RelativeCoordToAbsoluteCoord(o));
			angles.push_back(ang*RAD_TO_DEGREE);
			ang -= angstep;
			++counter;
			if (counter > m_maxDigitiseCount)
				throw VException((IDS_ArcDigExcMaxDig), 1);
		}
	}
	else
	{
		int counter = 0;
		while (endang > ang)
		{
			vreal x = r * cos(ang);
			vreal y = r * sin(ang);
			VPoint o(x, y, 0.0);
			sl.InsertPoint(cs.RelativeCoordToAbsoluteCoord(o));
			angles.push_back(ang*RAD_TO_DEGREE);
			ang += angstep;
			++counter;
			if (counter > m_maxDigitiseCount)
				throw VException((IDS_ArcDigExcMaxDig), 1);
		}
	}
	vreal xe = r * cos(endang);
	vreal ye = r * sin(endang);
	VPoint oe(xe, ye, 0.0);
	sl.InsertPoint(cs.RelativeCoordToAbsoluteCoord(oe));
	angles.push_back(endang*RAD_TO_DEGREE);
}

int FastArcDigitiser::GetDigitiseSegmentNoOutside(const VArc& a, const vreal tol)
{
	//Segmentation error outside arc = R(1 - cos(A / 2)) / cos(A / 2)
	//Segmentation error inside arc = R(1 - cos(A / 2))
	//tol = Segmentation error outside arc
	//A = 2 * (acos(R/(R+tol)))
	vreal r = a.GetRadius();
	vreal teta = vp_acos(r/(r + tol)) * 2.0;
	vreal sweepAngle = a.GetSweepAngle() * DEGREE_TO_RAD;
	int segmentNo = (static_cast<int>(fabs(sweepAngle / teta))) + 1;
	if (segmentNo < m_maxSegmentNo)
		return segmentNo;
	else
		throw VException((IDS_FasGetExcSegNoI), 1);
}

void FastArcDigitiser::DigitiseOutside(const VArc& a, int pointnumber, std::vector<VPoint>& pv)
{

}