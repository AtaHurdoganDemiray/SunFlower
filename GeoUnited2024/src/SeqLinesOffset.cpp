// SeqLinesOffset.cpp

#include "stdafx.h"
#include "SeqLinesOffset.h"
#include "SequentialLineSet2d.h"
#include "PosCubes2d.h"
#include "SweptCircleLineColl.h"
#include "VInterval.h"
#include "LineSet2d.h"
#include "LinearEquations.h"
#include "PolarCalc2d.h"
#include "VArc2d.h"
#include "ArcPointGenerator.h"

void SeqLinesOffset::Offset(const SequentialLineSet2d &sl, SequentialLineSet2d &res,const vreal offsval ,const vreal tol)
{
	bool isclosed = false;
	if (sl.End()->Compare (*sl.Start(),1.0e-7))
		isclosed = true;

	LineSet2d ls(sl);
	LineSet2d ls2 (sl.Size());

	int mysize = ls.Size();
	std::vector<VLine2d> &vec =ls.GetVec();
	
	for (int i=0;i<mysize;++i)
	{
		VLine2d &currl = vec[i];
		VPoint2d currn = currl.GetNormal();
		VPoint2d o1 = currn * offsval;
		VLine2d curro (currl.GetP1() + o1 , currl.GetP2() +o1);
		currl = curro;
	}
	vreal sgnm = offsval / fabs(offsval);
	int mdlo = mysize;
	if (isclosed == false)
		--mysize;

	for (int i=0;i<mysize;++i)
	{
		VLine2d &currl = vec[i];
		VLine2d &nextl = vec[(i+1)%mdlo];
		VPoint2d currn = currl.GetNormal() * sgnm;
		vreal dotp = currn % nextl.GetV();
		if ( dotp > EPSILON)
		{
			// lines should be trimmed after offset
			vreal t1,t2;
			const VPoint2d va = currl.GetV();
			const VPoint2d vb = nextl.GetV();
			bool ck = LinearEquations::SolveLinear2Unknown (va.x(),-vb.x(),currl.GetP1().x()-nextl.GetP1().x()
				,va.y(),-vb.y(),currl.GetP1().y() - nextl.GetP1().y(),t1,t2);
			if (ck)
			{
				if (t1<=1. && t1>=0.)
				{
					currl = VLine2d ( VLine2d (currl.GetP1(),currl.GetP(t1)) );
				}
				if (t2<=1. && t2>=0.)
				{
					nextl = VLine2d (nextl.GetP(t2),nextl.GetP2());
				}
				ls2.AddLine (currl);
			}
		}
		else if (fabs(dotp) < EPSILON)
		{// two seq lines are paralel
			ls2.AddLine (currl);
		}
		else
		{
			// arc should be added between lines
			ls2.AddLine (currl);
			VPoint2d cen (*sl.GetPoint ((i+1)%mdlo));
			vreal a1 = PolarCalc2d::GetPolarAngle (cen,currl.GetP2());
			vreal a2 = PolarCalc2d::GetPolarAngle (cen,nextl.GetP1());
			if (fabs (a2-a1) > 180.0)
				a2 = 360.0 - a2;//a1 - (360. - (a2-a1));
			VArc2d myarc(cen,fabs(offsval),a1,a2);
			SequentialLineSet2d sl2 (20);
			ArcPointGenerator::Digitise (myarc,tol,sl2);
			int mysize = sl2.Size();
			for (int w = 0;w<mysize;++w)
			{
				VLine2d cl = sl2[w];
				ls2.AddLine (cl);
			}
		}
	}
	if (isclosed && ls2.Size() && ls.Size())
	{
		(*ls2[0]) = *ls[0];
		//ls2.AddLine (ls.At(ls.Size()-1));
		if (ls2[0]->GetP1().Compare (ls2[ls2.Size()-1]->GetP2(),1.e-7) == false)//if end not equal start
			ls2.AddLine (VLine2d (ls2[ls2.Size()-1]->GetP2(),ls2[0]->GetP1()));//add a line to fill gap
	}
	
	// copy ls2 to result
	int mysize2 = ls2.Size();
	for (int i=0;i<mysize2;++i)
	{
		VLine2d *l = ls2[i];
		res.InsertPoint (l->GetP1());
	}
	if (mysize2)
		res.InsertPoint (ls2[mysize2-1]->GetP2());
}

