// SweptCircleArcColl.h

#ifndef __SweptCircleArcColl_h__
#define __SweptCircleArcColl_h__

#include "VPoint2d.h"

class VArc2d;
class VInterval;

class SweptCircleArcColl
{
public:
	SweptCircleArcColl (const vreal r , const VPoint2d &a , const VPoint2d &b);
	std::vector<VInterval> Coll (const VArc2d &a);
	std::vector<VInterval> CollToFullCircle (const VArc2d &a);
	static bool IsColliding (const VInterval &ck);
	int GetIntAngles (const VPoint2d &cs , double rs , double tm , double &a0 , double &a1 , double &a2);
	bool IsThereCollStatic (const VArc2d &a , double t);
	int GetPointInt (const VPoint2d &p , double roots[]);
	std::vector<VInterval> GetPointCollInt (VPoint2d &p);
	void SetBiggestT(std::vector<VInterval> &i1 , std::vector<VInterval> &i2,double collt0 , double collt1 , double &newt);
	void SetSmallestT (std::vector<VInterval> &i1 , std::vector<VInterval> &i2,double collt0 , double collt1 , double &newt);	
	VInterval LimitOneFullCircleIntToArc (const VInterval &colli , const VArc2d &a);
private:
	VPoint2d m_P0;
	VPoint2d m_v;
	double m_a,m_b,m_c,m_A,m_B;
	double m_r;
};

#endif