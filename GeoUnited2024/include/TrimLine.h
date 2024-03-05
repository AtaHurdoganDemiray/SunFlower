// TrimLine.h

#ifndef __TrimLine_h__
#define __TrimLine_h__

class TrimLine
{
public:
	TrimLine (const double &v , const double &u1 , const double &u2 , const double &upu1 , const double &upu2);

private:
	double m_v;
	double m_u1,m_u2;
	double m_upu1,m_upu2;
	std::vector<VPoint2d> m_points;
};