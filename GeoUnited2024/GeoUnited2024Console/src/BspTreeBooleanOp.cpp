// BspTreeBooleanOp.cpp

#include "stdafx.h"
#include "BspTreeBooleanOp.h"
#include "BspTree.h"
#include "LineSet2d.h"
#include "PartitionLine.h"
#include "BspTreeConstructor.h"

namespace geo
{
	BspTree * BspTreeBooleanOp::Unite (const BspTree &first , const BspTree &second)
	{
		BspTree *n1 = Negate (first);
		BspTree *n2 = Negate (second);
		BspTree *res = Intersect (*n1 , *n2);
		NegateThis (*res);
		delete n1;
		delete n2;
		return res;
	}

	BspTree * BspTreeBooleanOp::Subtract (const BspTree &first , const BspTree &second)
	{
		BspTree *n = Negate (second);
		BspTree *res = Intersect (first , *n);
		delete n;
		return res;
	}
	
	geo::BspTree * BspTreeBooleanOp::Intersect (const BspTree &first , const BspTree &second)
	{
		//CheckIfEmpty(first);
		//CheckIfEmpty(second);

		LineSet2d intr (50);
		if (first.IsEmptySet()==false)
			Intersect (first , second , intr);
		if (second.IsEmptySet() == false)
			Intersect (second ,first,intr);
		BspTreeConstructor calc;
		BspTree * result = calc.ConstructTree (intr);
		CheckIfEmpty (*result);

		return result;
	}

	void BspTreeBooleanOp::Intersect (const BspTree &first , const BspTree &second , LineSet2d &intr)
	{
		const std::vector<VLine2dE> &co1 = first.GetCoincident();
		int size1 = (int)(co1.size());
		for (int i=0;i<size1;i++)
		{
			const VLine2dE &l = co1[i];
			AddIntersection (second , l.L() , intr);
		}
		if (first.GetPosChild())
		{
			Intersect (*(first.GetPosChild()) , second , intr);
		}
		if (first.GetNegChild())
		{
			Intersect (*(first.GetNegChild()) , second , intr);
		}
	}

	void BspTreeBooleanOp::AddIntersection(const BspTree &T , const VLine2d &l , LineSet2d &intr)
	{
		LineSet2d Pos(5) , Neg(5) , CoSame(5) , CoDiff(5);
		geo::GetPartition (T , l , Pos , Neg , CoSame , CoDiff);	
		int size1 = Pos.Size();
		for (int i=0;i<size1;i++)
		{
			VLine2d *cl = Pos[i];
			intr.AddLine (*cl);
		}
		int size2 = CoSame.Size();
		for (int i=0;i<size2;i++)
		{
			VLine2d *cl = CoSame[i];
			intr.AddLine (*cl);
		}
	}

	void BspTreeBooleanOp::NegateThis (BspTree &t)
	{
		CheckIfEmpty(t);
		std::vector<VLine2dE> &co = t.GetCoincident();
		int size1 = (int)(co.size());
		for (int i=0; i<size1; i++)
		{
			VLine2dE &l = co[i];
			l.Reverse();
		}
		BspTree *tmpPos = t.GetPosChild();
		BspTree *tmpNeg = t.GetNegChild();

		t.negChild = tmpPos;
		if (tmpPos)
		{
			NegateThis (*(t.negChild));
		}
		
		t.posChild = tmpNeg;
		if (tmpNeg)
		{
			NegateThis (*(t.posChild));
		}
	}

	BspTree * BspTreeBooleanOp::Negate (const BspTree &t)
	{
		//CheckIfEmpty(t);
		BspTree * result = new BspTree;
		const std::vector<VLine2dE> &co = t.GetCoincident();
		std::vector<VLine2dE> &resCo = result->GetCoincident();

		int size1 = (int)(co.size());
		for (int i=0;i<size1;i++)
		{
			const VLine2dE &l = co[i];
			VLine2dE newl (l);
			newl.Reverse ();
			resCo.push_back (newl);
		}
	
		if (t.GetPosChild())
		{
			result->negChild = Negate (*(t.GetPosChild())); 
		}
		else
		{
			result->negChild = 0;
		}

		if (t.GetNegChild())
		{
			result->posChild = Negate (*(t.GetNegChild()));
		}
		else
		{
			result->posChild = 0;
		}

		return result;
	}

	BspTree * BspTreeBooleanOp::ExclusiveOr (const BspTree &first , const BspTree &second)
	{
		BspTree *r1 = Subtract (first , second);
		BspTree *r2 = Subtract (second , first);
		BspTree *res = Unite (*r1 , *r2);
		delete r1;
		delete r2;
		return res;
	}
	void BspTreeBooleanOp::CheckIfEmpty (BspTree &t)
	{
		if (t.GetCoincident().empty())
		{
			VLine2dE temp (VLine2dE::emptyset);
			t.GetCoincident().push_back (temp);			
		}
	}
};
