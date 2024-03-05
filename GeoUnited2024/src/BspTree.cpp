// BspTree.cpp
// class depends on book ("Geometric Tools For Computer Graphics") 
// written by Dr.Eberly & Christopher...

#include "stdafx.h"
#include "BspTree.h"

#include "LineSet2d.h"

namespace geo
{
	//const vreal BspTree::DOT_TOL=1.e-18;
	BspTree::BspTree()
	:negChild(0),posChild(0),parent(0)
	{
		
	}
	BspTree::~BspTree()
	{
		delete negChild;
		delete posChild;
	}

	const double BSP_COI_TOL = 1.e-3;

	BspTree::type BspTree::Classify(const VLine2dE &L , const VPoint2d &p)
	{
		vreal d= (L.N()%p) - L.C();
		if (fabs(d) <  BSP_COI_TOL)
			return COINCIDENT;
		if(d>0.0)
			return POSITIVE;
		return NEGATIVE;
	}


	BspTree::type BspTree::Classify(const VLine2d &L,const VLine2dE &E,VLine2d &SubPos,VLine2d &SubNeg)
	{
		vreal d0=E.N()%L.GetP1() - E.C();
		vreal d1=E.N()%L.GetP2() - E.C();
		
		if (fabs(d0) <  BSP_COI_TOL)
			d0 = 0.0;
		if (fabs(d1) <  BSP_COI_TOL)
			d1 = 0.0;
		
		if (d0 == 0.0 && d1 == 0.0)
		{
			//edge is contained by line
			return COINCIDENT;
		}
		
		vreal mult=d0*d1;
		
		if (mult<0.)
		{	
			//edge crosses line
			vreal t=d0/(d0-d1);
			VPoint2d I=L.GetP(t);
			if(d1>0.0)//DOT_TOL)
			{
				SubNeg.SetP1(L.GetP1());
				SubNeg.SetP2(I);
				SubPos.SetP1(I);
				SubPos.SetP2(L.GetP2());
			}
			else
			{
				SubPos.SetP1(L.GetP1());
				SubPos.SetP2(I);
				SubNeg.SetP1(I);
				SubNeg.SetP2(L.GetP2());
			}
			return CROSSES;
		}
		else if(d0> 0.0 || d1>0.0)
		{
			//edge on positive side of line
			return POSITIVE;
		}
		else if (d0<0.0 || d1<0.0)
		{	
			//edge on negative side of line
			return NEGATIVE;
		}
		else
		{
			throw VException(( IDS_BspClaFai),1);
		}
	} 

	VLine2dE & BspTree::GetLine()
	{
		return m_coincident[0];
	}
	
	const VLine2dE & BspTree::GetLine()const
	{
		return m_coincident[0];
	}
	
	BspTree * BspTree::GetPosChild()
	{
		return posChild;
	}
	
	const BspTree * BspTree::GetPosChild()const
	{
		return posChild;
	}	
	
	BspTree * BspTree::GetNegChild()
	{
		return negChild;
	}

	const BspTree * BspTree::GetNegChild()const
	{
		return negChild;
	}

	std::vector<VLine2dE> & BspTree::GetCoincident()
	{
		return m_coincident;
	}
	
	const std::vector<VLine2dE> & BspTree::GetCoincident()const
	{
		return m_coincident;
	}

	int BspTree::PointLocation (const VPoint2d &p)const
	{
		type mytype = Classify (GetLine() , p);
		if (mytype == POSITIVE)
		{
			if (posChild)
			{
				return posChild->PointLocation(p);
			}
			else
				return 1;
		}
		else if (mytype == NEGATIVE)
		{
			if (negChild)
			{
				return negChild->PointLocation(p);
			}
			else
				return -1;
		}
		else 
		{
			//type is COINCIDENT
			int mysize = (int)(m_coincident.size());
			
			for (int i=0;i<mysize;i++)
			{
				const VLine2dE &currL = m_coincident[i];
				VVector2d v = p - currL.GetP1();
				VVector2d u =  currL.GetV();
				vreal l2 = u.GetLength2();
				vreal t = (v % u) / l2;
				if (t >= 0. && t <= 1.)
					return 0;
			}
		}
		// does not matter which subtree you use
		if (posChild)
		{
			return posChild->PointLocation (p);
		}
		else if (negChild)
		{
			return negChild->PointLocation (p);
		}
		else
		{
			//Theoretically you should not get to this block. Numerical 
			//errors might cause the block to be reached, most likely 
			//because the test point is nearly an end point of a coincident edge
			// An implementationcould throw an exception or assert in debug mode
			// ,but still return a value in Release mode.For simplicity ,let's 
			// just return 0 in hopes the test point is nearly a coincident edge end point.
			return 0;
		}
	}

	bool BspTree::IsEmptySet()const
	{
		if (m_coincident.size())
		{
			if (m_coincident.front().GetNormal()%m_coincident.front().GetNormal() < 1.e-16)
				return true;
			else
				return false;
		}
		else 
			return true;
	}
};
