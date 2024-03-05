// FastArcDigitiser.h

#ifndef __FastArcDigitiser_h__
#define __FastArcDigitiser_h__

#include "VArc.h"
#include "SequentialLineSet.h"
struct VPoint;

class FastArcDigitiser
{
public:
	static int GetDigitiseSegmentNo (const VArc &a , const vreal tol);
	static int GetDigitiseSegmentNoByStepLength(const VArc &a, const vreal stepLength);
	static void Digitise (const VArc &a , int pointnumber , SequentialLineSet &sl);
	static void Digitise (const VArc &a , int pointnumber , std::vector<VPoint> &pv);
	static int m_maxSegmentNo;
	static void Digitise(const VArc &a, const vreal tol, SequentialLineSet &sl, std::vector<double> &angles);
	static int m_maxDigitiseCount;
	static int GetDigitiseSegmentNoOutside(const VArc& a , const vreal tol);
	static void DigitiseOutside(const VArc& a, int pointnumber, std::vector<VPoint>& pv);
};

#endif