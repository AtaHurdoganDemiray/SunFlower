// LinesDigitiser.cpp

#include "stdafx.h"
#include "LinesDigitiser.h"
#include "SequentialLineSet.h"
#include "LineSet.h"

LinesDigitiser::LinesDigitiser(const CoordinateSystem &cs, vreal tol,int maxArrayCount)
	:m_tol(tol), m_cs(cs), m_maxArrayCount(maxArrayCount)
{
	m_invTol = 1. / m_tol;
}
vreal LinesDigitiser::GetTol()const
{
	return m_tol;
}
const CoordinateSystem & LinesDigitiser::GetCs()const
{
	return m_cs;
}

void LinesDigitiser::DigitiseKeyX(const SequentialLineSet &sl)
{
	m_linesMin = sl.GetMinPointInCs(m_cs);
	m_linesMax = sl.GetMaxPointInCs(m_cs);
	int pointCount = (int) ((m_linesMax.x() - m_linesMin.x()) / m_tol) + 1;
	if (pointCount > m_maxArrayCount)
		throw VException("LinesDigitiser::Digitise exception , point count bigger than array capacity ",1);
	else
	{
		m_allInt = new std::list<int>[pointCount];
		m_arraySize = pointCount;
	}
	int lsize = sl.Size();
	for (int li = 0; li < lsize; ++li)
	{
		VLine l = sl.operator[](li);
		DigitiseLineForKeyX(l);
	}
	SortLists();
}
void LinesDigitiser::DigitiseKeyY(const SequentialLineSet &sl)
{
	m_linesMin = sl.GetMinPointInCs(m_cs);
	m_linesMax = sl.GetMaxPointInCs(m_cs);
	int pointCount = (int)((m_linesMax.y() - m_linesMin.y()) * m_invTol) + 1;
	if (pointCount > m_maxArrayCount)
		throw VException("LinesDigitiser::Digitise exception , point count bigger than array capacity ", 1);
	else
	{
		m_allInt = new std::list<int>[pointCount];
		m_arraySize = pointCount;
	}
	int lsize = sl.Size();
	for (int li = 0; li < lsize; ++li)
	{
		VLine l = sl.operator[](li);
		DigitiseLineForKeyY(l);
	}
	SortLists();
}
void LinesDigitiser::DigitiseKeyX(const LineSet &ls)
{
	m_linesMin = ls.GetMinPointInCs(m_cs);
	m_linesMax = ls.GetMaxPointInCs(m_cs);
	int pointCount = (int)((m_linesMax.x() - m_linesMin.x()) / m_tol) + 1;
	if (pointCount > m_maxArrayCount)
		throw VException("LinesDigitiser::Digitise exception , point count bigger than array capacity ", 1);
	else
	{
		m_allInt = new std::list<int>[pointCount];
		m_arraySize = pointCount;
	}
	int lsize = ls.Size();
	for (int li = 0; li < lsize; ++li)
	{
		const VLine *l = ls.operator[](li);
		DigitiseLineForKeyX(*l);
	}
	SortLists();
}
void LinesDigitiser::DigitiseKeyY(const LineSet &ls)
{
	m_linesMin = ls.GetMinPointInCs(m_cs);
	m_linesMax = ls.GetMaxPointInCs(m_cs);
	int pointCount = (int)((m_linesMax.y() - m_linesMin.y()) / m_tol) + 1;
	if (pointCount > m_maxArrayCount)
		throw VException("LinesDigitiser::Digitise exception , point count bigger than array capacity ", 1);
	else
	{
		m_allInt = new std::list<int>[pointCount];
		m_arraySize = pointCount;
	}
	int lsize = ls.Size();
	for (int li = 0; li < lsize; ++li)
	{
		const VLine *l = ls.operator[](li);
		DigitiseLineForKeyY(*l);
	}
	SortLists();
}

void LinesDigitiser::AddIntKeyX(const VPoint &relintp)
{
	vreal xc = relintp.x() - m_linesMin.x();
	vreal yc = relintp.y() - m_linesMin.y();
	int xci = (int)(xc * m_invTol);
	if (xci < 0 || xci >= m_arraySize)
		throw (VException("LinesDigitiser::AddIntKeyX exception , xc index out of range",1));
	int yci = (int)(yc * m_invTol);
	m_allInt[xci].push_back(yci);
}

void LinesDigitiser::AddIntKeyY(const VPoint &relintp)
{
	vreal xc = relintp.x() - m_linesMin.x();
	vreal yc = relintp.y() - m_linesMin.y();
	int yci = (int)(yc * m_invTol);
	if (yci < 0 || yci >= m_arraySize)
		throw (VException("LinesDigitiser::AddIntKeyY exception , yc index out of range", 1));
	int xci = (int)(xc * m_invTol);
	m_allInt[yci].push_back(xci);
}

bool LinesDigitiser::GetYForRelKeyX(vreal relkeyXc, std::list<vreal> &relYc, std::list<VPoint> &absP)
{
	bool ck(true);
	int myi = (int)(relkeyXc * m_invTol);
	if (myi < 0)
	{
		ck = false;
		myi = 0;
	}
	if (myi > m_arraySize - 1)
	{
		ck = false;
		myi = m_arraySize - 1;
	}
	std::list<int>::iterator li = m_allInt[myi].begin();
	std::list<int>::iterator le = m_allInt[myi].end();
	for (li; li != le; ++li)
	{
		vreal curRelYc = (vreal)(*li) * m_tol;
		relYc.push_back(curRelYc);
		VPoint relp(relkeyXc, curRelYc, 0.0);
		absP.push_back(m_cs.RelativeCoordToAbsoluteCoord(relp));
	}
	return ck;
}
bool LinesDigitiser::GetXForRelKeyY(vreal relkeyYc, std::list<vreal> &relXc, std::list<VPoint> &absP)
{
	bool ck(true);
	int myi = (int)(relkeyYc * m_invTol);
	if (myi < 0)
	{
		ck = false;
		myi = 0;
	}
	if (myi > m_arraySize - 1)
	{
		ck = false;
		myi = m_arraySize - 1;
	}
	std::list<int>::iterator li = m_allInt[myi].begin();
	std::list<int>::iterator le = m_allInt[myi].end();
	for (li; li != le; ++li)
	{
		vreal curRelXc = (vreal)(*li) * m_tol;
		relXc.push_back(curRelXc);
		VPoint relp(relkeyYc, curRelXc, 0.0);
		absP.push_back(m_cs.RelativeCoordToAbsoluteCoord(relp));
	}
	return ck;
}
bool LinesDigitiser::GetYForAbsKeyX(const VPoint &absKeyP, std::list<vreal> &relYc, std::list<VPoint> &absP)
{
	bool ck(true);
	vreal relkeyXc = m_cs.AbsoluteCoordToRelativeCoord(absKeyP).x();
	int myi = (int)(relkeyXc * m_invTol);
	if (myi < 0)
	{
		ck = false;
		myi = 0;
	}
	if (myi > m_arraySize - 1)
	{
		ck = false;
		myi = m_arraySize - 1;
	}
	std::list<int>::iterator li = m_allInt[myi].begin();
	std::list<int>::iterator le = m_allInt[myi].end();
	for (li; li != le; ++li)
	{
		vreal curRelYc = (vreal)(*li) * m_tol;
		relYc.push_back(curRelYc);
		VPoint relp(relkeyXc, curRelYc, 0.0);
		absP.push_back(m_cs.RelativeCoordToAbsoluteCoord(relp));
	}
	return ck;
}
bool LinesDigitiser::GetXForAbsKeyY(const VPoint &abskeyP, std::list<vreal> &relXc, std::list<VPoint> &absP)
{
	bool ck(true);
	vreal relkeyYc = m_cs.AbsoluteCoordToRelativeCoord(abskeyP).y();
	int myi = (int)(relkeyYc * m_invTol);
	if (myi < 0)
	{
		ck = false;
		myi = 0;
	}
	if (myi > m_arraySize - 1)
	{
		ck = false;
		myi = m_arraySize - 1;
	}
	std::list<int>::iterator li = m_allInt[myi].begin();
	std::list<int>::iterator le = m_allInt[myi].end();
	for (li; li != le; ++li)
	{
		vreal curRelXc = (vreal)(*li) * m_tol;
		relXc.push_back(curRelXc);
		VPoint relp(relkeyYc, curRelXc, 0.0);
		absP.push_back(m_cs.RelativeCoordToAbsoluteCoord(relp));
	}
	return ck;
}

void LinesDigitiser::DigitiseLineForKeyX(const VLine &l)
{
	VLine rell(m_cs.AbsoluteCoordToRelativeCoord(l.GetP1()), m_cs.AbsoluteCoordToRelativeCoord(l.GetP2()));
	if (rell.GetP1().x() > rell.GetP2().x())
		rell.Reverse();
	VPoint lvu = rell.GetV().Unit();
	if (fabs(lvu.x()) < EPSILON)
	{
		AddIntKeyX(rell.GetP1());
		AddIntKeyX(rell.GetP2());
	}
	else
	{
		VPoint lvus = lvu * (m_tol / lvu.x());
		VPoint relp = rell.GetP1();
		vreal xend = rell.GetP2().x();
		for (vreal xc = rell.GetP1().x(); xc < xend; xc += m_tol)
		{
			AddIntKeyX(relp);
			relp += lvus;
		}
		AddIntKeyX(rell.GetP2());
	}
}

void LinesDigitiser::DigitiseLineForKeyY(const VLine &l)
{
	VLine rell(m_cs.AbsoluteCoordToRelativeCoord(l.GetP1()), m_cs.AbsoluteCoordToRelativeCoord(l.GetP2()));
	if (rell.GetP1().y() > rell.GetP2().y())
		rell.Reverse();
	VPoint lvu = rell.GetV().Unit();
	if (fabs(lvu.y()) < EPSILON)
	{
		AddIntKeyY(rell.GetP1());
		AddIntKeyY(rell.GetP2());
	}
	else
	{
		VPoint lvus = lvu * (m_tol / lvu.y());
		VPoint relp = rell.GetP1();
		vreal yend = rell.GetP2().y();
		for (vreal yc = rell.GetP1().y(); yc < yend; yc += m_tol)
		{
			AddIntKeyY(relp);
			relp += lvus;
		}
		AddIntKeyY(rell.GetP2());
	}
}

void LinesDigitiser::SortLists()
{
	for (int i = 0; i < m_arraySize; ++i)
	{
		m_allInt[i].sort();
	}
}