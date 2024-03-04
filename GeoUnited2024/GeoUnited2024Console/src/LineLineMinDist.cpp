// LineLineMinDist.cpp

#include "stdafx.h"
#include "LineLineMinDist.h"
#include "VLine.h"
#include "VPoint.h"
#include "LinePointMinDist.h"

InfiniteLinesNearestPoints LineLineMinDist::calc;

void LineLineMinDist::Calc(const VLine &currl
						   , const VLine &nextl
						   , vreal &d
						   , VLine &l)
{
	d = MAX_NUMBER_LIMIT;

	vreal t1,t2;
	InfiniteLinesNearestPoints::info ck =
	calc.Calc (currl.GetP1() , currl.GetV() , nextl.GetP1() , nextl.GetV() , t1 , t2);			
	//vreal d;
	//VLine l;
	if (ck == InfiniteLinesNearestPoints::LINESPARALLEL)
	{
		vreal s1,s2,s3,s4;
		vreal md1,md2,md3,md4;
		ProjectPointToLine (currl.GetP1() , nextl , md1 , s1);
		if (s1<=1.0 && s1>=0.0)
		{
			d = md1;
			l.SetP1(currl.GetP1());
			l.SetP2(nextl.GetP(s1));
			return;
		}
		ProjectPointToLine (currl.GetP2() , nextl , md2 , s2);
		if (s2<=1.0 && s2>=0.0)
		{
			d=md2;
			l.SetP1(currl.GetP2());
			l.SetP2(nextl.GetP(s2));
			return;
		}
		ProjectPointToLine (nextl.GetP1() , currl , md3 , s3);
		if (s3<=1.0 && s3>=0.0)
		{
			d=md3;
			l.SetP2(nextl.GetP1());
			l.SetP1(currl.GetP(s3));
			return;
		}
		ProjectPointToLine (nextl.GetP2(),currl,md4,s4);
		if (s4<=1.0 && s4>=0.0)
		{
			d=md4;
			l.SetP2(nextl.GetP2());
			l.SetP1(currl.GetP(s4));
			return;
		}
		LinePointMinDist calc2;
		VPoint tmpp;
		vreal md5 = calc2.MinDist (currl , nextl.GetP1() , tmpp);
		vreal md6 = calc2.MinDist (currl , nextl.GetP2() , tmpp);
		if (md5 < md6)
		{
			d=md5;
			l.SetP1 (tmpp);
			l.SetP2 (nextl.GetP1());
		}
		else
		{
			d=md6;
			l.SetP1(tmpp);
			l.SetP2(nextl.GetP2());
		}
	}
	else
	{
		if(t1<0.0 || t2<0.0 || t1 > 1.0 || t2>1.0)
		{
			LinePointMinDist calc3;
			vreal md7,md8,md9,md10;
			VPoint myp1,myp2,myp3,myp4;
			md7=calc3.MinDist(currl,nextl.GetP1(),myp1);
			md8 =calc3.MinDist(currl,nextl.GetP2(),myp2);
			md9=calc3.MinDist(nextl,currl.GetP1(),myp3);
			md10=calc3.MinDist(nextl,currl.GetP2(),myp4);
			if (md7<d)
			{
				d=md7;
				l.SetP2(nextl.GetP1());
				l.SetP1(myp1);
			}
			if (md8<d)
			{
				d=md8;
				l.SetP2(nextl.GetP2());
				l.SetP1(myp2);
			}
			if(md9<d)
			{
				d=md9;
				l.SetP1(currl.GetP1());
				l.SetP2(myp3);
			}
			if(md10<d)
			{
				d=md10;
				l.SetP1(currl.GetP2());
				l.SetP2(myp4);
			}
		}
		else
		{	
			VPoint p1 = currl.GetP(t1);
			VPoint p2 = nextl.GetP(t2);
			l.SetP1(p1);
			l.SetP2(p2);
			d = l.GetV().GetLength();
		}
	}
}

void LineLineMinDist::ProjectPointToLine (const VPoint &p 
										  , const VLine &l
										  , vreal &mindist
										  , vreal &t)
{	
	VVector diff = p - l.GetP1();
	VVector v = l.GetV();
	vreal vlen = v.GetLength();
	v.NormThis();
	vreal prc = diff % v;
	t = prc / vlen;
	mindist = sqrt (diff.GetLength2() - prc * prc);						
}
