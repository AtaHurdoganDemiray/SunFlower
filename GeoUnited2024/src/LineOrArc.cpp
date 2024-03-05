// LineOrArc.cpp

#include "stdafx.h"
#include "LineOrArc.h"
#include "VLine.h"
#include "VArc.h"
#include "VLine2d.h"
#include "PointSet2d.h"
#include "LineIntersection2d.h"
//#include "VCurve.h"
#include "VLine2dE.h"
#include "LineIntersection.h"
#include "OffsLineOrArc.h"
#include "TransformationMatrix.h"
#include "SequentialLineSet.h"
#include "LineSetsMinDist.h"
#include "MultiWireCollCheck.h"
#include "ArcPointGenerator.h"
#include "LinePointMinDist.h"
#include "LineSetsMinDist.h"
#include "LinePointMinDist.h"
#include "CoordinateSystem.h"
#include "VariableOffsetManager.h"
#include "FastArcDigitiser.h"
#include "PolarCalc2d.h"
#include "VInterval.h"
#include "RotationMatrix.h"
#include "LineLineMinDist.h"
#include "LAIntersectionResult.h"
#include "VCombinations.h"
#include "ContourCCWCheck.h"

bool LineOrArc::m_ckLineMinLength = true;
double LineOrArc::m_lineMinLengthAllowed = 0.001 * VGlobalValues::ONEMM_TO_VUNIT;
bool LineOrArc::m_ckLineMaxLength = true;
double LineOrArc::m_lineMaxLengthAllowed = 1.e6* VGlobalValues::ONEMM_TO_VUNIT;
bool LineOrArc::m_ckArcMinPerimeter = true;
double LineOrArc::m_minArcPerimeterAllowed = 0.001* VGlobalValues::ONEMM_TO_VUNIT;
bool LineOrArc::m_ckArcMaxPerimeter = true;
double LineOrArc::m_maxArcPerimeterAllowed = 1.e6* VGlobalValues::ONEMM_TO_VUNIT;
bool LineOrArc::m_ckArcRadius = true;
double LineOrArc::m_minArcRadius = 0.001* VGlobalValues::ONEMM_TO_VUNIT;
bool LineOrArc::m_ckStartVectorRadiusConformance = true;
double LineOrArc::m_startVectorRadiusConformanceTolerance = 0.02* VGlobalValues::ONEMM_TO_VUNIT;
bool LineOrArc::m_ckIsAxisUnit = true;
double LineOrArc::m_axisVectorUnitTolerance = 0.02* VGlobalValues::ONEMM_TO_VUNIT;
bool LineOrArc::m_ckIsAxisPerpToStartVector = true;
double LineOrArc::m_axisPerpToStartVectorTolerance = 0.02* VGlobalValues::ONEMM_TO_VUNIT;
bool LineOrArc::m_ckSweepAngle = true;
double LineOrArc::m_minSweepAngleTolerance = 0.001* VGlobalValues::ONEMM_TO_VUNIT;

LineOrArc::LineOrArc()
//:m_crv (0)
{
	VLine tmp (VPoint(0.),VPoint(1.));
	m_type = line;
	m_geo.m_l = new VLine (tmp);
}

LineOrArc::LineOrArc(const VArc &a)
{
	m_type = arc;
	m_geo.m_a = new VArc (a);
}

LineOrArc::LineOrArc(const VLine &l)
{
	m_type = line;
	m_geo.m_l = new VLine (l);
}

void LineOrArc::SetArc(const VArc &a)
{
	DeleteData();
	m_type = arc;
	m_geo.m_a = new VArc (a);
}

void LineOrArc::SetLine(const VLine &l)
{
	DeleteData();
	m_type = line;
	m_geo.m_l = new VLine (l);
}

void LineOrArc::DeleteData()
{
	if (m_type == line)
	{
		delete m_geo.m_l;
	}
	else
	{
		delete m_geo.m_a;
	}
}

LineOrArc::~LineOrArc()
{
	DeleteData();
}

//void LineOrArc::SetCurve(VCurve *crv)
//{
//	m_crv = crv;
//}
//
//VCurve * LineOrArc::GetCurve()
//{
//	return m_crv;
//}

LineOrArc::LineOrArc(const LineOrArc &other)
{
	if (other.GetType() == line)
	{
		const VLine *l = other.GetLine();
		if (l == 0)
			throw VException (( IDS_LinCopConExcLin),1);
		m_type = line;
		m_geo.m_l = new VLine (*l);
		//m_crv = other.m_crv;
	}
	else
	{
		const VArc *a = other.GetArc();
		m_type = arc;
		m_geo.m_a = new VArc (*a);
		//m_crv = other.m_crv;
	}
}

LineOrArc & LineOrArc::operator =(const LineOrArc &other)
{
	if (other.GetType() == line)
	{
		this->SetLine(*other.m_geo.m_l);
	}
	else
	{
		this->SetArc(*other.m_geo.m_a);
	}
	m_type = other.m_type;
	//m_crv = other.m_crv;
	return *this;
}

const VArc * LineOrArc::GetArc() const
{
	if (m_type == arc)
		return m_geo.m_a;
	else
		return 0;
}

const VLine * LineOrArc::GetLine() const
{
	if (m_type == line)
		return m_geo.m_l;
	else
		return 0;
}

LineOrArc::type LineOrArc::GetType() const
{
	return m_type;
}
bool LineOrArc::IsPointInLoop(const std::vector<LineOrArc> &loop , const VPoint &p , vreal tol , bool& isPointOnLoop  ,bool choiceOnSuspicion)
{
	isPointOnLoop = false;
	VPoint minp,maxp;
	GetMinMax(loop,minp,maxp);
	minp.ReplaceMin(p);
	maxp.ReplaceMax(p);
	double diagLength = (maxp - minp).GetLength();
	diagLength *= 2.0;// for safety
	VPoint outp[9];
	VArc a(VPoint (0.0,0.0,0.0),1,UZ,VPoint (1,0,0),360.0);
	int incount = 0;
	int outcount = 0;
	double angle = 0.;
	for (int i=0;i<9;++i)
	{
		VPoint dir = a.GetP(angle);
		angle += 40.0;
		VPoint outp = p + (diagLength * dir);
		VLine fake (p , outp);
		VLine2d fake2d (VPoint2d (p.x(),p.y()) , VPoint2d (outp.x(),outp.y()));

		int ck = IsPointInLoop(loop,p,fake,fake2d,tol);
		if (ck == 1)
			++incount;
		else if (ck == 0)// on the loop within tolerance
		{
			isPointOnLoop = true;
			break;
		}
		else
			++outcount;

		if (abs(incount-outcount)==3)
			break;
	}

	if (incount == outcount || isPointOnLoop == true)
	{
		return choiceOnSuspicion;
		//throw VException((IDS_LinIsPExcCanNot), 1);
	}
	if (incount>outcount)
		return true;
	else
		return false;
}
bool LineOrArc::IsPointInLoop(const std::list<LineOrArc>& loop, const VPoint& p)
{
	VPoint minp, maxp;
	GetMinMax(loop, minp, maxp);
	minp.ReplaceMin(p);
	maxp.ReplaceMax(p);
	double diagLength = (maxp - minp).GetLength();
	diagLength += 10.0;
	VPoint outp[9];
	VArc a(VPoint(0.0, 0.0, 0.0), 1, UZ, VPoint(1, 0, 0), 360.0);
	int incount = 0;
	int outcount = 0;
	double angle = 0.;
	for (int i = 0; i < 9; ++i)
	{
		VPoint dir = a.GetP(angle);
		angle += 40.0;
		VPoint outp = p + (diagLength * dir);
		VLine fake(p, outp);
		VLine2d fake2d(VPoint2d(p.x(), p.y()), VPoint2d(outp.x(), outp.y()));

		bool ck = IsPointInLoop(loop, p, fake, fake2d);
		if (ck)
			++incount;
		else
			++outcount;

		if (abs(incount - outcount) == 3)
			break;
	}

	if (incount == outcount)
		throw VException((IDS_LinIsPExcCanNot), 1);

	if (incount > outcount)
		return true;
	else
		return false;
}

int LineOrArc::IsPointInLoop (const std::vector<LineOrArc> &loop , const VPoint &p,const VLine &fake ,const VLine2d &fake2d , vreal tol)
{
	PointSet2d rec;
	int intno = 0;

	size_t mysize = loop.size();
	for (size_t i=0;i<mysize;++i)
	{
		const LineOrArc &curg = loop[i];
		if (curg.GetType()==LineOrArc::line)
		{
			const VLine *currline = curg.GetLine();
			if (currline == 0)
				throw VException (( IDS_LinIsPExcLinIsN),1);

			VLine2d currline2d (VPoint2d (currline->GetP1().x(),currline->GetP1().y()) , VPoint2d(currline->GetP2().x(),currline->GetP2().y()));
			LineIntersection2d::LineIntersectionData2d info 
				= LineIntersection2d::CalcInt ( fake2d , currline2d );
			
			if ( info.intinfo == LineIntersection2d::LINESINTERSECT )
			{
				if(info.intno == 1)
				{
					int ind;
					if ((info.intpoint[0] - fake2d.GetP1()).GetLength() < tol)
						return 0;// point is on loop
					bool ck = rec.GetIndex (info.intpoint[0] , ind , tol);
					if (ck==false)
					{
						intno++;	
						rec.InsertPoint(info.intpoint[0]);
					}
				}
			}
			else if(info.intinfo == LineIntersection2d::LINESPARALLEL_INTERSECTING)
			{
				vreal tmpt;
				if (currline->IsOnLine(fake.GetP1(),tmpt,tol))
					return 0;// point on loop
			}
		}
		else
		{
			// this is arc find intersections
			const VArc *a = curg.GetArc();
			if (a->IsPointOnArc(fake.GetP1(), tol))
				return 0; // point on loop
			if (a == 0)
				throw VException (( IDS_LinIsPExcArcIsN),1);

			std::vector<VPoint> ivec = a->GetInt(fake);
			for (size_t q = 0;q<ivec.size();++q)
			{
				VPoint2d ppp (ivec[q].x(),ivec[q].y());
				int ind;
				bool ck = rec.GetIndex(ppp,ind,1.e-5);
				if (ck == false)
				{
					intno ++;
					rec.InsertPoint(ppp);
				}
			}
		}
	}
	if (intno % 2 == 1)
		return 1;// true;   inside
	else
		return 2;// false;		outside
}
bool LineOrArc::IsPointInLoop(const std::list<LineOrArc>& loop, const VPoint& p, const VLine& fake, const VLine2d& fake2d)
{



	PointSet2d rec;
	int intno = 0;

	//size_t mysize = loop.size();
	for (auto i = loop.begin(); i != loop.end(); ++i)
	{
		const LineOrArc& curg = *i;//loop[i];
		if (curg.GetType() == LineOrArc::line)
		{
			const VLine* currline = curg.GetLine();
			if (currline == 0)
				throw VException((IDS_LinIsPExcLinIsN), 1);

			VLine2d currline2d(VPoint2d(currline->GetP1().x(), currline->GetP1().y()), VPoint2d(currline->GetP2().x(), currline->GetP2().y()));
			LineIntersection2d::LineIntersectionData2d info
				= LineIntersection2d::CalcInt(fake2d, currline2d);
			if (info.intinfo == LineIntersection2d::LINESINTERSECT)
			{
				if (info.intno == 1)
				{
					int ind;
					bool ck = rec.GetIndex(info.intpoint[0], ind, 1.e-5);
					if (ck == false)
					{
						intno++;
						rec.InsertPoint(info.intpoint[0]);
					}
				}
			}
		}
		else
		{
			// this is arc find intersections
			const VArc* a = curg.GetArc();
			if (a == 0)
				throw VException((IDS_LinIsPExcArcIsN), 1);

			std::vector<VPoint> ivec = a->GetInt(fake);
			for (size_t q = 0; q < ivec.size(); ++q)
			{
				VPoint2d ppp(ivec[q].x(), ivec[q].y());
				int ind;
				bool ck = rec.GetIndex(ppp, ind, 1.e-5);
				if (ck == false)
				{
					intno++;
					rec.InsertPoint(ppp);
				}
			}
		}
	}
	if (intno % 2 == 1)
		return true;
	else
		return false;
}
bool LineOrArc::IsPointInLoop(const std::vector<LineOrArc *> &loop , const VPoint &p)
{
	VPoint minp,maxp;
	GetMinMax(loop,minp,maxp);
	minp.ReplaceMin(p);
	maxp.ReplaceMax(p);
	double diagLength = (maxp - minp).GetLength();
	diagLength += 10.0;
	VPoint outp[12];
	VArc a(VPoint (0.0,0.0,0.0),1,UZ,VPoint (1,0,0),360.0);
	int incount = 0;
	int outcount = 0;
	double angle = 0.;
	for (int i=0;i<12;++i)
	{
		VPoint dir = a.GetP(angle);
		angle += 30.0;
		VPoint outp = p + (diagLength * dir);
		VLine fake (p , outp);
		VLine2d fake2d (VPoint2d (p.x(),p.y()) , VPoint2d (outp.x(),outp.y()));

		bool ck = IsPointInLoop(loop,p,fake,fake2d);
		if (ck)
			++incount;
		else
			++outcount;

		if (abs(incount-outcount)==3)
			break;
	}

	if (incount == outcount)
		throw VException (( IDS_LinIsPExcCanNot),1);

	if (incount>outcount)
		return true;
	else
		return false;
}

bool LineOrArc::IsPointInLoop (const std::vector<LineOrArc *> &loop , const VPoint &p,const VLine &fake ,const VLine2d &fake2d)
{
	PointSet2d rec;
	int intno = 0;

	size_t mysize = loop.size();
	for (size_t i=0;i<mysize;++i)
	{
		const LineOrArc *curg = loop[i];
		if (curg->GetType()==LineOrArc::line)
		{
			const VLine *currline = curg->GetLine();
			if (currline == 0)
				throw VException (( IDS_LinIsPExcLinIsN),1);

			VLine2d currline2d (VPoint2d (currline->GetP1().x(),currline->GetP1().y()) , VPoint2d(currline->GetP2().x(),currline->GetP2().y()));
			LineIntersection2d::LineIntersectionData2d info 
				= LineIntersection2d::CalcInt ( fake2d , currline2d );	
			if ( info.intinfo == LineIntersection2d::LINESINTERSECT )
			{
				if(info.intno == 1)
				{
					int ind;
					bool ck = rec.GetIndex (info.intpoint[0] , ind , 1.e-5);
					if (ck==false)
					{
						intno++;	
						rec.InsertPoint(info.intpoint[0]);
					}
				}
			}
		}
		else
		{
			// this is arc find intersections
			const VArc *a = curg->GetArc();
			if (a == 0)
				throw VException (( IDS_LinIsPExcArcIsN),1);

			std::vector<VPoint> ivec = a->GetInt(fake);
			for (size_t q = 0;q<ivec.size();++q)
			{
				VPoint2d ppp (ivec[q].x(),ivec[q].y());
				int ind;
				bool ck = rec.GetIndex(ppp,ind,1.e-5);
				if (ck == false)
				{
					intno ++;
					rec.InsertPoint(ppp);
				}
			}
		}
	}
	if (intno % 2 == 1)
		return true;
	else
		return false;
}

bool LineOrArc::IsCCW (const std::vector<LineOrArc> &vec)
{
	vreal tol = 0.0001;
	ContourCCWCheck mychecker(&vec, tol);
	switch (mychecker.IsLoopCCW())
	{
	case ContourCCWCheck::CCW:
		return true;
		break;
	case ContourCCWCheck::CW:
		return false;
		break;
	case ContourCCWCheck::Linear:
		throw VException("LineOrArc::IsCCW exception. Open and linear loops are not handled yet.",1);
		break;
	case ContourCCWCheck::Sshaped:
		throw VException("LineOrArc::IsCCW exception. Open and S-shaped loops are not handled yet.", 1);
		break;
	}
	throw VException("LineOrArc::IsCCW exception. ContourCCWCheck result is none.",1);
	//// This functions assume vec is on XY plane and plane normal is UZ !!!
	//if (vec.size() == 0)
	//{
	//	throw VException((IDS_LinIsCExcEmpVec), 1);
	//}

	//int geoSize = (int)vec.size();

	//VPoint prevTan = vec.at(0).GetEndTangent();
	//VPoint nextTan;
	//vreal tol = 1.e-6;
	//VPoint nullvector(0.0, 0.0, 0.0);
	//std::vector<int> positiveGeoRefVec, negativeGeoRefVec;
	//VPoint midTan0 = vec.at(0).GetTan(0.5);
	//VPoint midTan1 = vec.at(0).GetTan(0.5 + tol);
	//VPoint cp = midTan0 ^ midTan1;
	//if (cp.Compare(nullvector, tol) == false)
	//{
	//	if (cp % UZ > 0.0)
	//		positiveGeoRefVec.push_back(0);
	//	else
	//		negativeGeoRefVec.push_back(0);
	//}
	//for (int gi = 1; gi < geoSize; ++gi)
	//{
	//	nextTan = vec.at(gi).GetStartTangent();
	//	VPoint crosspro = prevTan ^ nextTan;
	//	if (crosspro.Compare(nullvector, tol) == false)
	//	{
	//		if (crosspro % UZ > 0.0)
	//			positiveGeoRefVec.push_back(gi);
	//		else
	//			negativeGeoRefVec.push_back(gi);
	//	}

	//	VPoint midTan0 = vec.at(gi).GetTan(0.5);
	//	VPoint midTan1 = vec.at(gi).GetTan(0.5 + tol);
	//	VPoint cp = midTan0 ^ midTan1;
	//	if (cp.Compare(nullvector, tol) == false)
	//	{
	//		if (cp % UZ > 0.0)
	//			positiveGeoRefVec.push_back(gi);
	//		else
	//			negativeGeoRefVec.push_back(gi);
	//	}

	//	prevTan = vec.at(gi).GetEndTangent();
	//}

	//if (IsClosed(vec, tol))
	//{
	//	nextTan = vec.at(0).GetStartTangent();
	//	VPoint crosspro = prevTan ^ nextTan;
	//	if (crosspro.Compare(nullvector, tol) == false)
	//	{
	//		if (crosspro % UZ > 0.0)
	//			positiveGeoRefVec.push_back(geoSize - 1);
	//		else
	//			negativeGeoRefVec.push_back(geoSize - 1);
	//	}
	//	if (negativeGeoRefVec.size() == 0)
	//		return true;
	//	if (positiveGeoRefVec.size() == 0)
	//		return false;
	//	// there are convex and concave elements so it is hard to decide 
	//	// we need  is in loop check for some positions then decide by this data
	//	VPoint midTan0 = vec.at(0).GetTan(0.5);
	//	VPoint midNor0 = UZ ^ midTan0;
	//	VPoint midp = vec.at(0).GetP(0.5);
	//	bool isPointOnLoop;
	//	bool choiceOnSuspicion = true;

	//	if (LineOrArc::IsPointInLoop(vec, midp + 0.01 * midNor0, 0.01 ,isPointOnLoop, choiceOnSuspicion))
	//		return true;
	//	else
	//		return false;
	//}
	//else
	//{
	//	if (negativeGeoRefVec.size() == 0)
	//		return true;
	//	if (positiveGeoRefVec.size() == 0)
	//		return false;
	//	throw VException("LineOrArc::IsCCW exception. Open loop with S shape topology. IsCCW can not find solution.", 1);
	//}

	//VLine l;
	//const LineOrArc *g1 = 0;
	//const LineOrArc * ageo = 0;
	//for (size_t j=0;j<vec.size();++j)
	//{
	//	const LineOrArc &curgeo = vec.at(j);
	//	if (curgeo.GetType() == line)
	//	{
	//		g1 = &curgeo;
	//		break;
	//	}
	//	else
	//	{
	//		ageo = &curgeo;
	//		break;
	//	}
	//}
	//if (g1 == 0)
	//	g1 = ageo;
	//if (g1->GetType() == LineOrArc::line)
	//{
	//	const VLine *myl = g1->GetLine();
	//	if (myl == 0)
	//		throw VException (( IDS_LinIsCExcNulLin),1);
	//	l = *myl;
	//}
	//else 
	//{
	//	const VArc *mya = g1->GetArc();
	//	if (mya == 0)
	//		throw VException (( IDS_LinIsCExcNulArc),1);
	//	
	//	VPoint tanVec0 = mya->GetTan(0.0);
	//	VPoint p0 = mya->GetStartPoint();
	//	l = VLine (p0 , p0+tanVec0);
	//}
	//if (l.GetLength() < EPSILON)
	//	throw VException (( IDS_LinIsCExcLinLen),1);

	//VLine2d ll (VPoint2d(l.GetP1().x(),l.GetP1().y()),VPoint2d(l.GetP2().x(),l.GetP2().y()));
	//VPoint2d n = ll.GetNormal();

	//VVector N = VPoint (n.x(),n.y(),0.0);
	//VPoint ppp = l.GetP(0.5) + N * 6.e-2*VGlobalValues::ONEMM_TO_VUNIT;
	//bool isPointOnLoop;
	//bool inIfSuspicion = true;
	//if (IsPointInLoop(vec,ppp,VGlobalValues::ONEMM_TO_VUNIT*1.e-3,isPointOnLoop,inIfSuspicion))
	//	return true;
	//else
	//	return false;
}
bool LineOrArc::IsCCW (const std::vector<LineOrArc*> &vec)
{
	std::vector<LineOrArc> tmp;
	for (size_t i = 0; i < vec.size(); ++i)
	{
		tmp.push_back(*vec.at(i));
	}
	vreal tol = 0.0001;
	ContourCCWCheck mychecker(&tmp, tol);
	switch (mychecker.IsLoopCCW())
	{
	case ContourCCWCheck::CCW:
		return true;
		break;
	case ContourCCWCheck::CW:
		return false;
		break;
	case ContourCCWCheck::Linear:
		throw VException("LineOrArc::IsCCW exception. Open and linear loops are not handled yet.", 1);
		break;
	case ContourCCWCheck::Sshaped:
		throw VException("LineOrArc::IsCCW exception. Open and S-shaped loops are not handled yet.", 1);
		break;
	}
	throw VException("LineOrArc::IsCCW exception. ContourCCWCheck result is none.", 1);
	//// This functions assume vec is on XY plane and plane normal is UZ !!!
	//if (vec.size() == 0)
	//{
	//	throw VException((IDS_LinIsCExcEmpVec), 1);
	//}

	//int geoSize = (int)vec.size();

	//VPoint prevTan = vec.at(0)->GetEndTangent();
	//VPoint nextTan;
	//vreal tol = 1.e-6;
	//VPoint nullvector(0.0, 0.0, 0.0);
	//std::vector<int> positiveGeoRefVec,negativeGeoRefVec;
	//VPoint midTan0 = vec.at(0)->GetTan(0.5);
	//VPoint midTan1 = vec.at(0)->GetTan(0.5 + tol);
	//VPoint cp = midTan0 ^ midTan1;
	//if (cp.Compare(nullvector, tol) == false)
	//{
	//	if (cp % UZ > 0.0)
	//		positiveGeoRefVec.push_back(0);
	//	else
	//		negativeGeoRefVec.push_back(0);
	//}
	//for (int gi = 1; gi < geoSize; ++gi)
	//{
	//	nextTan = vec.at(gi)->GetStartTangent();
	//	VPoint crosspro = prevTan ^ nextTan;
	//	if (crosspro.Compare(nullvector, tol) == false)
	//	{
	//		if (crosspro % UZ > 0.0)
	//			positiveGeoRefVec.push_back(gi);
	//		else
	//			negativeGeoRefVec.push_back(gi);
	//	}

	//	VPoint midTan0 = vec.at(gi)->GetTan(0.5);
	//	VPoint midTan1 = vec.at(gi)->GetTan(0.5 + tol);
	//	VPoint cp = midTan0 ^ midTan1;
	//	if (cp.Compare(nullvector, tol) == false)
	//	{
	//		if (cp % UZ > 0.0)
	//			positiveGeoRefVec.push_back(gi);
	//		else
	//			negativeGeoRefVec.push_back(gi);
	//	}

	//	prevTan = vec.at(gi)->GetEndTangent();
	//}
	//
	//if (IsClosed(vec,tol))
	//{
	//	nextTan = vec.at(0)->GetStartTangent();
	//	VPoint crosspro = prevTan ^ nextTan;
	//	if (crosspro.Compare(nullvector, tol) == false)
	//	{
	//		if (crosspro % UZ > 0.0)
	//			positiveGeoRefVec.push_back(geoSize-1);
	//		else
	//			negativeGeoRefVec.push_back(geoSize-1);
	//	}
	//	if (negativeGeoRefVec.size() == 0)
	//		return true;
	//	if (positiveGeoRefVec.size() == 0)
	//		return false;
	//	// there are convex and concave elements so it is hard to decide 
	//	// we need  is in loop check for some positions then decide by this data
	//	VPoint midTan0 = vec.at(0)->GetTan(0.5);
	//	VPoint midNor0 = UZ ^ midTan0;
	//	VPoint midp = vec.at(0)->GetP(0.5);
	//	bool isPointOnLoop;
	//	bool choiceOnSuspicion = true;
	//
	//	std::vector<LineOrArc> tmp;
	//	for (auto gii = vec.begin(); gii != vec.end(); ++gii)
	//		tmp.push_back(*(*gii));
	//	if (LineOrArc::IsPointInLoop(tmp,midp+0.01*midNor0,0.01,isPointOnLoop,choiceOnSuspicion))
	//		return true;
	//	else
	//		return false;
	//}
	//else
	//{
	//	if (negativeGeoRefVec.size() == 0)
	//		return true;
	//	if (positiveGeoRefVec.size() == 0)
	//		return false;
	//	throw VException("LineOrArc::IsCCW exception. Open loop with S shape topology. IsCCW can not find solution.",1);
	//}


	////if (vec.size() == 0)
	////{
	////	throw VException (( IDS_LinIsCExcEmpVec),1);
	////}
	//////const VCurve *crv = vec.front().GetCurve();
	////
	//////if (crv->GetLines().Size() == 0)
	//////	throw VException (("LineOrArc::IsCCW exception , lines empty!"),1);
	//////VLine l = crv->GetLines()[0];	
	////VLine l;
	////const LineOrArc *g1 = 0;
	////const LineOrArc * ageo = 0;
	////for (size_t j=0;j<vec.size();++j)
	////{
	////	const LineOrArc *curgeo = vec.at(j);
	////	if (curgeo->GetType() == line)
	////	{
	////		g1 = curgeo;
	////		break;
	////	}
	////	else
	////	{
	////		ageo = curgeo;
	////	}
	////}
	////if (g1 == 0)
	////	g1 = ageo;
	////if (g1->GetType() == LineOrArc::line)
	////{
	////	const VLine *myl = g1->GetLine();
	////	if (myl == 0)
	////		throw VException (( IDS_LinIsCExcNulLin),1);
	////	l = *myl;
	////}
	////else 
	////{
	////	const VArc *mya = g1->GetArc();
	////	if (mya == 0)
	////		throw VException (( IDS_LinIsCExcNulArc),1);
	////	//VPoint p1 = mya->GetStartPoint();
	////	//double smalldelta = 2.;
	////	//if (mya->GetSweepAngle() < 0.)
	////	//	smalldelta *= -1.;
	////	//VPoint p2 = mya->GetP (smalldelta);
	////	//l = VLine (p1,p2);
	////	std::vector<VPoint> tmpP;
	////	FastArcDigitiser::Digitise (*mya , 360 , tmpP);
	////	l = VLine (tmpP.at(0) , tmpP.at(1));
	////}
	////if (l.GetLength() < EPSILON)
	////	throw VException (( IDS_LinIsCExcLinLen),1);

	////VLine2d ll (VPoint2d(l.GetP1().x(),l.GetP1().y()),VPoint2d(l.GetP2().x(),l.GetP2().y()));
	////VPoint2d n = ll.GetNormal();

	////VVector N = VPoint (n.x(),n.y(),0.0);
	////VPoint ppp = l.GetP(0.5) + N * 6.e-2;
	////if (IsPointInLoop(vec,ppp))
	////	return true;
	////else
	////	return false;
}
//const VCurve * LineOrArc::GetCurve() const
//{
//	return m_crv;
//}

void LineOrArc::ReverseIf(bool makeccw, std::vector<LineOrArc> &loop,vreal tol)
{
	bool isClosed = IsClosed(loop, tol);
	if (isClosed == false)
		return;
	bool isccw = LineOrArc::IsCCW(loop);
	if (isccw != makeccw)
	{
		Reverse(loop);
		//size_t mysize = loop.size();
		//for (size_t j=0;j<mysize;++j)
		//{
		//	LineOrArc &g = loop[j];
		//	if (g.GetType()==LineOrArc::line)
		//	{
		//		g.SetLine (g.GetLine()->GetReversed());
		//		//g.GetCurve()->Reverse();
		//	}
		//	else
		//	{
		//		g.SetArc(g.GetArc()->GetReversed());
		//		//g.GetCurve()->Reverse();
		//	}
		//}
		//// Order should be reversed last to be first and first to be last 
		//std::vector<LineOrArc> rloop;
		//mysize--;
		//for (size_t j=mysize; j>0;j--)
		//{
		//	rloop.push_back(loop[j]);
		//}
		//rloop.push_back(loop[0]);
		//loop = rloop;
	}
}
void LineOrArc::ReverseIf(bool makeccw, std::vector<LineOrArc *> &loop,vreal tol)
{
	bool isClosed = LineOrArc::IsClosed(loop, tol);
	if (isClosed == false)
		return;
	bool isccw = LineOrArc::IsCCW(loop);
	if (isccw != makeccw)
	{
		Reverse(loop);
	}
}
void LineOrArc::Reverse ()
{
	if (GetType() == line)
	{
		SetLine(GetLine()->GetReversed());
	}
	else
	{
		SetArc(GetArc()->GetReversed());
	}
}

void LineOrArc::Reverse (std::vector<LineOrArc> &vec)
{
	size_t mysize = vec.size();
	for (size_t j=0;j<mysize;++j)
	{
		LineOrArc &g = vec[j];
		g.Reverse();
	}
	// order should be reversed
	std::vector<LineOrArc> rloop;
	int bsize = (int)mysize - 1;
	for (int j=bsize; j>= 0;j--)
	{
		rloop.push_back(vec[j]);
	}
	vec = rloop;
}
void LineOrArc::Reverse (std::vector<LineOrArc *> &vec)
{
	size_t mysize = vec.size();
	for (size_t j=0;j<mysize;++j)
	{
		LineOrArc *g = vec[j];
		g->Reverse();
	}
	// order should be reversed
	std::vector<LineOrArc *> rloop;
	int bsize = (int)mysize - 1;
	for (int j=bsize; j>= 0;j--)
	{
		rloop.push_back(vec[j]);
	}
	vec = rloop;
}
void LineOrArc::Offset (const std::vector<LineOrArc> &vec , std::vector<OffsLineOrArc> &ofseted , double offsetval  , double joinTol)
{
	if (vec.size() == 0)
		return;

	bool isclosed;
	//if (vec.size() == 1)
	//	isclosed = false;
	//else
		isclosed = vec.front().GetStartPoint ().Compare (vec.back().GetEndPoint() , joinTol) ? true : false;
	
	std::vector<VPoint> lp;
	// First offset every item in the vector to the list 
	size_t mysizei = vec.size();

	for (size_t i = 0;i<mysizei;++i)
	{
		const LineOrArc &g = vec[i];
		OffsLineOrArc rog;
		bool ck = g.RoughOffsetInXY(offsetval,rog);
		//if (ck) ???
		//{
			rog.SetParenti ((int)i);
			ofseted.push_back(rog);
			lp.push_back(g.GetEndPoint());
		//}
	}

	// Trim intersections add arc between open edges
	if (ofseted.size() > 1)
	{
		size_t mysize = ofseted.size();
		size_t curi = 0;
		size_t nexti;
		while (curi < mysize)
		{
			if (isclosed == false)
			{
				if (curi == mysize -1)
					break; // No geometry left to trim to
			}
			nexti = curi + 1;
			OffsLineOrArc *g1 = &ofseted.at(curi);
			int startNeighbori = (int)curi;
			if (g1->IsNull())
			{
				curi = nexti;
				continue;
			}

			VPoint ep = lp.at(curi);
			OffsLineOrArc *g2 = &ofseted.at(nexti%mysize);
			int endNeighbori = (int)(nexti%mysize);
			std::vector<OffsLineOrArc *> nullGeoVec;
			
			while (g2->IsNull())
			{
				nullGeoVec.push_back(g2);
				nexti++;
				if (isclosed == false)
				{
					if (nexti == mysize)
						break; // No geometry left to trim to 
				}
				g2 = &ofseted.at(nexti%mysize);
				int endNeighbori = (int)(nexti%mysize);
			}

			g1->GetGeo().TrimBothIf(g2->GetGeo(),joinTol);
			
			size_t mynullsize = nullGeoVec.size();
			for (size_t k=0;k<mynullsize;++k)
			{
				nullGeoVec.at(k)->SetNullPoint(g1->GetGeo().GetEndPoint());
			}

		
			if (g1->GetGeo().GetEndPoint().Compare(g2->GetGeo().GetStartPoint() , joinTol) == false)
			{
				double mylocalOffset = (g1->GetGeo().GetEndPoint() - ep).GetLength();
				// start and end points are not coincident so add an arc between them
				VArc newa (ep , mylocalOffset , UZ , g1->GetGeo().GetEndPoint() , 360.0);
				double myangle = newa.GetAngle(g2->GetGeo().GetStartPoint());
				if (myangle > 180.0)
					myangle -= 360.0;
				VArc newa2(ep , newa.GetRadius() , UZ , g1->GetGeo().GetEndPoint() , myangle);
				LineOrArc newgeo;
				newgeo.SetArc(newa2);
				g1->SetNextGeo(newgeo);
				g1->SetNextStartNeighbori (startNeighbori);
				g1->SetNextEndNeighbori (endNeighbori);
				g1->SetEndNeighbori (-endNeighbori);
				g2->SetStartNeighbori (-startNeighbori);
			}
			else
			{
				g1->SetEndNeighbori (-endNeighbori);
				g2->SetStartNeighbori (-startNeighbori);
			}
			curi = nexti;
		}
	}
	// Finally copy result to ofseted vector
	//std::list<LineOrArc>::iterator i = l.begin();
	//std::list<LineOrArc>::iterator e = l.end();
	//for (i;i!=e;++i)
	//{
	//	ofseted.push_back(*i);
	//}
	//i = additionalarcs.begin();
	//e = additionalarcs.end();
	//for (i;i!=e;++i)
	//	ofseted.push_back(*i);
}
void LineOrArc::Offset (const std::vector<LineOrArc *> &vec , std::vector<OffsLineOrArc> &ofseted , double offsetval , double joinTol)
{
	if (vec.size() == 0)
		return;

	bool isclosed;
	//if (vec.size() == 1)
	//	isclosed = false;
	//else
		isclosed = vec.front()->GetStartPoint ().Compare (vec.back()->GetEndPoint() , joinTol) ? true : false;
	
	std::vector<VPoint> lp;
	// First offset every item in the vector to the list 
	size_t mysizei = vec.size();

	for (size_t i = 0;i<mysizei;++i)
	{
		const LineOrArc *g = vec[i];
		OffsLineOrArc rog;
		bool ck = g->RoughOffsetInXY(offsetval,rog);
		//if (ck) ???
		//{
			rog.SetParenti ((int)i);
			ofseted.push_back(rog);
			lp.push_back(g->GetEndPoint());
		//}
	}

	// Trim intersections add arc between open edges
	if (ofseted.size() > 1)
	{
		size_t mysize = ofseted.size();
		size_t curi = 0;
		size_t nexti;
		while (curi < mysize)
		{
			if (isclosed == false)
			{
				if (curi == mysize -1)
					break; // No geometry left to trim to
			}
			nexti = curi + 1;
			OffsLineOrArc *g1 = &ofseted.at(curi);
			int startNeighbori = (int)curi;
			if (g1->IsNull())
			{
				curi = nexti;
				continue;
			}

			VPoint ep = lp.at(curi);
			OffsLineOrArc *g2 = &ofseted.at(nexti%mysize);
			int endNeighbori = (int)(nexti%mysize);
			std::vector<OffsLineOrArc *> nullGeoVec;
			
			while (g2->IsNull())
			{
				nullGeoVec.push_back(g2);
				nexti++;
				if (isclosed == false)
				{
					if (nexti == mysize)
						break; // No geometry left to trim to 
				}
				g2 = &ofseted.at(nexti%mysize);
				int endNeighbori = (int)(nexti%mysize);
			}

			g1->GetGeo().TrimBothIf(g2->GetGeo(),joinTol);
			
			size_t mynullsize = nullGeoVec.size();
			for (size_t k=0;k<mynullsize;++k)
			{
				nullGeoVec.at(k)->SetNullPoint(g1->GetGeo().GetEndPoint());
			}

		
			if (g1->GetGeo().GetEndPoint().Compare(g2->GetGeo().GetStartPoint() , joinTol) == false)
			{
				double mylocalOffset = (g1->GetGeo().GetEndPoint() - ep).GetLength();
				// start and end points are not coincident so add an arc between them
				VArc newa (ep , mylocalOffset , UZ , g1->GetGeo().GetEndPoint() , 360.0);
				double myangle = newa.GetAngle(g2->GetGeo().GetStartPoint());
				if (myangle > 180.0)
					myangle -= 360.0;
				VArc newa2(ep , newa.GetRadius() , UZ , g1->GetGeo().GetEndPoint() , myangle);
				LineOrArc newgeo;
				newgeo.SetArc(newa2);
				g1->SetNextGeo(newgeo);
				g1->SetNextStartNeighbori (startNeighbori);
				g1->SetNextEndNeighbori (endNeighbori);
				g1->SetEndNeighbori (-endNeighbori);
				g2->SetStartNeighbori (-startNeighbori);
			}
			else
			{
				g1->SetEndNeighbori (-endNeighbori);
				g2->SetStartNeighbori (-startNeighbori);
			}
			curi = nexti;
		}
	}
}
void LineOrArc::OffsetByExtendingElements (const std::vector<LineOrArc> &vec , std::vector<OffsLineOrArc> &ofseted , double offsetval , double joinTol)
{
	if (vec.size() == 0)
		return;

	bool isclosed;
	//if (vec.size() == 1)
	//	isclosed = false;
	//else
		isclosed = vec.front().GetStartPoint ().Compare (vec.back().GetEndPoint() , joinTol) ? true : false;
	
	std::vector<VPoint> lp;
	// First offset every item in the vector to the list 
	size_t mysizei = vec.size();

	for (size_t i = 0;i<mysizei;++i)
	{
		const LineOrArc &g = vec[i];
		OffsLineOrArc rog;
		bool ck = g.RoughOffsetInXY(offsetval,rog);
		//if (ck) ???
		//{
			rog.SetParenti ((int)i);
			ofseted.push_back(rog);
			lp.push_back(g.GetEndPoint());
		//}
	}

	// Trim intersections add arc between open edges
	if (ofseted.size() > 1)
	{
		size_t mysize = ofseted.size();
		size_t curi = 0;
		size_t nexti;
		while (curi < mysize)
		{
			if (isclosed == false)
			{
				if (curi == mysize -1)
					break; // No geometry left to trim to
			}
			nexti = curi + 1;
			OffsLineOrArc *g1 = &ofseted.at(curi);
			int startNeighbori = (int)curi;
			if (g1->IsNull())
			{
				curi = nexti;
				continue;
			}

			VPoint ep = lp.at(curi);
			OffsLineOrArc *g2 = &ofseted.at(nexti%mysize);
			int endNeighbori = (int)(nexti%mysize);
			std::vector<OffsLineOrArc *> nullGeoVec;
			
			while (g2->IsNull())
			{
				nullGeoVec.push_back(g2);
				nexti++;
				if (isclosed == false)
				{
					if (nexti == mysize)
						break; // No geometry left to trim to 
				}
				g2 = &ofseted.at(nexti%mysize);
				int endNeighbori = (int)(nexti%mysize);
			}

			g1->GetGeo().TrimOrExtendTo (g2->GetGeo(),joinTol);//TrimBothIf(g2->GetGeo(),joinTol);
			
			size_t mynullsize = nullGeoVec.size();
			for (size_t k=0;k<mynullsize;++k)
			{
				nullGeoVec.at(k)->SetNullPoint(g1->GetGeo().GetEndPoint());
			}

		
			if (g1->GetGeo().GetEndPoint().Compare(g2->GetGeo().GetStartPoint() , joinTol) == false)
			{
				double mylocalOffset = (g1->GetGeo().GetEndPoint() - ep).GetLength();
				// start and end points are not coincident so add an arc between them
				VArc newa (ep , mylocalOffset , UZ , g1->GetGeo().GetEndPoint() , 360.0);
				double myangle = newa.GetAngle(g2->GetGeo().GetStartPoint());
				if (myangle > 180.0)
					myangle -= 360.0;
				VArc newa2(ep , newa.GetRadius() , UZ , g1->GetGeo().GetEndPoint() , myangle);
				LineOrArc newgeo;
				newgeo.SetArc(newa2);
				g1->SetNextGeo(newgeo);
				g1->SetNextStartNeighbori (startNeighbori);
				g1->SetNextEndNeighbori (endNeighbori);
				g1->SetEndNeighbori (-endNeighbori);
				g2->SetStartNeighbori (-startNeighbori);
			}
			else
			{
				g1->SetEndNeighbori (-endNeighbori);
				g2->SetStartNeighbori (-startNeighbori);
			}
			curi = nexti;
		}
	}
}
void LineOrArc::OffsetByExtendingElements (const std::vector<LineOrArc*> &vec , std::vector<OffsLineOrArc> &ofseted , double offsetval , double joinTol)
{
	if (vec.size() == 0)
		return;

	bool isclosed;
	//if (vec.size() == 1)
	//	isclosed = false;
	//else
		isclosed = vec.front()->GetStartPoint ().Compare (vec.back()->GetEndPoint() , joinTol) ? true : false;
	
	std::vector<VPoint> lp;
	// First offset every item in the vector to the list 
	size_t mysizei = vec.size();

	for (size_t i = 0;i<mysizei;++i)
	{
		const LineOrArc &g = *vec[i];
		OffsLineOrArc rog;
		bool ck = g.RoughOffsetInXY(offsetval,rog);
		//if (ck) ???
		//{
			rog.SetParenti ((int)i);
			ofseted.push_back(rog);
			lp.push_back(g.GetEndPoint());
		//}
	}

	// Trim intersections add arc between open edges
	if (ofseted.size() > 1)
	{
		size_t mysize = ofseted.size();
		size_t curi = 0;
		size_t nexti;
		while (curi < mysize)
		{
			if (isclosed == false)
			{
				if (curi == mysize -1)
					break; // No geometry left to trim to
			}
			nexti = curi + 1;
			OffsLineOrArc *g1 = &ofseted.at(curi);
			int startNeighbori = (int)curi;
			if (g1->IsNull())
			{
				curi = nexti;
				continue;
			}

			VPoint ep = lp.at(curi);
			OffsLineOrArc *g2 = &ofseted.at(nexti%mysize);
			int endNeighbori = (int)(nexti%mysize);
			std::vector<OffsLineOrArc *> nullGeoVec;
			
			while (g2->IsNull())
			{
				nullGeoVec.push_back(g2);
				nexti++;
				if (isclosed == false)
				{
					if (nexti == mysize)
						break; // No geometry left to trim to 
				}
				g2 = &ofseted.at(nexti%mysize);
				int endNeighbori = (int)(nexti%mysize);
			}

			g1->GetGeo().TrimOrExtendTo (g2->GetGeo(),joinTol);//TrimBothIf(g2->GetGeo(),joinTol);
			
			size_t mynullsize = nullGeoVec.size();
			for (size_t k=0;k<mynullsize;++k)
			{
				nullGeoVec.at(k)->SetNullPoint(g1->GetGeo().GetEndPoint());
			}

		
			if (g1->GetGeo().GetEndPoint().Compare(g2->GetGeo().GetStartPoint() , joinTol) == false)
			{
				double mylocalOffset = (g1->GetGeo().GetEndPoint() - ep).GetLength();
				// start and end points are not coincident so add an arc between them
				VArc newa (ep , mylocalOffset , UZ , g1->GetGeo().GetEndPoint() , 360.0);
				double myangle = newa.GetAngle(g2->GetGeo().GetStartPoint());
				if (myangle > 180.0)
					myangle -= 360.0;
				VArc newa2(ep , newa.GetRadius() , UZ , g1->GetGeo().GetEndPoint() , myangle);
				LineOrArc newgeo;
				newgeo.SetArc(newa2);
				g1->SetNextGeo(newgeo);
				g1->SetNextStartNeighbori (startNeighbori);
				g1->SetNextEndNeighbori (endNeighbori);
				g1->SetEndNeighbori (-endNeighbori);
				g2->SetStartNeighbori (-startNeighbori);
			}
			else
			{
				g1->SetEndNeighbori (-endNeighbori);
				g2->SetStartNeighbori (-startNeighbori);
			}
			curi = nexti;
		}
	}
}
void LineOrArc::OffsetByExtendingElements (const std::vector<LineOrArc> &vec , std::vector<OffsLineOrArc> &ofseted , std::vector<vreal> offsValVec , vreal joinTol)
{
	if (vec.size() == 0)
		return;

	bool isclosed;
	//if (vec.size() == 1)
	//	isclosed = false;
	//else
	isclosed = vec.front().GetStartPoint ().Compare (vec.back().GetEndPoint() , joinTol) ? true : false;
	
	std::vector<VPoint> lp;
	// First offset every item in the vector to the list 
	size_t mysizei = vec.size();

	for (size_t i = 0;i<mysizei;++i)
	{
		const LineOrArc &g = vec[i];
		OffsLineOrArc rog;
		vreal offsetval = offsValVec.at(i);
		bool ck = g.RoughOffsetInXY(offsetval,rog);
		//if (ck) ???
		//{
			rog.SetParenti ((int)i);
			ofseted.push_back(rog);
			lp.push_back(g.GetEndPoint());
		//}
	}

	// Trim intersections add arc between open edges
	if (ofseted.size() > 1)
	{
		size_t mysize = ofseted.size();
		size_t curi = 0;
		size_t nexti;
		while (curi < mysize)
		{
			if (isclosed == false)
			{
				if (curi == mysize -1)
					break; // No geometry left to trim to
			}
			nexti = curi + 1;
			OffsLineOrArc *g1 = &ofseted.at(curi);
			int startNeighbori = (int)curi;
			if (g1->IsNull())
			{
				curi = nexti;
				continue;
			}

			VPoint ep = lp.at(curi);
			OffsLineOrArc *g2 = &ofseted.at(nexti%mysize);
			int endNeighbori = (int)(nexti%mysize);
			std::vector<OffsLineOrArc *> nullGeoVec;
			
			while (g2->IsNull())
			{
				nullGeoVec.push_back(g2);
				nexti++;
				if (isclosed == false)
				{
					if (nexti == mysize)
						break; // No geometry left to trim to 
				}
				g2 = &ofseted.at(nexti%mysize);
				int endNeighbori = (int)(nexti%mysize);
			}

			g1->GetGeo().TrimOrExtendTo (g2->GetGeo(),joinTol);//TrimBothIf(g2->GetGeo(),joinTol);
			
			size_t mynullsize = nullGeoVec.size();
			for (size_t k=0;k<mynullsize;++k)
			{
				nullGeoVec.at(k)->SetNullPoint(g1->GetGeo().GetEndPoint());
			}

		
			if (g1->GetGeo().GetEndPoint().Compare(g2->GetGeo().GetStartPoint() , joinTol) == false)
			{
				double mylocalOffset = (g1->GetGeo().GetEndPoint() - ep).GetLength();
				// start and end points are not coincident so add an arc between them
				VArc newa (ep , mylocalOffset , UZ , g1->GetGeo().GetEndPoint() , 360.0);
				double myangle = newa.GetAngle(g2->GetGeo().GetStartPoint());
				if (myangle > 180.0)
					myangle -= 360.0;
				VArc newa2(ep , newa.GetRadius() , UZ , g1->GetGeo().GetEndPoint() , myangle);
				LineOrArc newgeo;
				newgeo.SetArc(newa2);
				g1->SetNextGeo(newgeo);
				g1->SetNextStartNeighbori (startNeighbori);
				g1->SetNextEndNeighbori (endNeighbori);
				g1->SetEndNeighbori (-endNeighbori);
				g2->SetStartNeighbori (-startNeighbori);
			}
			else
			{
				g1->SetEndNeighbori (-endNeighbori);
				g2->SetStartNeighbori (-startNeighbori);
			}
			curi = nexti;
		}
	}
}
int LineOrArc::GetNextValidElement (const std::vector<OffsLineOrArc> &ofsetSeq , int curi , bool isClosed)
{
	int mysize = (int)ofsetSeq.size();
	int nexti = curi + 1;
	while (nexti < mysize)
	{
		const OffsLineOrArc &nextElement = ofsetSeq.at(nexti);
		if (nextElement.IsNull () == false)
		{
			return nexti;
		}
		else
		{
			++nexti;
		}
	}
	if (isClosed)
	{
		nexti = 0;
		while (nexti < curi)
		{
			const OffsLineOrArc &nextElement = ofsetSeq.at(nexti);
			if (nextElement.IsNull () == false)
			{
				return nexti;
			}
			else
			{
				++nexti;
			}
		}
	}
	return -1;
}
bool LineOrArc::IsOffsetSinglePoint(const std::vector<LineOrArc>& vec, vreal offsetval , vreal tol , VArc& tofill)
{
	bool isResultPoint = false;
	bool isThereOverlap = false;
	//if (LineOrArc::IsSingleFullArc(vec, tol * 2, tofill) == true)
	if (LineOrArc::IsSingleArc(vec,tol*2,tofill,isThereOverlap) == true)
	{
		bool isCCW = tofill.IsCCW();
		if (isCCW == true && fabs(tofill.GetRadius() - offsetval) < tol * 2)
		{
			isResultPoint = true;
		}
		else if (isCCW == false && fabs(tofill.GetRadius() + offsetval) < tol * 2)
		{
			isResultPoint = true;
		}
	}
	return isResultPoint;
}
bool LineOrArc::IsOffsetSinglePoint(const std::vector<LineOrArc*>& vec, vreal offsetval, vreal tol, VArc& tofill)
{
	bool isResultPoint = false;
	bool isThereOverlap = false;
	//if (LineOrArc::IsSingleFullArc(vec, tol * 2, tofill) == true)
	if (LineOrArc::IsSingleArc(vec, tol * 2, tofill, isThereOverlap) == true)
	{
		bool isCCW = tofill.IsCCW();
		if (isCCW == true && fabs(tofill.GetRadius() - offsetval) < tol * 2)
		{
			isResultPoint = true;
		}
		else if (isCCW == false && fabs(tofill.GetRadius() + offsetval) < tol * 2)
		{
			isResultPoint = true;
		}
	}
	return isResultPoint;
}
void LineOrArc::NewOffset (const std::vector<LineOrArc> &vec , std::vector<LineOrArc> &ofseted , double offsetval  , double joinTol , const std::vector<LineOrArc> &collSl,double tol,bool isSharpOffset , VariableOffsetManager *va)
{
	if (va)
	{
		//VariableOffsetManager vom (offsetval , VariableOffsetManager::constantLinear , 0.0);//-0.05);
		va->SpiralOffset (vec , ofseted);
		return ;
	}
	VArc tempa(VPoint(0.0),1.0);
	bool isResultPoint = IsOffsetSinglePoint(vec, offsetval, tol , tempa);
	if (isResultPoint)
	{
		ofseted.push_back(LineOrArc(VLine(tempa.GetCenter(), tempa.GetCenter())));
		return;
	}
	std::vector<OffsLineOrArc> o1;
	isSharpOffset ? OffsetByExtendingElements(vec,o1,offsetval,joinTol) : Offset (vec,o1,offsetval,joinTol);
	// Trim resulting geometry by coll checking
	size_t mysize = o1.size();
	for (size_t i=0;i<mysize;++i)
	{
		const OffsLineOrArc &og = o1.at(i);
		if (og.IsNull() == false)
		{
			const LineOrArc &gr = og.GetGeo();
			int parenti = og.GetParenti();
			if (&vec != &collSl)
			{
				parenti = LineOrArc::FindOffsetParentIf (vec,collSl,parenti,tol);
			}
			AddTrimmed (gr,ofseted,collSl,offsetval,tol,joinTol,parenti , og.GetStartNeighbori() , og.GetEndNeighbori());
		}
		if (og.IsNextNull() == false)
		{
			const LineOrArc &gn = og.GetNextGeo();
			AddTrimmed (gn,ofseted,collSl,offsetval,tol,joinTol,-1,og.GetNextStartNeighbori(),og.GetNextEndNeighbori());
		}
	}
}
void LineOrArc::NewOffset (const std::vector<LineOrArc *> &vec , std::vector<LineOrArc *> &ofseted , double offsetval  , double joinTol , const std::vector<LineOrArc *> &collSl,double tol,bool isSharpOffset,VariableOffsetManager *va)
{
	//std::vector<OffsLineOrArc> o1;
	//isSharpOffset ? OffsetByExtendingElements(vec,o1,offsetval,joinTol):Offset (vec,o1,offsetval,joinTol);
	//// Trim resulting geometry by coll checking
	//size_t mysize = o1.size();
	//for (size_t i=0;i<mysize;++i)
	//{
	//	const OffsLineOrArc &og = o1.at(i);
	//	if (og.IsNull() == false)
	//	{
	//		const LineOrArc &gr = og.GetGeo();
	//		int parenti=og.GetParenti();
	//		if (&vec != &collSl)
	//		{
	//			parenti = LineOrArc::FindOffsetParentIf (vec,collSl,parenti,tol);
	//		}
	//		AddTrimmed (gr,ofseted,collSl,offsetval,tol,joinTol,parenti , og.GetStartNeighbori() , og.GetEndNeighbori());
	//	}
	//	if (og.IsNextNull() == false)
	//	{
	//		const LineOrArc &gn = og.GetNextGeo();
	//		AddTrimmed (gn,ofseted,collSl,offsetval,tol,joinTol,-1,og.GetNextStartNeighbori(),og.GetNextEndNeighbori());
	//	}
	//}

	//if (va)
	//{
	//	//VariableOffsetManager vom (offsetval , VariableOffsetManager::constantLinear , 0.0);//-0.05);
	//	va->SpiralOffset(vec, ofseted);
	//	return;
	//}
	VArc tempa(VPoint(0.0), 1.0);
	bool isResultPoint = IsOffsetSinglePoint(vec, offsetval, tol, tempa);
	if (isResultPoint)
	{
		ofseted.push_back(new LineOrArc(VLine(tempa.GetCenter(), tempa.GetCenter())));
		return;
	}
	std::vector<OffsLineOrArc> o1;
	isSharpOffset ? OffsetByExtendingElements(vec, o1, offsetval, joinTol) : Offset(vec, o1, offsetval, joinTol);
	// Trim resulting geometry by coll checking
	size_t mysize = o1.size();
	for (size_t i = 0; i < mysize; ++i)
	{
		const OffsLineOrArc& og = o1.at(i);
		if (og.IsNull() == false)
		{
			const LineOrArc& gr = og.GetGeo();
			int parenti = og.GetParenti();
			if (&vec != &collSl)
			{
				parenti = LineOrArc::FindOffsetParentIf(vec, collSl, parenti, tol);
			}
			AddTrimmed(gr, ofseted, collSl, offsetval, tol, joinTol, parenti, og.GetStartNeighbori(), og.GetEndNeighbori());
		}
		if (og.IsNextNull() == false)
		{
			const LineOrArc& gn = og.GetNextGeo();
			AddTrimmed(gn, ofseted, collSl, offsetval, tol, joinTol, -1, og.GetNextStartNeighbori(), og.GetNextEndNeighbori());
		}
	}
}
void LineOrArc::NewOffset(const std::vector<LineOrArc*>& vec, std::vector<LineOrArc*>& ofseted, double offsetval, double joinTol, const std::vector<LineOrArc>& collSl, double tol, bool isSharpOffset, VariableOffsetManager* va)
{
	//if (va)
	//{
	//	//VariableOffsetManager vom (offsetval , VariableOffsetManager::constantLinear , 0.0);//-0.05);
	//	va->SpiralOffset(vec, ofseted);
	//	return;
	//}
	VArc tempa(VPoint(0.0), 1.0);
	bool isResultPoint = IsOffsetSinglePoint(vec, offsetval, tol, tempa);
	if (isResultPoint)
	{
		ofseted.push_back(new LineOrArc(VLine(tempa.GetCenter(), tempa.GetCenter())));
		return;
	}
	std::vector<OffsLineOrArc> o1;
	isSharpOffset ? OffsetByExtendingElements(vec, o1, offsetval, joinTol) : Offset(vec, o1, offsetval, joinTol);
	// Trim resulting geometry by coll checking
	size_t mysize = o1.size();
	for (size_t i = 0; i < mysize; ++i)
	{
		const OffsLineOrArc& og = o1.at(i);
		if (og.IsNull() == false)
		{
			const LineOrArc& gr = og.GetGeo();
			int parenti = og.GetParenti();
			//if (&vec != &collSl)
			//{
				parenti = LineOrArc::FindOffsetParentIf(vec, collSl, parenti, tol);
			//}
			AddTrimmed(gr, ofseted, collSl, offsetval, tol, joinTol, parenti, og.GetStartNeighbori(), og.GetEndNeighbori());
		}
		if (og.IsNextNull() == false)
		{
			const LineOrArc& gn = og.GetNextGeo();
			AddTrimmed(gn, ofseted, collSl, offsetval, tol, joinTol, -1, og.GetNextStartNeighbori(), og.GetNextEndNeighbori());
		}
	}
}
void LineOrArc::NoTrimOffset(const std::vector<LineOrArc>& vec, std::vector<LineOrArc>& ofseted, double offsetval, double joinTol, const std::vector<LineOrArc>& collSl, double tol, bool isSharpOffset)
{
	std::vector<LineOrArc> geoVec;
	for (auto i = vec.begin(); i != vec.end(); ++i)
	{
		SequentialLineSet sl(100);
		(*i).DigitiseByLengthStep(joinTol, sl);
		LineOrArc::AddToLineOrArcVec(sl.GetPointSet().GetVec(), geoVec);
	}
	if (geoVec.size() == 0)
		throw VException("LineOrArc::NoTrimOffset exception. geoVec empty",1);
	if (geoVec.begin()->GetType() != line)
		throw VException("LineOrArc::NoTrimOffset exception , first element is not line",1);
	const VLine* l0 = geoVec.begin()->GetLine();
	VPoint prevP(l0->GetP2() + l0->GetNormalOnXYPlane() * offsetval);
	for (auto j = geoVec.begin(); j != geoVec.end(); ++j)
	{
		if ((*j).GetType() == line)
		{
			const VLine* l = (*j).GetLine();
			VPoint n = l->GetNormalOnXYPlane();
			VLine l2(l->GetP1() + n * offsetval, l->GetP2() + n * offsetval);
			if (prevP.Compare(l2.GetP1(), joinTol) == false)
				ofseted.push_back(VLine(prevP, l2.GetP1()));
			ofseted.push_back(l2);
			prevP = l2.GetP2();
		}
		else
			throw VException("LineOrArc::NoTrimOffset exception , element is not line",1);
	}
}
void LineOrArc::NewSharpOffsetForConvexLoop (const std::vector<LineOrArc> &vec , std::vector<LineOrArc> &ofseted , std::vector<double> offsetvalVec , double joinTol , const std::vector<LineOrArc> &collSl,double tol)
{

	std::vector<OffsLineOrArc> o1;
	OffsetByExtendingElements(vec,o1,offsetvalVec,joinTol);
	// Trim resulting geometry by coll checking
	size_t mysize = o1.size();
	for (size_t i=0;i<mysize;++i)
	{
		const OffsLineOrArc &og = o1.at(i);
		if (og.IsNull() == false)
		{
			vreal tmpoffsetval = 0.0;//offsetvalVec.at(i);
			const LineOrArc &gr = og.GetGeo();
			int parenti=og.GetParenti();
			if (&vec != &collSl)
			{
				parenti = LineOrArc::FindOffsetParentIf (vec,collSl,parenti,tol);
			}
			AddTrimmed (gr,ofseted,collSl,tmpoffsetval,tol,joinTol,parenti , og.GetStartNeighbori() , og.GetEndNeighbori());
		}
		if (og.IsNextNull() == false)
		{
			vreal tmpoffsetval = 0.0;//offsetvalVec.at(i);
			const LineOrArc &gn = og.GetNextGeo();
			AddTrimmed (gn,ofseted,collSl,tmpoffsetval,tol,joinTol,-1,og.GetNextStartNeighbori(),og.GetNextEndNeighbori());
		}
	}
}

void LineOrArc::AddTrimmed (const LineOrArc &gr , std::vector<LineOrArc> &ofseted,const std::vector<LineOrArc> &collSl , double offsetRad , double tol , double joinTol , int parenti , int startNeighbori , int endNeighbori)
{
	//ofseted.push_back(gr);
	//return;
	if (gr.GetType() == LineOrArc::line)
	{
		std::vector<VLine> remline = MultiWireCollCheck::TrimLine (*gr.GetLine(),collSl,fabs(offsetRad)/*-tol*4.0*/ , parenti , startNeighbori , endNeighbori , joinTol);
		// add to ofset
		size_t mysizel = remline.size();
		for (size_t k=0;k<mysizel;++k)
		{
			const VLine &cl = remline.at(k);
			ofseted.push_back (LineOrArc (cl));
		}
	}
	else
	{
		std::vector<VArc> remarc =
			MultiWireCollCheck::TrimArc (*gr.GetArc(),tol,collSl,fabs(offsetRad)/*-tol*4.0*/ , parenti , startNeighbori , endNeighbori , joinTol);
			//MultiWireCollCheck::NewTrimArc(*gr.GetArc(), tol, collSl, fabs(offsetRad)/*-tol*4.0*/, parenti, startNeighbori, endNeighbori, joinTol);

		// add to ofset
		size_t mysizea = remarc.size();
		for (size_t k=0;k<mysizea;++k)
		{
			const VArc &ca = remarc.at (k);
			ofseted.push_back (LineOrArc (ca));
		}
	}
}

void LineOrArc::AddTrimmed (const LineOrArc &gr , std::vector<LineOrArc *> &ofseted,const std::vector<LineOrArc *> &collSl , double offsetRad , double tol , double joinTol , int parenti , int startNeighbori , int endNeighbori)
{
	//ofseted.push_back(gr);
	//return;
	if (gr.GetType() == LineOrArc::line)
	{
		std::vector<VLine> remline = MultiWireCollCheck::TrimLine (*gr.GetLine(),collSl,fabs(offsetRad)/*-tol*4.0*/ , parenti , startNeighbori , endNeighbori , joinTol);
		// add to ofset
		size_t mysizel = remline.size();
		for (size_t k=0;k<mysizel;++k)
		{
			const VLine &cl = remline.at(k);
			ofseted.push_back (new LineOrArc (cl));
		}
	}
	else
	{
		std::vector<VArc> remarc = MultiWireCollCheck::TrimArc (*gr.GetArc(),tol,collSl,fabs(offsetRad)/*-tol*4.0*/ , parenti , startNeighbori , endNeighbori , joinTol);
		// add to ofset
		size_t mysizea = remarc.size();
		for (size_t k=0;k<mysizea;++k)
		{
			const VArc &ca = remarc.at (k);
			ofseted.push_back (new LineOrArc (ca));
		}
	}
}
void LineOrArc::AddTrimmed(const LineOrArc& gr, std::vector<LineOrArc*>& ofseted, const std::vector<LineOrArc>& collSl, double offsetRad, double tol, double joinTol, int parenti, int startNeighbori, int endNeighbori)
{
	//ofseted.push_back(gr);
	//return;
	if (gr.GetType() == LineOrArc::line)
	{
		std::vector<VLine> remline = MultiWireCollCheck::TrimLine(*gr.GetLine(), collSl, fabs(offsetRad)/*-tol*4.0*/, parenti, startNeighbori, endNeighbori, joinTol);
		// add to ofset
		size_t mysizel = remline.size();
		for (size_t k = 0; k < mysizel; ++k)
		{
			const VLine& cl = remline.at(k);
			ofseted.push_back(new LineOrArc(cl));
		}
	}
	else
	{
		std::vector<VArc> remarc = MultiWireCollCheck::TrimArc(*gr.GetArc(), tol, collSl, fabs(offsetRad)/*-tol*4.0*/, parenti, startNeighbori, endNeighbori, joinTol);
		// add to ofset
		size_t mysizea = remarc.size();
		for (size_t k = 0; k < mysizea; ++k)
		{
			const VArc& ca = remarc.at(k);
			ofseted.push_back(new LineOrArc(ca));
		}
	}
}
VPoint LineOrArc::GetStartPoint ()const
{
	if (m_type == line)
	{
		const VLine *l = GetLine ();
		return l->GetP1();
	}
	else
	{
		const VArc *a = GetArc();
		return a->GetStartPoint();
	}
}

VPoint LineOrArc::GetEndPoint ()const
{
	if (m_type == line)
	{
		const VLine *l = GetLine ();
		return l->GetP2();
	}
	else 
	{
		const VArc *a = GetArc();
		return a->GetEndPoint();		
	}
}

bool LineOrArc::RoughOffsetInXY (double offsetval  , OffsLineOrArc &oog) const
{
	LineOrArc og = (*this);
	bool retval = true;
	if (m_type == line)
	{
		const VLine *l = GetLine();
		VLine2d ll (VPoint2d (l->GetP1().x(),l->GetP1().y()) , VPoint2d (l->GetP2().x(),l->GetP2().y()));
		VPoint2d np = ll.GetNormal();
		VPoint ov (np.x(),np.y(),0.0);
		ov *= offsetval;
		VLine newline (l->GetP1() + ov , l->GetP2() + ov);
		og.SetLine(newline);
		oog.SetGeo(og);
	}
	else
	{
		const VArc *a = GetArc();
		double newradius = a->GetRadius();
		if (a->IsCCW()) //== isccw)
		{
			//if (isccw)
				newradius -= offsetval;
			//else
			//	newradius += offsetval;
		}
		else
		{
			//if (isccw)
				newradius += offsetval;
			//else
			//	newradius -= offsetval;
		}
		if (newradius < EPSILON)
		{
			// Add a fake arc for just holding the geometry , this will be processed as point later
			newradius = EPSILON * 0.5;
			retval = false;
		}
		VVector r = a->GetStartPoint() - a->GetCenter();
		r.NormThis();
		r*=newradius;

		VArc newarc (a->GetCenter(),newradius,a->GetAx(),a->GetCenter()+r,a->GetSweepAngle());
		og.SetArc(newarc);
		oog.SetGeo(og);
		if (retval == false)
		{
			oog.SetNull (true);
		}
	}
	return retval;
}

void LineOrArc::TrimBothIf(LineOrArc &g,double joinTol)
{
	if (GetEndPoint().Compare(g.GetStartPoint(),joinTol))
		return;
	if (m_type == line)
	{
		if (g.GetType() == line)
		{
			// line line
			const VLine *l1 = GetLine();
			const VLine *l2 = g.GetLine();
			LineIntersection inter;
			VPoint ip,pit;
			LineIntersection::intersectiontype tp = inter.LinesIntersect(l1->GetP1(),l1->GetV(),l2->GetP1(),l2->GetV(),ip,pit);
			if (tp == LineIntersection::LINESINTERSECT)
			{
				VLine nl1 (l1->GetP1(),ip);
				VLine nl2 (ip , l2->GetP2());
				SetLine(nl1);
				g.SetLine(nl2);
			}
		}
		else
		{
			// line arc
			const VLine *l1 = GetLine();
			const VArc *a2 = g.GetArc();
			std::vector<VPoint> ip = a2->GetInt(*l1);
			
			if (ip.size()==1)
			{
				VPoint ip0 = ip.front();
				VArc newa2 = LimitArcOnXYwith1PointOnit (*a2 , ip0 , true);
				VLine newl1 (l1->GetP1(),ip0);
				SetLine(newl1);
				g.SetArc(newa2);
			}
			else if (ip.size()== 2)
			{
				VPoint ip0 = ip.at(0);
				VPoint ip1 = ip.at(1);
				bool trimStartOfArc = true; // because arc is the next element and line should be trimmed at end and arc should be trimmed at start
				VArc newa2 = LimitArcOnXYwith2PointsOnit (*a2 , ip0 , ip1 , trimStartOfArc);
				//double t0 = l1->GetT (ip0);
				//double t1 = l1->GetT (ip1);
				VLine newl1 = VLine (l1->GetP1() , a2->GetStartPoint());//t0 < t1 ? VLine (ip0 , ip1) : VLine (ip1 , ip0);
				SetLine (newl1);
				g.SetArc(newa2);
			}
		}
	}
	else 
	{
		if (g.GetType() == line)
		{
			// arc line
			const VArc *a1 = GetArc();
			const VLine *l2 = g.GetLine();
			std::vector<VPoint> ip = a1->GetInt(*l2);
			if (ip.size()==1)
			{
				VPoint ip0 = ip.front();
				VArc newa1 = LimitArcOnXYwith1PointOnit (*a1 , ip0 , false);
				VLine newl2 (ip0 , l2->GetP2());
				SetArc (newa1);
				g.SetLine (newl2);
			}
			else if (ip.size()== 2)
			{
				// choose angle near to 0
				VPoint ip0 = ip.at(0);
				VPoint ip1 = ip.at(1);
				bool trimStartOfArc = false; // which means trim end of arc
				VArc newa1 = LimitArcOnXYwith2PointsOnit (*a1 , ip0 , ip1 , trimStartOfArc);
				//double t0 = l2->GetT (ip0);
				//double t1 = l2->GetT (ip1);
				VLine newl2 = VLine(newa1.GetEndPoint() , l2->GetP2());//t0 < t1 ? VLine (ip0 , ip1) : VLine (ip1 , ip0);
				SetArc (newa1);
				g.SetLine (newl2);
			}
		}
		else
		{
			// arc arc
			const VArc *a1 = GetArc();
			const VArc *a2 = g.GetArc();
			std::vector<VPoint> ip = a1->GetInt(*a2,joinTol);
			//VPoint ip0;
			//double sweepangle1 = a1->GetSweepAngle();
			//double sweepangle2 = a2->GetSweepAngle();
			if (ip.size()== 1)
			{
				//ip0 = ip.front();
				//double angle1 = a1->GetAngle(ip0);
				//if (sweepangle1 < 0.0)
				//	angle1 -= 360.;
				//VArc newa1 (a1->GetCenter(),a1->GetRadius(),a1->GetAx(),a1->GetStartPoint(),angle1);
				//double angle2 = a2->GetAngle(ip0);
				//if (sweepangle2 < 0.0)
				//	angle2 -= 360.;
				//VArc newa2 (a2->GetCenter(),a2->GetRadius(),a2->GetAx(),ip0, sweepangle2 - angle2);
				
				VArc newa1 = LimitArcOnXYwith1PointOnit (*a1 , ip.front() , false);
				VArc newa2 = LimitArcOnXYwith1PointOnit (*a2 , ip.front() , true);	
				SetArc(newa1);
				g.SetArc(newa2);
			}
			else if (ip.size()==2)
			{
				bool trimArcAtStart1 = false;
				VArc newa1 = LimitArcOnXYwith2PointsOnit (*a1 , ip.front() , ip.back(),trimArcAtStart1);
				SetArc (newa1);
				bool trimArcAtStart2 = true;
				VArc newa2 = LimitArcOnXYwith2PointsOnit (*a2 , ip.front() , ip.back(),trimArcAtStart2);
				g.SetArc (newa2);
			}
		}
	}
}

void LineOrArc::TrimOrExtendTo (LineOrArc &g,double joinTol)
{
	if (GetEndPoint().Compare(g.GetStartPoint(),joinTol))
		return;
	if (m_type == line)
	{
		if (g.GetType() == line)
		{
			// line line
			const VLine *l1 = GetLine();
			const VLine *l2 = g.GetLine();
			LineIntersection inter;
			VPoint ip,pit;
			//LineIntersection::intersectiontype tp = inter.LinesIntersect(l1->GetP1(),l1->GetV(),l2->GetP1(),l2->GetV(),ip,pit);
			InfiniteLinesNearestPoints myinfcalc;
			vreal inft1 , inft2;
			InfiniteLinesNearestPoints::info infck = myinfcalc.Calc (l1->GetP1() , l1->GetV() , l2->GetP2() , l2->GetV() , inft1 , inft2);
			if (infck == InfiniteLinesNearestPoints::LINESPARALLEL)
			{
				// lines are parallel doing nothing , leaving like it is
				//throw VException((IDS_LinTriExcParLin), 1);
				return;
			}
			ip = l1->GetP (inft1);
			//if (tp == LineIntersection::LINESINTERSECT)
			//{
			VLine nl1 (l1->GetP1(),ip);
			VLine nl2 (ip , l2->GetP2());
			SetLine(nl1);
			g.SetLine(nl2);
			//}
		}
		else
		{
			// line arc
			const VLine *l1 = GetLine();
			VArc fa2 = *g.GetArc();
			std::vector<VPoint> ip = fa2.GetIntForInfiniteLineAndCircleOfArc(*l1);
			
			if (ip.size()==1)
			{
				VPoint ip0 = ip.front();
				VArc newa2 = LimitArcOnXYwith1PointOnit (fa2 , ip0 , true);
				VLine newl1 (l1->GetP1(),ip0);
				SetLine(newl1);
				g.SetArc(newa2);
			}
			else if (ip.size()== 2)
			{
				VPoint ip0 = ip.at(0);
				VPoint ip1 = ip.at(1);
				bool trimAtStart = true;
				VArc newa2 = LimitArcOnXYwith2PointsOnit (fa2 , ip0 , ip1 ,trimAtStart);
				//double t0 = l1->GetT (ip0);
				//double t1 = l1->GetT (ip1);
				VLine newl1 = VLine(l1->GetP1(),newa2.GetStartPoint());//t0 < t1 ? VLine (ip0 , ip1) : VLine (ip1 , ip0);
				SetLine (newl1);
				g.SetArc(newa2);
			}
		}
	}
	else 
	{
		if (g.GetType() == line)
		{
			// arc line
			VArc fa1 = *GetArc();
			const VLine *l2 = g.GetLine();
			std::vector<VPoint> ip = fa1.GetIntForInfiniteLineAndCircleOfArc(*l2);
			if (ip.size()==1)
			{
				VPoint ip0 = ip.front();
				VArc newa1 = LimitArcOnXYwith1PointOnit (fa1 , ip0 , false);
				VLine newl2 (ip0 , l2->GetP2());
				SetArc (newa1);
				g.SetLine (newl2);
			}
			else if (ip.size()== 2)
			{
				// choose angle near to 0
				VPoint ip0 = ip.at(0);
				VPoint ip1 = ip.at(1);
				bool trimAtStart = false;
				VArc newa1 = LimitArcOnXYwith2PointsOnit (fa1 , ip0 , ip1, trimAtStart);
				//double t0 = l2->GetT (ip0);
				//double t1 = l2->GetT (ip1);
				VLine newl2 = VLine (newa1.GetEndPoint(),l2->GetP2());//t0 < t1 ? VLine (ip0 , ip1) : VLine (ip1 , ip0);
				SetArc (newa1);
				g.SetLine (newl2);
			}
		}
		else
		{
			// arc arc
			VArc fa1 = *GetArc();
			VArc fa2 = *g.GetArc();

			std::vector<VPoint> ip = fa1.GetIntForFullArcs (fa2,joinTol);
			//VPoint ip0;
			//double sweepangle1 = a1->GetSweepAngle();
			//double sweepangle2 = a2->GetSweepAngle();
			if (ip.size()== 1)
			{
				//ip0 = ip.front();
				//double angle1 = a1->GetAngle(ip0);
				//if (sweepangle1 < 0.0)
				//	angle1 -= 360.;
				//VArc newa1 (a1->GetCenter(),a1->GetRadius(),a1->GetAx(),a1->GetStartPoint(),angle1);
				//double angle2 = a2->GetAngle(ip0);
				//if (sweepangle2 < 0.0)
				//	angle2 -= 360.;
				//VArc newa2 (a2->GetCenter(),a2->GetRadius(),a2->GetAx(),ip0, sweepangle2 - angle2);
				
				VArc newa1 = LimitArcOnXYwith1PointOnit (fa1 , ip.front() , false);
				VArc newa2 = LimitArcOnXYwith1PointOnit (fa2 , ip.front() , true);	
				SetArc(newa1);
				g.SetArc(newa2);
			}
			else if (ip.size()==2)
			{
				bool limitArcAtStart1 = false;
				VArc newa1 = LimitArcOnXYwith2PointsOnit (fa1 , ip.front() , ip.back(),limitArcAtStart1);
				SetArc (newa1);
				bool limitArcAtStart2 = true;
				VArc newa2 = LimitArcOnXYwith2PointsOnit (fa2 , ip.front() , ip.back(),limitArcAtStart2);
				g.SetArc (newa2);
			}
		}
	}
}
void LineOrArc::GetIntersections (LineOrArc &g /*, double joinTol*/ , std::vector<VPoint> &wpthis , std::vector<VPoint> &wpg , vreal tol)
{
	//if (GetEndPoint().Compare(g.GetStartPoint(),joinTol))
	//	return;
	if (m_type == line)
	{
		if (g.GetType() == line)
		{
			// line line
			const VLine *l1 = GetLine();
			const VLine *l2 = g.GetLine();
			LineIntersection inter;
			VPoint ip,pit;
			LineIntersection::intersectiontype tp = inter.LinesIntersect(l1->GetP1(),l1->GetV(),l2->GetP1(),l2->GetV(),ip,pit,tol);
			if (tp == LineIntersection::LINESINTERSECT)
			{
				wpthis.push_back (ip);
				wpg.push_back (ip);
			}
		}
		else
		{
			// line arc
			const VLine *l1 = GetLine();
			const VArc *a2 = g.GetArc();
			std::vector<VPoint> ip = a2->GetInt(*l1);
			wpthis = ip;
			wpg = ip;
		}
	}
	else 
	{
		if (g.GetType() == line)
		{
			// arc line
			const VArc *a1 = GetArc();
			const VLine *l2 = g.GetLine();
			std::vector<VPoint> ip = a1->GetInt(*l2);
			wpthis = ip;
			wpg = ip;
		}
		else
		{
			// arc arc
			const VArc *a1 = GetArc();
			const VArc *a2 = g.GetArc();
			std::vector<VPoint> ip = a1->GetInt(*a2,tol);
			wpthis = ip;
			wpg = ip;
		}
	}
}

VArc LineOrArc::LimitArcOnXYwith1PointOnit (const VArc &tolimit , const VPoint &p , bool limitstart)
{
	if (limitstart)
	{
		VArc newa (tolimit.GetCenter(),tolimit.GetRadius(),UZ,p,tolimit.GetEndPoint());
		if (tolimit.GetSweepAngle()>0.0)
			return newa;
		else
		{
			VArc newa2 (newa.GetCenter(),newa.GetRadius(),UZ,newa.GetStartPoint(),newa.GetSweepAngle()-360.0);
			return newa2;
		}
	}
	else
	{
		VArc newa (tolimit.GetCenter(),tolimit.GetRadius(),UZ,tolimit.GetStartPoint(),p);
		if (tolimit.GetSweepAngle()>0.0)
			return newa;
		else
		{
			VArc newa2 (newa.GetCenter(),newa.GetRadius(),UZ,newa.GetStartPoint(),newa.GetSweepAngle()-360.0);
			return newa2;
		}
	}
}

VArc LineOrArc::LimitArcOnXYwith2PointsOnit (const VArc &tolimit , const VPoint &p0 , const VPoint &p1, bool limitstart)
{
	double angle0 = tolimit.GetAngle(p0);
	double angle1 = tolimit.GetAngle(p1);
	//double angledif = fabs(angle0) < fabs(angle1) ? angle1 - angle0 : angle0 - angle1;
	//VPoint startp = fabs(angle0) < fabs(angle1) ? p0 : p1;
	//VArc newarc (tolimit.GetCenter() , tolimit.GetRadius() , tolimit.GetAx() , startp , angledif);
	if ( limitstart == true)
	{
		double angularDist0 = PolarCalc2d::GetAbsShortestAngleBtw (angle0,0.0);
		double angularDist1 = PolarCalc2d::GetAbsShortestAngleBtw (angle1,0.0);
		if (angularDist0 < angularDist1)
		{
			return LimitArcOnXYwith1PointOnit (tolimit , p0 , limitstart);
		}
		else
		{
			return LimitArcOnXYwith1PointOnit (tolimit , p1 , limitstart);
		}
	}
	else
	{
		double sweepAngle = tolimit.GetSweepAngle();
		double angularDist0 = PolarCalc2d::GetAbsShortestAngleBtw (angle0,sweepAngle);
		double angularDist1 = PolarCalc2d::GetAbsShortestAngleBtw (angle1,sweepAngle);
		if (angularDist0 < angularDist1 )
		{
			return LimitArcOnXYwith1PointOnit (tolimit , p0 , limitstart);
		}
		else
		{
			return LimitArcOnXYwith1PointOnit (tolimit , p1 , limitstart);
		}
	}
	//return newarc;
}
void LineOrArc::GetMinMax(const std::list<LineOrArc>& loop, VPoint& minp, VPoint& maxp)
{
	minp = VPoint(MAX_NUMBER_LIMIT);
	maxp = VPoint(MIN_NUMBER_LIMIT);
	std::list<LineOrArc>::const_iterator i = loop.begin();
	std::list<LineOrArc>::const_iterator e = loop.end();
	for (i; i != e; ++i)
	{
		if (i->GetType() == line)
		{
			const VLine* l = i->GetLine();
			minp.ReplaceMin(l->GetP1());
			minp.ReplaceMin(l->GetP2());
			maxp.ReplaceMax(l->GetP1());
			maxp.ReplaceMax(l->GetP2());
		}
		else
		{
			const VArc* a = i->GetArc();
			a->ReplaceMinMax(minp, maxp, 20);
		}
	}
}
void LineOrArc::GetMinMax(const std::vector<LineOrArc> &vec, VPoint &minp, VPoint &maxp)
{
	minp = VPoint (MAX_NUMBER_LIMIT);
	maxp = VPoint (MIN_NUMBER_LIMIT);
	std::vector<LineOrArc>::const_iterator i = vec.begin();
	std::vector<LineOrArc>::const_iterator e = vec.end();
	for (i;i!=e;++i)
	{
		if (i->GetType() == line)
		{
			const VLine *l = i->GetLine();
			minp.ReplaceMin(l->GetP1());
			minp.ReplaceMin(l->GetP2());
			maxp.ReplaceMax(l->GetP1());
			maxp.ReplaceMax(l->GetP2());
		}
		else
		{
			const VArc *a = i->GetArc();
			a->ReplaceMinMax(minp,maxp,20);
		}
	}
}
void LineOrArc::GetMinMax(const std::vector<LineOrArc *> &vec, VPoint &minp, VPoint &maxp)
{
	minp = VPoint (MAX_NUMBER_LIMIT);
	maxp = VPoint (MIN_NUMBER_LIMIT);
	std::vector<LineOrArc *>::const_iterator i = vec.begin();
	std::vector<LineOrArc *>::const_iterator e = vec.end();
	for (i;i!=e;++i)
	{
		if ((*i)->GetType() == line)
		{
			const VLine *l = (*i)->GetLine();
			minp.ReplaceMin(l->GetP1());
			minp.ReplaceMin(l->GetP2());
			maxp.ReplaceMax(l->GetP1());
			maxp.ReplaceMax(l->GetP2());
		}
		else
		{
			const VArc *a = (*i)->GetArc();
			a->ReplaceMinMax(minp,maxp,20);
		}
	}
}
void LineOrArc::SetZ(double newz)
{
	if (m_type == line)
	{
		const VLine *l = GetLine();
		VPoint p1 (l->GetP1());
		p1.z (newz);
		VPoint p2 (l->GetP2());
		p2.z (newz);
		VLine newline (p1,p2);
		SetLine (newline);
	}
	else
	{
		const VArc *a = GetArc();
		VPoint cent (a->GetCenter());
		cent.z (newz);
		VPoint startp (a->GetStartPoint());
		startp.z (newz);
		VArc newarc (cent , a->GetRadius() , a->GetAx() , startp , a->GetSweepAngle());
		SetArc(newarc);
	}
}

void LineOrArc::SetZ (std::vector<OffsLineOrArc> &vec,double newz)
{
	size_t mysize = vec.size();
	for (size_t i = 0;i<mysize;++i)
	{
		OffsLineOrArc &ofg = vec[i];
		ofg.GetGeo().SetZ (newz);
		if (ofg.IsNextNull() == false)
			ofg.GetNextGeo().SetZ(newz);
	}
}

void LineOrArc::SetZ (std::vector<LineOrArc> &vec,double newz)
{
	size_t mysize = vec.size();
	for (size_t i = 0;i<mysize;++i)
	{
		LineOrArc &g = vec[i];
		g.SetZ (newz);
	}
}

void LineOrArc::SetZ(std::vector<LineOrArc*>& vec, double newz)
{
	size_t mysize = vec.size();
	for (size_t i = 0; i < mysize; ++i)
	{
		LineOrArc* g = vec[i];
		g->SetZ(newz);
	}
}

double LineOrArc::GetLength ()const
{
	if (m_type == line)
	{
		const VLine *l = GetLine();
		return l->GetLength();
	}
	else
	{
		const VArc *a = GetArc();
		return a->GetArcLength();
	}	
}

std::vector<LineOrArc> LineOrArc::Split(double lenToSplit, double threshold) const
{
	std::vector<LineOrArc> res;
	if (m_type == line)
	{
		const VLine *l = GetLine();
		double lineLength = l->GetLength();
		if (fabs(lineLength)<EPSILON)
			throw VException (( IDS_LinSplExcLinLen),1);
		double mypar = lenToSplit / lineLength;
		std::vector<VLine> svec = l->Split(mypar,threshold);
		std::vector<VLine>::iterator i = svec.begin();
		std::vector<VLine>::iterator e = svec.end();
		for (i;i!=e;++i)
		{
			LineOrArc tmp;
			tmp.SetLine(*i);
			res.push_back(tmp);
		}
	}
	else
	{
		const VArc *a = GetArc();
		if (fabs(a->GetRadius())< EPSILON)
			throw VException (( IDS_LinSplExcArcRad),1);
		double myangle = lenToSplit / (DEGREE_TO_RAD * a->GetRadius());
		if (a->GetSweepAngle() < 0.0)
			myangle *= -1.;
		std::vector<VArc> svec = a->Split(myangle , threshold);
		std::vector<VArc>::iterator i=svec.begin();
		std::vector<VArc>::iterator e=svec.end();
		for (i;i!=e;++i)
		{
			LineOrArc tmp;
			tmp.SetArc(*i);
			res.push_back(tmp);
		}
	}
	return res;
}

double LineOrArc::GetLength (const std::vector<LineOrArc> &vec)
{
	double lenres = 0;
	std::vector<LineOrArc>::const_iterator i = vec.begin();
	std::vector<LineOrArc>::const_iterator e = vec.end();
	for (i;i!=e;++i)
	{
		lenres += i->GetLength();
	}
	return lenres;
}

double LineOrArc::GetParLength(const double t) const
{
	if (m_type == line)
	{
		const VLine *l = GetLine();
		double parLen = l->GetLength() * t;
		return parLen;
	}
	else
	{
		const VArc *a = GetArc();
		double parLen = fabs (a->GetRadius() * t * a->GetSweepAngle() * DEGREE_TO_RAD);
		return parLen;
	}
}

bool LineOrArc::TrimLineTo(VLine &l)const
{
	if (m_type == line)
	{
		// line line
		const VLine *l1 = GetLine();
		LineIntersection inter;
		VPoint ip,pit;
		LineIntersection::intersectiontype tp = inter.LinesIntersect(l1->GetP1(),l1->GetV(),l.GetP1(),l.GetV(),ip,pit);
		if (tp == LineIntersection::LINESINTERSECT)
		{
			l.SetP2(ip);
			return true;
		}
	}
	else 
	{
		// arc line
		const VArc *a1 = GetArc();
		std::vector<VPoint> ip = a1->GetInt(l);
		VPoint ip0;
		if (ip.size()==1)
		{
			ip0 = ip.front();
			l.SetP2 (ip0);
			return true;
		}
		else if (ip.size()== 2)
		{
			double par1 = l.GetT (ip.front());
			double par2 = l.GetT (ip.back());
			if (par1 <= par2)
			{
				l.SetP2(ip.front());
				return true;
			}
			else
			{
				l.SetP2(ip.back());
				return true;
			}
		}
	}
	return false;
}

std::vector<LineOrArc> LineOrArc::Split(const VPoint &p, double threshold) const
{
	std::vector<LineOrArc> res;
	// Get the length 
	if (m_type == line)
	{
		const VLine *l = GetLine();
		double t = l->GetT(p);
		if (t<0 || t>1.0)
			return res;
		double len = (p-l->GetP1()).GetLength();
		return Split (len,threshold);
	}
	else
	{
		const VArc *a = GetArc();
		double angle = a->GetAngle(p);
		if (a->IsAngleOnArc(angle)==false)
			return res;
		VArc temp (a->GetCenter(),a->GetRadius(),a->GetAx(),a->GetStartPoint(),angle);
		double len = temp.GetArcLength();
		return Split(len,threshold);
	}
}

void LineOrArc::Transform(const TransformationMatrix &T)
{
	if (m_type == line)
	{
		*m_geo.m_l = T.Transform (*m_geo.m_l);
	}
	else
	{
		m_geo.m_a->Transform(T);
	}
}



bool LineOrArc::IsDegenerate ()const
{
	if (m_type == line)
	{
		const VLine *l = GetLine();
		//if (l->GetLength() < 1e-8 || l->GetLength() > 1.e8)
		//	return true;
		return l->IsDegenerate(m_ckLineMinLength, m_lineMinLengthAllowed* VGlobalValues::m_unitMultiplier
			, m_ckLineMaxLength, m_lineMaxLengthAllowed* VGlobalValues::m_unitMultiplier);
	}
	else
	{
		const VArc *a = GetArc();
		return a->IsDegenerate 
			(m_ckArcRadius, m_minArcRadius* VGlobalValues::m_unitMultiplier
			, m_ckStartVectorRadiusConformance, m_startVectorRadiusConformanceTolerance* VGlobalValues::m_unitMultiplier
			, m_ckIsAxisUnit, m_axisVectorUnitTolerance* VGlobalValues::m_unitMultiplier
			, m_ckIsAxisPerpToStartVector, m_axisPerpToStartVectorTolerance* VGlobalValues::m_unitMultiplier
			, m_ckSweepAngle, m_minSweepAngleTolerance* VGlobalValues::m_unitMultiplier
			, m_ckArcMinPerimeter, m_minArcPerimeterAllowed* VGlobalValues::m_unitMultiplier
			, m_ckArcMaxPerimeter, m_maxArcPerimeterAllowed* VGlobalValues::m_unitMultiplier);
	}
}

LAIntersectionResult LineOrArc::GetIntersections(const LineOrArc &g, vreal tol) const
{
	LAIntersectionResult rrr;
	rrr.m_isMoreThanPoints = false;
	std::vector<VPoint> res;
	if (m_type == line)
	{
		if (g.GetType() == line)
		{
			// line line
			const VLine *l1 = GetLine();
			const VLine *l2 = g.GetLine();
			LineIntersection inter;
			VPoint ip,pit;
			LineIntersection::intersectiontype tp = inter.LinesIntersect(l1->GetP1(),l1->GetV(),l2->GetP1(),l2->GetV(),ip,pit,tol);
			if (tp == LineIntersection::LINESINTERSECT)
			{
				res.push_back(ip);
			}
			else if (tp == LineIntersection::LINESPARALLEL_INTERSECTING)
			{
				bool ck = GetOverlapIf(*this, g, rrr.m_firstint, rrr.m_secondint, tol);
				//if (ck)
				rrr.m_isMoreThanPoints = true;
			}
			//else
			//{
			//	rrr.m_isMoreThanPoints = true;
			//}

		}
		else
		{
			// line arc
			const VLine *l1 = GetLine();
			const VArc *a2 = g.GetArc();
			std::vector<VPoint> ip = a2->GetInt(*l1);
			res = ip;
		}
	}
	else 
	{
		if (g.GetType() == line)
		{
			// arc line
			const VArc *a1 = GetArc();
			const VLine *l2 = g.GetLine();
			std::vector<VPoint> ip = a1->GetInt(*l2);
			res = ip;
		}
		else
		{
			// arc arc
			const VArc *a1 = GetArc();
			const VArc *a2 = g.GetArc();
			std::vector<VPoint> ip = a1->GetInt(*a2,tol);
			res = ip;
			if (a1->GetCenter().Compare(a2->GetCenter(), tol))
			{
				if (fabs(a1->GetRadius() - a2->GetRadius()) < tol)
				{
					//if (a1->IsOnArc(a2->GetStartPoint(), tol))
					//	rrr.m_isMoreThanPoints = true;
					//else if (a1->IsOnArc(a2->GetEndPoint(), tol))
					//	rrr.m_isMoreThanPoints = true;
					//else if (a2->IsOnArc(a1->GetStartPoint(), tol))
					//	rrr.m_isMoreThanPoints = true;
					//else if (a2->IsOnArc(a1->GetEndPoint(), tol))
					//	rrr.m_isMoreThanPoints = true;
					rrr.m_isMoreThanPoints = true;
					bool ckarcoverlap = GetOverlapIf(*this, g, rrr.m_firstint, rrr.m_secondint, tol);

				}
			}
		}
	}
	rrr.m_intp = res;
	return rrr;
}

SequentialLineSet LineOrArc::GetSl(double tol)const
{
	if (GetType() == line)
	{
		SequentialLineSet sl1 (2);
		const VLine *l = GetLine();
		sl1.InsertPoint(l->GetP1());
		sl1.InsertPoint(l->GetP2());
		return sl1;
	}
	else
	{
		//SequentialLineSet sl2 (breakno+1);
		//const VArc *a = GetArc();
		//double da = a->GetSweepAngle() / breakno;
		//for (double ang=0;fabs(ang)<=fabs(a->GetSweepAngle())+1.e-5;ang+=da)
		//{
		//	sl2.InsertPoint(a->GetP(ang));
		//}
		//return sl2;

		SequentialLineSet sl2 (50);
		const VArc *a = GetArc();
		
		vreal tolToUse = VGlobalValues::ONEMICRON_TO_VUNIT * 10;
		if (tolToUse > tol)
			tolToUse = tol;
		std::vector<double> tmpangles;
		FastArcDigitiser::Digitise(*a,tolToUse,sl2,tmpangles);
		return sl2;
	}
}

void LineOrArc::GetNearestPoints(const LineOrArc &g, double distthreshold , double arctol
								 ,std::vector<VPoint> &pvec1 , std::vector<VPoint> &pvec2)const
{
	SequentialLineSet sl1 = GetSl(arctol*0.5);
	SequentialLineSet sl2 = g.GetSl (arctol*0.5);
	double tol = distthreshold;
	double deltatol = (arctol - distthreshold) / 20;
	if (deltatol < 1.e-9)
		throw VException (( IDS_LinGetExcDelE),1);
	while (pvec1.size() == 0)
	{
		std::multimap<double,VLine> r;
		LineSetsMinDist::GetNearer(sl1,sl2,r,tol);
		if (r.size())
		{
			std::multimap<double,VLine>::iterator i=r.begin();
			std::multimap<double,VLine>::iterator e=r.end();
			for (i;i!=e;++i)
			{
				pvec1.push_back (i->second.GetP1());
				pvec2.push_back (i->second.GetP2());
			}
		}
		tol += deltatol;
		if (tol > arctol)
			break;
	}
	return;
}

LineOrArc LineOrArc::GetReversed() const
{
	if (GetType () == line)
	{
		VLine l = *GetLine ();
		l.Reverse ();
		LineOrArc r (l);
		return r;
	}
	else
	{
		const VArc *a = GetArc();
		VArc ra = a->GetReversed();
		LineOrArc r (ra);
		return r;
	}
}

void LineOrArc::FindMinPoint (const std::vector<LineOrArc> &loop , const VPoint &startpoint , size_t &mingeoi , VPoint &minp , double &mint)
{
	double mind = MAX_NUMBER_LIMIT;
	size_t mysize = loop.size();
	for (size_t j=0;j<mysize;++j)
	{
		const LineOrArc &g = loop[j];
		if (g.GetType() == LineOrArc::line)
		{
			VPoint tmpp;
			const VLine *l = g.GetLine();
			if (l == 0)
				throw VException (( IDS_LinFinExcNulLin),1);
			LinePointMinDist mindistcalc;
			double curdist = mindistcalc.MinDist(*l,startpoint,tmpp);
			if (curdist < mind)
			{
				mind = curdist;

				mingeoi = j;
				minp = tmpp;
				mint = l->GetT (minp);
			}
		}
		else if (g.GetType() == LineOrArc::arc)
		{
			double myangle;
			VPoint tmpp;
			const VArc *a = g.GetArc();
			if (a==0)
				throw VException (( IDS_LinFinExcNulArc),1);
			double curdist = a->GetMinDist(startpoint,tmpp,myangle);
			if (curdist < mind)
			{
				mind = curdist;
				mingeoi = j;
				minp = tmpp;
				mint = (myangle/a->GetSweepAngle());
			}
		}
	}
}

bool LineOrArc::FindIntPoint (const std::vector<LineOrArc> &loop , const VPoint &startpoint , double angle , size_t &mingeoi , VPoint &minp , double &mint)
{
	//IgesEntityAdderGeo myadder (m_prj->GetCommandToViewPoint());
	VPoint dirvec (cos(angle*DEGREE_TO_RAD) , sin(angle*DEGREE_TO_RAD),0.0);
	VLine myline (startpoint , dirvec * 100000);
	//myadder.SetLinkName(("MainLine"));
	//myadder.AddLine(myline,0.1);
	SequentialLineSet sl1 (1);
	sl1.InsertPoint(myline.GetP1());
	sl1.InsertPoint(myline.GetP2());
	double mind = MAX_NUMBER_LIMIT;
	double mind2 = MAX_NUMBER_LIMIT;
	size_t mysize = loop.size();
	for (size_t j=0;j<mysize;++j)
	{
		//CString temp;
		//temp.Format (("mindist%d"),j);
		//myadder.SetLinkName((LPCTSTR)temp);

		const LineOrArc &g = loop[j];
		if (g.GetType() == LineOrArc::line)
		{
			VPoint tmpp;
			const VLine *l = g.GetLine();
			if (l == 0)
				throw VException (( IDS_LinFinExcNulLin1),1);
			SequentialLineSet sl2 (1);
			sl2.InsertPoint(l->GetP1());
			sl2.InsertPoint(l->GetP2());
			double mydist;
			VLine intline;
			LineSetsMinDist::Calc(sl1,sl2,mydist,intline);
			double mydist2 = (intline.GetP2() - startpoint).GetLength();
			//myadder.AddCopiousData(sl2);
			//myadder.AddPoint(intline.GetP2());
			if (mydist < mind+1.e-4)
			{
				if (fabs(mydist-mind) < 1.e-4)
				{
					if (mydist2 < mind2 + 1.e-7)
					{
						mind = mydist;
						mind2 = mydist2;
						mingeoi = j;
						minp = intline.GetP2();
						mint = l->GetT(minp);
					}
				}
				else
				{
					mind = mydist;
					mind2 = mydist2;
					mingeoi = j;
					minp = intline.GetP2();
					mint = l->GetT(minp);
				}	
			}
		}
		else if (g.GetType() == LineOrArc::arc)
		{
			//double myangle;
			VPoint tmpp;
			const VArc *a = g.GetArc();
			if (a==0)
				throw VException (( IDS_LinFinExcNulArc1),1);
			SequentialLineSet sl2 (41);
			double da = a->GetSweepAngle() / 40;
			for (double ang=0;fabs(ang)<=fabs(a->GetSweepAngle())+1.e-5;ang+=da)
			{
				sl2.InsertPoint(a->GetP(ang));
			}
			double mydist;
			VLine minline;
			int mysizen = sl2.Size();
			for (int jjj = 0; jjj<mysizen; ++jjj)
			{
				VLine mycurline = sl2.operator [](jjj);
				SequentialLineSet sl3 (1);
				sl3.InsertPoint (mycurline.GetP1());
				sl3.InsertPoint (mycurline.GetP2());
				LineSetsMinDist::Calc (sl1 , sl3 , mydist , minline);

				double mydist2 = (minline.GetP2() - startpoint).GetLength();
				//myadder.AddCopiousData(sl2);
				//myadder.AddPoint(minline.GetP2());
				if (mydist < mind + 1.e-4)
				{
					if (fabs(mydist-mind)<1.e-4)
					{
						if (mydist2<mind2 + 1.e-7)
						{
							mind = mydist;
							mind2 = mydist2;
							mingeoi = j;
							minp = minline.GetP2();
							mint = a->GetAngle(minp)/a->GetSweepAngle();
						}
					}
					else
					{
						mind = mydist;
						mind2 = mydist2;
						mingeoi = j;
						minp = minline.GetP2();
						mint = a->GetAngle(minp)/a->GetSweepAngle();
					}
				}
			}
		}
	}
	return true;
}

const double LineOrArc::SplitThreshold = 0.001 * VGlobalValues::ONEMM_TO_VUNIT;

bool LineOrArc::IsClosed(const std::vector<LineOrArc> &loop , vreal tol)
{
	if (loop.size() == 0)
		return false;
	bool isClosed = loop.at(0).GetStartPoint().Compare(loop.at(loop.size() - 1).GetEndPoint(), tol);
	return isClosed;
}
bool LineOrArc::IsClosed(const std::vector<LineOrArc *> &loop, vreal tol)
{
	if (loop.size() == 0)
		return false;
	bool isClosed = loop.at(0)->GetStartPoint().Compare(loop.at(loop.size() - 1)->GetEndPoint(), tol);
	return isClosed;
}
void LineOrArc::ReorderLoop (std::vector<LineOrArc> &loop , size_t mingeoi , const VPoint &p , double mint , vreal tol)
{
	if (loop.size() == 0)
	{
		LogFileWriter::WriteUnknownExceptionLog("LineOrArc::ReorderLoop exception","Loop size zero");
		return;
	}
	bool isClosed = loop.at(0).GetStartPoint().Compare(loop.at(loop.size() - 1).GetEndPoint(), tol);
	if (isClosed == false)
	{
		vreal d1 = (loop.at(0).GetStartPoint() - p).GetLength();
		vreal d2 = (loop.at(loop.size() - 1).GetEndPoint() - p).GetLength();
		if (d2 < d1)
		{
			LineOrArc::Reverse(loop);
		}
		return;
	}

	size_t mysize = loop.size();
	if (mysize <= mingeoi)
		throw VException (( IDS_LinReoExcGeoInd),1);
	
	std::vector<LineOrArc> ordered;
	std::vector<LineOrArc> orderedEnd;
	LineOrArc &g = loop.at(mingeoi);
	if (g.GetType()==LineOrArc::line)
	{
		const VLine *l = g.GetLine();
		if (l == 0)
			throw VException (( IDS_LinReoExcLinNul),1);
		std::vector<VLine> splited = l->Split (mint,SplitThreshold * VGlobalValues::m_unitMultiplier);
		if (splited.size()==2)
		{
			LineOrArc tmp1;
			tmp1.SetLine(splited[1]);
			ordered.push_back(tmp1);
			LineOrArc tmp2;
			tmp2.SetLine(splited[0]);
			orderedEnd.push_back(tmp2);
		}
		else if (splited.size()==1)
		{
			LineOrArc tmp;
			tmp.SetLine (splited.front());
			if (fabs(mint-1.0)< fabs(mint))
			{
				orderedEnd.push_back(tmp);
			}
			else
			{
				ordered.push_back(tmp);
			}
		}
		else 
			throw VException (( IDS_LinReoExcSplLin),1);
	}
	else
	{
		const VArc *a = g.GetArc();
		if (a==0)
			throw VException (( IDS_LinReoExcArcNul),1);
		std::vector<VArc> splited = a->Split(mint*a->GetSweepAngle(),SplitThreshold * VGlobalValues::m_unitMultiplier);
		if (splited.size() == 2)
		{
			LineOrArc tmp1;
			tmp1.SetArc (splited[1]);
			ordered.push_back(tmp1);
			LineOrArc tmp2;
			tmp2.SetArc(splited[0]);
			orderedEnd.push_back(tmp2);
		}
		else if (splited.size() == 1)
		{
			LineOrArc tmp;
			tmp.SetArc(splited.front());
			if (fabs(mint - 1.0) < fabs (mint) )
			{
				orderedEnd.push_back(tmp);
			}
			else
			{
				ordered.push_back(tmp);
			}
		}
		else
			throw VException (( IDS_LinReoExcSplArc),1);
	}
	
	for (size_t w=mingeoi+1;w<mysize;++w)
	{
		ordered.push_back(loop.at(w));
	}
	for (size_t j=0;j<mingeoi;++j)
	{
		ordered.push_back(loop.at(j));
	}
	if (orderedEnd.size() == 1)
		ordered.push_back (orderedEnd.front());
	loop = ordered;
}

void LineOrArc::Serialize(Bofstream &f) const
{
	int t = (int)m_type;
	f.WriteInt(t);
	if (m_type == line)
	{
		m_geo.m_l->Serialize(f);
	}
	else
	{
		m_geo.m_a->Serialize(f);
	}
}

void LineOrArc::Serialize (Bifstream &f)
{
	int t;
	f.ReadInt(t);
	m_type = (type)t;
	if (m_type == line)
	{
		VLine tmpl;
		tmpl.Serialize(f);
		DeleteData();
		m_geo.m_l = new VLine (tmpl);
	}
	else 
	{
		VArc tmpa;
		tmpa.Serialize(f);
		DeleteData();
		m_geo.m_a = new VArc (tmpa);
	}
}

void LineOrArc::SerializeVec(const std::vector<LineOrArc> &vec , Bofstream &f)
{
	size_t mysize = vec.size();
	f.WriteInt((int)mysize);
	for (size_t i=0;i<mysize;++i)
	{
		vec.at(i).Serialize (f);
	}
}

void LineOrArc::SerializeVec(std::vector<LineOrArc> &vec, Bifstream &f)
{
	int mysize;
	f.ReadInt(mysize);
	for (int i=0;i<mysize;++i)
	{
		LineOrArc tmp;
		tmp.Serialize(f);
		vec.push_back(tmp);
	}
}

VPoint LineOrArc::GetStartTangent() const
{
	if (m_type == line)
	{
		return GetLine()->GetV().Unit();
	}
	else
	{
		return GetArc()->GetUnitStartTangentVec();
	}
}
VPoint LineOrArc::GetEndTangent() const
{
	if (m_type == line)
	{
		return GetLine()->GetV().Unit();
	}
	else
	{
		return GetArc()->GetUnitEndTangentVec();
	}
}
bool LineOrArc::RepairGaps (const std::vector<LineOrArc> &original , std::vector<LineOrArc> &repaired , double tol)
{
	size_t i=1;
	size_t mysize = original.size();
	repaired = original;
	for (size_t i=1; i<mysize;++i)
	{
		LineOrArc &first = repaired.at(i-1);
		LineOrArc &second = repaired.at(i);
		VPoint p0 = first.GetEndPoint();
		VPoint p1 = second.GetStartPoint();
		double dd = (p1-p0).GetLength();
		if (dd < EPSILON)
		{
			// nothing , just continue	
		}
		else if (dd > tol)
		{
			return false;
		}
		else
		{
			// we need to repair
		/*	LineOrArc fc (first);
			LineOrArc sc (second);
			bool ck = RepairGapByExtendAndCut(fc , sc);
			if (ck)
			{
				first = fc;
				second = sc;
			}*/
			//else
			{
				RepairGapByChangingFirstsEnd(first,second);
			}
		}
	}
	return true;
}

bool LineOrArc::RepairGapByExtendAndCut (LineOrArc &first , LineOrArc &second)
{



	return true;
}
void LineOrArc::RepairGapByChangingFirstsEnd (LineOrArc &first , LineOrArc &second)
{
	if (first.GetType() == line)
	{
		VLine tmp = *first.GetLine();
		tmp.SetP2 (second.GetStartPoint());
		first.SetLine(tmp);
	}
	else
	{
		VArc tmp = *first.GetArc();
		VArc newarc (tmp.GetStartPoint() , tmp.GetP(tmp.GetSweepAngle()*0.5) , second.GetStartPoint());
		if (tmp.GetAx() % newarc.GetAx() < 0.0)
			newarc.ReverseAxis();
		first.SetArc(newarc);
	}
}
void LineOrArc::RepairGapByChangingSecondsStart (/*LineOrArc &first*/const VPoint &ep , LineOrArc &second)
{
	if (second.GetType() == line)
	{
		VLine tmp = *second.GetLine();
		tmp.SetP1 (ep);
		second.SetLine(tmp);
	}
	else
	{
		VArc tmp = *second.GetArc();
		VArc newarc (ep , tmp.GetP(tmp.GetSweepAngle()*0.5) , tmp.GetEndPoint());
		if (tmp.GetAx() % newarc.GetAx() < 0.0)
			newarc.ReverseAxis();
		second.SetArc(newarc);
	}
}
double LineOrArc::MaxDeviation (const LineOrArc &original , const LineOrArc &modified)
{
	if (original.GetType() == line)
	{
		if (modified.GetType() != line)
			throw VException (( IDS_LinMaxExcModGeo),1);
		// Compare 2 lines by p1 to p1 and p2 to p2 distances
		const VLine *l1 = original.GetLine();
		const VLine *l2 = modified.GetLine();
		//double d1 = (l1->GetP1()-l2->GetP1()).GetLength();
		//double d2 = (l1->GetP2()-l2->GetP2()).GetLength();
		//double maxd = d1 > d2 ? d1 : d2;
		LinePointMinDist mycalc;
		VPoint tmpp;
		double maxd(MIN_NUMBER_LIMIT);
		vreal d1 = mycalc.MinDist (*l1,l2->GetP1(),tmpp);
		if (d1 > maxd)
			maxd = d1;
		vreal d2 = mycalc.MinDist (*l1,l2->GetP2(),tmpp);
		if (d2 > maxd)
			maxd = d2;
		vreal d3 = mycalc.MinDist (*l2 , l1->GetP1() , tmpp);
		if (d3 > maxd)
			maxd = d3;
		vreal d4 = mycalc.MinDist (*l2 , l1->GetP2() , tmpp);
		if (d4 > maxd)
			maxd = d4;
		return maxd;
	}
	else
	{
		if (modified.GetType() != arc)
			throw VException (( IDS_LinMaxExcModGeo1),1);
		// Compare 2 arcs by p1 to p1 and p2 to p2 distances 
		const VArc *a1 = original.GetArc();
		const VArc *a2 = modified.GetArc();
		double d1 = (a1->GetStartPoint() - a2->GetStartPoint()).GetLength();
		double d2 = (a1->GetEndPoint() - a2->GetEndPoint()).GetLength();
		double dmax = d1 > d2 ? d1 : d2;
		return dmax;
	}
}

void LineOrArc::LoopSort  (std::vector<LineOrArc> &toSort 
							, const VPoint &startPoint 
							, std::vector<std::vector<LineOrArc>> &loopsVec
							, std::vector<bool> &isClosedVec
							, double cointol
							, double maxSewDist)
{
	if (toSort.size()==0)
		return;
	vreal loopLen = 0.0;
	VPoint myep = startPoint;
	std::vector<LineOrArc>::iterator bcItr;
	std::vector<LineOrArc> ordered;
	bool isRev;
	int coinCount (0);
	double mydist;
	mydist = GetClosest (toSort , myep , bcItr , isRev ,cointol ,coinCount);

	if (isRev)
		(*bcItr).Reverse();
	ordered.push_back (*bcItr);
	loopLen += (*bcItr).GetLength();
	myep = (*bcItr).GetEndPoint();
	toSort.erase (bcItr);

	while (toSort.size())
	{
		std::vector<LineOrArc>::iterator bcItr;
		bool isRev;
		int coinCount;
		mydist = GetClosest (toSort , myep , bcItr , isRev,cointol,coinCount);
		
		if (isRev)
			(*bcItr).Reverse();

		// now analyze 
		// possibilities:
		// 1- nearest curve first point is 
		// a - coincident with the last end point 
		//or b- within sewdist 
		//or c- not close but far
		bool isCaseC = false;

		if (mydist < cointol)
		{
			// case a , todo:add to contour and continue with other if statements 
			ordered.push_back (*bcItr);
			loopLen += (*bcItr).GetLength();
			myep = (*bcItr).GetEndPoint();
			toSort.erase (bcItr);		
		}
		else if (mydist < maxSewDist)
		{
			// case b , todo
			//AddSewLine (myep , *(*bcItr)->GetLines().Start() , ordered , ap);
			RepairGapByChangingSecondsStart (myep , *bcItr);
			ordered.push_back (*bcItr);
			loopLen += (*bcItr).GetLength();
			myep = (*bcItr).GetEndPoint();
			toSort.erase (bcItr);	
			 
			// Don't add a line but modify the next or previous line or arc such that it is sewed!!!
		}
		else 
		{
			// case c , the curve is far so add ordered as a contour but it should be open
			isCaseC = true;
			if (ordered.size())
			{
				VPoint mysp1 = ordered.front().GetStartPoint();
				VPoint myep1 = ordered.back().GetEndPoint();
				double myclosedist = ( mysp1 - myep1 ).GetLength();
				if (myclosedist < cointol && loopLen>myclosedist*4.0)
				{
					isClosedVec.push_back (true);
				}
				else if (myclosedist < maxSewDist && loopLen>myclosedist*4.0)
				{
					//AddSewLine (myep1 , mysp1 , ordered , ap);
					// don ' t add a line but sew by modifying
					RepairGapByChangingSecondsStart (myep1 , ordered.front());
					isClosedVec.push_back (true);
				}
				else
				{
					isClosedVec.push_back (false);
				}
				loopsVec.push_back (ordered);
			}
			ordered.clear();
			loopLen = 0.0;
			ordered.push_back (*bcItr);
			loopLen += (*bcItr).GetLength();
			myep = (*bcItr).GetEndPoint();
			toSort.erase (bcItr);	
		}

		//if (isCaseC == false) // this if is wrong the new ordered may have only one curve but closed!
		//{
		// 2- nearest curve has closed the loop  
		// a-exact coincident 
		// b-near within sew dist
		// c-not closed the loop yet
		VPoint mysp = ordered.front().GetStartPoint();
		double sed = (myep-mysp).GetLength();
		if (sed < cointol && sed*4.0<loopLen)
		{
			// case a , loop closed add a closed contour and continue
			loopsVec.push_back (ordered);
			isClosedVec.push_back (true);
			ordered.clear();
			loopLen = 0.0;
			if (toSort.size())
			{
				//ordered.push_back (toSort.front());
				//loopLen += toSort.front().GetLength();
				//myep = toSort.front().GetEndPoint();
				//toSort.erase (toSort.begin());
				mydist = GetClosest(toSort, myep, bcItr, isRev, cointol, coinCount);
				if (isRev)
					(*bcItr).Reverse();
				ordered.push_back(*bcItr);
				loopLen += (*bcItr).GetLength();
				myep = (*bcItr).GetEndPoint();
				toSort.erase(bcItr);
			}
		}
		else if (sed < maxSewDist && sed*4.0<loopLen)
		{
			// case b , loop will be closed after adding sew line
			//AddSewLine (myep , mysp , ordered , ap);
			// !!! modify lineorarc
			RepairGapByChangingSecondsStart (myep , ordered.front());
			loopsVec.push_back (ordered);
			isClosedVec.push_back (true);
			ordered.clear();
			loopLen = 0.0;
			if (toSort.size())
			{
				//ordered.push_back (toSort.front());
				//loopLen += toSort.front().GetLength();
				//myep = toSort.front().GetEndPoint();
				//toSort.erase (toSort.begin());
				mydist = GetClosest(toSort, myep, bcItr, isRev, cointol, coinCount);
				if (isRev)
					(*bcItr).Reverse();
				ordered.push_back(*bcItr);
				loopLen += (*bcItr).GetLength();
				myep = (*bcItr).GetEndPoint();
				toSort.erase(bcItr);
			}
		}
		//else
		//{
		//	// case c
		//	// do nothing continue loop with the current myep
		//}

		// continue while
	}

	if (toSort.size()==0 && ordered.size())
	{
		isClosedVec.push_back(false);
		VPoint mysp = ordered.front().GetStartPoint();
		double ld = (myep-mysp).GetLength();
		if ( ld < cointol && ld*4.0<loopLen)
			isClosedVec.back() = true;
		else if (ld < maxSewDist && ld*4.0<loopLen)
		{
			//AddSewLine (myep , mysp , ordered , ap);	// Adds a line to the end to close	
			// !!! modify line or arc
			RepairGapByChangingSecondsStart (myep , ordered.front());
			isClosedVec.back () =true;
		}
		loopsVec.push_back(ordered);
	}
	myep = MergeOpenContours(startPoint, loopsVec, isClosedVec, cointol, maxSewDist);
}
void LineOrArc::LoopSortNoReverse
							(std::vector<LineOrArc> &toSort 
							, const VPoint &startPoint 
							, std::vector<std::vector<LineOrArc>> &loopsVec
							, std::vector<bool> &isClosedVec
							, double cointol
							, double maxSewDist)
{
	if (toSort.size()==0)
		return;
	vreal loopLen = 0.0;
	VPoint myep = startPoint;
	std::vector<LineOrArc>::iterator bcItr;
	std::vector<LineOrArc> ordered;
	int coinCount (0);
	double mydist;
	mydist = GetClosestNoReverse (toSort , myep , bcItr , cointol ,coinCount);

	ordered.push_back (*bcItr);
	loopLen += (*bcItr).GetLength();
	myep = (*bcItr).GetEndPoint();
	toSort.erase (bcItr);

	while (toSort.size())
	{
		std::vector<LineOrArc>::iterator bcItr;
		int coinCount;
		mydist = GetClosestNoReverse (toSort , myep , bcItr , cointol,coinCount);
		
		// now analyze 
		// possibilities:
		// 1- nearest curve first point is 
		// a - coincident with the last end point 
		//or b- within sewdist 
		//or c- not close but far
		bool isCaseC = false;

		if (mydist < cointol)
		{
			// case a , todo:add to contour and continue with other if statements 
			ordered.push_back (*bcItr);
			loopLen += (*bcItr).GetLength();
			myep = (*bcItr).GetEndPoint();
			toSort.erase (bcItr);		
		}
		else if (mydist < maxSewDist)
		{
			// case b , todo
			//AddSewLine (myep , *(*bcItr)->GetLines().Start() , ordered , ap);
			RepairGapByChangingSecondsStart (myep , *bcItr);
			ordered.push_back (*bcItr);
			loopLen += (*bcItr).GetLength();
			myep = (*bcItr).GetEndPoint();
			toSort.erase (bcItr);	
			 
			// Don't add a line but modify the next or previous line or arc such that it is sewed!!!
		}
		else 
		{
			// case c , the curve is far so add ordered as a contour but it should be open
			isCaseC = true;
			if (ordered.size())
			{
				VPoint mysp1 = ordered.front().GetStartPoint();
				VPoint myep1 = ordered.back().GetEndPoint();
				double myclosedist = ( mysp1 - myep1 ).GetLength();
				if (myclosedist < cointol && myclosedist*4.0<loopLen)
				{
					isClosedVec.push_back (true);
				}
				else if (myclosedist < maxSewDist && myclosedist*4.0<loopLen)
				{
					//AddSewLine (myep1 , mysp1 , ordered , ap);
					// don ' t add a line but sew by modifying
					RepairGapByChangingSecondsStart (myep1 , ordered.front());
					isClosedVec.push_back (true);
				}
				else
				{
					isClosedVec.push_back (false);
				}
				loopsVec.push_back (ordered);
			}
			ordered.clear();
			loopLen = 0.0;
			ordered.push_back (*bcItr);
			loopLen += (*bcItr).GetLength();
			myep = (*bcItr).GetEndPoint();
			toSort.erase (bcItr);	
		}

		//if (isCaseC == false) // this if is wrong the new ordered may have only one curve but closed!
		//{
		// 2- nearest curve has closed the loop  
		// a-exact coincident 
		// b-near within sew dist
		// c-not closed the loop yet
		VPoint mysp = ordered.front().GetStartPoint();
		double sed = (myep-mysp).GetLength();
		if (sed < cointol && sed*4.0 < loopLen)
		{
			// case a , loop closed add a closed contour and continue
			loopsVec.push_back (ordered);
			isClosedVec.push_back (true);
			ordered.clear();
			loopLen = 0.0;
			if (toSort.size())
			{
				ordered.push_back (toSort.front());
				loopLen += toSort.front().GetLength();
				myep = toSort.front().GetEndPoint();
				toSort.erase (toSort.begin());
			}
		}
		else if (sed < maxSewDist && sed*4.0 < loopLen)
		{
			// case b , loop will be closed after adding sew line
			//AddSewLine (myep , mysp , ordered , ap);
			// !!! modify lineorarc
			RepairGapByChangingSecondsStart (myep , ordered.front());
			loopsVec.push_back (ordered);
			isClosedVec.push_back (true);
			ordered.clear();
			loopLen = 0.0;
			if (toSort.size())
			{
				ordered.push_back (toSort.front());
				loopLen += toSort.front().GetLength();
				myep = toSort.front().GetEndPoint();
				toSort.erase (toSort.begin());
			}
		}
		else
		{
			// case c
			// do nothing continue loop with the current myep
		}
		// continue while
	}

	if (toSort.size()==0 && ordered.size())
	{
		isClosedVec.push_back(false);
		VPoint mysp = ordered.front().GetStartPoint();
		double ld = (myep-mysp).GetLength();
		if ( ld < cointol && ld*4.0 < loopLen)
			isClosedVec.back() = true;
		else if (ld < maxSewDist && ld*4.0 < loopLen)
		{
			//AddSewLine (myep , mysp , ordered , ap);	// Adds a line to the end to close	
			// !!! modify line or arc
			RepairGapByChangingSecondsStart (myep , ordered.front());
			isClosedVec.back () =true;
		}
		loopsVec.push_back(ordered);
	}
}
double LineOrArc::GetClosest (std::vector<LineOrArc> &toSort 
							  , const VPoint &p 
							  , std::vector<LineOrArc>::iterator &closeCrvItr
							  , bool &isReversed
							  , const double coinTol
							  , int &coinCount)
{
	double coinTol2 = coinTol * coinTol;
	coinCount = 0;
	std::vector<LineOrArc>::iterator i = toSort.begin();
	std::vector<LineOrArc>::iterator e = toSort.end();
	closeCrvItr = i;
	double minDist2 = MAX_NUMBER_LIMIT;
	for (i;i!=e;++i)
	{
		double db_2 = (p - (*i).GetStartPoint()).GetLength2();
		double	de_2 = (p - (*i).GetEndPoint()).GetLength2();
		if (db_2 < minDist2)
		{
			//if (coinCount == 0) // if there are more than one coincident points, the first one in vector is selected as closest
			//{
				closeCrvItr = i;
				isReversed = false;
				minDist2 = db_2;
			//}
		}
		if(db_2 < coinTol2)
			coinCount++;
		if (de_2 < minDist2)
		{
			//if (coinCount == 0) // if there are more than one coincident points, the first one in vector is selected as closest
			//{
				closeCrvItr = i;
				isReversed = true;
				minDist2 = de_2;
			//}
		}
		if (de_2 < coinTol2)
			coinCount++;
	}
	return sqrt (minDist2);
}
double LineOrArc::GetClosestNoReverse (std::vector<LineOrArc> &toSort 
							  , const VPoint &p 
							  , std::vector<LineOrArc>::iterator &closeCrvItr
							  , const double coinTol
							  , int &coinCount)
{
	double coinTol2 = coinTol * coinTol;
	coinCount = 0;
	std::vector<LineOrArc>::iterator i = toSort.begin();
	std::vector<LineOrArc>::iterator e = toSort.end();
	closeCrvItr = i;
	double minDist2 = MAX_NUMBER_LIMIT;
	for (i;i!=e;++i)
	{
		double db_2 = (p - (*i).GetStartPoint()).GetLength2();
		if (db_2 < minDist2)
		{
			if (coinCount == 0) // if there are more than one coincident points, the first one in vector is selected as closest
			{
				closeCrvItr = i;
				minDist2 = db_2;
			}
		}
		if(db_2 < coinTol2)
			coinCount++;
	}
	return sqrt (minDist2);
}
void LineOrArc::Transform(std::vector<LineOrArc> &vec, const TransformationMatrix &tm)
{
	size_t mysize = vec.size();
	for (size_t i=0;i<mysize;++i)
	{
		LineOrArc &g = vec[i];
		g.Transform (tm);
	}
}
void LineOrArc::TransformFromAbsToLocalCs (const std::vector<LineOrArc> &vecOnAbs , const CoordinateSystem &cs, std::vector<LineOrArc> &vecToCs)
{
	size_t mysize = vecOnAbs.size();
	for (size_t i=0;i<mysize;++i)
	{
		const LineOrArc &la = vecOnAbs[i];
		if (la.GetType() == line)
		{
			const VLine *l = la.GetLine();
			VLine lb (cs.AbsoluteCoordToRelativeCoord (l->GetP1()) , cs.AbsoluteCoordToRelativeCoord(l->GetP2()));
			LineOrArc nla (lb);
			vecToCs.push_back (nla);
		}
		else
		{
			const VArc *a = la.GetArc();
			VArc ab (cs.AbsoluteCoordToRelativeCoord (a->GetCenter()) , a->GetRadius() , cs.AbsoluteVectorToRelativeVector (a->GetAx()) , cs.AbsoluteCoordToRelativeCoord (a->GetStartPoint()) , a->GetSweepAngle());
			LineOrArc nla (ab);
			vecToCs.push_back (nla);
		}
	}
}
void LineOrArc::TransformBackFromLocalCsToAbs(const std::vector<LineOrArc> &vecOnLocalXY, const CoordinateSystem &cs, std::vector<LineOrArc> &vecToAbs)
{
	size_t mysize2 = vecOnLocalXY.size();
	for (size_t j=0;j<mysize2;++j)
	{
		const LineOrArc &la = vecOnLocalXY[j];
		if (la.GetType() == LineOrArc::line)
		{
			const VLine *l = la.GetLine();
			VLine lb (cs.RelativeCoordToAbsoluteCoord(l->GetP1()) , cs.RelativeCoordToAbsoluteCoord(l->GetP2()));
			LineOrArc nla (lb);
			vecToAbs.push_back(nla);
		}
		else
		{
			const VArc *a = la.GetArc();
			VArc ab (cs.RelativeCoordToAbsoluteCoord(a->GetCenter()) , a->GetRadius() , cs.RelativeVectorToAbsoluteVector(a->GetAx()),cs.RelativeCoordToAbsoluteCoord(a->GetStartPoint()) , a->GetSweepAngle());		
			LineOrArc nla (ab);
			vecToAbs.push_back(nla);
		}
	}
}
void LineOrArc::Transform(std::vector<LineOrArc*>& vec, const TransformationMatrix& tm)
{
	size_t mysize = vec.size();
	for (size_t i = 0; i < mysize; ++i)
	{
		LineOrArc& g = *vec[i];
		g.Transform(tm);
	}
}
void LineOrArc::TransformFromAbsToLocalCs(const std::vector<LineOrArc*>& vecOnAbs, const CoordinateSystem& cs, std::vector<LineOrArc*>& vecToCs)
{
	size_t mysize = vecOnAbs.size();
	for (size_t i = 0; i < mysize; ++i)
	{
		const LineOrArc& la = *vecOnAbs[i];
		if (la.GetType() == line)
		{
			const VLine* l = la.GetLine();
			VLine lb(cs.AbsoluteCoordToRelativeCoord(l->GetP1()), cs.AbsoluteCoordToRelativeCoord(l->GetP2()));
			LineOrArc* nla = new LineOrArc(lb);
			vecToCs.push_back(nla);
		}
		else
		{
			const VArc* a = la.GetArc();
			VArc ab(cs.AbsoluteCoordToRelativeCoord(a->GetCenter()), a->GetRadius(), cs.AbsoluteVectorToRelativeVector(a->GetAx()), cs.AbsoluteCoordToRelativeCoord(a->GetStartPoint()), a->GetSweepAngle());
			LineOrArc* nla = new LineOrArc(ab);
			vecToCs.push_back(nla);
		}
	}
}
void LineOrArc::TransformBackFromLocalCsToAbs(const std::vector<LineOrArc*>& vecOnLocalXY, const CoordinateSystem& cs, std::vector<LineOrArc*>& vecToAbs)
{
	size_t mysize2 = vecOnLocalXY.size();
	for (size_t j = 0; j < mysize2; ++j)
	{
		const LineOrArc& la = *vecOnLocalXY[j];
		if (la.GetType() == LineOrArc::line)
		{
			const VLine* l = la.GetLine();
			VLine lb(cs.RelativeCoordToAbsoluteCoord(l->GetP1()), cs.RelativeCoordToAbsoluteCoord(l->GetP2()));
			LineOrArc* nla = new LineOrArc(lb);
			vecToAbs.push_back(nla);
		}
		else
		{
			const VArc* a = la.GetArc();
			VArc ab(cs.RelativeCoordToAbsoluteCoord(a->GetCenter()), a->GetRadius(), cs.RelativeVectorToAbsoluteVector(a->GetAx()), cs.RelativeCoordToAbsoluteCoord(a->GetStartPoint()), a->GetSweepAngle());
			LineOrArc* nla = new LineOrArc(ab);
			vecToAbs.push_back(nla);
		}
	}
}
void LineOrArc::ReplaceMinMax(VPoint &mn , VPoint &mx , int arcDivideCount)const
{
	if (GetType() == line)
	{
		const VLine *l = GetLine();
		mn.ReplaceMin (l->GetMin());
		mx.ReplaceMax (l->GetMax());
	}
	else
	{
		const VArc * a = GetArc();
		a->ReplaceMinMax (mn,mx,arcDivideCount);
	}
}

void LineOrArc::ReplaceMinMaxInCs(const CoordinateSystem &cs , VPoint &mn , VPoint &mx , int arcDivideCount)const
{
	if (GetType() == line)
	{
		const VLine *l = GetLine();
		VPoint p1cs = cs.AbsoluteCoordToRelativeCoord (l->GetP1());
		VPoint p2cs = cs.AbsoluteCoordToRelativeCoord (l->GetP2());
		mn.ReplaceMin (p1cs);
		mx.ReplaceMax (p2cs);
	}
	else
	{
		const VArc * a = GetArc();
		a->ReplaceMinMaxInCs (cs,mn,mx,arcDivideCount);
	}
}

bool LineOrArc::IsParallelToCsXY (const std::vector<LineOrArc> &loop , const CoordinateSystem &cs , double tol )
{
	size_t mysize = loop.size();
	for (size_t i=0;i<mysize;++i)
	{
		const LineOrArc &loa = loop[i];
		if (loa.IsParallelToCsXY (cs,tol) == false)
			return false;
	}
	return true;
}

bool LineOrArc::IsParallelToCsXY (const CoordinateSystem &cs , double tol)const
{
	if (GetType() == line)
	{
		const VLine *l = GetLine ();
		double d1 = (l->GetP1() - cs.GetOrigin()) % cs.GetDirz();
		double d2 = (l->GetP2() - cs.GetOrigin()) % cs.GetDirz();
		if (fabs (d1 - d2) > tol)
			return false;
		else
			return true;
	}
	else
	{
		const VArc *a = GetArc();
		if ( fabs (1 - fabs (a->GetAx() % cs.GetDirz())) < EPSILON)
			return true;
		// control with 3 points and check the projected distance to X-Y plane
		double sweepangle = a->GetSweepAngle();
		if (fabs (sweepangle) < EPSILON)
			throw VException (( IDS_LinIsPExcArcSwe),1);
		VPoint p1 = a->GetP (0.);
		VPoint p2 = a->GetP (sweepangle * 0.3);
		VPoint p3 = a->GetP (sweepangle * 0.0);
		double d1 = (p1 - cs.GetOrigin()) % cs.GetDirz();
		double d2 = (p2 - cs.GetOrigin()) % cs.GetDirz();
		if (fabs (d1 - d2) > tol)
			return false;
		double d3 = (p3 - cs.GetOrigin()) % cs.GetDirz();
		if ( fabs (d1 - d3) > tol || fabs (d2 - d3) > tol)
			return false;
		return true;
	}
}

void LineOrArc::DigitiseByLengthStep (double lenStep , std::vector<VPoint> &digiVec)const
{
	double totalLen = this->GetLength();
	if (totalLen < 1.e-10)	
		return;	// too small , it might cause trouble while digitising!
	int pieceCount = (int) (totalLen / lenStep);
	double pieceCountDecimal = totalLen / lenStep;
	double singleStepPar = lenStep / totalLen;
	double remainingPart = pieceCountDecimal - pieceCount;
	double initialPar = singleStepPar * remainingPart * 0.5;
	// Add start point
	digiVec.push_back (GetP (0.0));
	if (initialPar < 1.e-8)
		initialPar = singleStepPar;
	if (pieceCount != 0)
	{
		for (double par = initialPar; par < 1.0-1.e-8; par+= singleStepPar)
		{
			digiVec.push_back (GetP (par));
		}
	}
	// Add end point
	digiVec.push_back (GetP (1.0));
}

VPoint LineOrArc::GetP(double par) const
{
	if (GetType () == line)
	{
		return GetLine()->GetP (par);
	}
	else
	{
		const VArc *a = GetArc();
		return a->GetP(par * a->GetSweepAngle());
	}
}

double LineOrArc::GetMinDist(const VPoint &p , VPoint &pointOnGeo) const
{
	double mindist;
	if (m_type == line)
	{
		const VLine *l = GetLine();
		LinePointMinDist calc;
		mindist = calc.MinDist (*l , p , pointOnGeo);
	}
	else
	{
		const VArc *a = GetArc ();
		double myangle;
		mindist = a->GetMinDist (p , pointOnGeo , myangle);
	}
	return mindist;
}

void LineOrArc::Get2dBox (std::vector<LineOrArc> &box2d , const VPoint &mn , const VPoint &mx , double z,double r , const VPoint &dirz)
{
	VPoint l1 (mn.x(),mx.y(),z);
	VPoint l2 (mx.x(),mx.y(),z);
	VPoint l3 (mx.x(),mn.y(),z);
	VPoint l4 (mn.x(),mn.y(),z);
	VLine ll1 (l1+UX*r,l2-UX*r);
	VLine ll2 (l2-UY*r,l3+UY*r);
	VLine ll3 (l3-UX*r,l4+UX*r);
	VLine ll4 (l4+UY*r,l1-UY*r);
	if (fabs(r)>EPSILON)
	{
		VArc la1 (VPoint (l2.x()-r,l2.y()-r,l2.z()),r , dirz , VPoint(l2.x()-r,l2.y(),l2.z()) , -90);
		VArc la2 (VPoint(l3.x()-r,l3.y()+r,l3.z()),r,dirz,VPoint(l3.x(),l3.y()+r,l3.z()),-90);
		VArc la3 (VPoint (l4.x()+r,l4.y()+r,l4.z()),r,dirz,VPoint(l4.x()+r,l4.y(),l4.z()),-90);
		VArc la4 (VPoint (l1.x()+r,l1.y()-r,l1.z()),r,dirz,VPoint(l1.x(),l1.y()-r,l1.z()),-90);

		box2d.push_back (LineOrArc (ll1));
		box2d.push_back (LineOrArc (la1));
		box2d.push_back (LineOrArc (ll2));
		box2d.push_back (LineOrArc (la2));
		box2d.push_back (LineOrArc (ll3));
		box2d.push_back (LineOrArc (la3));
		box2d.push_back (LineOrArc (ll4));
		box2d.push_back (LineOrArc (la4));
	}
	else
	{
		box2d.push_back (LineOrArc (ll1));
		box2d.push_back (LineOrArc (ll2));
		box2d.push_back (LineOrArc (ll3));
		box2d.push_back (LineOrArc (ll4));
	}
}
void LineOrArc::GetHeightLines (std::vector<LineOrArc> &heights , const VPoint &mn , const VPoint &mx , double r , const VPoint &dirz)
{
	VPoint l1 (mn.x(),mx.y(),mn.z());
	VPoint l2 (mx.x(),mx.y(),mn.z());
	VPoint l3 (mx.x(),mn.y(),mn.z());
	VPoint l4 (mn.x(),mn.y(),mn.z());
	VLine ll1 (l1+UX*r,l2-UX*r);
	VLine ll2 (l2-UY*r,l3+UY*r);
	VLine ll3 (l3-UX*r,l4+UX*r);
	VLine ll4 (l4+UY*r,l1-UY*r);
	double h = mx.z() - mn.z();
	if (fabs (h)<EPSILON)
		return;
	heights.push_back (LineOrArc(VLine (ll1.GetP2() , ll1.GetP2() + dirz * h)));	
	heights.push_back (LineOrArc(VLine (ll2.GetP2() , ll2.GetP2() + dirz * h)));
	heights.push_back (LineOrArc(VLine (ll3.GetP2() , ll3.GetP2() + dirz * h)));
	heights.push_back (LineOrArc(VLine (ll4.GetP2() , ll4.GetP2() + dirz * h)));

	if (fabs(r) > EPSILON)
	{
		heights.push_back (LineOrArc(VLine (ll1.GetP1() , ll1.GetP1() + dirz * h)));	
		heights.push_back (LineOrArc(VLine (ll2.GetP1() , ll2.GetP1() + dirz * h)));
		heights.push_back (LineOrArc(VLine (ll3.GetP1() , ll3.GetP1() + dirz * h)));
		heights.push_back (LineOrArc(VLine (ll4.GetP1() , ll4.GetP1() + dirz * h)));	
	}

}

bool LineOrArc::IsContourSingleFullCircle (const std::vector<LineOrArc> &contour)
{
	if (contour.size() != 1)
		return false;
	const LineOrArc &geo = contour.front();
	if (geo.GetType() == LineOrArc::line)
		return false;
	if (fabs(geo.GetArc()->GetSweepAngle() - 360.0) > 1.e-7)
		return false;	
	return true;
}

void LineOrArc::Digitise (const std::vector<LineOrArc> &contour , double tol , SequentialLineSet &sl)
{
	size_t mysize = contour.size();
	for (size_t i=0;i<mysize;++i)
	{
		const LineOrArc &geo = contour.at(i);
		SequentialLineSet cursl = geo.GetSl (tol);
		sl.AddBack (cursl);
	}
	sl.RemoveCoincidentWithinTol (tol*0.25);
}

bool LineOrArc::Compare (const LineOrArc &g2 , double tol)const
{
	if (this == &g2)
		return true;
	if (m_type != g2.GetType())
		return false;
	if (m_type == line)
	{
		const VLine *l = GetLine ();
		const VLine *l2 = g2.GetLine();
		if (l->Compare (*l2,tol) == false)
			return false;
	}
	else
	{
		const VArc *a = GetArc ();
		const VArc *a2 = g2.GetArc();
		if (a->Compare (*a2,tol) == false)
			return false;
	}
	return true;
}

bool LineOrArc::CompareReversed (const LineOrArc &g2 , double tol)const
{
	if (this == &g2)
		return false;
	LineOrArc g2reversed = g2.GetReversed();
	return ( this->Compare (g2reversed , tol) );
}

int LineOrArc::FindOffsetParentIf (const std::vector<LineOrArc> &originalSrcVec , const std::vector<LineOrArc> &collCkVec , int originalParenti , double tol)
{
	const LineOrArc &originalGeo = originalSrcVec.at(originalParenti);
	int mysize = (int)collCkVec.size();
	for (int i=0;i<mysize;++i)
	{
		const LineOrArc &g = collCkVec.at(i);
		if (g.Compare (originalGeo,tol))
			return i;
		if (g.CompareReversed (originalGeo,tol))
			return i;
	}
	return -1;
}
int LineOrArc::FindOffsetParentIf (const std::vector<LineOrArc *> &originalSrcVec , const std::vector<LineOrArc *> &collCkVec , int originalParenti , double tol)
{
	const LineOrArc &originalGeo = *originalSrcVec.at(originalParenti);
	int mysize = (int)collCkVec.size();
	for (int i=0;i<mysize;++i)
	{
		const LineOrArc &g = *collCkVec.at(i);
		if (g.Compare (originalGeo,tol))
			return i;
		if (g.CompareReversed (originalGeo,tol))
			return i;
	}
	return -1;
}
int LineOrArc::FindOffsetParentIf(const std::vector<LineOrArc*>& originalSrcVec, const std::vector<LineOrArc>& collCkVec, int originalParenti, double tol)
{
	const LineOrArc& originalGeo = *originalSrcVec.at(originalParenti);
	int mysize = (int)collCkVec.size();
	for (int i = 0; i < mysize; ++i)
	{
		const LineOrArc& g = collCkVec.at(i);
		if (g.Compare(originalGeo, tol))
			return i;
		if (g.CompareReversed(originalGeo, tol))
			return i;
	}
	return -1;
}
bool LineOrArc::IsCircle() const
{
	if (m_type == arc)
		return m_geo.m_a->IsFull();
		
	return false;
}

void LineOrArc::DestroyVector (std::vector<LineOrArc *> &toDelete)
{
	size_t mysize = toDelete.size();
	for (size_t i=0;i<mysize;++i)
	{
		LineOrArc *g = toDelete.at(i);
		delete g;
	}
	toDelete.clear();
}

void LineOrArc::AddToLineOrArcVec (const std::vector<VPoint> &pVec , std::vector<LineOrArc> &geoVec)
{
	if (pVec.size() == 0)
		return;
	const VPoint *prevP = &pVec.front();
	size_t mysize = pVec.size();
	for (size_t i=1;i<mysize;++i)
	{
		const VPoint *p = &pVec.at(i);
		VLine l (*prevP,*p);
		prevP = p;
		geoVec.push_back (l);
	}
}
void LineOrArc::AddToLineOrArcVec (const std::vector<VPoint> &pVec , std::vector<LineOrArc *> &geoVec)
{
if (pVec.size() == 0)
		return;
	const VPoint *prevP = &pVec.front();
	size_t mysize = pVec.size();
	for (size_t i=1;i<mysize;++i)
	{
		const VPoint *p = &pVec.at(i);
		VLine l (*prevP,*p);
		prevP = p;
		geoVec.push_back (new LineOrArc(l));
	}
}

void LineOrArc::SerializeVecVec (const std::vector<std::vector<LineOrArc>> &vecvec , Bofstream &f)
{
	size_t mysize = vecvec.size();
	f.WriteInt((int)mysize);
	for (size_t i=0;i<mysize;++i)
	{
		const std::vector<LineOrArc> &vec = vecvec.at(i);
		LineOrArc::SerializeVec(vec,f);
	}
}
void LineOrArc::SerializeVecVec (std::vector<std::vector<LineOrArc>> &vecvec , Bifstream &f)
{
	int mysize;
	f.ReadInt(mysize);
	for (int i=0;i<mysize;++i)
	{
		std::vector<LineOrArc> vec;
		LineOrArc::SerializeVec(vec,f);
		vecvec.push_back(vec);
	}
}
void LineOrArc::SerializeVecVec (const std::vector<std::vector<double>> &vecvec , Bofstream &f)
{
	size_t mysize = vecvec.size();
	f.WriteInt((int)mysize);
	for (size_t i=0;i<mysize;++i)
	{
		const std::vector<double> &vec = vecvec.at(i);
		VectorSerializer::SerializeBasicTypeVec<double> (f,vec);
	}	
}
void LineOrArc::SerializeVecVec (std::vector<std::vector<double>> &vecvec , Bifstream &f)
{
	int mysize;
	f.ReadInt(mysize);
	for (int i=0;i<mysize;++i)
	{
		std::vector<double> vec;
		VectorSerializer::SerializeBasicTypeVec<double>(f,vec);
		vecvec.push_back(vec);
	}
}

void LineOrArc::FillTapFrontVec(std::vector<LineOrArc> &frontVec , bool isstart , bool isccw , int tapiCCW , int tapiCW , const std::vector<LineOrArc> &geoVec)
{
	frontVec.clear();
	int myi;
	if  (isccw)
		myi = tapiCCW;
	else
		myi = tapiCW;
	// This fills the offs vec by segments upto tap distance
	if (isstart)
	{
		int mysize = myi != -1 ? myi : (int)geoVec.size()-1;
		for (int i=0; i<= mysize;++i)
		{
			LineOrArc newg(geoVec.at(i));
			frontVec.push_back(newg);
		}
	}
	else
	{
		// CW
		int mysize = (int)geoVec.size();
		int begini = myi != -1 ? myi + 1 : 0;
		for (int i=begini; i<mysize; ++i)
		{
			LineOrArc newg (geoVec.at(i));
			frontVec.push_back(newg);
		}
	}
}

void LineOrArc::FillTapRemainingVec(std::vector<LineOrArc> &remainingVec , bool isstart , bool isccw , int tapiCCW , int tapiCW , const std::vector<LineOrArc> &geoVec)
{
	remainingVec.clear();
	int myi;
	if (isccw)
		myi = tapiCCW;
	else
		myi = tapiCW;

	if (isstart)
	{
		if (myi == -1)
			return;  // nothing to fill
		int mysize = (int)geoVec.size();
		for (int i=myi+1; i<mysize; ++i)
		{
			LineOrArc tmp (geoVec.at(i));
			remainingVec.push_back(tmp);
		}
	}
	else
	{
		// CW
		if (myi == -1)
			return; // nothing to fill
		for (int i=0;i<=myi;++i)
		{
			LineOrArc tmp(geoVec.at(i));
			remainingVec.push_back (tmp);
		}
	}
}

int LineOrArc::AddTapPoint(std::vector<LineOrArc> &vecGeo , double distFromBegining)
{
	std::vector<LineOrArc> newVec;
	int newPi = -1;
	double distTravelled = 0.;
	size_t mysize = vecGeo.size();
	for (size_t i = 0; i<mysize; ++i)
	{
		LineOrArc &g = vecGeo[i];
		double curDist = g.GetLength();
		double myleftDist = distFromBegining - distTravelled;
		if (curDist > myleftDist - (LineOrArc::SplitThreshold * VGlobalValues::m_unitMultiplier + EPSILON) && myleftDist > LineOrArc::SplitThreshold * VGlobalValues::m_unitMultiplier)
		{
			if (myleftDist<LineOrArc::SplitThreshold * 2 * VGlobalValues::m_unitMultiplier)
				myleftDist = LineOrArc::SplitThreshold * 4 * VGlobalValues::m_unitMultiplier;
			else if (fabs(curDist - myleftDist) < LineOrArc::SplitThreshold * 2 * VGlobalValues::m_unitMultiplier)
				myleftDist = curDist - 4 * LineOrArc::SplitThreshold* VGlobalValues::m_unitMultiplier;
			std::vector <LineOrArc> svec = g.Split(myleftDist, LineOrArc::SplitThreshold* VGlobalValues::m_unitMultiplier);
			size_t mysize2 = svec.size();
			if (mysize2 == 1)
				throw VException (( IDS_LinAddExcSplRes),1);
			for (size_t j=0;j<mysize2;++j)
				newVec.push_back(svec[j]);
			newPi = (int)i;
			distFromBegining = MAX_NUMBER_LIMIT; // make sure not to split again
		}
		else
		{
			newVec.push_back(g);
		}
		distTravelled += curDist;
	}
	if (newPi == -1)
		throw VException (( IDS_LinAddExcNewCan),1);
	vecGeo = newVec;
	return newPi;
}

bool LineOrArc::ExtendGeoByLengthAtStart (vreal extensionLen)
{
	if (m_type == line)
	{
		// line
		VLine *l = m_geo.m_l;
		VPoint newStartPoint = l->GetP1() - (l->GetV().Unit() * extensionLen);
		l->SetP1 (newStartPoint);
	}
	else
	{
		// arc
		VArc *a = m_geo.m_a;
		vreal curLen = fabs(a->GetSweepAngle() * DEGREE_TO_RAD * a->GetRadius());
		vreal goalLen = curLen + extensionLen;
		vreal fulLen = TWO_PI * a->GetRadius();
		if (goalLen > fulLen)
			return false;
		else
		{
			vreal newSweepAngle = goalLen / a->GetRadius();
			newSweepAngle *= RAD_TO_DEGREE;
			if (a->GetSweepAngle() < 0.0)
				newSweepAngle *= -1.;
			VArc revA (a->GetCenter(),a->GetRadius(),a->GetAx(),a->GetEndPoint(),-newSweepAngle);
			(*a) = VArc (revA.GetCenter() , revA.GetRadius() , revA.GetAx(),  revA.GetEndPoint() , newSweepAngle);
		}
	}

	return true;
}

bool LineOrArc::ExtendGeoByLengthAtEnd (vreal extensionLen)
{
	if (m_type == line)
	{
		// line
		VLine *l = m_geo.m_l;
		VPoint newEndPoint = l->GetP2() + (l->GetV().Unit() * extensionLen);
		l->SetP2 (newEndPoint);
	}
	else
	{
		// arc
		VArc *a = m_geo.m_a;
		vreal curLen = fabs(a->GetSweepAngle() * DEGREE_TO_RAD * a->GetRadius());
		vreal goalLen = curLen + extensionLen;
		vreal fulLen = TWO_PI * a->GetRadius();
		if (goalLen > fulLen)
			return false;
		else
		{
			vreal newSweepAngle = goalLen / a->GetRadius();
			newSweepAngle *= RAD_TO_DEGREE;
			if (a->GetSweepAngle() < 0.0)
				newSweepAngle *= -1.;
			VArc newA (a->GetCenter(),a->GetRadius(),a->GetAx(),a->GetStartPoint(),newSweepAngle);
			(*a) = newA;
		}
	}
	return true;
}

void LineOrArc::AddExtensionToClosedLoopAtEnd (const std::vector<LineOrArc> &originalLoop,vreal endExtension,std::vector<LineOrArc> &loopToAdd)
{	
	if (endExtension < EPSILON) 
		return; // No extension to add at end
		
	double leftdist = endExtension;
	int mysize = (int)originalLoop.size();
	std::vector<LineOrArc> svec;
	int breakpointi = -1;
	for (int i=0; i<mysize; ++i)
	{
		const LineOrArc & geo = originalLoop[i];
		double nowdist = geo.GetLength();
		if (leftdist <= nowdist)
		{
			// Split geo such that 
			svec = geo.Split(leftdist, LineOrArc::SplitThreshold* VGlobalValues::m_unitMultiplier);
			breakpointi = i;
			if (svec.size())
			{
				loopToAdd.push_back(svec.front());
			}
			break;
		}
		else 
		{
			leftdist -= nowdist;
			loopToAdd.push_back(geo);
		}
	}
	if (breakpointi == -1)
		throw VException (( IDS_WirAddExcExtLen),1);
}

void LineOrArc::AddExtensionToClosedLoopAtBegining (const std::vector<LineOrArc> &originalLoop , vreal frontExtension , std::vector<LineOrArc> &loopToAdd)
{
	std::vector<LineOrArc> temp = originalLoop;
	LineOrArc::Reverse (temp);
	std::vector<LineOrArc> temp2;
	AddExtensionToClosedLoopAtEnd (temp,frontExtension,temp2);
	LineOrArc::Reverse (temp2);
	size_t mysize = loopToAdd.size();
	for (size_t i=0;i<mysize;++i)
	{
		temp2.push_back (loopToAdd.at(i));
	}
	loopToAdd = temp2;
}

bool LineOrArc::IsContourCircle (const std::vector<LineOrArc> &loop , vreal tol , VArc &resultCircle)
{
	size_t mysize = loop.size();
	for (size_t i=0;i<mysize;++i)
	{
		const LineOrArc &curGeo = loop.at(i);
		if (curGeo.GetType() != arc)
		{
			return false;
		}
		else
		{
			if (i==0)
			{
				resultCircle = *curGeo.GetArc ();
				resultCircle.MakeFullCircle();
			}
			else
			{
				VArc tempCircle = *curGeo.GetArc();
				tempCircle.MakeFullCircle();
				if (resultCircle.Compare (tempCircle , tol)==false)
					return false;
				if (loop.at(i-1).GetEndPoint().Compare (loop.at(i).GetStartPoint() , tol) == false)
					return false;
			}
		}
	}
	return true;
}

void LineOrArc::DigitiseByLengthStep (const std::vector<LineOrArc> &laVec, vreal step, SequentialLineSet &sl)
{
	std::vector<VPoint> pVec;
	int laSize = (int)laVec.size();
	for (int i = 0; i < laSize; ++i)
	{
		laVec.at(i).DigitiseByLengthStep(step, sl);
	}
}
void LineOrArc::DigitiseByLengthStep (double lenStep , SequentialLineSet &sl)const
{
	double totalLen = this->GetLength();
	if (totalLen < 1.e-10)	
		return;	// too small , it might cause trouble while digitising!
	int pieceCount = (int) (totalLen / lenStep);
	double pieceCountDecimal = totalLen / lenStep;
	double singleStepPar = lenStep / totalLen;
	double remainingPart = pieceCountDecimal - pieceCount;
	double initialPar = singleStepPar * remainingPart * 0.5;
	// Add start point
	sl.InsertPoint(GetP (0.0));
	if (initialPar < 1.e-8)
		initialPar = singleStepPar;
	if (pieceCount != 0)
	{
		for (double par = initialPar; par < 1.0-1.e-8; par+= singleStepPar)
		{
			sl.InsertPoint(GetP (par));
		}
	}
	// Add end point
	sl.InsertPoint(GetP (1.0));
}
/*
void LineOrArc::GetSubLineOrArcVec(const std::vector<LineOrArc> &laVec, const VPoint &p1, const VPoint &p2, std::vector<LineOrArc> &result)
{
	size_t beg, end;
	vreal mint1, mint2;
	VPoint minp1, minp2;
	LineOrArc::FindMinPoint(laVec, p1, beg, minp1, mint1);
	LineOrArc::FindMinPoint(laVec, p2, end, minp2, mint2);
	if ( beg == end && fabs ( mint1 - mint2) < EPSILON) 
		return; // empty set
	if (beg > end)
	{
		std::swap(minp1,minp2);
		std::swap(mint1,mint2);
		std::swap(beg,end);
	}
	else if ( beg == end)
	{
		if(	mint1 > mint2)  
		{
			std::swap(minp1,minp2);
			std::swap(mint1,mint2);
			std::swap(beg,end);
		}
		LineOrArc l1 = laVec.at(beg);
		std::vector<LineOrArc> lVec1 = l1.Split(minp1,  VGlobalValues::ONEMICRON_TO_VUNIT * 10);
		if (lVec1.size() == 0)
			throw VException( IDS_LinGetSpl, 1);
	}
	LineOrArc lbeg = laVec.at(beg);
	std::vector<LineOrArc> lVec1 = lbeg.Split(minp1, VGlobalValues::ONEMICRON_TO_VUNIT * 10);
	if (lVec1.size() == 0)
		throw VException( IDS_LinGetSpl, 1);
	result.push_back(lVec1.back());

	for (size_t i = beg + 1; i < end; ++i)
		result.push_back(laVec.at(i));

	LineOrArc lend = laVec.at(end);
	std::vector<LineOrArc> lVec2 = lend.Split(minp2, VGlobalValues::ONEMICRON_TO_VUNIT * 10);
	if (lVec2.size() == 0)
		throw VException( IDS_LinGetSpl, 1);
	result.push_back(lVec2.back());
}


void LineOrArc::GetSubLineOrArcVec2(const std::vector<LineOrArc> &laVec, const VPoint &p1, const VPoint &p2, std::vector<LineOrArc> &result)
{
	int sz = laVec.size();
	int begItem = -1;
	int endItem = -1;
	vreal t0, t1;
	for (int i = 0; i < sz; ++i)
	{
		const LineOrArc &la = laVec.at(i);
		if (la.GetType() == line)
		{
			const VLine *ln = la.GetLine();
			if (ln->IsOnLine(p1))
			{
				begItem = i;
			}
			if (ln->IsOnLine(p2))
			{
				endItem = i;
			}
		}
		else
		{
			const VArc *arc = la.GetArc();
			if (arc->IsOnArc(p1, VGlobalValues::ONEMICRON_TO_VUNIT))
			{
				begItem = i;
			}
			if (arc->IsOnArc(p2, VGlobalValues::ONEMICRON_TO_VUNIT))
			{
				endItem = i;
			} 
		}
	}
	if (begItem > endItem || begItem == -1 || endItem == -1)
		return;
	//else if (begItem == endItem)
	//{

	//}

	LineOrArc la = laVec.at(begItem);
	if (la.GetType() == line)
	{
		const VLine *ln = la.GetLine();
		VLine l1(ln->GetP(ln->GetT(p1)), ln->GetP2());
		result.push_back(LineOrArc(l1));
	}
	else
	{
		const VArc *a = la.GetArc();
		vreal ang = a->GetSweepAngle() - a->GetAngle(p1);
		VArc arc(a->GetCenter(), a->GetRadius(), UZ, p1, ang);
		result.push_back(LineOrArc(arc));
	}


	for (int i = begItem + 1; i < endItem; ++i)
		result.push_back(laVec.at(i));
	
	LineOrArc endLa = laVec.at(endItem);
	if (la.GetType() == line)
	{
		const VLine *ln = la.GetLine();
		VLine l1(ln->GetP1(), ln->GetP(ln->GetT(p2)));
		result.push_back(LineOrArc(l1));
	}
	else
	{
		const VArc *a = endLa.GetArc();
		vreal ang = a->GetSweepAngle() - a->GetAngle(p2);
		VArc arc(a->GetCenter(), a->GetRadius(), UZ, p1, ang); 
		result.push_back(LineOrArc(arc));
	}

}
*/
VPoint LineOrArc::GetTan(vreal par) const
{ 
	VPoint p2;
	if (m_type == line)
	{
		p2 = m_geo.m_l->GetP2();
		return (p2 - m_geo.m_l->GetP1()).Unit();
	}
	else
	{
		p2 = GetP(par);
		VPoint rVec = (p2 - m_geo.m_a->GetCenter()).Unit();
		vreal fac = m_geo.m_a->GetSweepAngle() < 0.0 ? 1. : -1.;
		return (rVec ^ m_geo.m_a->GetAx()) * fac; 
	}
}


bool LineOrArc::GetParIf ( const VPoint &p , vreal &t,vreal tol)const
{
	if (this->GetType () == line)
	{
		const VLine *l = GetLine();
		return l->IsOnLine (p,t,tol);
	}
	else
	{
		const VArc *a = GetArc();
		CoordinateSystem arccs = a->GetCoordinateSystem();
		VPoint localp = arccs.AbsoluteCoordToRelativeCoord(p);
		if (localp.z() > tol)
			return false;
		vreal curRad2 = localp.x()*localp.x() + localp.y()*localp.y();//(p-a->GetCenter()).GetLength();
		//if (fabs (curRad - a->GetRadius()) > tol)
		//	return false;
		if (fabs(curRad2-(a->GetRadius()*a->GetRadius())) > tol * tol)
			return false;
		vreal ang = PolarCalc2d::GetPolarAngle(localp.x(), localp.y());//a->GetAngle(p);

		//if (a->GetSweepAngle() < 0.0 && fabs(ang) > tol*0.1)
		//	ang -= 360.0;
		vreal sweepAng = a->GetSweepAngle();
		//vreal angularTol = tol / a->GetRadius();
		//angularTol *= RAD_TO_DEGREE;
		//VInterval toCk(-angularTol, sweepAng+angularTol);
		VInterval toCk(0.0,sweepAng);
		t = ang / sweepAng;// a->GetSweepAngle();
		if (toCk.IsContaining(ang) || fabs(ang) < 0.00001 || fabs(360.0 - ang) < 0.00001)
			return true;
		vreal ang1 = ang + 360.0;
		t = ang1 / sweepAng;
		if (toCk.IsContaining(ang1) || fabs(ang1) < 0.00001 || fabs(360.0 - ang1) < 0.00001)
			return true;
		vreal ang2 = ang - 360.0;
		 t = ang2 / sweepAng;
		 if (toCk.IsContaining(ang2) || fabs(ang2) < 0.00001 || fabs(360.0 - ang2) < 0.00001)
			 return true;
		 return false;
		//if (sweepAng < 0.0)
		//{
		//	sweepAng += 360.0;
		//	toCk = VInterval(sweepAng-angularTol, 360.0+angularTol);
		//	if (ang < tol)
		//		ang += 360.;
		//	t = (360.0 - ang) / (360.0 - sweepAng);
		//}

		//if (ang > sweepAng+angularTol)
		//	return false;
		//if (toCk.IsContaining(ang) == false)
		//	return false;
	}
	return true;
}

void LineOrArc::GetMinMaxInCs (const std::vector<LineOrArc> &geoVec , const CoordinateSystem &cs , VPoint &mn , VPoint &mx)
{
	mn = VPoint (MAX_NUMBER_LIMIT);
	mx = VPoint (MIN_NUMBER_LIMIT);
	size_t mysize = geoVec.size();
	for (size_t i=0;i<mysize;++i)
	{
		const LineOrArc &curgeo = geoVec.at(i);
		VPoint curmn,curmx;
		curgeo.GetMinMaxInCs (cs,curmn,curmx);
		mn.ReplaceMin (curmn);
		mx.ReplaceMax (curmx);
	}
}

VPoint LineOrArc::GetMinInCs (const CoordinateSystem &cs)const
{
	VPoint mn (MAX_NUMBER_LIMIT);
	if (GetType() == line)
	{
		const VLine *l = GetLine ();
		mn.ReplaceMin (cs.AbsoluteCoordToRelativeCoord (l->GetP1()));
		mn.ReplaceMin (cs.AbsoluteCoordToRelativeCoord (l->GetP2()));
	}
	else
	{
		const VArc *a = GetArc();
		vreal R = a->GetRadius ();
		VPoint q1 ( a->GetCenter() + cs.GetDirx() * R);
		VPoint q2 ( a->GetCenter() - cs.GetDirx() * R);
		VPoint q3 ( a->GetCenter() + cs.GetDiry() * R);
		VPoint q4 ( a->GetCenter() - cs.GetDiry() * R);
		VPoint q5 ( a->GetCenter() + cs.GetDirz() * R);
		VPoint q6 ( a->GetCenter() - cs.GetDirz() * R);
		vreal ang;
		VPoint m1,m2,m3,m4,m5,m6;
		a->GetMinDist (q1,m1,ang);
		a->GetMinDist (q2,m2,ang);
		a->GetMinDist (q3,m3,ang);
		a->GetMinDist (q4,m4,ang);
		a->GetMinDist (q5,m5,ang);
		a->GetMinDist (q6,m6,ang);
		m1 = cs.AbsoluteCoordToRelativeCoord (m1);
		m2 = cs.AbsoluteCoordToRelativeCoord (m2);
		m3 = cs.AbsoluteCoordToRelativeCoord (m3);
		m4 = cs.AbsoluteCoordToRelativeCoord (m4);
		m5 = cs.AbsoluteCoordToRelativeCoord (m5);
		m6 = cs.AbsoluteCoordToRelativeCoord (m6);
		mn.ReplaceMin (m1);
		mn.ReplaceMin (m2);
		mn.ReplaceMin (m3);
		mn.ReplaceMin (m4);
		mn.ReplaceMin (m5);
		mn.ReplaceMin (m6);
	}
	return mn;
}

VPoint LineOrArc::GetMaxInCs (const CoordinateSystem &cs)const
{
	VPoint mx (MIN_NUMBER_LIMIT);
	if (GetType() == line)
	{
		const VLine *l = GetLine ();
		mx.ReplaceMax (cs.AbsoluteCoordToRelativeCoord (l->GetP1()));
		mx.ReplaceMax (cs.AbsoluteCoordToRelativeCoord (l->GetP2()));
	}
	else
	{
		const VArc *a = GetArc();
		vreal R = a->GetRadius ();
		VPoint q1 ( a->GetCenter() + cs.GetDirx() * R);
		VPoint q2 ( a->GetCenter() - cs.GetDirx() * R);
		VPoint q3 ( a->GetCenter() + cs.GetDiry() * R);
		VPoint q4 ( a->GetCenter() - cs.GetDiry() * R);
		VPoint q5 ( a->GetCenter() + cs.GetDirz() * R);
		VPoint q6 ( a->GetCenter() - cs.GetDirz() * R);
		vreal ang;
		VPoint m1,m2,m3,m4,m5,m6;
		a->GetMinDist (q1,m1,ang);
		a->GetMinDist (q2,m2,ang);
		a->GetMinDist (q3,m3,ang);
		a->GetMinDist (q4,m4,ang);
		a->GetMinDist (q5,m5,ang);
		a->GetMinDist (q6,m6,ang);
		m1 = cs.AbsoluteCoordToRelativeCoord (m1);
		m2 = cs.AbsoluteCoordToRelativeCoord (m2);
		m3 = cs.AbsoluteCoordToRelativeCoord (m3);
		m4 = cs.AbsoluteCoordToRelativeCoord (m4);
		m5 = cs.AbsoluteCoordToRelativeCoord (m5);
		m6 = cs.AbsoluteCoordToRelativeCoord (m6);
		mx.ReplaceMax (m1);
		mx.ReplaceMax (m2);
		mx.ReplaceMax (m3);
		mx.ReplaceMax (m4);
		mx.ReplaceMax (m5);
		mx.ReplaceMax (m6);
	}
	return mx;
}

void LineOrArc::GetMinMaxInCs (const CoordinateSystem &cs,VPoint &mn , VPoint &mx)const
{
	mn = VPoint (MAX_NUMBER_LIMIT);
	mx = VPoint (MIN_NUMBER_LIMIT);
	if (GetType() == line)
	{
		const VLine *l = GetLine ();
		mn.ReplaceMin (cs.AbsoluteCoordToRelativeCoord (l->GetP1()));
		mn.ReplaceMin (cs.AbsoluteCoordToRelativeCoord (l->GetP2()));
		mx.ReplaceMax (cs.AbsoluteCoordToRelativeCoord (l->GetP1()));
		mx.ReplaceMax (cs.AbsoluteCoordToRelativeCoord (l->GetP2()));
	}
	else
	{
		CoordinateSystem arcCs = GetArc()->GetCoordinateSystem();
		// is arcCs and cs XY parallel
		vreal dotpro = arcCs.GetDirz() % cs.GetDirz();
		bool ckOnPlane = false;
		if (fabs(fabs(dotpro) - 1.) < 1.e-6)
			ckOnPlane = true;
		if (ckOnPlane)
		{
			const VArc* a = GetArc();
			vreal R = a->GetRadius();
			VPoint q1(a->GetCenter() + cs.GetDirx() * R);
			VPoint q2(a->GetCenter() - cs.GetDirx() * R);
			VPoint q3(a->GetCenter() + cs.GetDiry() * R);
			VPoint q4(a->GetCenter() - cs.GetDiry() * R);
			//VPoint q5(a->GetCenter() + cs.GetDirz() * R);
			//VPoint q6(a->GetCenter() - cs.GetDirz() * R);
			//vreal ang;
			//VPoint m1,m2,m3,m4,m5,m6;
			//a->GetMinDist (q1,m1,ang);
			//a->GetMinDist (q2,m2,ang);
			//a->GetMinDist (q3,m3,ang);
			//a->GetMinDist (q4,m4,ang);
			//a->GetMinDist (q5,m5,ang);
			//a->GetMinDist (q6,m6,ang);
			//m1 = cs.AbsoluteCoordToRelativeCoord (m1);
			//m2 = cs.AbsoluteCoordToRelativeCoord (m2);
			//m3 = cs.AbsoluteCoordToRelativeCoord (m3);
			//m4 = cs.AbsoluteCoordToRelativeCoord (m4);
			//m5 = cs.AbsoluteCoordToRelativeCoord (m5);
			//m6 = cs.AbsoluteCoordToRelativeCoord (m6);
			//mn.ReplaceMin(m1);
			//mn.ReplaceMin(m2);
			//mn.ReplaceMin(m3);
			//mn.ReplaceMin(m4);
			//mn.ReplaceMin(m5);
			//mn.ReplaceMin(m6);
			//mx.ReplaceMax(m1);
			//mx.ReplaceMax(m2);
			//mx.ReplaceMax(m3);
			//mx.ReplaceMax(m4);
			//mx.ReplaceMax(m5);
			//mx.ReplaceMax(m6);
			if (a->IsPointOnArc(q1, 1.e-6))
			{
				VPoint lq1 = cs.AbsoluteCoordToRelativeCoord(q1);
				mn.ReplaceMin(lq1);
				mx.ReplaceMax(lq1);
			}
			if (a->IsPointOnArc(q2, 1.e-6))
			{
				VPoint lq2 = cs.AbsoluteCoordToRelativeCoord(q2);
				mn.ReplaceMin(lq2);
				mx.ReplaceMax(lq2);
			}
			if (a->IsPointOnArc(q3, 1.e-6))
			{
				VPoint lq3 = cs.AbsoluteCoordToRelativeCoord(q3);
				mn.ReplaceMin(lq3);
				mx.ReplaceMax(lq3);
			}
			if (a->IsPointOnArc(q4, 1.e-6))
			{
				VPoint lq4 = cs.AbsoluteCoordToRelativeCoord(q4);
				mn.ReplaceMin(lq4);
				mx.ReplaceMax(lq4);
			}
			VPoint lsp = cs.AbsoluteCoordToRelativeCoord(GetStartPoint());
			VPoint lep = cs.AbsoluteCoordToRelativeCoord(GetEndPoint());
			mn.ReplaceMin(lsp);
			mx.ReplaceMax(lsp);
			mn.ReplaceMin(lep);
			mx.ReplaceMax(lep);
		}
		else
		{
			// 3D so digitise 
			FastArcDigitiser ad;
			int sgno = ad.GetDigitiseSegmentNo(*GetArc(), 5.e-4);
			SequentialLineSet sl(sgno + 1);
			ad.Digitise(*GetArc(), sgno, sl);
			mn = sl.GetMinPointInCs(cs);
			mx = sl.GetMaxPointInCs(cs);
		}
	}
}

LineOrArc LineOrArc::GetSubSet (vreal t1 , vreal t2)const
{
	if (t1 < -EPSILON || t1 > 1.0+EPSILON || t2 < -EPSILON || t2 > 1.0+EPSILON || fabs (t2-t1) < EPSILON)
		throw VException (( IDS_LinGetExcTOrTOu),1);
	VPoint pstart = GetP (t1);
	VPoint pend = GetP (t2);
	if (m_type == line)
	{
		return LineOrArc (VLine (pstart,pend));
	}
	else
	{
		vreal compTol = 1.e-7;
		if (pstart.Compare(pend, compTol))
		{
			// full circle
			return *this;
		}
		VPoint pmid = GetP ((t1+t2) * 0.5);
		VArc tmp(pstart, pmid, pend);
		if (tmp.GetAx() % this->GetArc()->GetAx() < 0.0)
		{
			tmp.ReverseAxis();// Make sure axis orientation is not reversed
		}
		if (tmp.IsCCW() != this->GetArc()->IsCCW())
		{
			tmp = tmp.GetReversed(); // Make sure 
		}
		return LineOrArc (tmp);
	}
}

void LineOrArc::FindNearestAngle (const std::vector<LineOrArc> &geoVec , vreal angle , size_t &mingeoi , VPoint &minp , vreal &mint,vreal tol)
{
	vreal minDif = MAX_NUMBER_LIMIT;
	size_t mysize = geoVec.size();
	for (size_t i=0;i<mysize;++i)
	{
		const LineOrArc &geo = geoVec.at(i);
		if (geo.GetType() == LineOrArc::line)
		{
			const VLine *l = geo.GetLine();
			VPoint lv = l->GetV();
			lv.NormThis();
			vreal ang = PolarCalc2d::GetPolarAngle(lv.x(),lv.y());
			vreal curDif = fabs(angle - ang);
			if (curDif < minDif)
			{
				mingeoi = i;
				minp = l->GetP1();//l->GetMidPoint();
				mint = 0.0;//0.5;
				minDif = curDif;
			}
		}
		else
		{
			const VArc *a = geo.GetArc();
			std::vector<VPoint>pv;
			int pno = FastArcDigitiser::GetDigitiseSegmentNo(*a,tol);
			FastArcDigitiser::Digitise (*a,pno,pv);
			size_t nsize = pv.size();
			vreal invSweepAngle = 1.0 / a->GetSweepAngle();
			for (size_t jjj=0;jjj<nsize;++jjj)
			{
				VPoint arcpnt = pv.at(jjj);
				VPoint lv = a->GetTangAt(arcpnt);
				lv.NormThis();
				vreal ang = PolarCalc2d::GetPolarAngle(lv.x(),lv.y());
				vreal curDif = fabs(angle - ang);
				if (curDif < minDif)
				{
					mingeoi = i;
					minp = arcpnt;
					mint = a->GetAngle(arcpnt)*invSweepAngle;
					minDif = curDif;
				}
			}
		}
	}
}

//void LineOrArc::GroupByTangentAngleRange (std::list<std::pair<VInterval , std::list<VSignedInterval>>> &angleRangeGroupMap)const
//{
//	bool revFlg = false;
//	std::list<std::pair<VInterval,std::list<VSignedInterval>>>::iterator i = angleRangeGroupMap.begin();
//	std::list<std::pair<VInterval,std::list<VSignedInterval>>>::iterator e = angleRangeGroupMap.end();
//	if (GetType() == LineOrArc::line)
//	{
//		VPoint tanVecAtStart = GetTan (0.5);		
//		vreal tAstart = PolarCalc2d::GetPolarAngle (tanVecAtStart.x(),tanVecAtStart.y());
//
//		const VLine *l = GetLine();
//		VInterval ai (tAstart,tAstart);
//		for (i;i!=e;++i)
//		{
//			const VInterval &curi = i->first;
//			if (curi.IsContaining(ai))
//			{
//				VInterval geoi (0.0,1.0);
//				VSignedInterval geosi (geoi,1);
//				i->second.push_back (geosi);			
//			}
//		}
//	}
//	else
//	{
//		const VArc *a = GetArc();
//		std::vector<VSignedInterval> ivec;
//		int quadi[4];
//		a->SplitTo4Quadrants (ivec,quadi);
//		size_t mysize2 = ivec.size();
//		for(i;i!=e;++i)
//		{
//			for (size_t jj=0;jj<mysize2;++jj)
//			{
//				const VSignedInterval &sii = ivec.at(jj);
//				if (i->first.IsContaining(sii.GetInterval()))
//				{
//					i->second.push_back (sii);
//				}
//			}
//		}
//	}
//}
//	
//void LineOrArc::GroupByTangentAngleRange (const std::vector<LineOrArc> &contour , std::list<std::pair<VInterval , std::vector<LineOrArc>>> &angleRangeGroupMap)
//{
//
//	size_t mysize = contour.size();
//	for (size_t i=0;i<mysize;++i)
//	{
//		std::list<std::pair<VInterval,std::list<VSignedInterval>>> tmp;
//		std::list<std::pair<VInterval,std::vector<LineOrArc>>>::iterator ii = angleRangeGroupMap.begin();
//		std::list<std::pair<VInterval,std::vector<LineOrArc>>>::iterator ee = angleRangeGroupMap.end();
//		for (ii;ii!=ee;++ii)
//			tmp.push_back (std::pair<VInterval,std::list<VSignedInterval>>(ii->first,std::list<VSignedInterval>()));
//		const LineOrArc &curgeo = contour.at(i);
//		curgeo.GroupByTangentAngleRange (tmp);
//		ii = angleRangeGroupMap.begin();
//		std::list<std::pair<VInterval,std::list<VSignedInterval>>>::iterator iii=tmp.begin();
//		for (ii;ii!=ee;++ii)
//		{
//			std::list<VSignedInterval>::const_iterator ji = iii->second.begin();
//			std::list<VSignedInterval>::const_iterator je = iii->second.end();
//			for (ji;ji!=je;++ji)
//			{
//				LineOrArc cursub = curgeo.GetSubSet(ji->GetInterval().GetT0(),ji->GetInterval().GetT1());
//				if (ji->GetSignF() < 0)
//					cursub = cursub.GetReversed();
//				ii->second.push_back(cursub);
//			}
//			++iii;
//		}
//	}
//}

void LineOrArc::GroupByTangentAngle (const std::vector<LineOrArc> &contour 
	, std::list<std::pair<VInterval,std::list<LineOrArc>>> &angleRangeLineGeoGroups
	, std::list<std::pair<VInterval,std::list<LineOrArc>>> &angleRangeArcGeoGroups
	, bool letReversingGeometry)
{
	int contourSize = (int)contour.size();
	for (int ci=0;ci<contourSize;++ci)
	{
		const LineOrArc &curgeo = contour.at(ci);
		curgeo.GroupByTangentAngle(angleRangeLineGeoGroups,angleRangeArcGeoGroups,letReversingGeometry);
	}
}

void LineOrArc::GroupByTangentAngle (std::list<std::pair<VInterval,std::list<LineOrArc>>> &angleRangeLineGeoGroups
									,std::list<std::pair<VInterval,std::list<LineOrArc>>> &angleRangeArcGeoGroups
									,bool letReversingGeometry)const
{
	if ( m_type == line)
	{
		const VLine *l = GetLine();
		VPoint vec = l->GetV();
		vreal tangentAngle = PolarCalc2d::GetPolarAngle(vec.x(),vec.y());
		//if (fabs(tangentAngle) < 1.e-6)
		//	tangentAngle = 360.0;// not universal but for MKS we need this 
		
		////if (fabs(tangentAngle - 180.0) < EPSILON)
		////{
		////	tangentAngle += EPSILON;
		////}
		VInterval gi (tangentAngle,tangentAngle);
		std::list<std::pair<VInterval,std::list<LineOrArc>>>::iterator li = angleRangeLineGeoGroups.begin();
		std::list<std::pair<VInterval,std::list<LineOrArc>>>::iterator le = angleRangeLineGeoGroups.end();
		for (li;li!=le;++li)
		{
			if (li->first.IsContaining (gi))
			{
				LineOrArc lineToAdd (*l);
				//if (li->first == VInterval (0,90.0) || li->first == VInterval (90.0,180.0))
				//	lineToAdd = lineToAdd.GetReversed();
				li->second.push_back (lineToAdd);
				break;
			}
		}
	}
	else
	{
		const VArc *tmp = GetArc();
		VArc copyArc (*tmp);
		TransformationMatrix R;
		TransformationMatrix antiR;
		if (tmp->IsCCW() == true)
		{
			R = RotationMatrix(copyArc.GetCenter(), copyArc.GetAx(), 90.0);
			antiR = RotationMatrix(copyArc.GetCenter(), copyArc.GetAx(), -90.0);
		}
		else
		{
			R = RotationMatrix(copyArc.GetCenter(), copyArc.GetAx(), -90.0);
			antiR = RotationMatrix(copyArc.GetCenter(), copyArc.GetAx(), 90.0);
		}

		copyArc.Transform(R);
		std::list<std::pair<VInterval,std::list<LineOrArc>>>::iterator li = angleRangeArcGeoGroups.begin();
		std::list<std::pair<VInterval,std::list<LineOrArc>>>::iterator le = angleRangeArcGeoGroups.end();
		//std::vector<LineOrArc> res;
		std::list<VSignedInterval> geoInt;
		VPoint sp = copyArc.GetStartPoint()-copyArc.GetCenter();
		vreal sa = PolarCalc2d::GetPolarAngle (sp.x(),sp.y());
		//if (tmp->IsCCW() == false)
		//	sa+=180.0;
		vreal sw = copyArc.GetSweepAngle();
		PolarCalc2d::UnwrapArcToLineInterval (sa,sw,geoInt);
		for (li;li!=le;++li)
		{
			if (li->first.GetLength() < 1.e-3)
				continue;
			std::list<VSignedInterval>::const_iterator gi = geoInt.begin();
			std::list<VSignedInterval>::const_iterator ge = geoInt.end();
			for (gi;gi!=ge;++gi)
			{
				VInterval tangentInt (gi->GetInterval().GetT0(),gi->GetInterval().GetT1());
				std::vector<VInterval> intersection = li->first.Intersect(tangentInt);
				int intersectionSize = (int)intersection.size();
				for (int ii=0;ii<intersectionSize;++ii)
				{
					const VInterval &curint = intersection.at(ii);
					vreal curStartAngle = curint.GetT0();
					vreal curEndAngle = curint.GetT1();
					curStartAngle -= (sa);
					curEndAngle -= (sa);
					VPoint curStartPoint = copyArc.GetP(curStartAngle);
					vreal curSweepAng = curEndAngle - curStartAngle;
					VArc curResult (copyArc.GetCenter(),copyArc.GetRadius(),copyArc.GetAx(),curStartPoint,curSweepAng);
					if (letReversingGeometry && gi->GetSignF() < 0)
						curResult = curResult.GetReversed();
					curResult.Transform(antiR);
					li->second.push_back(LineOrArc(curResult));
				} 
			}
		}
	}

}

VPlane::IntType LineOrArc::GetIntPoints (const VPlane &pln , std::vector<VPoint> &intPoints , std::vector<VPoint> &intPointsFull)const
{
	if (m_type == line)
	{
		const VLine *l = GetLine();
		vreal intt;
		VPlane::IntType ck = pln.Int (*l,intt);
		if (intt == VPlane::REGULAR)
		{
			VPoint ip = l->GetP(intt);
			intPointsFull.push_back (ip);
			if (intt > -EPSILON && intt < 1+EPSILON)
			{
				intPoints.push_back (ip);
			}
		}
		else if (intt == VPlane::COINCIDENT)
		{
			intPointsFull.push_back (l->GetP1());
			intPointsFull.push_back (l->GetP2());
			intPoints.push_back (l->GetP1());
			intPoints.push_back (l->GetP2());
		}
		return ck;
	}
	else
	{
		const VArc *a = GetArc();
		VPlane::IntType ck = pln.Int(*a,intPoints,intPointsFull);
		return ck;
	}
}

LineOrArc LineOrArc::GetTrimmedAtStart (vreal distance)const//  Trim the geo by distance at start
{
	if (/*distance < EPSILON ||*/ distance  > this->GetLength())
		throw VException ("LineOrArc::GetTrimmedAtStartAndEnd exception , geometry L < trim distance",1);
	if (this->GetType() == line)
	{
		const VLine *l = this->GetLine();
		VPoint vec = l->GetV();
		vec.NormThis();
		VLine ltrimmed (l->GetP1() + vec * distance , l->GetP2());
		return LineOrArc (ltrimmed);
	}
	else
	{
		const VArc *a = this->GetArc();
		if (/*a->IsFull() &&*/ fabs(distance) < 1.e-8)
			return LineOrArc(*a);
		//if (distance > TWO_PI * a->GetRadius() || distance < EPSILON)
		//	throw VException ("LineOrArc::GetTrimmedAtStart exception , distance invalid",1);
		vreal anglex = distance / a->GetRadius();
		anglex *= RAD_TO_DEGREE;
		if (a->GetSweepAngle() < 0.0)
			anglex *= -1.;
		vreal midang = (anglex + a->GetSweepAngle()) * 0.5;
		VArc atrimmed ( a->GetP (anglex) , a->GetP(midang) , a->GetEndPoint());
		return LineOrArc (atrimmed);
	}
}

LineOrArc LineOrArc::GetTrimmedAtEnd (vreal distance)const// Trim the geo by distance at end
{
	if (/*distance < EPSILON ||*/ distance  > this->GetLength())
		throw VException ("LineOrArc::GetTrimmedAtStartAndEnd exception , geometry L < trim distance",1);
	if (this->GetType() == line)
	{
		const VLine *l = this->GetLine();
		VPoint vec = l->GetV();
		vec.NormThis();
		VLine ltrimmed (l->GetP1() , l->GetP2() -  vec * distance );
		return LineOrArc (ltrimmed);
	}
	else
	{
		const VArc *a = this->GetArc();
		if (/*a->IsFull() &&*/ fabs(distance) < 1.e-8)
			return LineOrArc(*a);
		if (distance > TWO_PI * a->GetRadius() || distance < EPSILON)
			throw VException ("LineOrArc::GetTrimmedAtStart exception , distance invalid",1);
		vreal anglex = distance / a->GetRadius();
		anglex *= RAD_TO_DEGREE;
		anglex = fabs(a->GetSweepAngle()) - anglex;
		if (a->GetSweepAngle() < 0.0)
			anglex *= -1.;
		vreal midang = anglex  * 0.5;
		VArc atrimmed ( a->GetStartPoint() , a->GetP(midang) , a->GetP(anglex));
		return LineOrArc (atrimmed);
	}
}

LineOrArc LineOrArc::GetTrimmedAtStartAndEnd (vreal distance)const
{
	if (/*distance < EPSILON ||*/ distance * 2. > this->GetLength())
		throw VException ("LineOrArc::GetTrimmedAtStartAndEnd exception , geometry L < trim distance",1);
	if (this->GetType() == line)
	{
		const VLine *l = this->GetLine();
		VPoint vec = l->GetV();
		vec.NormThis();
		VLine ltrimmed (l->GetP1() + vec * distance , l->GetP2() - vec * distance);
		return LineOrArc (ltrimmed);
	}
	else
	{
		const VArc *a = this->GetArc();
		if (distance > TWO_PI * a->GetRadius() || distance < EPSILON)
			throw VException ("LineOrArc::GetTrimmedAtStart exception , distance invalid",1);
		vreal anglex = distance / a->GetRadius();
		anglex *= RAD_TO_DEGREE;
		vreal angley = anglex;
		angley = fabs (a->GetSweepAngle()) - angley;

		if (a->GetSweepAngle() < 0.0)
		{
			anglex *= -1.;
			angley *= -1.;
		}
		vreal midang = (anglex + angley) * 0.5;
		VArc atrimmed ( a->GetP (anglex) , a->GetP(midang) , a->GetP(angley));
		return LineOrArc (atrimmed);
	}
}
bool LineOrArc::IsInside(const std::vector<LineOrArc>* bigLoop, const std::vector<LineOrArc>* smallLoop, vreal tol)// , bool fullCheck)
{
	//VPoint minBigP;
	//VPoint maxBigP;
	VPoint minSmallP, minBigLp;
	VPoint maxSmallP, maxBigLp;
	//bool fullCheck = true;
	//LineOrArc::GetMinMax(bigLoop,minBigP, maxBigP);
	LineOrArc::GetMinMax(*smallLoop, minSmallP, maxSmallP);
	LineOrArc::GetMinMax(*bigLoop, minBigLp, maxBigLp);

	bool ck1 = minBigLp.ReplaceMin(maxSmallP);//XXXXXXXX !!!!!!! (minSmallP);
	bool ck2 = maxBigLp.ReplaceMax(minSmallP);// (maxSmallP);

	//bool ck1 = LineOrArc::IsPointInLoop(*bigLoop, minSmallP);
	//bool ck2 = LineOrArc::IsPointInLoop(*bigLoop, maxSmallP);

	if (ck1 == false && ck2 == false)
	{
		// check all points of small
		//if (fullCheck == true)
		//{
		bool isPointOnLoop;
		bool choiceOnSuspicion(true);
			SequentialLineSet tempsl(40);
			LineOrArc::Digitise(*smallLoop, tol, tempsl);
			SequentialLineSet tempsl2(tempsl.GetPointSize());
			//tempsl.DigitiseByLengthStep(tempsl2, tol * 10.0, 0.0, 1.0);
			tempsl.DigitiseByLengthStepNew(tempsl2, tol * 10.0);
			int psize = tempsl2.GetPointSize();
			for (int pi = 0; pi < psize; ++pi)
			{
				const VPoint* curp = tempsl2.GetPoint(pi);
				bool ck3 = LineOrArc::IsPointInLoop(*bigLoop, *curp,tol,isPointOnLoop,choiceOnSuspicion);
				if (ck3 == false)
					return false;
			}
		//}
		//int loopSize = (int)smallLoop->size();
		//for (int i = 0; i < loopSize; ++i)
		//{
		//	const LineOrArc& curGeo = smallLoop->at(i);
		//	VPoint p0 = curGeo.GetP(0.25);
		//	VPoint p1 = curGeo.GetP(0.5);
		//	VPoint p2 = curGeo.GetP(0.75);
		//	bool pointOnLoop;
		//	bool choiceIfSuspicion = false;
		//	if (IsPointInLoop(*bigLoop, p0 , tol , pointOnLoop , choiceIfSuspicion) == false)
		//		return false;
		//	if (IsPointInLoop(*bigLoop, p1 , tol , pointOnLoop , choiceIfSuspicion) == false)
		//		return false;
		//	if (IsPointInLoop(*bigLoop, p2 , tol , pointOnLoop , choiceIfSuspicion) == false)
		//		return false;
		//}
		return true;
	}
	else
	{
		return false;
	}
}
//bool LineOrArc::IsInside(const std::vector<LineOrArc>* bigLoop, const std::vector<LineOrArc>* smallLoop, vreal tol, bool fullCheck)
//{
//	//VPoint minBigP;
//	//VPoint maxBigP;
//	VPoint minSmallP, minBigLp;
//	VPoint maxSmallP, maxBigLp;
//
//	//LineOrArc::GetMinMax(bigLoop,minBigP, maxBigP);
//	LineOrArc::GetMinMax(*smallLoop, minSmallP, maxSmallP);
//	LineOrArc::GetMinMax(*bigLoop, minBigLp, maxBigLp);
//
//	bool ck1 = minBigLp.ReplaceMin(minSmallP);
//	bool ck2 = maxBigLp.ReplaceMax(maxSmallP);
//
//	//bool ck1 = LineOrArc::IsPointInLoop(*bigLoop, minSmallP);
//	//bool ck2 = LineOrArc::IsPointInLoop(*bigLoop, maxSmallP);
//
//	if (ck1 == false && ck2 == false)
//	{
//		// check all points of small
//		if (fullCheck == true)
//		{
//			SequentialLineSet tempsl(40);
//			LineOrArc::Digitise(*smallLoop, tol, tempsl);
//			SequentialLineSet tempsl2(tempsl.GetPointSize());
//			tempsl.DigitiseByLengthStep(tempsl2, tol * 10.0, 0.0, 1.0);
//			int psize = tempsl2.GetPointSize();
//			for (int pi = 0; pi < psize; ++pi)
//			{
//				const VPoint* curp = tempsl2.GetPoint(pi);
//				bool ck3 = LineOrArc::IsPointInLoop(*bigLoop, *curp);
//				if (ck3 == false)
//					return false;
//			}
//		}
//		return true;
//	}
//	else
//	{
//		return false;
//	}
//}
//bool LineOrArc::IsInside(const std::vector<LineOrArc> *bigLoop, const std::vector<LineOrArc> *smallLoop , vreal tol) //, bool fullCheck)
//{
//	int loopSize = (int)smallLoop->size();
//	int insideCount = 0;
//	int outsideCount = 0;
//	for (int i = 0; i < loopSize; ++i)
//	{
//		const LineOrArc& curGeo = smallLoop->at(i);
//		VPoint p0 = curGeo.GetP(0.25);
//		VPoint p1 = curGeo.GetP(0.5);
//		VPoint p2 = curGeo.GetP(0.75);
//		if (IsPointInLoop(*bigLoop, p0))
//			++insideCount;
//		else
//			++outsideCount;
//		if (IsPointInLoop(*bigLoop, p1))
//			++insideCount;
//		else
//			++outsideCount;
//		if (IsPointInLoop(*bigLoop, p2))
//			++insideCount;
//		else
//			++outsideCount;
//	}
//	return (insideCount >= outsideCount);
//}

bool LineOrArc::IsIntersecting(const std::vector<LineOrArc> *bigLoop, const std::vector<LineOrArc> *smallLoop, vreal tol)
{
	//VPoint minBigP;
	//VPoint maxBigP;
	VPoint minSmallP, minBigLp;
	VPoint maxSmallP, maxBigLp;

	//LineOrArc::GetMinMax(bigLoop,minBigP, maxBigP);
	LineOrArc::GetMinMax(*smallLoop, minSmallP, maxSmallP);
	LineOrArc::GetMinMax(*bigLoop, minBigLp, maxBigLp);

	bool ck1 = minBigLp.ReplaceMin(maxSmallP);
	bool ck2 = maxBigLp.ReplaceMax(minSmallP);

	//bool ck1 = LineOrArc::IsPointInLoop(*bigLoop, minSmallP);
	//bool ck2 = LineOrArc::IsPointInLoop(*bigLoop, maxSmallP);

	if (ck1 == false && ck2 == false)
	{
		// check all points of small
		//if (fullCheck == true)
		//{
			SequentialLineSet tempsl(40);
			LineOrArc::Digitise(*smallLoop, tol, tempsl);
			SequentialLineSet tempsl2(tempsl.GetPointSize());
			//tempsl.DigitiseByLengthStep(tempsl2, tol*10.0, 0.0, 1.0);
			tempsl.DigitiseByLengthStepNew(tempsl2, tol * 10.0);
			int psize = tempsl2.GetPointSize();
			int insideCounter = 0;
			int outsideCounter = 0;
			bool isPointOnLoop;
			bool choiceOnSuspicion(true);
			for (int pi = 0; pi < psize; ++pi)
			{
				const VPoint *curp = tempsl2.GetPoint(pi);
				bool ck3 = LineOrArc::IsPointInLoop(*bigLoop, *curp,tol,isPointOnLoop,choiceOnSuspicion);
				if (ck3 == true)
					++insideCounter;//return true;
				else
					++outsideCounter;
				if (insideCounter > 0 && outsideCounter > 0)
					return true;
			}
		//}
		return false;
	}
	else
	{
		return false;
	}
}


void LineOrArc::GroupForSolid(std::multimap<vreal, std::pair<std::vector<LineOrArc> *,bool>> &allLoops, vreal tol
	, std::list<std::pair<std::vector<LineOrArc>*,std::vector<std::vector<LineOrArc>*>>> &allGroups)
{
	typedef std::multimap<vreal, std::pair<std::vector<LineOrArc> *,bool>>::reverse_iterator  mitr;
	mitr i = allLoops.rbegin();
	mitr e = allLoops.rend();
	bool fullCheck = true;
	for (i; i != e; ++i)
	{
		if (i->second.second == true)
			continue;
		std::pair<std::vector<LineOrArc>*,std::vector<std::vector<LineOrArc>*>> newGroup;
		newGroup.first = i->second.first;
		allGroups.push_back(newGroup);
		i->second.second = true;
		mitr ii = i;
		++ii;
		for (ii; ii != e; ++ii)
		{
			bool ck = LineOrArc::IsInside(i->second.first, ii->second.first, tol);// , fullCheck);
			if (ck == true)
			{
				allGroups.back().second.push_back(ii->second.first);
				ii->second.second = true;
			}
		}
	}
}

void LineOrArc::ExplodeArcsToLines(std::vector<LineOrArc> &loop, vreal tol)
{
	std::vector<LineOrArc> temp;
	int lsize = (int)loop.size();
	for (int li = 0; li < lsize; ++li)
	{
		LineOrArc &geo = loop.at(li);
		if (geo.GetType() == LineOrArc::line)
		{
			temp.push_back(geo);
		}
		else
		{
			SequentialLineSet tempsl = geo.GetSl(tol);
			int lsize2 = tempsl.Size();
			for (int lii = 0; lii < lsize2; ++lii)
			{
				VLine curl = tempsl.operator[](lii);
				temp.push_back(curl);
			}
		}
	}
	loop = temp;
}

int LineOrArc::GetParIf(const VPoint &p
	, const std::vector<LineOrArc> &contour
	, const std::vector<vreal> &lengthVec
	, vreal contourLen
	, vreal &t
	, vreal &parongeo
	, vreal tol)
{
	if (contour.size() == 0)
		throw VException("LineOrArc::GetParIf exception , contour is empty", 1);

	const std::vector<LineOrArc> *geoVec = &contour;
	int mysize = (int)geoVec->size();
	vreal totalLen = 0.;
	for (int i = 0; i<mysize; ++i)
	{
		const LineOrArc &geo = geoVec->at(i);
		vreal len = lengthVec.at(i);
		vreal curt;
		bool ck = geo.GetParIf(p, curt, tol);
		if (ck)
		{
			if (curt >= 0.0 && curt <= 1.0)
			{
				vreal localLen = len * curt;
				t = (localLen + totalLen) / contourLen;

				parongeo = curt;//localLen;

				return i;
			}
		}
		totalLen += len;
	}
	return -1;
}
void LineOrArc::FillLengthVec(const std::vector<LineOrArc> &contour, std::vector<vreal> &lengthVec,vreal &contourLen)
{
	lengthVec.clear();
	const std::vector<LineOrArc> *geoVec = &contour;
	size_t mysize = geoVec->size();
	contourLen = 0;
	for (size_t i = 0; i<mysize; ++i)
	{
		const LineOrArc &geo = geoVec->at(i);
		lengthVec.push_back(geo.GetLength());
		contourLen += lengthVec.back();
	}
}

void LineOrArc::GetSubsetGeoVec(const std::vector<LineOrArc> &first
	, const std::vector<vreal> &firstLenVec
	, vreal firstContourLen
	, const std::vector<LineOrArc> &second
	, const std::vector<vreal> &secondLenVec
	, vreal secondContourLen
	, const std::vector<VPoint> & vPvec
	, std::vector<LineOrArc> &geoVec
	, bool trueForInsideFalseForOutside
	, vreal tol
	, std::vector<LineOrArc> &overlapsVec)
{
	std::map<vreal, VPoint> tmpMap;

	int size = (int)vPvec.size();
	vreal tol2 = tol*tol;
	for (int i = 0; i<size; i++)
	{
		vreal tmpT = 0.0;
		vreal parongeo;
		if (GetParIf(vPvec.at(i) , first , firstLenVec, firstContourLen, tmpT, parongeo, tol) != -1)
		{
			tmpMap[tmpT] = vPvec.at(i);
			if (fabs(fabs(tmpT) - 1.0) < tol2)
			{
				tmpMap[0.0] = vPvec.at(i);
			}
			else if (fabs(tmpT) < tol2)
			{
				tmpMap[1.0] = vPvec.at(i);
			}
		}
	}

	std::vector<VInterval> ivec;
	std::vector<VInterval> overlapsInterval1,overlapsInterval2;
	bool isThereOverlap = GetIntervalsInOtherContour(first, firstLenVec, firstContourLen
		, tmpMap, ivec
		, second, secondLenVec, secondContourLen, overlapsInterval1,overlapsInterval2, tol);

	if (trueForInsideFalseForOutside == false)
	{
		// negade ivec
		std::vector<VInterval> tempI;
		tempI.push_back(VInterval(0.0, 1.0));
		if (isThereOverlap)
			ivec.insert(ivec.end(), overlapsInterval1.begin(), overlapsInterval1.end());
		std::vector<VInterval> invIVec = VInterval::Subtract(tempI, ivec);
		ivec = invIVec;
	}
	else
	{

	}

	/*int ovSize = (int)overlapsVec.size();
	std::vector<VInterval> overlapIntVec;
	for (int o = 0; o < ovSize; ++o)
	{ 
		GetOverlapIntervalOnContour(first, firstLenVec, firstContourLen, overlapsVec.at(o), overlapIntVec, tol, trueForInsideFalseForOutside);
	}
	*/

	//std::vector<VInterval> resVi = VInterval::Subtract(ivec, overlapsInt);

	//VPoint pp;
	//vreal l;
	//GetP(first , firstLenVec , firstContourLen , ivec.at(0).GetT0(), pp, l);


	int isize = (int)ivec.size();

	for (int i = 0; i<isize; i++)
	{

		AppendSubset(first, firstLenVec, firstContourLen,
			ivec.at(i).GetT0(), ivec.at(i).GetT1(), geoVec);
	}  
	if (trueForInsideFalseForOutside == true)
	{
		int oSize = (int)overlapsInterval1.size();
		for (int o = 0; o < oSize; ++o)
		{
			AppendSubset(first, firstLenVec, firstContourLen,
				overlapsInterval1.at(o).GetT0(), overlapsInterval1.at(o).GetT1(), overlapsVec);
		}
	}
}

int LineOrArc::GetP(const std::vector<LineOrArc> &contour
	, const std::vector<vreal> &lenVec
	, vreal contourLen
	, vreal t
	, VPoint &p
	, vreal & parongeo)
{
	if (lenVec.size() != contour.size())
	{
		throw VException("LineOrArc::GetP exception , lenVec size not eq contour element size",1);
	}
	const std::vector<LineOrArc> *geoVec = &contour;
	int mysize = (int)geoVec->size();
	vreal totalLen = 0.;
	for (int i = 0; i<mysize; ++i)
	{
		const LineOrArc &geo = geoVec->at(i);
		vreal len = lenVec.at(i);
		vreal curt0 = totalLen / contourLen;
		vreal curt1 = (totalLen + len) / contourLen;

		if (t >= curt0 && t <= curt1)
		{
			vreal lenongeo = t*contourLen - totalLen;
			parongeo = lenongeo / len;
			p = geo.GetP(parongeo);
			return i;
		}

		totalLen += len;
	}
	return -1;
}

bool LineOrArc::GetIntervalsInOtherContour (const std::vector<LineOrArc> &contour
	, const std::vector<vreal> &contourLenVec
	, vreal contourLen
	, const std::map<vreal, VPoint> &pointVec
	, std::vector<VInterval> &intervals
	, const std::vector<LineOrArc> & other
	, const std::vector<vreal> &otherLenVec
	, vreal otherContourLen
	, std::vector<VInterval> &overlapsFirst
	, std::vector<VInterval> &overlapsSecond
	, vreal tol)
{
	// Check overlaps initially 
	int mysize11 = (int)contour.size();
	int mysize22 = (int)other.size();
	bool isthereoverlap = false;
	for (int iii = 0; iii < mysize11; ++iii)
	{
		const LineOrArc &firstla = contour.at(iii);
		for (size_t jjj = 0; jjj < mysize22; ++jjj)
		{
			const LineOrArc &secondla = other.at(jjj);
			bool ckoverlap = GetOverlapIf(firstla, secondla, overlapsFirst, overlapsSecond, tol);
			if (ckoverlap)
				isthereoverlap = true;
		}
	}
	//if (isthereoverlap)
	//	return true;

	if (pointVec.size() <= 1)
		return isthereoverlap;

	std::map<vreal, VPoint> tmpPointVec = pointVec;

	VPoint p0 = contour.at(0).GetStartPoint();

	//vreal parongeo0;
	//if (GetP(contour , lenVec , contourLen , 0.0 , p0, parongeo0) == -1)
	//{
	//	throw VException("LineOrArc::GetIntervalsInOtherContour exception , GetP failed to find point",1);
	//}

	bool isPointOnLoop;
	bool choiceOnSuspicion = true;
	if (LineOrArc::IsPointInLoop(other , p0 , tol , isPointOnLoop , choiceOnSuspicion) == true)
	{
		tmpPointVec[0.0] = p0;

		//VPoint p1;
		//vreal parongeo1;

		//if (GetP(contour, lenVec , contourLen , 1.0, p1, parongeo1) == -1)
		//{
		//	throw VException("LineOrArc::GetIntervalsInOtherContour , unexpected GetP fails for end point",1);
		//}

		tmpPointVec[1.0] = p0;
	}

	for (std::map<vreal, VPoint>::const_iterator item = tmpPointVec.begin(); item != tmpPointVec.end(); ++item)
	{

		vreal t0 = item->first;
		vreal t1 = 1;

		++item;

		if (item != tmpPointVec.end())
		{
			//break; // end Point
			t1 = item->first;
			--item;

		}
		else
		{
			//t1 = tmpPointVec.begin()->first;
			--item;
		}
		

		VInterval v(t0, t1);


		vreal  tControl = (t0 + t1)*0.5;
		VPoint pControl;
		vreal  parongeo2;

		if (GetP(contour, contourLenVec, contourLen, tControl, pControl, parongeo2) == -1)
		{
			throw  VException("LineOrArc::GetIntervalsInOtherContour exception , GetP failed 3",1);
		}
		
		//if (LineOrArc::IsPointOnLoop(pControl, other, otherLenVec, otherContourLen, tol))
		//{
		//	overlaps.push_back(v);
		//}
		/*else*/if (LineOrArc::IsPointInLoop(other, pControl,tol,isPointOnLoop,choiceOnSuspicion) == true) //(other.IsPointInsideIfClosed(pControl))
		{

			intervals.push_back(v);
		}

	}
	return isthereoverlap;
}

void LineOrArc::AppendSubset(const std::vector<LineOrArc> &contour
	, const std::vector<vreal> &lenVec
	, vreal contourLen 
	, vreal t1, vreal t2
	, std::vector<LineOrArc> &subGeoVec)
{
	vreal localt1 = t1 <= t2 ? t1 : t2;
	vreal localt2 = t1 <= t2 ? t2 : t1;
	//VPoint localp1 = t1 <= t2 ? p1 : p2;
	//VPoint localp2 = t1 <= t2 ? p2 : p1;
	std::vector<LineOrArc> subSet;
	if (lenVec.size() != contour.size())
		throw VException("LineOrArc::AppendSubSet exception , len vec size noteq contour size",1);
	size_t mysize = contour.size();
	vreal lenSum = 0.;
	vreal invTotalLen = 1. / contourLen;
	vreal parSoFar = 0.;
	VInterval subSetInt(localt1, localt2);
	for (size_t i = 0; i<mysize; ++i)
	{
		const LineOrArc &curgeo = contour.at(i);
		vreal curElementLen = lenVec.at(i);
		vreal curElementEndPar = (lenSum + curElementLen) * invTotalLen;
		VInterval curElementInt(parSoFar, curElementEndPar);
		std::vector<VInterval> intInterval = subSetInt.Intersect(curElementInt);
		for (size_t ww = 0; ww<intInterval.size(); ++ww)
		{
			const VInterval &curinterval = intInterval.at(ww);
			vreal invdeltacur = 1. / (curElementEndPar - parSoFar);
			vreal trt0 = (curinterval.GetT0() - parSoFar) * invdeltacur;
			vreal trt1 = (curinterval.GetT1() - parSoFar) * invdeltacur;
			subSet.push_back(curgeo.GetSubSet(trt0, trt1));
		}
		lenSum += curElementLen;
		parSoFar = curElementEndPar;
		if (parSoFar > localt2)
			break;
	}
	if (t1 > t2)
		LineOrArc::Reverse(subSet);
	subGeoVec.insert(subGeoVec.end(), subSet.begin(), subSet.end());
}

void LineOrArc::Intersect(const std::vector<LineOrArc> &firstContour
	, const std::vector<vreal> &firstLenVec
	, vreal firstContourLen
	, const std::vector<LineOrArc> &secondContour
	, const std::vector<vreal> &secondLenVec
	, vreal secondContourLen
	, std::vector<std::vector<LineOrArc>> &intersection
	, std::vector<bool> &isClosedVec
	, vreal tol)
{
	std::vector<VPoint> intersectionPoints;
	std::vector<LineOrArc> firstIntElements, secondIntElements;
	//std::vector<VInterval> firstOverlapInt, secondOverlapInt;
	GetIntersectionPoints(firstContour, secondContour, intersectionPoints, firstIntElements, secondIntElements , tol);
	if (intersectionPoints.size() != 0)
	{
		std::vector<LineOrArc> igeo;
		GetSubsetGeoVec(firstContour, firstLenVec, firstContourLen
			, secondContour, secondLenVec, secondContourLen
			, intersectionPoints, igeo, true, tol, firstIntElements);
		GetSubsetGeoVec //(*this, pVec, lnOrArc2);
			(secondContour, secondLenVec, secondContourLen
			, firstContour, firstLenVec, firstContourLen
			, intersectionPoints, igeo, true, tol, secondIntElements);

		//if (igeo.size() != 0)
		//{
			VPoint sp(0.0);
			LineOrArc::LoopSort(igeo, sp, intersection, isClosedVec, tol*0.5, tol*0.5);
		//}
	}
	else
	{
		const std::vector<LineOrArc> * bigLoop;
		const std::vector<LineOrArc> * smallLoop;
		VPoint mn1, mx1, mn2, mx2;
		LineOrArc::GetMinMax(firstContour, mn1, mx1);
		LineOrArc::GetMinMax(secondContour, mn2, mx2);
		VPoint delta1 = mx1 - mn1;
		VPoint delta2 = mx2 - mn2;
		vreal a1 = delta1.x()*delta1.y();
		vreal a2 = delta2.x()*delta2.y();
		if (a1 > a2)
		{
			bigLoop = &firstContour;
			smallLoop = &secondContour;
		}
		else
		{
			bigLoop = &secondContour;
			smallLoop = &firstContour;
		}
		bool fullCheck = true;
		if (LineOrArc::IsInside(bigLoop,smallLoop,tol)/*, fullCheck)*/ == true)
		{
			intersection.push_back(*smallLoop);
			isClosedVec.push_back(true);
		}
	}
}

// returns 0 if no intersection , returns 1 if regular intersection , returns 2 if second is inside first 
// , returns 3 if first contour is inside second

int LineOrArc::Subtract(const std::vector<LineOrArc> &firstContour
	, const std::vector<vreal> &firstLenVec
	, vreal firstContourLen
	, const std::vector<LineOrArc> &secondContour
	, const std::vector<vreal> &secondLenVec
	, vreal secondContourLen
	, std::vector<std::vector<LineOrArc>> &subtraction
	, std::vector<bool> &isClosedVec
	, vreal tol)
{
	std::vector<VPoint> intersectionPoints;
	std::vector<LineOrArc> firstIntElements, secondIntElements;
	GetIntersectionPoints(firstContour, secondContour, intersectionPoints, firstIntElements, secondIntElements,tol);
	if (intersectionPoints.size() != 0)
	{
		std::vector<LineOrArc> igeo;
		GetSubsetGeoVec(firstContour, firstLenVec, firstContourLen
			, secondContour, secondLenVec, secondContourLen
			, intersectionPoints, igeo, false, tol, firstIntElements);
		GetSubsetGeoVec //(*this, pVec, lnOrArc2);
			(secondContour, secondLenVec, secondContourLen
			, firstContour, firstLenVec, firstContourLen
			, intersectionPoints, igeo, true, tol, secondIntElements);

		VPoint sp(0.0);
		LineOrArc::LoopSort(igeo, sp, subtraction, isClosedVec, tol*0.5, tol*0.5);
		return 1;
	}
	else
	{
		bool fullCheck = true;
		bool isInsideCk = LineOrArc::IsInside(&firstContour, &secondContour, tol);// , fullCheck);
		if (isInsideCk == true)
		{
			subtraction.push_back(firstContour);
			isClosedVec.push_back(true);
			//subtraction.push_back(secondContour);
			//isClosedVec.push_back(true);
			return 2;
		}
		else if (LineOrArc::IsInside(&secondContour, &firstContour, tol/*, fullCheck*/) == true)
		{
			return 3;
		}
		return 0; // no intersection between loops
	}
}

void LineOrArc::GetIntersectionPoints(const std::vector<LineOrArc> &firstContour
	, const std::vector<LineOrArc> &secondContour
	, std::vector<VPoint> &intersectionPoints
	, std::vector<LineOrArc> &firstIntElements
	, std::vector<LineOrArc> &secondIntElements
	, vreal tol)
{
	size_t c1size = (int)firstContour.size();
	size_t c2size = (int)secondContour.size();
	for (size_t i = 0; i<c1size; i++)
	{
		for (size_t j = 0; j<c2size; j++)
		{
			LAIntersectionResult ir = firstContour[i].GetIntersections(secondContour[j],tol);
			//intersectionPoints = ir.m_intp;
			//firstOverlapInt = ir.m_firstint;
			//secondOverlapInt = ir.m_secondint;
			size_t s = ir.m_intp.size();

			for (size_t k = 0; k<s; k++)
			{
				intersectionPoints.push_back(ir.m_intp.at(k));
			}
			if (ir.m_isMoreThanPoints == true)
			{
				int intSize = (int)ir.m_firstint.size();
				for (int r = 0; r < intSize; r++)
				{
					for (size_t jjj = 0;jjj< ir.m_firstint.size(); ++jjj)
					{
						const VInterval &cursubint = ir.m_firstint.at(jjj);
						LineOrArc subGeo = firstContour[i].GetSubSet(cursubint.GetT0(), cursubint.GetT1());
						firstIntElements.push_back(subGeo);//(ir.m_firstint.at(r));
					}
					for (size_t kkk = 0; kkk < ir.m_secondint.size(); ++kkk)
					{
						const VInterval &cursubint = ir.m_secondint.at(kkk);
						LineOrArc &subGeo = secondContour[j].GetSubSet(cursubint.GetT0(), cursubint.GetT1());
						secondIntElements.push_back(subGeo);//(ir.m_secondint.at(r));
					}
				}  
			}
		}
	}
}

void LineOrArc::MakeAllArcNormalsUZ(std::vector<LineOrArc> *loop)
{
	int lsize = (int)loop->size();
	for (int li = 0; li < lsize; ++li)
	{
		LineOrArc &geo = loop->at(li);
		if (geo.GetType() == LineOrArc::arc)
		{
			VArc a = *geo.GetArc();
			if (a.GetAx() % UZ < 0.0)
			{
				a.ReverseAxis();
				geo.SetArc(a);
			}
		}
	}
}

// returns 0 if no intersection , returns 1 if regular intersection , returns 2 if second is inside first 
// , returns 3 if first contour is inside second
int LineOrArc::Unite(const std::vector<LineOrArc> &firstContour
	, const std::vector<vreal> &firstLenVec
	, vreal firstContourLen
	, const std::vector<LineOrArc> &secondContour
	, const std::vector<vreal> &secondLenVec
	, vreal secondContourLen
	, std::vector<std::vector<LineOrArc>> &uniteRes
	, std::vector<bool> &isClosedVec
	, vreal tol)
{
	std::vector<VPoint> intersectionPoints;
	std::vector<LineOrArc> firstIntElements, secondIntElements;
	//std::vector<VInterval> firstOverlapInt, secondOverlapInt;
	GetIntersectionPoints(firstContour, secondContour, intersectionPoints, firstIntElements, secondIntElements ,tol);
	if (intersectionPoints.size() != 0)
	{
	    std::vector<LineOrArc> overlaps1, overlaps2;
		std::vector<LineOrArc> igeo;
		GetSubsetGeoVec(firstContour, firstLenVec, firstContourLen
			, secondContour, secondLenVec, secondContourLen
			, intersectionPoints, igeo, false, tol, overlaps1);
		GetSubsetGeoVec //(*this, pVec, lnOrArc2);
			(secondContour, secondLenVec, secondContourLen
			, firstContour, firstLenVec, firstContourLen
			, intersectionPoints, igeo, false, tol,overlaps2);

		//if (igeo.size() != 0)
		//{

		VPoint sp(0.0);
		
		//
		std::vector<LineOrArc> tmpGeo = igeo;
		LineOrArc::LoopSort(igeo, sp, uniteRes, isClosedVec, tol*0.5, tol*0.5);
		return 1;
		//if (isClosedVec.size() == 1 && isClosedVec.at(0) == true)
		//{
		//	return 1;
		//}
		//else
		//{
		//	/*std::vector<LineOrArc> ovlp = overlaps1;
		//	for (int i = 0; i < overlaps2.size();i++)
		//	{
		//		bool isExist = false;
		//		for (int j = 0; j < overlaps1.size(); j++)
		//		{
		//			if (overlaps2.at(i).GetStartPoint() == overlaps1.at(j).GetStartPoint() &&
		//				overlaps2.at(i).GetEndPoint() == overlaps1.at(j).GetEndPoint())
		//			{
		//				isExist = true;
		//				break;
		//			}
		//		}
		//		if (!isExist)
		//		{
		//			ovlp.push_back(overlaps2.at(i));
		//		}
		//	}*/
		//	std::vector<int> set;
		//	for (int s = 0; s < overlaps1.size(); ++s)
		//	{
		//		set.push_back(s);
		//	}
		//	std::list<std::vector<int>> comb;
		//	std::vector<int> t(set.size());
		//	VCombinations::CollectCombinations(set, comb, set.size(), t);

		//	int maxComb = 500000;
		//	int maxCounter = 0;
		//	std::list<std::vector<int>>::iterator i = comb.begin();
		//	std::list<std::vector<int>>::iterator e = comb.end();
		//	for (i; i != e; ++i)
		//	{
		//		std::vector<LineOrArc> tmp = tmpGeo;
		//		int iSize = (int)i->size();
		//		for (int c = 0; c < iSize; ++c)
		//		{
		//			tmp.push_back(overlaps1.at(i->at(c)));
		//		}

		//		std::vector<bool> closeVec;
		//		std::vector<std::vector<LineOrArc>> resVec;
		//		LoopSort(tmp, sp, resVec, closeVec, tol*0.5, tol*0.5);
		//		if (closeVec.size() == 1 && closeVec.at(0) == true)
		//		{
		//			uniteRes = resVec;
		//			isClosedVec = closeVec;
		//			return 1;
		//		}
		//		maxCounter++;
		//		if (maxCounter > maxComb)
		//		{
		//			return 0;
		//		}

		//	}
		//	 
		//	return 0;
		//}

		//}
		
		
	}
	else
	{
		const std::vector<LineOrArc> * bigLoop;
		const std::vector<LineOrArc> * smallLoop;
		VPoint mn1, mx1, mn2, mx2;
		LineOrArc::GetMinMax(firstContour, mn1, mx1);
		LineOrArc::GetMinMax(secondContour, mn2, mx2);
		VPoint delta1 = mx1 - mn1;
		VPoint delta2 = mx2 - mn2;
		vreal a1 = delta1.x()*delta1.y();
		vreal a2 = delta2.x()*delta2.y();
		int tempRetRes = 0;
		if (a1 > a2)
		{
			bigLoop = &firstContour;
			smallLoop = &secondContour;
			tempRetRes = 2;
		}
		else
		{
			bigLoop = &secondContour;
			smallLoop = &firstContour;
			tempRetRes = 3;
		}
		bool fullCheck = true;
		if (LineOrArc::IsInside(bigLoop, smallLoop, tol/*, fullCheck*/) == true)
		{
			uniteRes.push_back(*bigLoop);
			isClosedVec.push_back(true);
			return tempRetRes;
		}
		else
		{
			uniteRes.push_back(*smallLoop);
			uniteRes.push_back(*bigLoop);
			return 0; // no intersection 2 seperate loops
		}
	}
}

std::vector<std::vector<LineOrArc> *> LineOrArc::MakeStarForClosed(const std::vector<std::vector<LineOrArc>> &src
	, const std::vector<bool> &isClosed)
{
	std::vector<std::vector<LineOrArc>*> res;
	int ssz = (int)src.size();
	for (int si = 0; si < ssz; ++si)
	{
		if (isClosed.at(si) == false)
			continue;
		const std::vector<LineOrArc> &curloop = src.at(si);
		std::vector<LineOrArc> *curloopstar = new std::vector<LineOrArc>(curloop);
		res.push_back(curloopstar);
	}
	return res;
}

void LineOrArc::GiveElevationAtStart(vreal h , vreal tol , std::vector<LineOrArc> &rampVec , vreal bigAngle , vreal plungeH , vreal geoLenOn2d)const
{
	if (GetType() == line)
	{
		//VLine l = *GetLine();
		//VPoint p1 = l.GetP1();
		//p1.z(p1.z() + h);
		//l.SetP1(p1);
		//rampVec.push_back(l);

		vreal cumulH = h;
		vreal prevH = 0.0;
		VLine l = *GetLine();
		VLine ll(l.GetP1(), l.GetP1() + l.GetV().Unit() * geoLenOn2d);
		std::vector<LineOrArc> tmpVec;
		while (cumulH < plungeH)
		{
			VLine lll(ll);
			lll.GetP1().z(lll.GetP1().z() + prevH);
			lll.GetP2().z(lll.GetP2().z() + cumulH);
			prevH = cumulH;
			cumulH += h;
			tmpVec.push_back(lll);
			ll.Reverse();
		}
		std::vector<LineOrArc>::reverse_iterator ri = tmpVec.rbegin();
		std::vector<LineOrArc>::reverse_iterator re = tmpVec.rend();
		for (ri; ri != re; ++ri)
			rampVec.push_back((*ri).GetReversed());
		rampVec.push_back(*this);
	}
	else
	{
		//VArc *a = new VArc(*GetArc());
		//a->MakeFullCircle();
		////FastArcDigitiser ad;
		//int sno = FastArcDigitiser::GetDigitiseSegmentNo(*a, tol);
		//vreal sa = bigAngle;//a->GetSweepAngle();
		//sno *= (int)(sa/fabs(a->GetSweepAngle()));
		//vreal invsno = 1.0 / ((vreal)sno);
		//VPoint prevp;
		//std::vector<LineOrArc> tmpVec;
		//for (int i = 0; i <= sno; ++i)
		//{
		//	vreal t = ((vreal)i) * invsno;
		//	VPoint curp = a->GetP(sa*t);
		//	vreal curh = t*h;//(1.0-t) * h;
		//	curp.z(curp.z() + curh);
		//	if (i != 0)
		//	{
		//		VLine curl(prevp, curp);
		//		tmpVec.push_back(curl);
		//	}
		//	prevp = curp;
		//}
		VArc *a = new VArc (GetArc()->GetReversed());
		int sno = FastArcDigitiser::GetDigitiseSegmentNo(*a, tol);
		vreal tStep = 1.0 / sno;
		vreal angStep = a->GetSweepAngle() / sno;
		vreal invsno = 1.0 / ((vreal)sno);
		VPoint prevp(a->GetStartPoint());
		vreal t = tStep;
		vreal ang = angStep;
		vreal hStep = h / sno;
		vreal curh = hStep;
		std::vector<LineOrArc> tmpVec;
		while (curh < plungeH)
		{
			VPoint curp = a->GetP(ang);
			curp.z(curp.z() + curh);
			VLine curl(prevp, curp);
			tmpVec.push_back(curl);
			prevp = curp;
			t += tStep;
			ang += angStep;
			curh += hStep;
		}
		delete a;
		std::vector<LineOrArc>::reverse_iterator ri = tmpVec.rbegin();
		std::vector<LineOrArc>::reverse_iterator re = tmpVec.rend();
		for (ri; ri != re; ++ri)
		{
			rampVec.push_back(ri->GetReversed());
		}
	}
}

void LineOrArc::GiveElevationAtEnd(vreal h, vreal tol , std::vector<LineOrArc> &rampVec , vreal bigAngle , vreal plungeH , vreal geoLen)const
{
	if (GetType() == line)
	{
		//VLine l = *GetLine();
		//VPoint p2 = l.GetP2();
		//p2.z(p2.z() + h);
		//l.SetP2(p2);
		//rampVec.push_back(l);
		vreal cumulH = h;
		vreal prevH = 0.0;
		VLine l = *GetLine();
		VLine ll(l.GetP1(), l.GetP1() + l.GetV().Unit() * geoLen);
		while (cumulH < plungeH)
		{
			VLine lll(ll);
			lll.GetP1().z(lll.GetP1().z() + prevH);
			lll.GetP2().z(lll.GetP2().z() + cumulH);
			prevH = cumulH;
			cumulH += h;
			rampVec.push_back(lll);
			ll.Reverse();
		}
	}
	else
	{
		//VArc *a = new VArc(*GetArc());
		//a->MakeFullCircle();
		////FastArcDigitiser ad;		
		//int sno = FastArcDigitiser::GetDigitiseSegmentNo(*a, tol);
		////real sa = bigAngle;//a->GetSweepAngle();
		//sno *= (int)(bigAngle / fabs(a->GetSweepAngle()));
		//vreal invsno = 1.0 / ((vreal)sno);
		//VPoint prevp;
		//for (int i = 0; i <= sno; ++i)
		//{
		//	vreal t = ((vreal)i) * invsno;

		//	VPoint curp = a->GetP((bigAngle*t));
		//	vreal curh = t * h;
		//	curp.z(curp.z() + curh);
		//	if (i != 0)
		//	{
		//		VLine curl(prevp, curp);
		//		rampVec.push_back(curl);
		//	}
		//	prevp = curp;
		//}
		//delete a;

		VArc *a = new VArc(*GetArc());
		int sno = FastArcDigitiser::GetDigitiseSegmentNo(*a, tol);
		vreal tStep = 1.0 / sno;
		vreal angStep = a->GetSweepAngle() / sno;
		vreal invsno = 1.0 / ((vreal)sno);
		VPoint prevp(a->GetStartPoint());
		vreal t = tStep;
		vreal ang = angStep;
		vreal hStep = h / sno;
		vreal curh = hStep;
		while (curh < plungeH)
		{
			VPoint curp = a->GetP(ang);
			curp.z(curp.z() + curh);
			VLine curl(prevp, curp);
			rampVec.push_back(curl);
			prevp = curp;
			t += tStep;
			ang += angStep;
			curh += hStep;
		}
		delete a;
	}
}

//vreal LineOrArc::GetMinDistOnXY(LineOrArc &other)const
//{
//	LineOrArc::type mytype = GetType();
//	LineOrArc::type othertype = other.GetType();
//	if (mytype == LineOrArc::line && othertype == LineOrArc::line)
//	{
//		VLine myline = GetLine()->GetLineOnXY();
//		VLine otherline = other.GetLine()->GetLineOnXY();
//		LineLineMinDist calc;
//		vreal mindist;
//		VLine minline;
//		calc.Calc(myline, otherline, mindist, minline);
//		return mindist;
//	}
//	else if (mytype == LineOrArc::line && othertype == LineOrArc::arc)
//	{
//		VLine myline = GetLine()->GetLineOnXY();
//		VArc otherarc = other.GetArc()->GetArcOnXY();
//
//	}
//}

bool LineOrArc::Compare(const std::vector<LineOrArc> &vec1, const std::vector<LineOrArc> &vec2, vreal tol)
{
	int size1 = (int)vec1.size();
	int size2 = (int)vec2.size();
	if (size1 != size2)
		return false;
	for (int i = 0; i < size1; ++i)
	{
		const LineOrArc &g1 = vec1.at(i);
		const LineOrArc &g2 = vec2.at(i);
		if (g1.Compare(g2, tol) == false)
		{
			return false;
		}
	}
	return true;
}

int LineOrArc::MergeLineOrArcsIf(std::vector<LineOrArc> &connectedLoop,vreal tol)// returns the number of merged loops 
{
	bool isThereAnyMerge = false;
	int mergeCount = 0;
	while (true)
	{
		int mysize = (int)connectedLoop.size();
		for (int li = 0; li < mysize; ++li)
		{
			bool ckmerge = CanIMerge(connectedLoop, li, (li + 1)%mysize,tol);
			if (ckmerge)
			{
				isThereAnyMerge = true;
				++mergeCount;
				LineOrArc mergedgeo = MergeSingle(connectedLoop, li, (li + 1)%mysize,tol);
				MergeUpdate(connectedLoop, li, (li + 1)%mysize, mergedgeo);
				if (mergedgeo.IsCircle())
					isThereAnyMerge = false;
				break;
			}
		}
		if (isThereAnyMerge == false)
			break;
		isThereAnyMerge = false;
	}
	return mergeCount;
}
bool LineOrArc::CanIMerge(std::vector<LineOrArc> &connectedLoop, int gi, int ginext,vreal tol)
{
	LineOrArc &g1 = connectedLoop.at(gi);
	LineOrArc &g2 = connectedLoop.at(ginext);
	
	if (g1.GetEndPoint().Compare(g2.GetStartPoint(), tol)) // two geoemtry connected , lin starts where li ends
	{
		if (g1.GetType() == g2.GetType()) // same type
		{
			if (g1.GetType() == LineOrArc::arc)
			{
				const VArc *a1 = g1.GetArc();
				const VArc *a2 = g2.GetArc();
				if (fabs(a1->GetRadius() - a2->GetRadius()) < tol 
					&& a1->GetAx().Compare(a2->GetAx(),tol) == true 
					&& a1->GetSweepAngle() * a2->GetSweepAngle() > 0.0 // means not overlapping arcs both ccw or both cw
					&& a1->GetCenter().Compare(a2->GetCenter(), tol) == true)
				{
					// this is same circle continueing , merge 2 arcs
					return true;
				}
			}
			else
			{
				// line 
				VPoint t1 = g1.GetStartTangent();
				VPoint t2 = g2.GetStartTangent();
				if (t1.Compare(t2, tol))
				{
					return true;
				}
			}
		}
	}
	return false;
}
//LineOrArc LineOrArc::MergeSingle(std::vector<LineOrArc> &connectedLoop, int gi, int ginext,vreal tol)
//{
//	LineOrArc &g1 = connectedLoop.at(gi);
//	LineOrArc &g2 = connectedLoop.at(ginext);
//	LineOrArc mergeGeo(g1);
//	if (g1.GetType() == arc)
//	{
//		const VArc *a1 = g1.GetArc();
//		const VArc *a2 = g2.GetArc();
//		bool ck11 = a1->GetStartPoint().Compare(a2->GetEndPoint(), tol);
//		VArc a3;
//		if (ck11 == false)
//		{
//			a3 = VArc(a1->GetStartPoint(), a1->GetEndPoint(), a2->GetEndPoint());
//		}
//		else
//		{
//			a3 = *a1;
//			a3.MakeFullCircle();
//		}
//		if (a3.GetStartPoint().Compare(a1->GetStartPoint(), tol) == false)
//		{
//			a3 = a3.GetReversed();
//			if (a3.GetStartPoint().Compare(a1->GetStartPoint(), tol) == false)
//				throw VException("LineOrArc::MergeSingle exception , a3 is not starting as a1", 1);
//		}
//		if (a3.GetAx() % a1->GetAx() < 0.0)
//		{
//			a3.ReverseAxis();
//		}
//		mergeGeo = LineOrArc(a3);
//	}
//	else
//	{
//		VLine l3(g1.GetStartPoint(), g2.GetEndPoint());
//		mergeGeo = LineOrArc (l3);
//	}
//	return mergeGeo;
//}
LineOrArc LineOrArc::MergeSingle(std::vector<LineOrArc> &connectedLoop, int gi, int ginext, vreal tol)
{
	LineOrArc &g1 = connectedLoop.at(gi);
	LineOrArc &g2 = connectedLoop.at(ginext);
	LineOrArc mergeGeo(g1);
	if (g1.GetType() == arc)
	{
		const VArc *a1 = g1.GetArc();
		const VArc *a2 = g2.GetArc();
		vreal sweepangle = fabs(a1->GetSweepAngle() + a2->GetSweepAngle());
		if (a1->GetSweepAngle() < 0.0)
			sweepangle *= -1.0;
		VArc a3(a1->GetCenter(), a1->GetRadius(), a1->GetAx(), a1->GetStartPoint(), sweepangle);
		mergeGeo = LineOrArc(a3);
	}
	else
	{
		VLine l3(g1.GetStartPoint(), g2.GetEndPoint());
		mergeGeo = LineOrArc(l3);
	}
	return mergeGeo;
}
void LineOrArc::MergeUpdate(std::vector<LineOrArc> &connectedLoop, int gi, int ginext, LineOrArc &mergeGeo)
{
	int loopsize = (int)connectedLoop.size();
	std::vector<LineOrArc> newVec;
	newVec.reserve(loopsize - 1);
	for (int li = 0; li < loopsize; ++li)
	{
		if (li == gi)
		{
			newVec.push_back(mergeGeo);
		}
		else if (li != ginext)
		{
			newVec.push_back(connectedLoop.at(li));
		}
	}
	connectedLoop = newVec;
}
void LineOrArc::PhiXY(const std::vector<LineOrArc> & l1, const std::vector<LineOrArc> &l2,
	std::vector<std::pair<VPoint, VPoint>> & toolPartMinLeft,
	std::vector<std::pair<VPoint, VPoint>> & toolPartMinRigth,
	std::vector<std::pair<VPoint, VPoint>> & toolPartMaxLeft,
	std::vector<std::pair<VPoint, VPoint>> & toolPartMaxRigth)
{
	
	int l1Size = (int)l1.size();
	int l2Size = (int)l2.size();
	for (int i = 0; i < l1Size; i++)
	{
		for (int j = 0; j < l2Size; j++)
		{
			//
		}
	}
}
void LineOrArc::PhiXY(const LineOrArc & l1, const LineOrArc & l2, 
	std::pair<VPoint, VPoint> & toolPartMinLeft,
	std::pair<VPoint, VPoint> & toolPartMinRigth,
	std::pair<VPoint, VPoint> & toolPartMaxLeft,
	std::pair<VPoint, VPoint> & toolPartMaxRigth)
{ 
	if (l1.m_type == line && l2.m_type == line)
	{
		PhiXYLineLine(l1, l2, toolPartMinLeft, toolPartMinRigth, toolPartMaxLeft, toolPartMaxRigth);
	}
	else if (l1.m_type == line && l2.m_type == arc)
	{
		PhiXYLineArc(l1, l2, toolPartMinLeft, toolPartMinRigth, toolPartMaxLeft, toolPartMaxRigth);
	}
	else if (l1.m_type == arc && l2.m_type == line)
	{
		PhiXYArcLine(l1, l2, toolPartMinLeft, toolPartMinRigth, toolPartMaxLeft, toolPartMaxRigth);
	}
	else if (l1.m_type == arc && l2.m_type == arc)
	{
		PhiXYArcArc(l1, l2, toolPartMinLeft, toolPartMinRigth, toolPartMaxLeft, toolPartMaxRigth);
    }
}
void LineOrArc::PhiXYLineLine(const LineOrArc & l1, const LineOrArc &l2,
	std::pair<VPoint, VPoint> & toolPartMinLeft,
	std::pair<VPoint, VPoint> & toolPartMinRigth,
	std::pair<VPoint, VPoint> & toolPartMaxLeft,
	std::pair<VPoint, VPoint> & toolPartMaxRigth)
{
	VPoint tn = l1.GetTan(0.0);
	LineOrArc tmpL = l2;
	//vreal dist = l1.GetMinDistOnXY(tmpL);


}
void LineOrArc::PhiXYLineArc(const LineOrArc & l1, const LineOrArc &l2,
	std::pair<VPoint, VPoint> & toolPartMinLeft,
	std::pair<VPoint, VPoint> & toolPartMinRigth,
	std::pair<VPoint, VPoint> & toolPartMaxLeft,
	std::pair<VPoint, VPoint> & toolPartMaxRigth)
{

}
void LineOrArc::PhiXYArcLine(const LineOrArc & l1, const LineOrArc &l2,
	std::pair<VPoint, VPoint> & toolPartMinLeft,
	std::pair<VPoint, VPoint> & toolPartMinRigth,
	std::pair<VPoint, VPoint> & toolPartMaxLeft,
	std::pair<VPoint, VPoint> & toolPartMaxRigth)
{

}
void LineOrArc::PhiXYArcArc(const LineOrArc & l1, const LineOrArc &l2, 
	std::pair<VPoint, VPoint> & toolPartMinLeft,
	std::pair<VPoint, VPoint> & toolPartMinRigth,
	std::pair<VPoint, VPoint> & toolPartMaxLeft,
	std::pair<VPoint, VPoint> & toolPartMaxRigth)
{

}
bool LineOrArc::GetOverlapIf(const LineOrArc &first, const LineOrArc &second, std::vector<VInterval> &firstoverlap, std::vector<VInterval> &secondoverlap, vreal tol)
{
	if (first.m_type == LineOrArc::line && second.m_type == LineOrArc::line)
	{
		std::vector<LineOrArc> la;
		la.push_back(first);
		la.push_back(second);
		VPoint minP, maxP;
		LineOrArc::GetMinMax(la, minP, maxP);
		VLine l(minP, maxP);
		LineOrArc bigLine(l);
		
		vreal t0,t1,t2,t3;
		bool ck0 = bigLine.GetParIf(first.GetStartPoint(), t0, tol);
		bool ck1 = bigLine.GetParIf(first.GetEndPoint(), t1, tol);
		bool ck2 = bigLine.GetParIf(second.GetStartPoint(), t2, tol);
		bool ck3 = bigLine.GetParIf(second.GetEndPoint(), t3, tol);
		if (!ck0 || !ck1  || !ck2 || !ck3)
		{
			return false; // not intersecting 
		}
	    
		VInterval vi0(t0, t1);
		VInterval vi1(t2, t3);
		std::vector<VInterval> intVec = vi0.Intersect(vi1);
		int iSize = (int)intVec.size();
		if (iSize < 1)
		{
			//throw new VException("Line overlap size != 1 ,LineOrArc::GetOverlapIf", 1);
			return false; // not intersecting
		}
		 
		//VPoint p0 = bigLine.GetP(intVec.at(0).GetT0());
		//VPoint p1 = bigLine.GetP(intVec.at(0).GetT1());

		//VLine olp(p0, p1);
		//VLine l0 = *first.GetLine();
		//VLine l1 = *second.GetLine();

		//vreal dp0 = olp.GetV() % l0.GetV();
		//if (dp0 < 0)
		//{
		//	VLine tmpL0(olp);
		//	tmpL0.Reverse();
		//	LineOrArc la0(tmpL0);
		//    secondoverlap.push_back(la0);

		//}
		//else
		//{
		//	VLine tmpL0(olp); 
		//	LineOrArc la0(tmpL0);
		//	secondoverlap.push_back(la0);
		//	

		//}

		//vreal dp1 = olp.GetV() % l1.GetV();
		//if (dp1 < 0)
		//{
		//	VLine tmpL1(olp);
		//	tmpL1.Reverse();
		//	LineOrArc la1(tmpL1);
		//	firstoverlap.push_back(la1);
		//	

		//}
		//else
		//{
		//	VLine tmpL1(olp);
		//	LineOrArc la1(tmpL1);
		//	firstoverlap.push_back(la1);
		//	

		//}

		int myivecsize = (int)intVec.size();
		for (int iii = 0; iii < myivecsize; ++iii)
		{
			VPoint p0 = bigLine.GetP(intVec.at(iii).GetT0());
			VPoint p1 = bigLine.GetP(intVec.at(iii).GetT1());
			vreal tfirst0, tfirst1, tsecond0, tsecond1;
			bool ckfirst0 = first.GetParIf(p0, tfirst0, tol);
			bool ckfirst1 = first.GetParIf(p1, tfirst1, tol);
			bool cksecond0 = second.GetParIf(p0, tsecond0, tol);
			bool cksecond1 = second.GetParIf(p1, tsecond1, tol);
			if (ckfirst0 == false || ckfirst1 == false || cksecond0 == false || cksecond1 == false)
				throw VException("LineOrArc::GetOverlapIf overlap search unexpected par back 1",1);
			VInterval vfirst(tfirst0, tfirst1);
			VInterval vsecond(tsecond0, tsecond1);
			firstoverlap.push_back(vfirst);
			secondoverlap.push_back(vsecond);
		}
		return true;
	}
	else if (first.m_type == LineOrArc::arc && second.m_type == LineOrArc::arc)
	{
		const VArc *A1 = first.GetArc();
		const VArc *A2 = second.GetArc();

		//VArc bigArc(A1->GetCenter(), A1->GetRadius(), A1->GetAx(), A1->GetStartPoint(), 360.0);
		//LineOrArc bigLA(bigArc);
		//VInterval iA1(0, (A1->GetSweepAngle() / 360.0));
		//VInterval iA2(0, (A2->GetSweepAngle() / 360.0));
		//vreal par1, par2;
		//bool ck1 = bigLA.GetParIf(A2->GetStartPoint(), par1, tol);
		//bool ck2 = bigLA.GetParIf(A2->GetEndPoint(), par2, tol);
		//if (!ck2 || !ck2)
		//{
		//	throw new VException("A2 parIif failed. LineOrArc::GetOverlapIf", 1);
		//}

		//VPoint t1 = bigLA.GetTan(0.0);
		//VPoint t2 = bigLA.GetTan(par1);
		//bool isOpposite = t1%t2 < 0.0 ? true : false;
		if (A1->GetCenter().Compare(A2->GetCenter(), tol) == false)
			return false;
		if (fabs(A1->GetRadius() - A2->GetRadius()) > tol)
			return false;
		VLine Arc1Line(0.0, A1->GetSweepAngle());
		CoordinateSystem csArc1 = A1->GetCoordinateSystem();
		VPoint sp2 = csArc1.AbsoluteCoordToRelativeCoord(A2->GetStartPoint());
		VPoint ep2 = csArc1.AbsoluteCoordToRelativeCoord(A2->GetEndPoint());
		vreal xs = sp2.x();
		vreal ys = sp2.y();
		vreal xe = ep2.x();
		vreal ye = ep2.y();
		vreal ang0 = PolarCalc2d::GetPolarAngle(xs, ys);
		vreal ang1 = PolarCalc2d::GetPolarAngle(xe, ye);
		VLine Arc2Line(ang0, ang1);

		VLine lll(0.0, 360.0);
		std::vector<LineOrArc> la;
		LineOrArc firstLine(Arc1Line);
		LineOrArc secondLine(Arc2Line);
		la.push_back(firstLine);
		la.push_back(secondLine);
		LineOrArc bigLine(lll);

		vreal t0, t1, t2, t3;
		bool ck0 = bigLine.GetParIf(firstLine.GetStartPoint(), t0, tol);
		bool ck1 = bigLine.GetParIf(firstLine.GetEndPoint(), t1, tol);
		bool ck2 = bigLine.GetParIf(secondLine.GetStartPoint(), t2, tol);
		bool ck3 = bigLine.GetParIf(secondLine.GetEndPoint(), t3, tol);
		if (!ck0 || !ck1 || !ck2 || !ck3)
		{
			return false; // not intersecting 
		}

		VInterval vi0(t0, t1);
		VInterval vi1(t2, t3);
		std::vector<VInterval> intVec = vi0.Intersect(vi1);
		int iSize = (int)intVec.size();
		if (iSize < 1)
		{
			//throw new VException("Line overlap size != 1 ,LineOrArc::GetOverlapIf", 1);
			return false; // not intersecting
		}

		int myivecsize = (int)intVec.size();
		for (int iii = 0; iii < myivecsize; ++iii)
		{
			VPoint p0 = bigLine.GetP(intVec.at(iii).GetT0());
			VPoint p1 = bigLine.GetP(intVec.at(iii).GetT1());
			vreal tfirst0, tfirst1, tsecond0, tsecond1;
			bool ckfirst0 = firstLine.GetParIf(p0, tfirst0, tol);
			bool ckfirst1 = firstLine.GetParIf(p1, tfirst1, tol);
			bool cksecond0 = secondLine.GetParIf(p0, tsecond0, tol);
			bool cksecond1 = secondLine.GetParIf(p1, tsecond1, tol);
			if (ckfirst0 == false || ckfirst1 == false || cksecond0 == false || cksecond1 == false)
				throw VException("LineOrArc::GetOverlapIf overlap search unexpected par back 2", 1);
			VInterval vfirst(tfirst0, tfirst1);
			VInterval vsecond(tsecond0, tsecond1);
			firstoverlap.push_back(vfirst);
			secondoverlap.push_back(vsecond);
		}
		return true;

	}
	else
	{

	}

	return false;
}
bool LineOrArc::SubtractOverlaps(LineOrArc &geo, const std::vector<LineOrArc> &overlaps, std::vector<LineOrArc> &subractionResult)
{
	int size = (int)overlaps.size();
	for (int i = 0; i < size; i++)
	{
		if (geo.m_type == LineOrArc::line && overlaps.at(i).m_type == LineOrArc::line)
		{ 
			VLine lgeo = *geo.GetLine();
			VLine llp  = *overlaps.at(i).GetLine();
			vreal d    = lgeo.GetV() % llp.GetV();
			if (d < 0)
			{
				subractionResult.push_back(overlaps.at(i));
			}
		}
	}

	return true;
}
bool LineOrArc::SubtractOverlaps(std::vector<LineOrArc> &contour, const std::vector<LineOrArc> &overlaps)
{
	std::vector<LineOrArc> tmpResult;
	int size = (int)contour.size();
	for (int i = 0; i < size; i++)
	{
		bool ck = SubtractOverlaps(contour.at(i), overlaps, tmpResult);
		if (ck)
		{
			SubtractOverlaps(tmpResult, overlaps);
		}
	}

	contour = tmpResult;

	return true;
}
void LineOrArc::GetOverlapIntervalOnContour(const std::vector<LineOrArc> &contour, const std::vector<vreal> &contourLenVec, vreal contourLen, const LineOrArc &overlap, std::vector<VInterval> &viVec, vreal tol, bool trueForInsideFalseForOutside)
{ 
    
	VPoint p0 = overlap.GetStartPoint();
	VPoint tan0 = overlap.GetTan(0.0);
	VPoint p1 = overlap.GetEndPoint();
	vreal t0, t1;
	vreal parongeo0, parongeo1;
	int index0 = LineOrArc::GetParIf(p0, contour, contourLenVec, contourLen, t0, parongeo0, tol);
	int index1 = LineOrArc::GetParIf(p1, contour, contourLenVec, contourLen, t1, parongeo1, tol);
	if (index0 == -1 && index1 != -1)
	{
		throw new VException("Get interval on contour failed LineOrArc::GetOverlapIntervalOnContour",1);
	}

	VPoint cTan = contour.at(index0).GetTan(parongeo0);
	vreal d = cTan%tan0;
	bool isOpposite = d < 0 ? true : false;
	
	if (isOpposite == trueForInsideFalseForOutside)
	{
		VPoint pMid = overlap.GetP(0.5);
		vreal tmid, parongeoMid;
		int indexMid = LineOrArc::GetParIf(pMid, contour, contourLenVec, contourLen, tmid, parongeoMid, tol);
		if (indexMid == -1)
		{
			throw VException("Get interval on contour failed LineOrArc::GetOverlapIntervalOnContour", 1);
		}

		VInterval tmpVi(t0, t1);
		if (tmpVi.IsContaining(tmid))
		{
			viVec.push_back(tmpVi);
		}
		else
		{
			VInterval tmpVi1(0, t0);
			VInterval tmpVi2(t1, 1);
			viVec.push_back(tmpVi1);
			viVec.push_back(tmpVi2);
		} 
	}
}
bool LineOrArc::IsPointOnLoop(const VPoint& p, const std::vector<LineOrArc> &contour, std::vector<vreal> contourLenVec, vreal contourLen, vreal tol)
{
	vreal t, parongeo;
	int ck = LineOrArc::GetParIf(p, contour, contourLenVec, contourLen, t, parongeo, tol);
	
	if (ck == -1)
		return false;

	return true;
}
void LineOrArc::DrawInTryCatch(const std::vector<LineOrArc> &vec)
{
	Bofstream b(VGlobalFunctions::GetSolidCncExeFolder() + "\\MessageLog\\UniteGraphics.txt");
	SerializeVec(vec, b);
	throw VException("Draw LA", 2018);
}

bool LineOrArc::IsSingleFullArc(const std::vector<LineOrArc> &connectedLoop , vreal tol ,VArc &singleFullArc)
{
	//bool resck = false;
	//int mysize = (int)vec.size();
	//std::vector<VInterval> arcsum;
	//for (int i = 0; i < mysize; ++i)
	//{
	//	const LineOrArc &geo = vec.at(i);
	//	if (geo.GetType() == LineOrArc::line)
	//		return false;
	//	if (i == 0)
	//	{
	//		singleFullArc = *geo.GetArc();
	//		VInterval tempi(0.0, fabs(singleFullArc.GetSweepAngle()/360.0));
	//		arcsum.push_back(tempi);
	//		singleFullArc.MakeFullCircle();
	//		if (singleFullArc.IsCCW() != geo.GetArc()->IsCCW())
	//			singleFullArc = singleFullArc.GetReversed();
	//	}
	//	else
	//	{
	//		const VArc *a2 = geo.GetArc();
	//		if (fabs(singleFullArc.GetRadius() - a2->GetRadius()) > tol || singleFullArc.GetCenter().Compare(a2->GetCenter(), tol) == false
	//			|| singleFullArc.GetAx().Compare(a2->GetAx(), tol) == false)
	//			return false;
	//		VInterval tempi(0.0, fabs(a2->GetSweepAngle()/360.0));
	//		std::vector<VInterval> tempivec;
	//		tempivec.push_back(tempi);
	//		arcsum = VInterval::Merge(arcsum, tempivec);
	//	}
	//}
	//if (arcsum.size() != 1)
	//	return false;
	//VInterval &uniti = arcsum.at(0);
	//if (fabs(uniti.GetT0()) > 1.e-7 && fabs(1.0-uniti.GetT1()) > 1.e-7)
	//	return false;
	//return true;

	std::vector<LineOrArc> tempconnected = connectedLoop;
	MergeLineOrArcsIf(tempconnected, tol);
	if (tempconnected.size() == 1)
	{
		LineOrArc &geo = tempconnected.at(0);
		if (geo.GetType() == LineOrArc::arc)
		{
			if (geo.GetArc()->IsFull())
			{
				singleFullArc = *geo.GetArc();
				return true;
			}
		}
	}
	return false;
}
bool LineOrArc::IsSingleArc(const std::vector<LineOrArc>& connectedLoop, vreal tol, VArc& singleArc , bool& isThereOverlap)
{
	bool isArc = false;
	for (auto a = connectedLoop.begin(); a != connectedLoop.end(); ++a)
	{
		if (a->GetType() == LineOrArc::arc)
		{
			if (isArc == false)
			{
				isArc = true;
				singleArc = *a->GetArc();
				isThereOverlap = false;
			}
			else
			{
				if (fabs(singleArc.GetRadius()-a->GetArc()->GetRadius()) > tol || singleArc.GetCenter().Compare(a->GetArc()->GetCenter(),tol) == false || singleArc.GetEndPoint().Compare(a->GetArc()->GetStartPoint(),tol) == false)
				{
					isArc = false;
					break;
				}
				else
				{
					if (singleArc.GetSweepAngle() * a->GetArc()->GetSweepAngle() < 0.0)
						isThereOverlap = true;
					singleArc = VArc(singleArc.GetCenter(), singleArc.GetRadius(), singleArc.GetAx(), singleArc.GetStartPoint(), singleArc.GetSweepAngle() + a->GetArc()->GetSweepAngle());
				}
			}
		}
		else
		{
			isArc = false;
			break;
		}
	}
	return isArc;
}
bool LineOrArc::IsSingleArc(const std::vector<LineOrArc*>& connectedLoop, vreal tol, VArc& singleArc, bool& isThereOverlap)
{
	bool isArc = false;
	for (auto a = connectedLoop.begin(); a != connectedLoop.end(); ++a)
	{
		if ((*a)->GetType() == LineOrArc::arc)
		{
			if (isArc == false)
			{
				isArc = true;
				singleArc = *(*a)->GetArc();
				isThereOverlap = false;
			}
			else
			{
				if (fabs(singleArc.GetRadius() - (*a)->GetArc()->GetRadius()) > tol || singleArc.GetCenter().Compare((*a)->GetArc()->GetCenter(), tol) == false || singleArc.GetEndPoint().Compare((*a)->GetArc()->GetStartPoint(), tol) == false)
				{
					isArc = false;
					break;
				}
				else
				{
					if (singleArc.GetSweepAngle() * (*a)->GetArc()->GetSweepAngle() < 0.0)
						isThereOverlap = true;
					singleArc = VArc(singleArc.GetCenter(), singleArc.GetRadius(), singleArc.GetAx(), singleArc.GetStartPoint(), singleArc.GetSweepAngle() + (*a)->GetArc()->GetSweepAngle());
				}
			}
		}
		else
		{
			isArc = false;
			break;
		}
	}
	return isArc;
}
void LineOrArc::SplitBy(const std::vector<LineOrArc> &contour, const VPlane &pln, std::vector<LineOrArc> &backVec, std::vector<LineOrArc> &frontVec,std::vector<VPoint> &intPoints,vreal tol)
{
	int mysize = (int)contour.size();
	if (mysize == 0)
	{
		LogFileWriter::WriteUnknownExceptionLog("LineOrArc::SplitBy vec ","contour is empty");
		return;
	}
	for (int ci = 0; ci < mysize; ++ci)
	{
		const LineOrArc &geo = contour.at(ci);
		geo.SplitBy(pln, backVec, frontVec,intPoints,tol);
	}

	// loop sort half vectors
	//VPoint sp = contour.at(0).GetStartPoint();
	//VPoint temp;
	//vreal backDist(MAX_NUMBER_LIMIT);
	//if (backVec.size() > 0)
	//{
	//	backDist = backVec.at(0).GetMinDist(sp, temp);
	//	vreal backDist2 = backVec.at(backVec.size() - 1).GetMinDist(sp, temp);
	//	if (backDist2 < backDist)
	//		backDist = backDist2;
	//}
	//vreal frontDist(MAX_NUMBER_LIMIT);
	//if (frontVec.size() > 0)
	//{
	//	frontDist = frontVec.at(0).GetMinDist(sp, temp);
	//	vreal frontDist2 = frontVec.at(frontVec.size() - 1).GetMinDist(sp, temp);
	//	if (frontDist2 < frontDist)
	//		frontDist = frontDist2;
	//}
	//std::vector<LineOrArc> * vec1 = backDist < frontDist ? &backVec : &frontVec;
	//std::vector<LineOrArc> * vec2 = backDist < frontDist ? &frontVec : &backVec;

	//if (vec1->size() > 0)
	//{
	//	std::vector<std::vector<LineOrArc>>loops;
	//	std::vector<bool> isclosed;
	//	LineOrArc::LoopSort(*vec1, sp, loops, isclosed, tol, tol);
	//	if (loops.size() == 1)
	//	{
	//		*vec1 = loops.at(0);
	//		sp = vec1->at(vec1->size() - 1).GetEndPoint();
	//	}
	//	else
	//	{
	//		throw VException("LineOrArc::SplitBy exception , loop sort unexpected result",1);
	//	}
	//}
	//if (vec2->size() > 0)
	//{
	//	std::vector<std::vector<LineOrArc>>loops;
	//	std::vector<bool> isclosed;
	//	LineOrArc::LoopSort(*vec2, sp, loops, isclosed, tol, tol);
	//	if (loops.size() == 1)
	//	{
	//		*vec2 = loops.at(0);
	//	}
	//	else
	//	{
	//		throw VException("LineOrArc::SplitBy exception , loop sort unexpected result", 1);
	//	}
	//}
}

void LineOrArc::SplitBy(const VPlane &pln, std::vector<LineOrArc> &backVec, std::vector<LineOrArc> &frontVec,std::vector<VPoint> &intPoints,vreal tol)const
{
	if (m_type == line)
	{
		VLine l = *GetLine();
		vreal t;
		VPlane::IntType ck = pln.Int(l,t);
		if (ck == VPlane::REGULAR)
		{
			if (t < 0.0 || t > 1.0)
			{
				VPoint diff = l.GetP1() - pln.pointonplane;
				if (diff % pln.normal > 0.0)
					frontVec.push_back(*this);
				else
					backVec.push_back(*this);
			}
			else
			{
				VPoint pmid = l.GetP(t);
				intPoints.push_back(pmid);
				VLine l1(l.GetP1(),pmid);
				VLine l2(pmid, l.GetP2());
				VPoint diff = l1.GetP(0.5) - pln.pointonplane;
				if (diff % pln.normal > 0.0)
				{
					frontVec.push_back(LineOrArc(l1));
					backVec.push_back(LineOrArc(l2));
				}
				else
				{
					frontVec.push_back(LineOrArc(l2));
					backVec.push_back(LineOrArc(l1));
				}
			}
		}
		else
		{
			// not regular
			VPoint diff = l.GetP1() - pln.pointonplane;
			if (diff % pln.normal > 0.0)
				frontVec.push_back(*this);
			else
				backVec.push_back(*this);
		}
	}
	else
	{
		VArc a = *GetArc();
		// get intersection points
		std::vector<VPoint> intpoints, intpointsCircleOfArc;
		VPlane::IntType ck = pln.Int(a, intpoints, intpointsCircleOfArc);
		int psize = (int)intpoints.size();

		if (ck == VPlane::REGULAR)
		{
			std::map<vreal, vreal> intpar;
			if (psize != 0)
			{
				for (int pi = 0; pi < psize; ++pi)
				{
					const VPoint &curp = intpoints.at(pi);
					vreal t;
					bool ckpar = GetParIf(curp, t, tol);
					if (ckpar)
					{
						intpar[t] = t;
					}
					intPoints.push_back(curp);
				}
				intpar[0.0] = 0.0;
				intpar[1.0] = 1.0;
				std::map<vreal, vreal>::iterator ii = intpar.begin();
				std::map<vreal, vreal>::iterator ee = intpar.end();
				std::map<vreal, vreal>::iterator pi = ii;
				++ii;
				for (ii; ii != ee; ++ii)
				{
					LineOrArc temp = this->GetSubSet(pi->first, ii->first);
					VPoint midp = temp.GetP(0.5);
					VPoint diff = midp - pln.pointonplane;
					if (diff % pln.normal > 0.0)
						frontVec.push_back(temp);
					else
						backVec.push_back(temp);
					pi = ii;
				}
			}
			else //if (psize == 0)
			{
				VPoint midp = GetP(0.5);
				VPoint diff = midp - pln.pointonplane;
				if (diff % pln.normal > 0.0)
					frontVec.push_back(*this);
				else
					backVec.push_back(*this);
			}
		}
		else
		{
			// not regular
			VPoint midp = GetP(0.5);
			VPoint diff = midp - pln.pointonplane;
			if (diff % pln.normal > 0.0)
				frontVec.push_back(*this);
			else
				backVec.push_back(*this);

		}
	}
}

void LineOrArc::AddAnglesForConvexHullCalculation(const std::vector<LineOrArc> &vec, const CoordinateSystem &cs, std::map<vreal, vreal> &allAngles , vreal tol)
{
	int csize = (int)vec.size();
	for (int i = 0; i < csize; ++i)
	{
		const LineOrArc &geo = vec.at(i);
		geo.AddAnglesForConvexHullCalculation(cs, allAngles , tol);
	}
}

void LineOrArc::AddAnglesForConvexHullCalculation(const CoordinateSystem &cs, std::map<vreal, vreal> &allAngles,vreal tol)const
{
	if (m_type == line)
	{
		const VLine *l = GetLine();
		VPoint v = l->GetV();
		v = cs.AbsoluteVectorToRelativeVector(v);
		vreal ang = PolarCalc2d::GetPolarAngle(v.x(), v.y());
		if (ang > 180.0)
		{
			ang -= 180.0;
		}
		allAngles[ang] = ang;
		if (ang < 90)
			allAngles[ang + 90.0] = ang + 90.0;
		else
			allAngles[ang - 90.0] = ang - 90.0;
	}
	else
	{
		SequentialLineSet lines = GetSl(tol);
		int lsize = lines.Size();
		for (int li = 0; li < lsize; ++li)
		{
			VLine l = lines.operator[](li);
			VPoint v = l.GetV();
			v = cs.AbsoluteVectorToRelativeVector(v);
			vreal ang = PolarCalc2d::GetPolarAngle(v.x(), v.y());
			if (ang > 180.0)
			{
				ang -= 180.0;
			}
			allAngles[ang] = ang;
			if (ang < 90)
				allAngles[ang + 90.0] = ang + 90.0;
			else
				allAngles[ang - 90.0] = ang - 90.0;
		}
	}
}

void LineOrArc::UpdateMinMaxPointAlongCsX(const CoordinateSystem &cs, const std::vector<LineOrArc> &vec, VPoint &mn, VPoint &mx , vreal tol)
{
	int geosize = (int)vec.size();
	for (int gi = 0; gi < geosize; ++gi)
	{
		const LineOrArc &geo = vec.at(gi);
		geo.UpdateMinMaxPointAlongCsX(cs, mn, mx , tol);
	}
}

void LineOrArc::UpdateMinMaxPointAlongCsX(const CoordinateSystem &cs, VPoint &mn, VPoint &mx , vreal tol)const
{
	SequentialLineSet sl = GetSl(tol);
	int pcount = sl.GetPointSize();
	for (int i = 0; i<pcount; ++i)
	{
		const VPoint *p = sl.GetPoint(i);
		VPoint lp = cs.AbsoluteCoordToRelativeCoord(*p);
		if (lp.x() < mn.x())
		{
			mn = lp;
		}
		if (lp.x() > mx.x())
		{
			mx = lp;
		}
	}
}

void LineOrArc::GreedySortLevelOfContours(const VPoint &sortingStartPoint, std::list<std::vector<LineOrArc>> &oneLevel , bool makeCCW)
{
	if (oneLevel.size() == 0)
		return;
	typedef std::list<std::vector<LineOrArc>>::iterator ciItr;
	VPoint sp = sortingStartPoint;
	std::list<std::vector<LineOrArc>> sorted;
	while (true)
	{
		ciItr res = GetNearestContour(sp, oneLevel);
		if (res != oneLevel.end())
		{
			if (makeCCW != LineOrArc::IsCCW(*res))
			{
				LineOrArc::Reverse(*res);
			}
			sorted.push_back(*res);
			sp = res->at(res->size() - 1).GetEndPoint();
			oneLevel.erase(res);
		}
		else
			throw (VException("LineOrArc::GreedySortLevelOfContours exception , GetNearestContour failed", 1));
		if (oneLevel.size() == 0)
			break;
	}
	oneLevel = sorted;
}

std::list<std::vector<LineOrArc>>::iterator LineOrArc::GetNearestContour(const VPoint &sp, std::list<std::vector<LineOrArc>> &oneLevel)
{
	typedef std::list<std::vector<LineOrArc>>::iterator ciItr;
	ciItr ci = oneLevel.begin();
	ciItr ce = oneLevel.end();
	ciItr res = ce;
	vreal minDist = MAX_NUMBER_LIMIT;
	VPoint minp;
	vreal mint = 0.0;
	size_t mingeoi = 0;
	for (ci; ci != ce; ++ci)
	{
		size_t curmingeoi;
		VPoint curminp;
		vreal curmint;
		LineOrArc::FindMinPoint(*ci, sp, curmingeoi, curminp, curmint);
		vreal curdist = (sp - curminp).GetLength();
		if (curdist < minDist)
		{
			minDist = curdist;
			minp = curminp;
			mint = curmint;
			mingeoi = curmingeoi;
			res = ci;
		}
	}
	if (res != ce)
	{
		LineOrArc::ReorderLoop(*res, mingeoi, minp, mint, 0.04*VGlobalValues::ONEMM_TO_VUNIT);// 0.04 * VGlobalValues::m_unitMultiplier * VGlobalValues::ONEMETER_TO_VUNIT);
	}
	return res;
}

//void LineOrArc::CheckConcaveCorners(bool isCCW, bool isToolRight, LineOrArc &prevGeo, LineOrArc &geo, LineOrArc &nextGeo, bool &isPrevConcave, bool &isNextConcave , vreal tol )
//{
//	VPoint tanprev = prevGeo.GetTan(0.5);//prevGeo.GetEndTangent();
//	VPoint tanstart = geo.GetTan(0.5);// geo.GetStartTangent();
//	VPoint tanend = geo.GetTan(0.5);//geo.GetEndTangent();
//	VPoint tannext = nextGeo.GetTan(0.5);//nextGeo.GetStartTangent();
//
//	VPoint ckStart = tanprev ^ tanstart;
//	VPoint ckEnd = tanend ^ tannext;
//	vreal tol2 = tol*tol;
//	if (ckStart.Compare(VPoint(0.0), tol2))
//	{
//		tanprev = prevGeo.GetTan(0.9);
//		tanstart = geo.GetTan(0.1);
//		ckStart = tanprev ^ tanstart;
//	}
//	if (ckEnd.Compare(VPoint(0.0), tol2))
//	{
//		tanend = geo.GetTan(0.5); //(0.9);
//		tannext = nextGeo.GetTan(0.5);//(0.1);
//		ckEnd = tanend ^ tannext;
//	}
//
//	if (isCCW)
//	{
//		if (ckStart.z() < -tol2)
//		{
//			isPrevConcave = true;
//		}
//		else
//		{
//			isPrevConcave = false;
//		}
//
//		if (ckEnd.z() < -tol2)
//		{
//			isNextConcave = true;
//		}
//		else
//		{
//			isNextConcave = false;
//		}
//
//		if (isToolRight == false)
//		{
//			isPrevConcave = (!isPrevConcave);
//			isNextConcave = (!isNextConcave);
//		}
//	}
//	else
//	{
//		if (ckStart.z() < tol2)
//		{
//			isPrevConcave = false;
//		}
//		else
//		{
//			isPrevConcave = true;
//		}
//
//		if (ckEnd.z() < tol2)
//		{
//			isNextConcave = false;
//		}
//		else
//		{
//			isNextConcave = true;
//		}
//		if (isToolRight == true)
//		{
//			isPrevConcave = (!isPrevConcave);
//			isNextConcave = (!isNextConcave);
//		}
//	}
//}

bool LineOrArc::IsCornerConcave(const LineOrArc &geo, const LineOrArc &nextGeo, bool isToolRight, vreal tol)
{
	VPoint t1 = geo.GetEndTangent();
	VPoint t2 = nextGeo.GetStartTangent();
	VPoint n = t1 ^ t2;
	vreal tock = n.z();
	if (fabs(tock) < tol)
	{
		// tangent are parallel 
		return false;
	}
	if (isToolRight)
	{
		if (tock < 0.0)
			return true;
		else
			return false;
	}
	else
	{
		if (tock < 0.0)
			return false;
		else
			return true;
	}
}

void LineOrArc::CheckConcaveCorners(bool isToolRight, const LineOrArc& prevGeo, const LineOrArc& geo, const LineOrArc& nextGeo, bool &isPrevConcave, bool &isNextConcave, vreal tol)
{
	isPrevConcave = IsCornerConcave(prevGeo, geo, isToolRight ,tol);
	isNextConcave = IsCornerConcave(geo, nextGeo, isToolRight , tol);
}
VPoint LineOrArc::MergeOpenContours(const VPoint &startPoint, std::vector<std::vector<LineOrArc>> &contourVec, std::vector<bool> &isClosedVec, double coinTol, double sewMaxDist)
{
	size_t mysize = contourVec.size();
	std::vector<bool> movedVec(mysize, false);

	for (size_t i = 0; i<mysize; ++i)
	{
		bool curclosed = isClosedVec[i];
		if (curclosed || movedVec[i])
			continue;
		else
		{
			std::vector<LineOrArc> &cv0 = contourVec[i];
			for (size_t j = i + 1; j<mysize; ++j)
			{
				bool curclosed2 = isClosedVec[j];
				if (curclosed2)
					continue;
				else
				{
					std::vector<LineOrArc> &cv1 = contourVec[j];
					bool canmerge = Mergeif(cv0, cv1, sewMaxDist);
					if (canmerge)
					{
						movedVec[i] = true;
						break;
					}
				}

			}

		}
	}
	// Change the contour vec by removing the moved vectors
	std::vector<std::vector<LineOrArc>> newContourVec;
	for (size_t i = 0; i<mysize; ++i)
	{
		if (movedVec[i] == false)
			newContourVec.push_back(contourVec.at(i));
	}
	//contourVec = newContourVec;
	// Update isclosed vec
	size_t mysizeNew = newContourVec.size();
	std::vector<bool> newIsClosedVec;
	VPoint endPoint;
	VPoint sp;
	std::vector<std::vector<LineOrArc>> newnewContourVec;
	for (size_t i = 0; i<mysizeNew; ++i)
	{
		sp = newContourVec.at(i).front().GetStartPoint();
		endPoint = newContourVec.at(i).back().GetEndPoint();
		if (sp.Compare(endPoint, coinTol))
			newIsClosedVec.push_back(true);
		else
			newIsClosedVec.push_back(false);
		std::vector<LineOrArc> ctorev = newContourVec.at(i);
		if ((sp - startPoint).GetLength2() >(endPoint - startPoint).GetLength2())
		{
			LineOrArc::Reverse(ctorev);
		}
		newnewContourVec.push_back(ctorev);
		endPoint = ctorev.back().GetEndPoint();
	}
	isClosedVec = newIsClosedVec;
	contourVec = newnewContourVec;
	return endPoint;
}

bool LineOrArc::Mergeif(std::vector<LineOrArc> &cv0, std::vector<LineOrArc> &cv1, double sewMaxDist)
{
	if (cv0.size() == 0 || cv1.size() == 0)
		throw VException((IDS_CloMerExcOneOfT), 1);
	VPoint p0begin = cv0.front().GetStartPoint();
	VPoint p0end = cv0.back().GetStartPoint();
	VPoint p1begin = cv1.front().GetStartPoint();
	VPoint p1end = cv1.back().GetEndPoint();

	if (p1end.Compare(p0begin, sewMaxDist))
	{
		// add cv0 to end of cv1,no reversing
		cv1.insert(cv1.end(), cv0.begin(), cv0.end());
		return true;
	}
	else if (p1end.Compare(p0end, sewMaxDist))
	{
		// reverse cv0 and  add to end of cv1
		std::vector<LineOrArc>::reverse_iterator rb = cv0.rbegin();
		std::vector<LineOrArc>::reverse_iterator re = cv0.rend();
		for (rb; rb != re; ++rb)
		{
			(*rb).Reverse();
			cv1.push_back(*rb);
		}
		return true;
	}
	else if (p1begin.Compare(p0end, sewMaxDist))
	{
		// add cv1 to end of cv0 and set cv1 = cv0
		cv0.insert(cv0.end(), cv1.begin(), cv1.end());
		cv1 = cv0;
		return true;
	}
	else if (p1begin.Compare(p0begin, sewMaxDist))
	{
		// reverse cv0 and add cv1 and set cv1
		std::vector<LineOrArc> tmp;
		std::vector<LineOrArc>::reverse_iterator rb = cv0.rbegin();
		std::vector<LineOrArc>::reverse_iterator re = cv0.rend();
		for (rb; rb != re; ++rb)
		{
			(*rb).Reverse();
			tmp.push_back(*rb);
		}
		tmp.insert(tmp.end(), cv1.begin(), cv1.end());
		cv1 = tmp;
		return true;
	}

	return false;
}

void LineOrArc::GetPositiveAngleIntervalsOfArcToAbsXAxis(const VArc *a, std::vector<VInterval> &anglesInDegrees , vreal tol)
{
	if (a->GetAx().Compare(UZ, tol) == false)
		throw VException("LineOrArc::GetPositiveAngleIntervalsOfArcToAbsXAxis" , 1);

	VArc copyA(*a);
	if (copyA.GetSweepAngle() < 0.0)
	{
		copyA = VArc(a->GetCenter(), a->GetRadius(), a->GetAx(), a->GetEndPoint(), -a->GetSweepAngle());
	}
	vreal i0 = PolarCalc2d::GetPolarAngle(copyA.GetStartPoint().x(), copyA.GetStartPoint().y());
	vreal i1 = i0 + copyA.GetSweepAngle();
	if (i1 < 360.0 + tol)
	{
		if (i1 < 360.0 - tol)
		{
			anglesInDegrees.push_back(VInterval(i0,i1));
		}
		else
		{
			anglesInDegrees.push_back(VInterval(i0, 360.0));
		}
	}
	else
	{
		// split to 2 intervals
		anglesInDegrees.push_back(VInterval(i0 , 360.0));
		anglesInDegrees.push_back(VInterval(0.0, i1 - 360.0));
	}
}

void LineOrArc::WriteLoopToFileAsciiAsOneLine(const std::vector<LineOrArc> &loop, std::ofstream &f, char coordinateSeperator,int floatPrecision , vreal arcDigitiseTol)
{

	std::vector<LineOrArc>::const_iterator i = loop.begin();
	std::vector<LineOrArc>::const_iterator e = loop.end();
	if ((*i).GetType() == LineOrArc::line)
	{
		WriteSinglePoint((*i).GetStartPoint(), f, coordinateSeperator, floatPrecision);
	}
	for (i; i != e; ++i)
	{
		(*i).GetType() == LineOrArc::line ? WriteSinglePoint((*i).GetEndPoint(), f, coordinateSeperator, floatPrecision):WriteArcGeo(*i,f,coordinateSeperator,floatPrecision,arcDigitiseTol);
	}
	f << "\n";
}

void LineOrArc::WriteSinglePoint(const VPoint &p, std::ofstream &f, char coordinateSeperator, int floatPrecision)
{
	f << std::setprecision(floatPrecision) << p.x() << coordinateSeperator << p.y() << coordinateSeperator << p.z() << coordinateSeperator;
}

void LineOrArc::WriteArcGeo(const LineOrArc &arcGeo, std::ofstream &f, char coordinateSeperator, int floatPrecision , vreal arcDigitiseTol)
{
	SequentialLineSet sl = arcGeo.GetSl(arcDigitiseTol);
	int slPointSize = sl.GetPointSize();
	for (int i = 0; i < slPointSize; ++i)
	{
		const VPoint *p = sl.GetPoint(i);
		WriteSinglePoint(*p, f, coordinateSeperator, floatPrecision);
	}
}

std::vector<std::vector<LineOrArc>> LineOrArc::BreakAtSharpCorners(const std::vector<LineOrArc> &toBreak, vreal sharpAngleThreshold)
{
	std::vector<std::vector<LineOrArc>> rVec;
	int totalSize = (int)toBreak.size();
	int counter = 0;
	vreal cosAngleLimit = cos(sharpAngleThreshold*DEGREE_TO_RAD);
	bool doAdd = true;
	const LineOrArc *prevGeo = NULL;
	while (counter < totalSize)
	{
		if (doAdd)
		{
			prevGeo = &toBreak.at(counter);
			rVec.push_back(std::vector<LineOrArc>());
			rVec.back().push_back(*prevGeo);
			doAdd = false;
		}
		++counter;
		for (int i = counter; i < totalSize;++i)
		{
			const LineOrArc *nextGeo = &toBreak.at(i);
			counter = i;
			bool ckSharpCorner = VPoint::IsSharpCornerU(prevGeo->GetEndTangent(), nextGeo->GetStartTangent(), cosAngleLimit);
			prevGeo = nextGeo;
			if (ckSharpCorner == false)
			{
				rVec.back().push_back(*nextGeo);
			}
			else
			{
				if (rVec.back().size() > 0)
					doAdd = true;
				break;
			}
		}
	}
	return rVec;
}

void LineOrArc::MakeAndAddToVec(const std::vector<LineOrArc>& srcVec, std::vector<LineOrArc*>& toAdd)
{
	for (size_t i = 0; i < srcVec.size(); ++i)
	{
		const LineOrArc& geo = srcVec.at(i);
		LineOrArc* newGeo = new LineOrArc(geo);
		toAdd.push_back(newGeo);
	}
}

void LineOrArc::DestroyVecVec(std::vector<std::vector<LineOrArc*>>& vec)
{
	for (size_t i = 0; i < vec.size(); ++i)
	{
		std::vector<LineOrArc*>& curVec = vec.at(i);
		DestroyVector(curVec);
		//curVec.clear();
	}
	vec.clear();
}
void LineOrArc::DestroyVecVec(std::vector<std::vector<LineOrArc*>*>& vec)
{
	for (size_t i = 0; i < vec.size(); ++i)
	{
		std::vector<LineOrArc*>* curVec = vec.at(i);
		DestroyVector(*curVec);
		//curVec.clear();
		delete curVec;
	}
	vec.clear();
}
std::vector<LineOrArc*>* LineOrArc::CopyClone(const std::vector<LineOrArc*>* src)
{
	std::vector<LineOrArc*>* newVec = new std::vector<LineOrArc*>(src->size());
	for (size_t i = 0; i < src->size(); ++i)
	{
		newVec->at(i) = new LineOrArc(*src->at(i));
	}
	return newVec;
}

void LineOrArc::FillVectorByMaking(const std::vector<LineOrArc*>& src, std::vector<LineOrArc*>& tofill)
{
	tofill.reserve(src.size());
	for (size_t i = 0; i < src.size(); ++i)
	{
		tofill.push_back(new LineOrArc(*src.at(i)));
	}
}

std::vector<LineOrArc> LineOrArc::FromPointer(const std::vector<LineOrArc*>& src)
{
	std::vector<LineOrArc>vec;
	vec.reserve(src.size());
	for (size_t i = 0; i < src.size(); ++i)
	{
		vec.push_back(*src.at(i));
	}
	return vec;
}

std::vector<LineOrArc*> ToPointer(const std::vector<LineOrArc>& src)
{
	std::vector<LineOrArc*> vec;
	vec.reserve(src.size());
	for (size_t i = 0; i < src.size(); ++i)
	{
		vec.push_back(new LineOrArc(src.at(i)));
	}
	return vec;
}

LineOrArc LineOrArc::GetOffseted(vreal offsVal , const VPoint& planeNormal)const
{
	VPoint sp = GetStartPoint();
	VPoint st = GetStartTangent();
	VPoint sn = st ^ planeNormal;
	VPoint ep = GetEndPoint();
	VPoint et = GetEndTangent();
	VPoint en = et ^ planeNormal;
	if (m_type == line)
	{
		return LineOrArc(VLine(sp+sn*offsVal,ep+en*offsVal));
	}
	else
	{
		VPoint mp = GetP(0.5);
		VPoint mt = GetTan(0.5);
		VPoint mn = mt ^ planeNormal;
		return LineOrArc(VArc(sp + sn * offsVal,mp + mn * offsVal , ep + en * offsVal));
	}
}