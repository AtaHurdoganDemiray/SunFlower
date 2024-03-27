#include "stdafx.h"
#include "VMakeHull.h"


VMakeHull::VMakeHull()
{
	m_tol = 1;
}


VMakeHull::~VMakeHull()
{

}
void VMakeHull::SetLineOrArc(const std::vector<std::vector<LineOrArc>> &larVec, vreal tol)
{
	m_larVec = larVec;
	m_tol = tol;
	m_lineSet.Clear();
	for (int i = 0; i < m_larVec.size(); ++i)
	{
		SequentialLineSet ls(100);
		LineOrArc::Digitise(m_larVec.at(i),tol, ls);
		for (int i = 0; i < ls.Size(); ++i)
		{
			m_lineSet.PushBack(ls[i]);
		}
	}  
}
void VMakeHull::Calc(vreal digitiseStep)
{
	CalcX(digitiseStep);
	CalcY(digitiseStep);
}
std::map<vreal, std::vector<VPoint>> VMakeHull::GetResultPointsX()
{
	return m_xMap;
}
std::map<vreal, std::vector<VPoint>> VMakeHull::GetResultPointsY()
{
	return m_yMap;
}
void VMakeHull::LineSetToRelativeCS()
{
	VPoint minP = m_lineSet.GetMin();
	CoordinateSystem relCS(minP);
	m_relCS = relCS;
	for (int i = 0; i < m_lineSet.Size(); i++)
	{
		VPoint p1 = m_lineSet.At(i).GetP1();
		VPoint p2 = m_lineSet.At(i).GetP2();
		p1 = m_relCS.AbsoluteCoordToRelativeCoord(p1);
		p2 = m_relCS.AbsoluteCoordToRelativeCoord(p2);
		m_lineSet.At(i).SetP1(p1);
		m_lineSet.At(i).SetP2(p2);
	}
}
void VMakeHull::LineSetToAbsoluteCS()
{
	for (int i=0; i < m_lineSet.Size(); i++)
	{
		
		VPoint p1 = m_lineSet.At(i).GetP1();
		VPoint p2 = m_lineSet.At(i).GetP2();
		p1 = m_absCS.RelativeCoordToAbsoluteCoord(p1);
		p2 = m_absCS.RelativeCoordToAbsoluteCoord(p2);
		m_lineSet.At(i).SetP1(p1);
		m_lineSet.At(i).SetP2(p2);
	}
}
void VMakeHull::CalcX(vreal digitiseStep)
{
	VPoint minP = m_lineSet.GetMin();
	VPoint maxP = m_lineSet.GetMax();

	vreal xlen = maxP.x() - minP.x();
	vreal index = minP.x();
	m_xMap.clear();

	do
	{
		std::vector<VPoint> vec;
		VPoint pmin(index, MAX_NUMBER_LIMIT, 0);
		VPoint pmax(index, MIN_NUMBER_LIMIT, 0);

		vec.push_back(pmax);
		vec.push_back(pmin);
		m_xMap[index] = vec;

		index += digitiseStep;
		xlen -= digitiseStep;
	} while (xlen > 0);

	std::map<vreal, std::vector<VPoint>> tmpResultMapX;
	std::map<vreal, std::vector<VPoint>>::iterator b = m_xMap.begin();
	std::map<vreal, std::vector<VPoint>>::iterator e = m_xMap.end();

	for (b; b != e; ++b)
	{
		for (int i = 0; i < m_lineSet.Size(); ++i)
		{
			VPoint min = m_lineSet.At(i).GetMin();
			VPoint max = m_lineSet.At(i).GetMax();

			if (min.x() <= b->first +digitiseStep && max.x() >= b->first-digitiseStep)
			{
				vreal deltax = m_lineSet.At(i).GetP2().x() - m_lineSet.At(i).GetP1().x();
				vreal deltay = m_lineSet.At(i).GetP2().y() - m_lineSet.At(i).GetP1().y();

				if (fabs(deltax) < EPSILON)
				{
					if (max.y() > b->second.at(0).y())
					{
						b->second[0] = max;
					}
					if (min.y() < b->second.at(1).y())
					{
						b->second[1] = min;
					}
				}
				else
				{
					vreal m = deltay / deltax;
					vreal dx = b->first - min.x();
					vreal y = m*dx;
					if (m > 0)
					{
						y += min.y();
					}
					else
					{
						y += max.y();
					}

					VPoint tmpP(b->first, y, 0);

					if (tmpP.y() > b->second.at(0).y())
					{
						b->second[0] = tmpP;
					}
					if (tmpP.y() < b->second.at(1).y())
					{
						b->second[1] = tmpP;
					}
				}

				tmpResultMapX[b->first] = b->second;
			}
		}
	}

	m_xMap = tmpResultMapX;
}

void VMakeHull::CalcY(vreal digitiseStep)
{
	VPoint minP = m_lineSet.GetMin();
	VPoint maxP = m_lineSet.GetMax();

	vreal ylen = maxP.y() - minP.y();
	vreal index = minP.x();
	m_yMap.clear();

	do
	{
		std::vector<VPoint> vec;
		VPoint pmin(index, MAX_NUMBER_LIMIT, 0);
		VPoint pmax(index, MIN_NUMBER_LIMIT, 0);

		vec.push_back(pmax);
		vec.push_back(pmin);
		m_yMap[index] = vec;

		index += digitiseStep;
		ylen -= digitiseStep;
	} while (ylen > 0);





}