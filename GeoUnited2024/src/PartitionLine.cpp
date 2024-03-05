// PartitionLine.cpp

#include "stdafx.h"
#include "PartitionLine.h"
#include "VLine2d.h"
#include "LineSet2d.h"
#include "BspTree.h"
#include "VInterval.h"
#include "SetMerger.h"
namespace geo
{
	void GetPartition (const BspTree &T , const VLine2d &E 
					, LineSet2d &Pos , LineSet2d &Neg
					, LineSet2d &CoSame , LineSet2d &CoDiff)
	{
		VLine2d SubPos;
		VLine2d SubNeg;
		BspTree::type mytype = T.Classify (E , T.GetLine() , SubPos , SubNeg);
		if (mytype == BspTree::CROSSES)
		{
			GetPosPartition (T , SubPos , Pos , Neg , CoSame , CoDiff);
			GetNegPartition (T , SubNeg , Pos , Neg , CoSame , CoDiff);
		}
		else if (mytype == BspTree::POSITIVE)
		{
			GetPosPartition (T , E , Pos , Neg , CoSame , CoDiff);
		}
		else if (mytype == BspTree::NEGATIVE)
		{
			GetNegPartition (T , E , Pos , Neg , CoSame , CoDiff);
		}
		else 
		{
			// mytype is COINCIDENT
			// Construct segments of E intersecting coincident edges
			LineSet2d A(5);
			LineSet2d B(5);

			const std::vector<VLine2dE> &mycoincident = T.GetCoincident();
			int mysize = (int)(mycoincident.size());
			for (int i=0;i<mysize;i++)
			{
				const VLine2dE & EU = mycoincident[i];
				AddIntersecting (E , EU.L() , CoSame , CoDiff);
			}
			//int size2 = A.Size();
			//for (int j=0;j<size2;j++)
			//{
			//	VLine2d *cl = A.operator [](j);
			//	if (cl->GetV() % T.GetLine().GetV() > 0.0)//if directions are same as T.line
			//	{
			//		CoSame.AddLine (*cl);
			//	}
			//	else
			//	{
			//		CoDiff.AddLine (*cl);
			//	}
			//}
			
			SetMerger merge;
			merge.Merge (CoSame,A);
			merge.Merge (CoDiff,A);

			SubtractFromLine (E , A , B);
		
			int size3 = B.Size();
			for (int j=0;j<size3;j++)
			{
				VLine2d *cl = B[j]; 
				GetPosPartition (T , *cl , Pos , Neg , CoSame , CoDiff);
				GetNegPartition (T , *cl , Pos , Neg , CoSame , CoDiff);
			}
		}
	}

	void GetPosPartition (const BspTree &T , const VLine2d &E
							,LineSet2d &Pos , LineSet2d &Neg
							,LineSet2d &CoSame , LineSet2d &CoDiff)
	{
		if (T.GetPosChild())
		{
			GetPartition (*(T.GetPosChild()) , E , Pos , Neg , CoSame , CoDiff);
		}
		else
		{
			Pos.AddLine (E);
		}
	}

	void GetNegPartition (const BspTree &T , const VLine2d &E
						,LineSet2d &Pos , LineSet2d &Neg
						,LineSet2d &CoSame , LineSet2d &CoDiff)
	{
		if (T.GetNegChild())
		{
			GetPartition (*(T.GetNegChild()) , E , Pos , Neg , CoSame , CoDiff);
		}
		else
		{
			Neg.AddLine (E);
		}
	}

	void AddIntersecting (const VLine2d &l1 , const VLine2d &l2 , LineSet2d &CoSame , LineSet2d &CoDiff)
	{
		vreal s1,s2;
		GetParameterForCoincidentLine (l1 , l2 , s1 , s2);
		bool same = false;
		if (s2 > s1)
			same = true;

		VInterval first (0.0,1.0);
		VInterval second (s1,s2);
		std::vector <VInterval> common = first.Intersect (second);
		int mysize = (int)common.size();
		for (int i=0;i<mysize;++i)
		{
			VInterval &curr = common[i];
			std::vector<VInterval> clipped =
			curr.ClipedTo_0_1();
			if (clipped.size()==1)
			{
				VInterval &f=clipped.at(0);
				VPoint2d p1 = l1.GetP (f.GetT0());
				VPoint2d p2 = l1.GetP (f.GetT1());
				VLine2d l (p1,p2);
				if (same)
					CoSame.AddLine (l);
				else
					CoDiff.AddLine (l);
			}
		}
	}

	void SubtractFromLine (const VLine2d &l , const LineSet2d &coincidentSet , LineSet2d &diff)
	{
		std::vector<VInterval> difference;
		VInterval whl(0.0,1.0);
		VInterval emptyInterval (0.0,0.0);
		difference.push_back (whl);	
		for (int i=0; i < coincidentSet.Size(); i++)
		{
			const VLine2d *cl = coincidentSet[i];
			vreal s1,s2;
			GetParameterForCoincidentLine (l , *cl , s1 , s2);
			VInterval i1(s1 , s2);
			
			int mysize = (int)difference.size();
			for (int j=0;j<mysize;++j)
			{
				VInterval &ci = difference.at(j);
				if (ci==emptyInterval)
					continue;
				std::vector<VInterval>minorTmp = ci.Subtract(i1);
				ci = emptyInterval;
				//appending minorTmp to tmp
				int mysize2 = (int)minorTmp.size();
				for (int k=0; k<mysize2;++k)
				{
					VInterval &cci = minorTmp.at (k);
					difference.push_back(cci);
				}
			}
		}
		int mysize3 = (int)difference.size();
		for (int w=0;w<mysize3;++w)
		{
			VInterval &ci = difference.at(w);
			if (ci==emptyInterval)
				continue;
			VPoint2d p1 = l.GetP (ci.GetT0());
			VPoint2d p2 = l.GetP (ci.GetT1());
			VLine2d aline (p1,p2);
			diff.AddLine (aline);
		}
	}

	void GetParameterForCoincidentLine (const VLine2d &l1 , const VLine2d &l2 , vreal &s1, vreal &s2)
	{
		const VPoint2d &p0 = l1.GetP1();
		VVector2d v = l1.GetV();
		const VPoint2d &P1 = l2.GetP1();
		const VPoint2d &P2 = l2.GetP2();
		VVector2d v1 = P1 - p0;
		VVector2d v2 = P2 - p0;
		
		if ( fabs(v.x()) > fabs(v.y()) )
		{
			s1 = v1.x()/v.x();
			s2 = v2.x()/v.x();
		}
		else
		{
			s1 = v1.y() / v.y();
			s2 = v2.y() / v.y();
		}
	}

};
