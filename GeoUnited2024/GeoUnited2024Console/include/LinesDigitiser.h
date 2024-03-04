// LinesDigitiser.h

#ifndef __LinesDigitiser_h__
#define __LinesDigitiser_h__

#include "CoordinateSystem.h"

class SequentialLineSet;
class LineSet;

class LinesDigitiser
{
public:
	LinesDigitiser(const CoordinateSystem &cs, vreal tol , int maxArrayCount);
	vreal GetTol()const;
	const CoordinateSystem & GetCs()const;
	void DigitiseKeyX (const SequentialLineSet &sl);
	void DigitiseKeyY(const SequentialLineSet &sl);
	void DigitiseKeyX(const LineSet &ls);
	void DigitiseKeyY(const LineSet &ls);
private:
	CoordinateSystem m_cs;
	vreal m_tol;
	std::list<int> * m_allInt;
	int m_maxArrayCount;
	VPoint m_linesMin;
	VPoint m_linesMax;

	void AddIntKeyX(const VPoint &relintp);
	void AddIntKeyY(const VPoint &relintp);
	vreal m_invTol;
	int m_arraySize;
	void DigitiseLineForKeyX(const VLine &l);
	void DigitiseLineForKeyY(const VLine &l);
	void SortLists();
public:
	bool GetYForRelKeyX(vreal relkeyXc, std::list<vreal> &relYc, std::list<VPoint> &absP);
	bool GetXForRelKeyY(vreal relkeyYc, std::list<vreal> &relXc, std::list<VPoint> &absP);
	bool GetYForAbsKeyX(const VPoint &abskeyP, std::list<vreal> &relYc, std::list<VPoint> &absP);
	bool GetXForAbsKeyY(const VPoint &abskeyP, std::list<vreal> &relXc, std::list<VPoint> &absP);

};

#endif