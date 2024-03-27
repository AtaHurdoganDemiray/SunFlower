// SweptCircleArcColl.cpp

#include "stdafx.h"
#include "SweptCircleArcColl.h"
#include "VInterval.h"
#include "VPoint2d.h"
#include "VLine2d.h"
#include "QdrEqSolver.h"

#include "VArc2d.h"
#include "PolarCalc2d.h"

SweptCircleArcColl::SweptCircleArcColl (const vreal r , const VPoint2d &a , const VPoint2d &b)
: m_r (r)
{
	m_P0 = a;
	m_v = b - a;
}
std::vector<VInterval> SweptCircleArcColl::Coll (const VArc2d &a)
{
	std::vector<VInterval> resvec;
	std::vector<VInterval> colli = CollToFullCircle (a);
	size_t mysize = colli.size();
	if (mysize == 0)
		return resvec;	// empty set means no collision
	bool isArcFull = fabs(fabs(a.GetEndAngle()-a.GetStartAngle())-360) < EPSILON ? true : false;
	for (size_t i=0;i<mysize;++i)
	{
		const VInterval &curri = colli.at(i);
		if (isArcFull)
		{
			resvec.push_back(curri);
		}
		else
		{
			VInterval limitedi = LimitOneFullCircleIntToArc (curri , a);
			if (IsColliding (limitedi))
				resvec.push_back (limitedi);
		}
	}
	return resvec;
}

VInterval SweptCircleArcColl::LimitOneFullCircleIntToArc(const VInterval &colli , const VArc2d &a)
{
	// check if the angles are inside our arc sweep angles
	double t0 = colli.GetT0();
	double t1 = colli.GetT1();
	double dt = (t1 - t0) * 1.e-3;
	t0 += dt;
	t1 -= dt;
	bool ck0 = IsThereCollStatic (a , t0);
	bool ck1 = IsThereCollStatic (a , t1);

	if (ck0 && ck1)		// both positions are colliding
		return colli;
	double newt0(-1.0) , newt1(-1.0);
	std::vector<VInterval> spi = GetPointCollInt(a.GetStartPoint());
	std::vector<VInterval> epi = GetPointCollInt(a.GetEndPoint());

	if (!ck0)
	{
		SetSmallestT(spi,epi,colli.GetT0(),colli.GetT1(),newt0);
	}
	else
	{
		newt0 = colli.GetT0();
	}

	if (!ck1)
	{
		SetBiggestT(spi,epi,colli.GetT0(),colli.GetT1(),newt1);
	}
	else
	{
		newt1 = colli.GetT1();
	}

	if (newt0 == -1 && newt1 == -1)
	{
		VInterval nocoll (-2,-1);
		return nocoll;
	}
	if (newt0 == -1 || newt1 == -1)
	{
		//throw VException (_T("SweptCircleArcColl::Coll exception , either entry or exit root is missing!"),1);
		// let s return nocoll for now
		VInterval nocoll (-2,-1);
		return nocoll;
	}

	VInterval newcolli (newt0 , newt1);
	return newcolli;
}

void SweptCircleArcColl::SetBiggestT(std::vector<VInterval> &i1 , std::vector<VInterval> &i2,double collt0 , double collt1 , double &newt)
{
	double tbig = MIN_NUMBER_LIMIT;
	for (size_t i=0;i<i1.size();++i)
	{
		double curt = i1.at(i).GetT1();
		if (curt >= collt0 && curt <= collt1)
			if (curt > tbig)
				tbig = curt;

		curt = i1.at(i).GetT0();
		if (curt >= collt0 && curt <= collt1)
			if (curt > tbig)
				tbig = curt;
	}
	for (size_t i=0;i<i2.size();++i)
	{
		double curt = i2.at(i).GetT1();
		if (curt >= collt0 && curt <= collt1)
			if (curt > tbig)
				tbig = curt;

		curt = i2.at (i).GetT0();
		if (curt >= collt0 && curt <= collt1)
			if (curt > tbig)
				tbig = curt;
	}
	if (fabs (tbig - MIN_NUMBER_LIMIT) > EPSILON)
		newt = tbig;
}

void SweptCircleArcColl::SetSmallestT(std::vector<VInterval> &i1 , std::vector<VInterval> &i2 , double collt0 , double collt1 , double &newt)
{
	double tsmall = MAX_NUMBER_LIMIT;
	for (size_t i=0;i<i1.size();++i)
	{
		double curt = i1.at(i).GetT0();
		if (curt >= collt0 && curt <= collt1)
			if (curt < tsmall)
				tsmall = curt;
		
		curt = i1.at(i).GetT1();
		if (curt >= collt0 && curt <= collt1)
			if (curt < tsmall)
				tsmall = curt;
	}
	for (size_t i=0;i<i2.size();++i)
	{
		double curt = i2.at(i).GetT0();
		if (curt >= collt0 && curt <= collt1)
			if (curt < tsmall)
				tsmall = curt;

		curt = i2.at (i).GetT1();
		if (curt >= collt0 && curt <= collt1)
			if (curt < tsmall)
				tsmall = curt;
	}
	if (fabs (tsmall - MAX_NUMBER_LIMIT) > EPSILON)
		newt = tsmall;
}

std::vector<VInterval> SweptCircleArcColl::GetPointCollInt (VPoint2d &p)
{
	double rts[2];
	int ckt1 = GetPointInt(p,rts);
	std::vector<VInterval> res;
	if (ckt1 == 2)
	{
		VInterval tmp1 (rts[0],rts[1]);
		VInterval tmp2 (0.,1.);
		res = tmp2.Intersect(tmp1);
	}
	return res;	
}

int SweptCircleArcColl::GetPointInt(const VPoint2d &p , double roots[])
{
	VPoint2d G = m_P0 - p;
	double as = m_v % m_v;
	double bs = 2 * G % m_v;
	double cs = G % G - m_r * m_r;
	int rtno = QdrEqSolver<double>::FindRoots(as,bs,cs,roots);
	return rtno;
}

bool SweptCircleArcColl::IsThereCollStatic (const VArc2d &a , double t)
{
	double teta0,teta1,teta2;
	int ano = GetIntAngles (a.GetCenter() , a.GetRadius() , t , teta0,teta1,teta2);
	if (ano != 2)
		return false;
	//if (a.IsAngleInArc (teta0))
	//	return true;
	if (a.IsAngleInArc (teta1))
		return true;
	if (a.IsAngleInArc (teta2))
		return true;
	return false;
}

std::vector<VInterval> SweptCircleArcColl::CollToFullCircle (const VArc2d &a)
{
	const VPoint2d &c = a.GetCenter();
	m_A = m_P0.x() - c.x();
	m_B = m_P0.y() - c.y();
	m_a = m_v.x() * m_v.x() + m_v.y()*m_v.y();
	m_b = 2 * m_A * m_v.x() + 2 * m_B * m_v.y();
	m_c = m_A * m_A + m_B * m_B;
	double rtot = a.GetRadius() + m_r;
	double rdif = a.GetRadius() - m_r;
	double csmall = m_c - rdif * rdif;
	m_c -= (rtot*rtot);

	if (QdrEqSolver<double>::IsPositiveIn01(m_a,m_b,m_c))
	{
		// There is no collision!
		std::vector<VInterval> nocoll;
		return nocoll;
	}
	
	std::vector<VInterval> smallRvec;
	if (rdif > EPSILON)
		smallRvec = QdrEqSolver<double>::GetNegative (m_a,m_b,csmall);
	std::vector<VInterval> bigRvec = QdrEqSolver<double>::GetNegative (m_a,m_b,m_c);

	std::vector<VInterval> resdif = VInterval::Subtract (bigRvec , smallRvec);
	VInterval tmpi (0.0,1.0);
	std::vector<VInterval> finalres = VInterval::Intersect (resdif,tmpi);
	return finalres;

	//double roots[2];
	//int rtno = QdrEqSolver<double>::FindRoots(m_a,m_b,m_c,roots);
	//if (rtno == 0)
	//{
	//	// This is somehow unexpected , throw
	//	throw VException (_T("SweptCircleArcColl exception , always colliding"),1);
	//}
	//else if (rtno == 1)
	//{
	//	// one root means at one position the tool circle touch arc circle the others non colliding
	//	VInterval nocoll (-2.0 , -1.0);
	//	return nocoll;
	//}
	//else if  (rtno == 2)
	//{
	//	// the interval between 2 roots is the collision interval 
	//	double rt1 = roots[0] < roots[1] ? roots[0] : roots[1];
	//	double rt2 = roots[1] > roots[0] ? roots[1] : roots[0];
	//	VInterval tmp (rt1 , rt2);
	//	VInterval i01 (0.0 , 1.0);
	//	std::vector<VInterval> soli = i01.Intersect(tmp);
	//	if (soli.size() == 0)
	//	{
	//		VInterval nocoll (-2.0 , -1.0);
	//		return nocoll;
	//	}
	//	if (soli.size() == 1)
	//		return soli.front();
	//	else 
	//		throw VException (_T("SweptCircleArcColl::"),1);
	//}
	//
	//throw VException (_T("QdrEqSolver returns unexpected root count , SweptCircleArcColl::CollToFýllCircle"),1);
}

int SweptCircleArcColl::GetIntAngles (const VPoint2d &cs , double rs , double tm , double &a0 , double &a1 , double &a2)
{
	// There is a moving circle (swept circle) and a static arc 
	// cs is the center point of static arc and rs is the radius of static arc
	// tm is the parameter indicating the position of moving circle center = m_P0 + m_v.tm
	// this function gives the angle pair for the 2 points where 2 circles intersect
	// a1 and a2 are the angles for the int points measured from x axis starting from static arc's center
	// return value is 2 if two circles intersect , 1 if they are tangent and 0 if no intersection
	
	VPoint2d cm = m_P0 + m_v * tm;
	VPoint2d dvec = cm - cs;
	double L = dvec.GetLength();
	if (L > m_r + rs || L < fabs (m_r - rs))
		return 0; // There is no intersection between circles
	double x = ((m_r*m_r - rs*rs) + L*L) / (2*L); 
	// Decide the sign of x , if L < rs then x should be negative
	double y = L - x;
	if (y > rs)
		throw VException (_T( IDS_SweGetExcYRs),1);
	if (y < 0)	
	{
		dvec *= -1.;
		y = fabs (y);
	}
	a0 = PolarCalc2d::GetPolarAngle (dvec.x() , dvec.y());
	if ( fabs (fabs(y)-rs) < EPSILON)
	{
		a1 = a0;
		a2 = a0;
		return 1;
	}
	double da = acos (y/rs);
	da *= RAD_TO_DEGREE;
	a1 = a0 + da;
	a2 = a0 - da;
	return 2;
}

bool SweptCircleArcColl::IsColliding(const VInterval &ck)
{
	if (ck.GetT0() != -2 && ck.GetT1() != -1 && ck.GetLength() > EPSILON)
	{
		return true;
	}
	else
	{
		return false;
	}
}
