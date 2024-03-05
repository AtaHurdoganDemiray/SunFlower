// BspTreeConstructor.cpp

#include "stdafx.h"
#include "BspTreeConstructor.h"
#include "BspTree.h"
#include "LineSet2d.h"
#include "VLine2d.h"
#include "SequentialLineSet2d.h"

namespace geo
{
	BspTree * BspTreeConstructor::ConstructTree (LineSet2d &L)
	{
		BspTree * T=new BspTree;
		int size1 = L.Size();
		if (size1 == 0)
			return T;

		//use an edge to determine the splitting line for the tree node
		VLine2d *lineOfT = L[0];
		VLine2dE Tl (*lineOfT);
		T->m_coincident.push_back(Tl);
		std::list<VLine2d*> posList,negList;			
		
		//for all edges in L
		for(int i=1;i<L.Size();i++)
		{
			VLine2d &E=*(L[i]);
			// Determine how edge and line relate to each other.If the edge 
			// crosses the line, the subedges on the positive and negative
			// side of the line are returned.
			VLine2d SubPos,SubNeg;
			BspTree::type currType=T->Classify(E,Tl,SubPos,SubNeg);
			
			if(currType==BspTree::CROSSES)
			{
				// Dot(N,X)-c < 0 for one vertex and Dot(N,X)-c > 0
				// for the other vertex
				E = SubPos;
				m_divided.push_back(SubNeg);
				posList.push_back(&E);
				negList.push_back(&(m_divided.back()));
			}
			else if(currType==BspTree::POSITIVE)
			{
				// Dot(N,X) -c >= 0 for both vertices, at least one positive 
				posList.push_back(&E);
			}
			else if(currType==BspTree::NEGATIVE)
			{
				// Dot(N,X) - c <= 0 for both vertices at least one negative
				negList.push_back(&E);
			}
			else 
			{
				// type is COINCIDENT
				// Dot(N,X) - c = 0 for both vertices
				T->m_coincident.push_back(E);
			}
		} // for all edges in L
		
		if(posList.size()>0)
		{
			T->posChild = ConstructTree(posList);
			T->posChild->parent = T;
		}
		else
		{
			T->posChild =0;
		}
		
		if(negList.size()>0)
		{
			T->negChild=ConstructTree(negList);
			T->negChild->parent = T;
		}	
		else
		{
			T->negChild=0;
		}

		return T;
	}
	BspTree * BspTreeConstructor::ConstructTree (std::list<VLine2d *> &L)
	{
		BspTree * T=new BspTree;
		int size1 = (int)(L.size());
		if (size1 == 0)
			return T;

		//use an edge to determine the splitting line for the tree node
		VLine2d *lineOfT = L.front();
		VLine2dE Tl (*lineOfT);
		T->m_coincident.push_back(Tl);
		std::list<VLine2d*> posList,negList;			
		
		//for all edges in L
		std::list <VLine2d *>::iterator i = L.begin();
		++i;
		std::list <VLine2d *>::iterator e = L.end();
		for(i;i!=e;++i)
		{
			VLine2d &E= *(*i);
			// Determine how edge and line relate to each other.If the edge 
			// crosses the line, the subedges on the positive and negative
			// side of the line are returned.
			VLine2d SubPos,SubNeg;
			BspTree::type currType=T->Classify(E,Tl,SubPos,SubNeg);
			
			if(currType==BspTree::CROSSES)
			{
				// Dot(N,X)-c < 0 for one vertex and Dot(N,X)-c > 0
				// for the other vertex
				E = SubPos;
				m_divided.push_back(SubNeg);
				posList.push_back(&E);
				negList.push_back(&(m_divided.back()));
			}
			else if(currType==BspTree::POSITIVE)
			{
				// Dot(N,X) -c >= 0 for both vertices, at least one positive 
				posList.push_back(&E);
			}
			else if(currType==BspTree::NEGATIVE)
			{
				// Dot(N,X) - c <= 0 for both vertices at least one negative
				negList.push_back(&E);
			}
			else 
			{
				// type is COINCIDENT
				// Dot(N,X) - c = 0 for both vertices
				T->m_coincident.push_back(E);
			}
		} // for all edges in L
		
		if(posList.size()>0)
		{
			T->posChild=ConstructTree(posList);
			T->posChild->parent = T;
		}
		else
		{
			T->posChild=0;
		}
		
		if(negList.size()>0)
		{
			T->negChild=ConstructTree(negList);
			T->negChild->parent = T;
		}	
		else
		{
			T->negChild=0;
		}

		return T;
	}
	BspTree * BspTreeConstructor::ConstructTree (std::list<VLine2d> &L)
	{
		BspTree * T=new BspTree;
		int size1 = (int)(L.size());
		if (size1 == 0)
			return T;

		//use an edge to determine the splitting line for the tree node
		VLine2d &lineOfT = L.front();
		VLine2dE Tl (lineOfT);
		T->m_coincident.push_back(Tl);
		std::list<VLine2d*> posList,negList;			
		
		//for all edges in L
		std::list<VLine2d>::iterator i=L.begin();
		++i;
		std::list <VLine2d>::iterator e=L.end();

		for(i;i!=e;++i)
		{
			VLine2d &E= *i;
			// Determine how edge and line relate to each other.If the edge 
			// crosses the line, the subedges on the positive and negative
			// side of the line are returned.
			VLine2d SubPos,SubNeg;
			BspTree::type currType=T->Classify(E,Tl,SubPos,SubNeg);
			
			if(currType==BspTree::CROSSES)
			{
				// Dot(N,X)-c < 0 for one vertex and Dot(N,X)-c > 0
				// for the other vertex
				E = SubPos;
				m_divided.push_back(SubNeg);
				posList.push_back(&SubPos);
				negList.push_back(&(m_divided.back()));
			}
			else if(currType==BspTree::POSITIVE)
			{
				// Dot(N,X) -c >= 0 for both vertices, at least one positive 
				posList.push_back(&E);
			}
			else if(currType==BspTree::NEGATIVE)
			{
				// Dot(N,X) - c <= 0 for both vertices at least one negative
				negList.push_back(&E);
			}
			else 
			{
				// type is COINCIDENT
				// Dot(N,X) - c = 0 for both vertices
				T->m_coincident.push_back(E);
			}
		} // for all edges in L
		
		if(posList.size()>0)
		{
			T->posChild=ConstructTree(posList);
			T->posChild->parent = T;
		}
		else
		{
			T->posChild=0;
		}
		
		if(negList.size()>0)
		{
			T->negChild=ConstructTree(negList);
			T->negChild->parent = T;
		}	
		else
		{
			T->negChild=0;
		}

		return T;
	}
	BspTree * BspTreeConstructor::ConstructTree(const SequentialLineSet2d &SL)
	{
		//int lsize = SL.Size();
		//LineSet2d tmp (lsize);
		//for (int i=0;i<lsize;i++)
		//{
		//	VLine2d lc = SL.operator [](i);
		//	//if (lc.GetV().GetLength2() > 1.e-16)
		//	tmp.AddLine (lc);
		//}
		LineSet2d tmp (SL);
		geo::BspTree *result = ConstructTree (tmp);
		return result;
	}
	BspTree * BspTreeConstructor::MakeInfiniteSetBSP ()
	{
		VPoint2d p1 (0.);
		VPoint2d p2 (1.,0.);
		VLine2d l1 (p1,p2);
		VLine2d l2 (p2,p1);
		LineSet2d ls (2);
		ls.AddLine (l1);
		ls.AddLine (l2);
		BspTreeConstructor mymaker;
		BspTree * infiniteset = mymaker.ConstructTree (ls);
		return infiniteset;
	}
};
