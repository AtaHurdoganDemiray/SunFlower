// VariableOffsetManager.cpp

#include "stdafx.h"
#include "VariableOffsetManager.h"

#include "VPoint2d.h"
#include "VLine.h"
#include "VLine2d.h"
#include "VArc.h"
#include "FastArcDigitiser.h"
#include "ArcFit.h"
#include "InfiniteLinesNearestPoints.h"

VariableOffsetManager::VariableOffsetManager(double tol , double initialOffsetValue, VariableOffsetManager::FunctionType functionType, double deltaOffsetPerUnitLength , double initialDiameter)
:m_initialOffsetValue (initialOffsetValue) , m_offsetValue (initialOffsetValue) , m_functionType (functionType) , m_deltaOffsetPerUnitLength (deltaOffsetPerUnitLength) , m_walkLength (0),m_optSharpCorners(round)
,m_doArcFit (false),m_initialDiameter(initialDiameter),m_tol(tol)
{
		
}

VariableOffsetManager::FunctionType VariableOffsetManager::GetFunctionType() const
{
	return m_functionType;
}

void VariableOffsetManager::Walk(double length)
{
	m_walkLength += length;
	if (m_functionType == constantLinear)
	{
		m_offsetValue += (m_deltaOffsetPerUnitLength * length);
	}
	else if (m_functionType == changingLinear)
	{
		m_offsetValue +=  (m_deltaOffsetPerUnitLength * length * m_initialDiameter*0.5 / m_offsetValue);
	}
}
void VariableOffsetManager::WalkBack (double length)
{
	m_walkLength -= length;
	if (m_functionType == constantLinear)
	{
		m_offsetValue -= (m_deltaOffsetPerUnitLength * length);
	}
	else if (m_functionType == changingLinear)
	{
		m_offsetValue -=  (m_deltaOffsetPerUnitLength * length * m_initialDiameter*0.5 / m_offsetValue);
	}
}
double VariableOffsetManager::GetNextOffsetValueWithoutApplying(double walkLength) const
{
	double offsetValue;
	if (m_functionType == constantLinear)
	{
		offsetValue = m_offsetValue + m_deltaOffsetPerUnitLength * walkLength;
	}
	else
	{
		offsetValue =  m_offsetValue + (m_deltaOffsetPerUnitLength * walkLength * m_initialDiameter*0.5 / m_offsetValue);	
	}
	return offsetValue;
}
double VariableOffsetManager::GetCurOffsetValue ()const
{
	return m_offsetValue;
}

void VariableOffsetManager::SpiralOffset(const std::vector<LineOrArc> &srcVec, std::vector<LineOrArc> &goalVec)
{
	try
	{
		// Digitise input
		DigitiseSrcVec (srcVec);
		// offset and reconstruct
		ReconstructOffsetLineOrArcVec (srcVec , goalVec);
	}
	catch (VException &e)
	{
		LogFileWriter::WriteUnknownExceptionLog (( IDS_VarSpiExc));
		LogFileWriter::WriteExceptionLog (e);
	}
	catch (std::exception &e)
	{
		LogFileWriter::WriteUnknownExceptionLog (( IDS_VarSpiExc));
		LogFileWriter::WriteExceptionLog (e);
	}
	catch (...)
	{
		LogFileWriter::WriteUnknownExceptionLog (( IDS_VarSpiUnkExc));
	}
}

void VariableOffsetManager::DigitiseSrcVec(const std::vector<LineOrArc> &srcVec)
{
	size_t mysize = srcVec.size();
	for (size_t i=0;i<mysize;++i)
	{
		m_seq.push_back (std::vector<VPoint>());
		m_nor.push_back (std::vector<VPoint2d>());
		const LineOrArc &geo = srcVec[i];
		std::vector<VPoint> digiVec;
		geo.DigitiseByLengthStep ( m_tol , digiVec);
		size_t mysize2 = digiVec.size();
		if (geo.GetType() == LineOrArc::line)
		{
			const VLine *l = geo.GetLine();
			VLine2d ll (VPoint2d (l->GetP1().x() , l->GetP1().y() ) , VPoint2d (l->GetP2().x() , l->GetP2().y()));
			VPoint2d ln = ll.GetNormal();
			ln.NormThis();
			for (size_t j=0; j<mysize2; ++j)
			{
				const VPoint &p = digiVec[j];
				m_seq.back().push_back (p);
				m_nor.back().push_back (ln);
			}
		}
		else
		{
			const VArc *a = geo.GetArc(); 
			bool isCCW = a->IsCCW();
			VPoint cen = a->GetCenter();
			for (size_t j=0;j<mysize2; ++j)
			{
				const VPoint &p = digiVec[j];
				m_seq.back().push_back (p);	
				VPoint nr (p - cen);
				nr.NormThis();
				if (isCCW)
					nr *= -1.0;
				m_nor.back().push_back ( VPoint2d (nr.x() , nr.y()));
			}
		}
	}
}

void VariableOffsetManager::ReconstructOffsetLineOrArcVec(const std::vector<LineOrArc> &srcVec , std::vector<LineOrArc> &goalVec)
{
	double cosThreshold = cos (20.0 * DEGREE_TO_RAD);
	size_t allvecsize = m_seq.size();
	std::vector<std::vector<VPoint>> allOfpoVec(allvecsize);
	if (m_seq.size() == 0 || m_seq.at(0).size() == 0 || m_nor.size() == 0 || m_nor.front().size() == 0)
		throw VException (( IDS_VarRecMSeqMNorE),1);
	VPoint prevp = m_seq.front().front();
	VPoint2d prevn = m_nor.front().front();
	VPoint firstp (prevp);
	VPoint2d firstn (prevn);
	double curoffset = GetCurOffsetValue();
	VPoint prevopi (prevp.x() + prevn.x() * curoffset , prevp.y() + prevn.y() * curoffset , prevp.z());
	if (IsColliding (srcVec , prevopi , curoffset , m_tol) == false)
		allOfpoVec.front().push_back (prevopi);
	
	for (size_t q=0;q<allvecsize;++q)
	{
		std::vector<VPoint> &seq = m_seq.at(q);
		std::vector<VPoint2d> &nor = m_nor.at(q);
		std::vector<VPoint> &ofpoVec = allOfpoVec.at(q);
		
		int mysize = (int)seq.size();
		for (int i=0;i<mysize;++i)
		{
			const VPoint &curP = seq.at(GetPeriodicIndex(i,mysize));
			const VPoint2d &curN = nor.at(GetPeriodicIndex(i,mysize));
			OnePointJob (srcVec , prevp , prevn , curP , curN , m_tol , ofpoVec , cosThreshold);
		}
	}

	bool isClosed = m_seq.back().back().Compare (m_seq.front().front() , m_tol);
	if (isClosed)
		OnePointJob (srcVec , prevp , prevn , firstp , firstn , m_tol , allOfpoVec.back() , cosThreshold);
	
	for (size_t w=0;w<allvecsize;++w)
	{
		std::vector<VPoint> &ofpoVec = allOfpoVec.at(w);
		if (m_doArcFit)
		{
			ArcFit afit;
			afit.FitRecursive (ofpoVec , m_tol , goalVec);
		}
		else
		{
			for (size_t k=1;k<ofpoVec.size();++k)
			{
				goalVec.push_back (VLine (ofpoVec[k-1] , ofpoVec[k]));
			}
		}
	}

}

void VariableOffsetManager::OnePointJob (const std::vector<LineOrArc> &srcVec , VPoint &prevp , VPoint2d &prevn , const VPoint &curP , const VPoint2d &curN , double tol , std::vector<VPoint> &ofpoVec , double cosThreshold)
{
	double curoffset = GetCurOffsetValue ();
	if (curP.Compare (prevp , tol*0.1) == false)
	{
		double walkLength = (curP - prevp).GetLength();
		Walk (walkLength);
		VPoint opoi (curP.x() + curN.x() * curoffset , curP.y() + curN.y() * curoffset , curP.z());
		if (IsColliding (srcVec , opoi , curoffset , tol) == false)
			ofpoVec.push_back (opoi);
		else
			WalkBack (walkLength);
	}
	else
	{
		// check normals
		if (prevn % curN < cosThreshold)
		{
			HandleSharpCorners (srcVec , tol , prevp , prevn , curP , curN , ofpoVec , curoffset);
		}
	}

	prevp = curP;
	prevn = curN;
}
void VariableOffsetManager::HandleSharpCorners (const std::vector<LineOrArc> &srcVec , double tol , const VPoint &prevp , const VPoint2d &prevn , const VPoint &curP , const VPoint2d &curN , std::vector<VPoint> &ofpoVec , double curoffset)
{
	VPoint2d midN = (prevn + curN)*0.5;
	midN.NormThis();
	curoffset = GetCurOffsetValue();
	// angle difference is bigger than 20 degrees so make a circle and digitise
	VPoint p1 (prevp.x() + prevn.x() * curoffset , prevp.y() + prevn.y() * curoffset , prevp.z());
	VPoint p2 (prevp.x() + midN.x() * curoffset , prevp.y() + midN.y() * curoffset , prevp.z());
	VPoint p3 (prevp.x() + curN.x() * curoffset , prevp.y() + curN.y() * curoffset , prevp.z());
	if (m_optSharpCorners == round)
	{				
		VArc a (p1,p2,p3);
		int digiNo = FastArcDigitiser::GetDigitiseSegmentNo (a , tol);
		if (ofpoVec.size())
			ofpoVec.pop_back();// remove prevp because it will be added once again !
		std::vector<VPoint> newPoints;
		FastArcDigitiser::Digitise (a , digiNo , newPoints);
		CollCheckAndAddIf (srcVec , newPoints , ofpoVec , curoffset , tol);
	}
	else if (m_optSharpCorners == straight)
	{
		//int i0 = ofpoVec.size()-2;
		//int i3 = i+1;
		//int mysizeO = (int)ofpoVec.size();
		//if (isClosed == false && (i0 < 0  ||  i3 > (mysize-1)))
		//	throw VException (("VariableOffsetManager::ReconstructOffsetLineOrArcVec exception , sharp corner on an unexpected position!"),1);
		//VPoint beforeSharpCorner = ofpoVec.at(GetPeriodicIndex (i0,mysizeO));
		//VPoint afterSharpCornerTouch = seq.at(GetPeriodicIndex(i3,mysize));
		//VPoint2d afterSharpCornerN = nor.at(GetPeriodicIndex(i3,mysize));
		//double nextoffset = GetNextOffsetValueWithoutApplying ((afterSharpCornerTouch - prevp).GetLength());
		//VPoint afterSharpCorner (afterSharpCornerTouch.x() + afterSharpCornerN.x() * nextoffset , afterSharpCornerTouch.y() + afterSharpCornerN.y() * nextoffset , afterSharpCornerTouch.z());
		//double t,s;
		//InfiniteLinesNearestPoints inp;
		//VPoint t1 = (p1 - beforeSharpCorner);
		//InfiniteLinesNearestPoints::info myck = inp.Calc (p1 , t1 , p3 , p3 - afterSharpCorner , t , s);
		//VPoint ip = p1 + t * t1;
		//LineOrArc l1 ( VLine (p1 , ip) );
		//LineOrArc l2 ( VLine (ip , p3));
		//ofpoVec.pop_back();
		//std::vector<VPoint> newPoints;
		//l1.DigitiseByLengthStep ( tol * 100 , newPoints);
		//ofpoVec.pop_back();
		//l2.DigitiseByLengthStep ( tol * 100 , newPoints);
		//CollCheckAndAddIf (srcVec , newPoints , ofpoVec , curoffset , tol);
	}	
}

void VariableOffsetManager::CollCheckAndAddIf (const std::vector<LineOrArc> &srcVec , const std::vector<VPoint> &newPoints , std::vector<VPoint> &resPoints , double curRadius , double collTol)
{
	size_t mysize = newPoints.size();
	for (size_t i=0;i<mysize;++i)
	{
		const VPoint &p = newPoints[i];
		if (IsColliding (srcVec , p,curRadius,collTol) == false)
			resPoints.push_back (p);
	}
}

VariableOffsetManager::OptSharpCorners VariableOffsetManager::GetOptSharpCorners()const
{
	return m_optSharpCorners;
}

void VariableOffsetManager::SetOptSharpCorners(VariableOffsetManager::OptSharpCorners optSharpCorners)
{
	m_optSharpCorners = optSharpCorners;
}

int VariableOffsetManager::GetPeriodicIndex (int i,int mysize)
{
	while (i < 0)
	{
		i+=mysize;	
	}
	
	while (i >= mysize)
	{
		i-=mysize;
	}
	return i;
}

bool VariableOffsetManager::IsColliding (const std::vector<LineOrArc> &srcVec , const VPoint &p , double curRadius,double collTol)
{
	//double myr2 = curRadius*curRadius;
	//size_t mysize = m_seq.size();
	//for (size_t i=0;i<mysize;++i)
	//{
	//	const VPoint &ckPoint = m_seq[i];
	//	if (myr2 - (ckPoint-p).GetLength2() > collTol)
	//		return true;
	//}
	//return false;
	double ckDist = fabs(curRadius) - collTol;
	VPoint ponGeo;
	size_t mysize = srcVec.size();
	for (size_t i=0;i<mysize;++i)
	{
		const LineOrArc &geo = srcVec[i];
		double mydis = geo.GetMinDist (p , ponGeo);
		if (mydis < ckDist)
			return true;
	}
	return false;
}

bool VariableOffsetManager::GetDoArcFit() const
{
	return m_doArcFit;
}

void VariableOffsetManager::SetDoArcFit (bool doArcFit)
{
	m_doArcFit = doArcFit;
}

void VariableOffsetManager::SetDeltaOffsetPerUnitLength (double deltaOffsetPerUnitLength)
{
	m_deltaOffsetPerUnitLength = deltaOffsetPerUnitLength;
}

double VariableOffsetManager::GetDeltaOffsetPerUnitLength()const
{
	return m_deltaOffsetPerUnitLength;
}

void VariableOffsetManager::SetInitialOffsetValue (double initialOffsetValue)
{
	m_initialOffsetValue = initialOffsetValue;
}

double VariableOffsetManager::GetInitialOffsetValue()const
{
	return m_initialOffsetValue;
}

void VariableOffsetManager::NegateOffsetValues()
{
	m_initialOffsetValue *= -1.;
	m_offsetValue *= -1.;
	m_deltaOffsetPerUnitLength *= -1.;
	m_initialDiameter *= -1.;
}

double VariableOffsetManager::GetWalkLength() const
{
	return m_walkLength;
}

void VariableOffsetManager::SetWalkLength(double walkLength)
{
	m_walkLength = walkLength;
}

double VariableOffsetManager::GetInitialDiameter()const
{
	return m_initialDiameter;
}

void VariableOffsetManager::SetInitialDiameter(double initialDiameter)
{
	m_initialDiameter = initialDiameter;
}

double VariableOffsetManager::GetTol() const
{
	return m_tol;
}

void VariableOffsetManager::SetTol(double tol) 
{
	m_tol = tol;
}
