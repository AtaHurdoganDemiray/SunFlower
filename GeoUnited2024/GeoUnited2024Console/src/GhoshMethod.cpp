#include "stdafx.h"
#include "GhoshMethod.h"
#include "PolarCalc2d.h"
#include "RotationMatrix.h"
#include "MoveMatrix.h"

GhoshMethod::GhoshMethod()
	:m_geo(100), m_tool(100), m_resultLS(100)
{ 
	m_digitiseStep = 1000;
}

GhoshMethod::~GhoshMethod()
{
}
void GhoshMethod::SetGeo(const std::vector<LineOrArc>&geo)
{
	SequentialLineSet tmpSeqL(100);
	std::vector <LineOrArc> tmp = geo;
	if (!LineOrArc::IsCCW(tmp))
	{
		LineOrArc::ReverseIf(true, tmp,VGlobalValues::ONEMICRON_TO_VUNIT);
	}

	VPoint minP;
	VPoint maxP;
	LineOrArc::GetMinMax(tmp, minP, maxP); 
	size_t mgeoi;
	VPoint mP;
	vreal t;
	LineOrArc::FindMinPoint(tmp, minP, mgeoi, mP, t);
	LineOrArc::ReorderLoop(tmp, mgeoi, mP, t , 0.04 * VGlobalValues::m_unitMultiplier);
	LineOrArc::DigitiseByLengthStep(tmp, m_digitiseStep, tmpSeqL);
	m_geo = tmpSeqL;
	
	
}
void GhoshMethod::SetTool(const std::vector<LineOrArc> &tool)
{
	SequentialLineSet tmpSeqL(100);
	std::vector <LineOrArc> tmp = tool;   
	if (!LineOrArc::IsCCW(tmp))
	{
		LineOrArc::ReverseIf(true, tmp,VGlobalValues::ONEMICRON_TO_VUNIT);
	}
	VPoint minP;
	VPoint maxP;
	LineOrArc::GetMinMax(tmp, minP, maxP);
	size_t mgeoi;
	VPoint mP;
	vreal t;
	LineOrArc::FindMinPoint(tmp, minP, mgeoi, mP, t);
	LineOrArc::ReorderLoop(tmp, mgeoi, mP, t , 0.04 * VGlobalValues::m_unitMultiplier);
	VPoint v(0, 0, 1);
	RotationMatrix rm(m_toolRefPoint, v, 180);
	LineOrArc::Transform(tmp, rm); 
	LineOrArc::DigitiseByLengthStep(tmp, m_digitiseStep / 10000, tmpSeqL);
	m_tool = tmpSeqL;
	
}
void GhoshMethod::SetToolRefPoint(const VPoint& p)
{
	m_toolRefPoint = p;
}
void GhoshMethod::SetDigitiseStep(vreal digitiseStep)
{
	m_digitiseStep = digitiseStep * 10000;
}
VPoint GhoshMethod::GetToolRefPoint()
{
	return m_toolRefPoint;
}
SequentialLineSet GhoshMethod::GetGeo()
{
	return m_geo;
}
SequentialLineSet GhoshMethod::GetTool()
{
	return m_tool;
}
vreal GhoshMethod::GetDigitiseStep()
{
	return m_digitiseStep;
}
void GhoshMethod::CreateMergeList()
{  
	 
	std::vector<GhoshItem> tmpMergeVec;
	int size = m_geo.Size();
	int size2 = m_tool.Size();
	 
	if (size > 0)
	{
		VLine tmpL = m_geo[0];
		GhoshItem gt(tmpL, GhoshItem::GhsType::Geo, 0);
		gt.m_name = "A" + TypeConverters::ConvertIntToString(0);
		tmpMergeVec.push_back(gt);
	}
	for (int i = 1; i < size; i++)
	{
		VLine tmpL = m_geo[i];
		GhoshItem gt(tmpL, GhoshItem::GhsType::Geo, i);
		gt.m_name = "A" + TypeConverters::ConvertIntToString(i);
		tmpMergeVec.push_back(gt);
	}

	if (size2 > 0)
	{
		VLine tmpL = m_tool[0];
		GhoshItem gt(tmpL, GhoshItem::GhsType::Tool, 0);
		gt.m_name = "B" + TypeConverters::ConvertIntToString(0);
		tmpMergeVec.push_back(gt);

	}
	for (int i = 1; i < size2; i++)
	{
		VLine tmpL = m_tool[i];
		GhoshItem gt(tmpL, GhoshItem::GhsType::Tool, i);
		gt.m_name = "B" + TypeConverters::ConvertIntToString(i);
		tmpMergeVec.push_back(gt); 
	}

	GhoshItem::Sort(tmpMergeVec);

	//int aSize = (int)tmpMergeVecA.size();
	//int bSize = (int)tmpMergeVecB.size();
	////GhsTableItem::Sort(tmpMergeVecA);
	//GhsTableItem::Sort(tmpMergeVecB); 
	//int aStartIndex = 0;
	//int bStartIndex = 0;
	//while (aStartIndex < aSize)
	//{
	//	while (bStartIndex < bSize)
	//	{
	//		if (tmpMergeVecA.at(aStartIndex).m_angle > tmpMergeVecB.at(bStartIndex).m_angle )
	//		{
	//			tmpMergeVec.push_back(tmpMergeVecB.at(bStartIndex));
	//			bStartIndex++;
	//		}
	//		else
	//		{
	//			tmpMergeVec.push_back(tmpMergeVecA.at(aStartIndex));
	//			aStartIndex++;
	//			break;
	//		} 
	//	}
	//} 
	//while (bStartIndex < bSize)
	//{
	//	tmpMergeVec.push_back(tmpMergeVecB.at(bStartIndex));
	//	bStartIndex++;
	//}

	//int lsSize = m_geo.Size();
	//for (int i = 0; i < lsSize; i++)
	//{
	//	int aIndex = this->FindVecIndexByItemIndex(i, tmpMergeVec);
	//	if (m_geo.IsTurningLine(i))
	//	{
	//		tmpMergeVec.at(aIndex).m_isTurningPoint = true;
	//		/*if (i +1 < lsSize)
	//		{
	//			int aIndexPlus = this->FindVecIndexByItemIndex(i+1, tmpMergeVec);
	//			if (aIndexPlus != -1)
	//			{
	//				tmpMergeVec.at(aIndexPlus).m_isTurningPoint = true;
	//				i++;
	//			}
	//		}*/
	//	}
	//}
	 
	m_mergeVec = tmpMergeVec;
}
void GhoshMethod::GhoshFunc()
{ 
	
	int p = FindVecIndexByItemIndex(0, m_mergeVec);
	if (p == -1)
	{
		throw new VException("Geo first index not found. GhoshMethod::GhoshFunc()", 1);
	}
	int i = 0;
	int dir = 1; 
	do
	{
		if (m_mergeVec.at(p).GetGType() == GhoshItem::Geo)
		{
			if (m_mergeVec.at(p).m_index == i)
			{
				GhoshItem tg(m_mergeVec.at(p));
				AddGhsItemToList(tg);
				if (tg.m_isTurningPoint)
				{ 
					dir = -1 * dir;
				}
				
				i++;

				if (i >= m_geo.Size())
				{
					i = 0;
				}
			}

		}
		else
		{ 
			GhoshItem tg(m_mergeVec.at(p));
			tg.ReverseDirection(dir);
			AddGhsItemToList(tg);
		}
		p += dir;
		//if (p < 0)
		//	p = m_mergeVec.size() - 1;
		//if (p > m_mergeVec.size()-1)
		//	p = 0;
	} 
	while (i != 0);
	 
	int size = (int)m_ghosList.size();
	if (size < m_mergeVec.size() &&  size > 1)
	{ 
		VLine l(m_ghosList.at(size - 1).GetCurL().GetP2(), m_ghosList.at(0).GetCurL().GetP1());
		GhoshItem gt(l,GhoshItem::Tool,size-1);
		m_ghosList.push_back(gt);
	}
}

std::vector<GhoshItem>& GhoshMethod::GetGhoshResult()
{ 
	return m_ghosList;
}
void GhoshMethod::Calc()
{
	m_ghosList.clear(); 
	m_mergeVec.clear();

	CreateMergeList();
	//GhoshFunc();
	GhoshFunc2();
} 
void GhoshMethod::AddGhsItemToList(GhoshItem& ghs)
{
	int size = (int)m_ghosList.size();
	if (size > 0)
	{  
		VPoint p1 = m_ghosList.at(size - 1).GetCurL().GetP2();
	    VPoint dp = p1 - ghs.GetCurL().GetP1();
		VPoint p2 = ghs.GetCurL().GetP2();
		p2 = p2 + dp;
		VLine l(p1, p2);
		ghs.SetLine(l); 
	}
	m_ghosList.push_back(ghs);
}
int GhoshMethod::FindVecIndexByItemIndex(int ghsItemIndex, const std::vector<GhoshItem> &vec)
{
	int size = (int)vec.size(); 
	for (int i = 0; i < size; i++)
	{
		if (vec.at(i).m_index == ghsItemIndex && vec.at(i).m_type == GhoshItem::Geo)
		{
			return i;
		}
	}

	return -1;
}


void GhoshMethod::GetLineSet(const std::vector<GhoshItem>& ghsVec, SequentialLineSet &ls)
{

	std::vector<LineOrArc> lar;
	int size = (int)ghsVec.size();
	for (int i = 0; i < size; i++)
	{
		VLine l = ghsVec.at(i).m_line;
		lar.push_back(l);
	}
	LineOrArc::DigitiseByLengthStep(lar, 100000, ls);
}

std::vector<LineOrArc> *GhoshMethod::GetLineResult()
{
	int size = (int)m_resultLS.Size();
	std::vector<LineOrArc> *lar = new std::vector<LineOrArc>(0);
	for (int i = 0; i < size; i++)
	{ 
		lar->push_back(m_resultLS[i]);
	}
	return lar;
}
void GhoshMethod::SetGeo(const SequentialLineSet& geoLS)
{
	m_geo = geoLS;
}
void GhoshMethod::SetTool(const SequentialLineSet& toolLS)
{
	m_tool = toolLS;
}
void GhoshMethod::GhoshFunc2()
{
	int size = (int)m_mergeVec.size();
	if (size > 0)
	{
		int p = FindVecIndexByItemIndex(0, m_mergeVec);
		if (p == -1)
		{
			throw new VException("Geo first index not found. GhoshMethod::GhoshFunc()", 1);
		}
		//VPoint geoRefP = m_mergeVec.at(p).m_line.GetP1();
		std::vector<VLine> tmpMergeVec;

		tmpMergeVec.push_back(m_mergeVec.at(0).GetCurL());
		for (int i = 1; i < size; i++)
		{
			int tmpSize = (int)tmpMergeVec.size();
			VPoint p1 = tmpMergeVec.at(tmpSize - 1).GetP2();
			VPoint dp = p1 - m_mergeVec.at(i).GetCurL().GetP1();
			VPoint p2 = m_mergeVec.at(i).GetCurL().GetP2();
			p2 = p2 + dp;
			VLine l(p1, p2);
			tmpMergeVec.push_back(l);
		}
		 

		VPoint tMinP = m_tool.GetMin();
		double dxLen = m_toolRefPoint.x() - tMinP.x();
		double dyLen = m_toolRefPoint.y() - tMinP.y();
		VPoint geominP = m_geo.GetMin(); 
		double targetX = geominP.x() - dxLen;
		double targetY = geominP.y() - dyLen;

		//SequentialLineSet tmpTool(m_tool);
		//VPoint v = geoRefP - m_toolRefPoint;
		//MoveMatrix mm(v);
		//tmpTool.Transform(mm); 
		//VPoint minP = tmpTool.GetMin();  

		//double dY = minP.y() - geoRefP.y(); 
		//double targetY = geoRefP.y() + dY;

		VPoint startP(targetX, targetY, 0);
		  
		
		
		 
		std::vector<LineOrArc> *lar = new std::vector<LineOrArc>(0);
		int tmpSize = (int)tmpMergeVec.size();
		for (int i = 0; i < tmpSize; i++)
		{
			VLine l = tmpMergeVec.at(i);
			lar->push_back(l);
		}
		 

		SequentialLineSet tmpLs(100);
		LineOrArc::DigitiseByLengthStep(*lar, m_digitiseStep, tmpLs);
		VPoint resMinP = tmpLs.GetMin();
		VPoint nv = startP - resMinP;
		MoveMatrix mm2(nv);
		tmpLs.Transform(mm2);
		m_resultLS = tmpLs;  
	}
}
SequentialLineSet GhoshMethod::GetResultLs()
{
	return m_resultLS;
}