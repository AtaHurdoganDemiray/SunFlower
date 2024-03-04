// BspTreeToLineSet2d.cpp

#include "stdafx.h"
#include "BspTreeToLineSet2d.h"
#include "LineSet2d.h"
#include "BspTree.h"
#include "PartitionLine.h"
#include "BspTreeConstructor.h"

namespace geo
{
	LineSet2d * BspTreeToLineSet2d::Make (const BspTree &T)
	{
		LineSet2d *result = new LineSet2d (100);
		Add (T,*result);
		return result;
	}

	void BspTreeToLineSet2d::Add (const BspTree &T , LineSet2d &s)
	{
		if (T.IsEmptySet())
			return;

		const std::vector<VLine2dE> &coin = T.GetCoincident();
		std::list<const VLine2d *>tmp;
		int mysize = (int)(coin.size());
		for (int i=0;i<mysize;i++)
		{
			const VLine2d &cl = coin[i].L();
			AddLineIf (s,tmp,cl);
		}
		if (T.GetNegChild())
			Add (*(T.GetNegChild()) , s);
		if (T.GetPosChild())
			Add (*(T.GetPosChild()) , s);
	}

	void BspTreeToLineSet2d::AddLineIf (LineSet2d &s , std::list<const VLine2d*> &tmp,const VLine2d &l)
	{
		if (l.GetV().GetLength2() < EPSILON)
			return;
		std::list<const VLine2d *>::const_iterator i = tmp.begin();
		std::list<const VLine2d *>::const_iterator e = tmp.end();
		for (i; i!=e; i++)
		{
			vreal s1,s2;
			GetParameterForCoincidentLine (l , *(*i) , s1 , s2);
			
			bool noInt = false;
			if (s1 < EPSILON && s2 < EPSILON)
				noInt = true;
			if (s1 > 1.-EPSILON && s2 > 1.-EPSILON)
				noInt = true;
			 
			if (noInt ==false)				//(*(*i) == l)
				return;//lines overlap		//line has been recorded
		}
		s.AddLine (l);
		tmp.push_back (&l);//(s[s.Size()-1]);
	}	
};
