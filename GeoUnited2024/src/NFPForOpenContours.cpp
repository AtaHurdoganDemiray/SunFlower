#include "stdafx.h"
#include "NFPForOpenContours.h"
#include "MoveMatrix.h"
#include "RotationMatrix.h"

NFPForOpenContours::NFPForOpenContours()
	:m_tool(0),
	m_geo(0),
	m_cutTool(0),
	m_result(0)
{

} 
NFPForOpenContours::~NFPForOpenContours()
{
}
void NFPForOpenContours::SetTool(const std::vector<LineOrArc> & closedTool)
{
	m_closedTool = closedTool;
	if (!LineOrArc::IsCCW(m_closedTool))
	{
		LineOrArc::ReverseIf(true, m_closedTool,m_tol);
	}
	VPoint minP;
	VPoint maxP;
	LineOrArc::GetMinMax(m_closedTool, minP, maxP);
	size_t mgeoi;
	VPoint mP;
	vreal t;
	LineOrArc::FindMinPoint(m_closedTool, minP, mgeoi, mP, t);
	LineOrArc::ReorderLoop(m_closedTool, mgeoi, mP, t , m_tol);
	m_tool = new SequentialLineSet(100);
	LineOrArc::DigitiseByLengthStep(m_closedTool, m_tol, *m_tool);
}
void NFPForOpenContours::SetTool(SequentialLineSet * tool)
{
	m_tool = tool;
}
SequentialLineSet * NFPForOpenContours::GetToolLS()
{
	return m_tool;
}
void NFPForOpenContours::SetCuttingTool(const std::vector<LineOrArc> & cuttingTool)
{
	m_cuttingTool = cuttingTool; 
	m_cutTool = new SequentialLineSet(100);
	LineOrArc::DigitiseByLengthStep(m_cuttingTool, m_tol, *m_cutTool);
}
void NFPForOpenContours::SetContour(const std::vector<LineOrArc> & orderedOpneOrClosedContour)
{
	m_openOrClosedContour = orderedOpneOrClosedContour;
	 
	if (!LineOrArc::IsCCW(m_openOrClosedContour))
	{
		LineOrArc::ReverseIf(true, m_openOrClosedContour,m_tol);
	}
	VPoint minP;
	VPoint maxP;
	LineOrArc::GetMinMax(m_openOrClosedContour, minP, maxP);
	size_t mgeoi;
	VPoint mP;
	vreal t;
	LineOrArc::FindMinPoint(m_openOrClosedContour, minP, mgeoi, mP, t);
	LineOrArc::ReorderLoop(m_openOrClosedContour, mgeoi, mP, t , m_tol);
	
	m_geo = new SequentialLineSet(100); 
	LineOrArc::DigitiseByLengthStep(m_openOrClosedContour, m_tol, *m_geo);
}
void NFPForOpenContours::SetToolRefPoint(const VPoint & p)
{
	m_toolRefPoint = p;
}
void NFPForOpenContours::SetTol(vreal tol)
{
	m_tol = tol;
}
void NFPForOpenContours::SetLeftRightBoth(int lrb)
{
	m_leftRightBoth = lrb;
} 
std::vector<LineOrArc> NFPForOpenContours::GetTool()
{
	return m_closedTool;
}
std::vector <LineOrArc> NFPForOpenContours::GetCuttingTool()
{
	return m_cuttingTool;
}
std::vector<LineOrArc> NFPForOpenContours::GetContour()
{
	return m_openOrClosedContour;
}
VPoint NFPForOpenContours::GetToolRefPoint()
{
	return m_toolRefPoint;
}
vreal NFPForOpenContours::GetTol()
{
	return m_tol;
}
int NFPForOpenContours::GetLeftRightBoth()
{
	return m_leftRightBoth;
}
void NFPForOpenContours::Calc()
{
	SequentialLineSet tmpGeo(100);
	LineOrArc::DigitiseByLengthStep(m_openOrClosedContour, m_tol, tmpGeo);
	std::vector<VPoint> pointVec;
	UnitVector uv(0, 0, 1);
	SequentialLineSet tmp(*m_tool);
	RotationMatrix rm(m_toolRefPoint, uv, 180);
	tmp.Transform(rm);
	int size = m_geo->Size();
	for (int i = 0; i < size; i++)
	{
		SequentialLineSet tmp2(tmp);
		Move(m_toolRefPoint, (*m_geo)[i].GetP1(), tmp2);

		PointSet ps1 = tmp2.GetPointSet();
		int tmp2Size = ps1.Size();
		for (int k = 0; k < tmp2Size; k++)
		{
			SequentialLineSet tmpOrigTool(*m_tool);
			Move(m_toolRefPoint, ps1.At(k), tmpOrigTool);
			PointSet ps2 = tmpOrigTool.GetPointSet();
			int origSize = ps2.Size();
			bool isRight = false;
			if (m_leftRightBoth == 2)
			{
				isRight = true;
			}
			else
			{
				for (int t = 0; t < origSize; t++)
				{
					int res = IsPointLeftRightOnLine(ps2.At(t), (*m_geo)[i]);
				 
					if (res == 2 || res == m_leftRightBoth)
					{
						isRight = true;
					}
					else
					{
						isRight = false;
						break;
					}
				}
			}

			if (isRight)
			{
				bool ck = IsPointExist(ps1.At(k), pointVec);
				if (!ck)
				{
					/*bool ck0 = IsToolInGeo(tmpGeo);
					if (!ck0)*/
					{
						pointVec.push_back(ps1.At(k));
					}
				}
			}

			/*bool ck = IsToolInGeo();
			if (!ck)
			{
				bool ck2 = IsPointExist(ps1.At(k), pointVec);
				if (!ck2)
				{
					pointVec.push_back(ps1.At(k));
				}
			}*/
		} 
	}
	int sizeP = (int)pointVec.size();
	std::vector<LineOrArc> *lar = new std::vector<LineOrArc>(0);
	if (sizeP > 1)
	{
		VLine l(pointVec.at(0), pointVec.at(1));
		lar->push_back(l);
	}
	for (int i = 2; i < sizeP; i++)
	{
		VLine l(pointVec.at(i - 1), pointVec.at(i));
		lar->push_back(l);
	}
	if (lar->size() > 1)
	{
		VLine l(pointVec.at(pointVec.size() - 1), pointVec.at(0));
		lar->push_back(l);
	} 
	m_resultLA = lar;
	m_result = new SequentialLineSet(100);
	LineOrArc::DigitiseByLengthStep(*lar, m_tol, *m_result);


}
SequentialLineSet* NFPForOpenContours::GetResult()
{
	return m_result;
}
std::vector<LineOrArc>* NFPForOpenContours::GetResultLA()
{
	return m_resultLA;
}
void NFPForOpenContours::Move(const VPoint & moveP, const VPoint & targetPos, SequentialLineSet& ls)
{
	VPoint v= targetPos - moveP;
	MoveMatrix mm(v); 
	ls.Transform(mm);  
}
int NFPForOpenContours::IsPointLeftRightOnLine(const VPoint& p, const VLine & line)
{
	int res = 0; // res == 0 on res<0 right  res>0 left.
	VLine l = line;
	double d = ((p.y() - l.GetP1().y())*(l.GetP2().x() - l.GetP1().x())) - ((p.x() - l.GetP1().x())*(l.GetP2().y() - l.GetP1().y()));
	if (fabs(d) < EPSILON)
	{
		// On
		return 2;
	}
	if (d > 0)
	{
		//left
		return 0; 
	}
	if (d < 0)
	{
		//right
		return 1;
	}

	return -1;

}
bool NFPForOpenContours::IsToolInGeo(const SequentialLineSet & ls)
{
	 
	PointSet psGeo = ls.GetPointSet();
	int size = psGeo.Size();
	for (int i = 0; i < size; i++)
	{
		VPoint p = psGeo.At(i);
		bool isPointOnLoop;
		bool choiceOnSuspicion = false;
		bool ck = LineOrArc::IsPointInLoop(m_closedTool, p , GetTol(), isPointOnLoop , choiceOnSuspicion);
		
		if (ck)
			return true;
	}
	return false;
}
bool NFPForOpenContours::IsPointExist(const VPoint& p, const std::vector<VPoint> & pointVec)
{
	int size = pointVec.size();
	for (int i = 0; i < size; i++)
	{
		bool ck = p.Compare(pointVec.at(i), 0.1);
		if (ck)
			return true;
	}

	return false;
}