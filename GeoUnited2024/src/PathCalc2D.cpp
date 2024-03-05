#include "stdafx.h"
#include "PathCalc2D.h"
#include "TransformationMatrix.h"
#include "RotationMatrix.h"
#include "MoveMatrix.h"

PathCalc2D::PathCalc2D()
	:m_toolSeqLineSet(0), m_geoSeqLineSet(0)
{
	m_toolDigitise = 10.0;
	m_geoDigitise = 10.0;
}
PathCalc2D::PathCalc2D(const std::vector<LineOrArc>& tool, const VPoint &toolRefPoint, const std::vector<LineOrArc>& geo, const vreal &toolDigitise, const vreal &geoDigitise)
	:m_toolSeqLineSet(0), m_geoSeqLineSet(0)
{
	m_tool = tool;
	m_geo = geo;
	m_toolRefPoint = toolRefPoint;
	m_toolDigitise = toolDigitise;
	m_geoDigitise = geoDigitise;
	MakeSeqLineSet(m_tool, *m_toolSeqLineSet, m_toolDigitise);
	MakeSeqLineSet(m_geo, *m_geoSeqLineSet, m_geoDigitise);

}
PathCalc2D::~PathCalc2D()
{
	try
	{
	/*	if (m_toolSeqLineSet != NULL)
			delete m_toolSeqLineSet;
		if (m_geoSeqLineSet != NULL)
			delete m_geoSeqLineSet;*/
	}
	catch (...)
	{
		LogFileWriter::WriteUnknownExceptionLog("Delete members failed. PathCalc2D::~PathCalc2D()");
	}
}
void PathCalc2D::MakeSeqLineSet(const std::vector <LineOrArc>& lineOrArc, SequentialLineSet & seqLineSet, vreal digitiseStep)
{
	LineOrArc::DigitiseByLengthStep(lineOrArc, digitiseStep, seqLineSet);
}
bool PathCalc2D::Calc(SequentialLineSet& path, std::vector<LineOrArc>& laVec)
{
	std::vector<VPoint> pointVec;
	UnitVector uv(0, 0, 1);
	SequentialLineSet tmp(*m_toolSeqLineSet);
	RotationMatrix rm(m_toolRefPoint, uv, 180);
	tmp.Transform(rm); 
	int sSize = m_geoSeqLineSet->Size(); 
	for (int i = 0; i < sSize; i++)
	{ 
		VPoint tmpV =(*m_geoSeqLineSet)[i].GetP1() - m_toolRefPoint;
		MoveMatrix mm(tmpV);
		SequentialLineSet tmp2 (tmp);
		tmp2.Transform(mm);
		//path.AddBack(tmp2); 
		 
		PointSet ps2 = tmp2.GetPointSet();
		int tmp2Size = ps2.Size();
		for (int k = 0; k < tmp2Size; k++)
		{
			VPoint moveV = ps2.At(k) - m_toolRefPoint;
			MoveMatrix mm(moveV);
			SequentialLineSet tmp3(*m_toolSeqLineSet);
			tmp3.Transform(mm);

			bool isRight = false;
			PointSet ps3 = tmp3.GetPointSet();
			int tmp3Size = ps3.Size();
			for (int t = 0; t < tmp3Size; t++)
			{
				int res  = IsPointRightLeftOnLine(ps3.At(t), (*m_geoSeqLineSet)[i]); //LineOrArc::IsPointInLoop(m_geo, ps3.At(t));
				if (res == 0 || res == 2)
				{
					isRight = true;
				}
				else
				{
					isRight = false;
					break;
				}
			}

			if (isRight)
			{
				bool ck = IsPointExist(ps2.At(k), pointVec);
				if (!ck)
				{
					pointVec.push_back(ps2.At(k));
				}
			}
		} 
	}
	int size = (int)pointVec.size();
	std::vector<LineOrArc> lar;
	if (size > 1)
	{
		VLine l(pointVec.at(0), pointVec.at(1));
		lar.push_back(l);
	}
	for (int i = 2; i < size; i++)
	{
		VLine l(pointVec.at(i - 1), pointVec.at(i));
		lar.push_back(l);
	}
	if (lar.size() > 1)
	{
		VLine l(pointVec.at(pointVec.size() - 1), pointVec.at(0));
		lar.push_back(l);
	}
	laVec = lar;
	LineOrArc::DigitiseByLengthStep(lar, m_geoDigitise, path);

	return true;
}
void PathCalc2D::SetTool(const std::vector<LineOrArc> &tool)
{ 
	std::vector <LineOrArc> tmp = tool;
	if (!LineOrArc::IsCCW(tmp))
	{
		LineOrArc::ReverseIf(true, tmp,VGlobalValues::ONEMICRON_TO_VUNIT*10.0);
	}
	VPoint minP;
	VPoint maxP;
	LineOrArc::GetMinMax(tmp, minP, maxP);
	size_t mgeoi;
	VPoint mP;
	vreal t;
	LineOrArc::FindMinPoint(tmp, minP, mgeoi, mP, t);
	LineOrArc::ReorderLoop(tmp, mgeoi, mP, t , 0.04 * VGlobalValues::m_unitMultiplier * VGlobalValues::ONEMM_TO_VUNIT);

	m_tool = tmp;
	m_toolSeqLineSet = new  SequentialLineSet(100);
	MakeSeqLineSet(m_tool, *m_toolSeqLineSet, m_toolDigitise);
	
}
void PathCalc2D::SetGeo(const std::vector<LineOrArc> & geo)
{
	std::vector <LineOrArc> tmp = geo;
	if (!LineOrArc::IsCCW(tmp))
	{
		LineOrArc::ReverseIf(true, tmp,VGlobalValues::ONEMICRON_TO_VUNIT*10.0);
	}
	VPoint minP;
	VPoint maxP;
	LineOrArc::GetMinMax(tmp, minP, maxP);
	size_t mgeoi;
	VPoint mP;
	vreal t;
	LineOrArc::FindMinPoint(tmp, minP, mgeoi, mP, t);
	LineOrArc::ReorderLoop(tmp, mgeoi, mP, t , 0.04 * VGlobalValues::m_unitMultiplier * VGlobalValues::ONEMM_TO_VUNIT);
	m_geo = tmp;
	m_geoSeqLineSet = new  SequentialLineSet(100);
	MakeSeqLineSet(m_geo, *m_geoSeqLineSet, m_geoDigitise);
}
void PathCalc2D::SetToolDigitiseStepLength(vreal stepLen)
{
	m_toolDigitise = stepLen;
}
void PathCalc2D::SetGeoDigitiseStepLength(vreal stepLen)
{
	m_geoDigitise = stepLen;
}
void PathCalc2D::SetToolRefPoint(const VPoint& p)
{
	m_toolRefPoint = p;
}
VPoint PathCalc2D::GetToolRefP()
{
	return m_toolRefPoint;
}
double PathCalc2D::GetToolDigitiseSteLength()
{
	return m_toolDigitise;;
}
double PathCalc2D::GetGeoDigitiseSteLength()
{
	return m_geoDigitise;;
}
int PathCalc2D::IsPointRightLeftOnLine(const VPoint& p, const VLine & line)
{ 
	VLine l = line; 
	double d = ((p.y() - l.GetP1().y())*(l.GetP2().x() - l.GetP1().x())) - ((p.x() - l.GetP1().x())*(l.GetP2().y() - l.GetP1().y()));
	if (fabs(d) < EPSILON)
		return 0;  
	if (d > 0) 
		return 1;
	if (d < 0)
		return 2;

	return -1;
	 
}
bool PathCalc2D::IsPointExist(const VPoint& p, const std::vector<VPoint> & pointVec)
{
	int size = pointVec.size();
	for (int i = 0; i < size; i++)
	{
		bool ck = p.Compare(pointVec.at(i),0.1);
		if (ck)
			return true;
	}

	return false;
}
