// LineGroupsToPolygons.cpp

#include "stdafx.h"
#include "LineGroupsToPolygons.h"
#include "LineSet2d2SequentialLineSet2d.h"
#include "LineSet2d.h"
#include "SequentialLineSet2d.h"
#include "VLine2dE.h"
#include "PolarCalc2d.h"

bool comp_angle_index (LineGroupsToPolygons::Angle_Index &a1 , LineGroupsToPolygons::Angle_Index &a2)
{
	if (a1.a < a2.a)
		return true;
	else
		return false;
}

void LineGroupsToPolygons::Convert(const std::vector<std::vector<VLine2dE> > &linegroups
								   , std::vector<std::vector<VPoint2d> > &polygons)
{
	std::vector<std::vector<VLine2dE>>::const_iterator i = linegroups.begin();
	std::vector<std::vector<VLine2dE>>::const_iterator e = linegroups.end();
	for (i;i!=e;++i)
	{
		std::vector<VPoint2d> newpolygon;
		polygons.push_back (newpolygon);
		ConvertSinglePolygon (*i,polygons.back());
	}
}

void LineGroupsToPolygons::ConvertSinglePolygon(const std::vector<VLine2dE> &linegroup
												, std::vector<VPoint2d> &polygon)
{
	int mysize = linegroup.size();
	std::vector<Angle_Index> aivec(mysize);
	for (int i=0;i<mysize;++i)
	{
		const VLine2dE &currl = linegroup[i];
		VPoint2d v = currl.GetV();
		v.NormThis();
		Angle_Index tmp;
		tmp.a = vp_acos (v.x());
		if (v.y()<0.0)
			tmp.a = TWO_PI - tmp.a;
		tmp.i = i;
		aivec[i] = tmp;
	}
	std::sort (aivec.begin(),aivec.end(),comp_angle_index);

	double comptol = 1.0e-5;
	std::vector<Angle_Index>::iterator i = aivec.begin();
	std::vector<Angle_Index>::iterator e = aivec.end();
	const VLine2dE *prevl = & linegroup[i->i];
	polygon.push_back (prevl->GetP1());
	polygon.push_back (prevl->GetP2());
	++i;
	for (i;i!=e;++i)
	{
		const VLine2dE *currl = &linegroup[i->i];
		if (currl->GetP1().Compare (prevl->GetP2(),comptol) == false)
		{
			polygon.push_back (currl->GetP1());
		}
		polygon.push_back (currl->GetP2());
		prevl=currl;
	}
}
