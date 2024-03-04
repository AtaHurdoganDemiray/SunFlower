#include "stdafx.h"
#include "ContourOffseter.h"

ContourOffseter::ContourOffseter(const std::vector<LineOrArc>* originalGeoVec, vreal initialOffsetValue, vreal tol) // adds arcs with 0 diameter on non-tangent meeting vertex and fills m_originalGeoVec
	:m_originalGeoVec(new std::vector<LineOrArc>(*originalGeoVec)), m_tol(tol), m_initialOffsetValue(initialOffsetValue)
{
	m_originalExtendedGeoVec = InitialOffset(initialOffsetValue);
	//size_t myoriginalSize = originalGeoVec->size();
	//if (myoriginalSize > 1)
	//{
	//	for (size_t i = 1; i < myoriginalSize; ++i)
	//	{
	//		const LineOrArc& curGeo = originalGeoVec->at(i - 1);
	//		const LineOrArc& nextGeo = originalGeoVec->at(i);
	//		VPoint tn0 = curGeo.GetEndTangent();
	//		VPoint tn1 = nextGeo.GetStartTangent();
	//		m_originalGeoVec->push_back(curGeo);
	//		if (fabs((tn0 % tn1) - 1.0) > EPSILON)
	//		{
	//			// tangent vectors are not same direction
	//			// there is discontinuety so add an arc with 0 diameter
	//			VArc tmpa(curGeo.GetEndPoint(), 0.0);
	//			OffsetHelperArc tmpa1()
	//			m_originalGeoVec->push_back(tmpa);
	//		}
	//	}
	//	const LineOrArc& lastGeo = originalGeoVec->at(myoriginalSize-1);
	//	m_originalGeoVec->push_back(lastGeo);
	//	const LineOrArc& firstGeo = originalGeoVec->at(0);
	//	if (firstGeo.GetStartPoint().Compare(lastGeo.GetEndPoint(), m_tol))
	//	{
	//		// this is a closed contour
	//		VPoint tn0 = firstGeo.GetEndTangent();
	//		VPoint tn1 = lastGeo.GetStartTangent();
	//		if (fabs((tn0 % tn1) - 1.0) > EPSILON)
	//		{
	//			// tangent vectors are not same direction
	//			// there is discontinuety so add an arc with 0 diameter
	//			VArc tmpa(lastGeo.GetEndPoint(),0.0, tn0 , tn1);
	//			m_originalGeoVec->push_back(tmpa);
	//		}
	//	}
	//}
	//else
	//{
	//	if (originalGeoVec->size() == 1)
	//		m_originalGeoVec->push_back(originalGeoVec->at(0));
	//	else
	//		LogFileWriter::WriteUnknownExceptionLog("ContourOffseter::ContourOffseter exception.","OriginalGeoVec is empty.");
	//}
}
const LineOrArcVecPtr ContourOffseter::GetOriginalExtendedGeoVec()const
{
	return m_originalExtendedGeoVec;
}
LineOrArcVecPtr ContourOffseter::GetOriginalExtendedGeoVec()
{
	return m_originalExtendedGeoVec;
}
const LineOrArcVecPtr ContourOffseter::GetOriginalGeoVec()const
{
	return m_originalGeoVec;
}
LineOrArcVecPtr ContourOffseter::GetOriginalGeoVec()
{
	return m_originalGeoVec;
}

LineOrArcVecPtr ContourOffseter::InitialOffset(vreal offsVal)const
{
	//  TO DO: implement
	//throw VException("ContourOffseter::Offset not implemented yet.",1);
	LineOrArcVecPtr offsetedGeoVec(new std::vector<LineOrArc>);
	// offset each element
	size_t myoriginalSize = m_originalGeoVec->size();
	if (myoriginalSize > 1)
	{
		for (size_t i = 1; i < myoriginalSize; ++i)
		{
			const LineOrArc& curGeo = m_originalGeoVec->at(i - 1);
			const LineOrArc& nextGeo = m_originalGeoVec->at(i);
			VPoint tn0 = curGeo.GetEndTangent();
			VPoint tn1 = nextGeo.GetStartTangent();
			offsetedGeoVec->push_back(curGeo.GetOffseted(offsVal, UZ));
			if (fabs((tn0 % tn1) - 1.0) > EPSILON)
			{
				// tangent vectors are not same direction
				// there is discontinuety so add an arc with 0 diameter
				VPoint mycorner = curGeo.GetEndPoint();
				OffsetHelperArc tmpa1(tn0, mycorner, tn1, UZ);
				offsetedGeoVec->push_back(tmpa1.GetArc(offsVal));
			}
		}
		const LineOrArc& lastGeo = m_originalGeoVec->at(myoriginalSize - 1);
		offsetedGeoVec->push_back(lastGeo.GetOffseted(offsVal, UZ));
		const LineOrArc& firstGeo = m_originalGeoVec->at(0);
		if (firstGeo.GetStartPoint().Compare(lastGeo.GetEndPoint(), m_tol))
		{
			// this is a closed contour
			VPoint tn0 = lastGeo.GetStartTangent();
			VPoint tn1 = firstGeo.GetEndTangent();
			if (fabs((tn0 % tn1) - 1.0) > EPSILON)
			{
				// tangent vectors are not same direction
				// there is discontinuety so add an arc with 0 diameter
				VPoint mycorner = lastGeo.GetEndPoint();
				OffsetHelperArc tmpa1(tn0, mycorner, tn1, UZ);
				offsetedGeoVec->push_back(tmpa1.GetArc(offsVal));
			}
		}
	}
	else
	{
		if (m_originalGeoVec->size() == 1)
			offsetedGeoVec->push_back(m_originalGeoVec->at(0).GetOffseted(offsVal, UZ));
		else
			LogFileWriter::WriteUnknownExceptionLog("ContourOffseter::ContourOffseter exception.", "OriginalGeoVec is empty.");
	}
	return offsetedGeoVec;
}