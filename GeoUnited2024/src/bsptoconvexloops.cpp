// BspToConvexLoops.cpp

#include "stdafx.h"
#include "BspToConvexLoops.h"
#include "VLine2dE.h"

BspToConvexLoops::BspToConvexLoops (geo::BspTree *rbsp
									,std::list< std::vector<VLine2dE> > &convexLoops)
									:m_rbsp (rbsp)
									,m_convexLoops (convexLoops)
{
	//std::list<geo::BspTree *> startList;
	//startList.push_back (rbsp);
	//while (startList.size())
	//{
	//	geo::BspTree *r = startList.front();
	//	startList.pop_front();
	//	std::vector<VLine2dE> loop;
	//	convexLoops.push_back ( loop );
	//	convexLoops.back().insert (convexLoops.back().end(),r->GetCoincident().begin(),r->GetCoincident().end());
	//	PosToLoopNegToList (r,convexLoops.back(),startList);
	//}
	std::list<geo::BspTree *> myLeafs;
	CollectLeafs (rbsp,myLeafs);
	std::list<geo::BspTree*>::iterator i = myLeafs.begin();
	std::list<geo::BspTree *>::iterator e=myLeafs.end();
	for (i;i!=e;++i)
	{
		LeafToLoop (*i);
	}
}

void BspToConvexLoops::PosToLoopNegToList(geo::BspTree *r
										  , std::vector<VLine2dE> &loop
										  , std::list<geo::BspTree*> &startList)
{
	geo::BspTree *b=r;
	if (b->GetNegChild())
	{
		geo::BspTree *mytmp = b->GetNegChild();
		std::list<geo::BspTree*>::iterator i = startList.begin();
		std::list <geo::BspTree *>::iterator e=startList.end();
		bool isexist(false);
		for (i;i!=e;++i)
		{
			if (*i == mytmp)
			{
				isexist=true;
			}
		}
		if (!isexist)
			startList.push_back (b->GetNegChild());
	}
	while (b->GetPosChild())
	{
		b = b->GetPosChild();
		loop.insert (loop.end(),b->GetCoincident().begin(),b->GetCoincident().end());
		PosToLoopNegToList (b,loop,startList);
	};
}

void BspToConvexLoops::DivideTreePart(geo::BspTree *parent, 
									  std::list<geo::BspTree*> &purepositive, 
									  std::list<geo::BspTree*> &others, 
									  std::list<geo::BspTree*> &leafs)
{
	geo::BspTree *p = parent->GetPosChild();
	if (p)
	{
		geo::BspTree *pp = p->GetPosChild ();
		geo::BspTree *pn = p->GetNegChild();
		if (pp == 0 && pn == 0)
		{
			leafs.push_back (p);
		}
		else if (pn == 0 )//&& pp !=0)
		{
			purepositive.push_back (p);
			DivideTreePart (pp,purepositive,others,leafs);
		}
		else
		{
			others.push_back (p);
			DivideTreePart (pn,purepositive,others,leafs);
			if (pp)
				DivideTreePart (pp,purepositive,others,leafs);
		}
	}

	geo::BspTree *n = parent->GetNegChild();
	if (n)
	{
		geo::BspTree *np = n->GetPosChild();
		geo::BspTree *nn = n->GetNegChild();

		if (np == 0 && np == 0)
		{
			leafs.push_back (n);
		}
		else if (nn == 0)// && np!=0
		{
			purepositive.push_back (n);
			DivideTreePart (np,purepositive,others,leafs);
		}
		else
		{
			others.push_back (n);
			DivideTreePart (nn,purepositive,others,leafs);
			if (np)
				DivideTreePart (np,purepositive,others,leafs);
		}
	}
}

void BspToConvexLoops::CollectLeafs (geo::BspTree *b,std::list<geo::BspTree *> &allLeaf)
{
	geo::BspTree *p = b->GetPosChild();
	geo::BspTree *n = b->GetNegChild();
	if (p==0)
	{
		allLeaf.push_back (b);
	}
	else
		CollectLeafs (p,allLeaf);
	if (n)
		CollectLeafs (n,allLeaf);
}

void BspToConvexLoops::LeafToLoop(geo::BspTree *leaf)
{
	std::vector<VLine2dE> tmp;
	m_convexLoops.push_back (tmp);
	std::vector<VLine2dE> &vec = m_convexLoops.back();

	geo::BspTree *b = leaf;
	while (b->parent)
	{
		vec.push_back (b->GetLine());
		if (b->parent->GetPosChild() != b)
		{
			//vec.push_back (b->parent->GetLine());
			break;
		}
		b = b->parent;
	};
}
