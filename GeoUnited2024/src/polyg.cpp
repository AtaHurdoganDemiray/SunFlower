// Polyg.cpp

#include "stdafx.h"
#include "Polyg.h"

Polyg::Polyg (const SequentialLineSet2d &sl
			  , const VPoint2d &min
			  , const VPoint2d &max
			  , const vreal dstep)
			  :m_min (min),m_max(max)
			  ,m_dstep(dstep)
			  ,X(1/dstep)
			  ,bx(min.x())
			  ,by(min.y())
{
	xc = (int)((max.x() - bx)*X) +1;
	yc = (int)((max.y() - by)*X) +1;
	maxyo = (m_max.y() - m_min.y()) * X + 1.;
	s = new PLR* [xc];
	for (int i=0;i<xc;++i)
	{
		s[i]=new PLR [yc];
	}
	if (sl.Size() < 3)
		throw VException (( IDS_PolMusHavMinCor),1);
	VLine2d fl ( sl[0]);
	VLine2dE fle ( (fl.GetP1()-m_min)*X , (fl.GetP2()-m_min)*X );
	h = new PL (fle);
	PutToSpace (h);
	PL *prl = h;
	int mysize = sl.Size();
	for (int i=1;i<mysize;++i)
	{
		VLine2d currl ( sl[i]);
		VLine2dE le ((currl.GetP1()-m_min)*X,(currl.GetP2()-m_min)*X);
		PL *nl = new PL(le);
		prl->next = nl;
		nl->prev = prl;
		prl = nl;
		PutToSpace (nl);
	}
	prl->next = h;
	h->prev = prl;
}

Polyg::~Polyg()
{
	PL *p = h->next;
	while (p!=h)
	{
		p = p->next;
		delete p->prev;
	}
	delete h;
}

void Polyg::PutToSpace (PL *nl)
{
	int xs = (int)nl->l.GetP1().x();
	int ys = (int)nl->l.GetP1().y();
	int xe = (int)nl->l.GetP2().x();
	int ye = (int)nl->l.GetP2().y();
	if (xs>xe)
		std::swap (xs,xe);
	if (ys>ye)
		std::swap (ys,ye);
	++xe;
	++ye;
	if (xe>xc-1)
		xe = xc-1;
	if (ye>yc-1)
		ye = yc-1;
	for (int x=xs;x<=xe;++x)
	{
		for (int y=ys;y<=ye;++y)
		{
			if ( IsCellPLint (x,y,nl))
				s[x][y].Add(nl);
		}
	}
}

bool Polyg::IsCellPLint (const int x,const int y,PL *l)
{
	vreal a0,a1,b0,b1;
	VPoint2d q1 (x,y);
	VPoint2d q2 (x+1,y);
	VPoint2d q3 (x+1,y+1);	
	VPoint2d q4 (x,y+1);
	
	if ( q1.x() <= l->l.GetP1().x() && l->l.GetP1().x() <= q3.x()
		&& q1.y() <= l->l.GetP1().y() && l->l.GetP1().y() <= q3.y() )
		return true;
	if (q1.x() <= l->l.GetP2().x() && l->l.GetP2().x() <= q3.x()
		&& q1.y() <= l->l.GetP2().y() && l->l.GetP2().y() <= q3.y() )
		return true;

	VLine2dE ql1 (q1,q2);
	LineInt (ql1,l->l,a0,a1,b0,b1);
	if (a0*a1 < 0. && b0*b1 < 0.)
	{
		return true;
	}	
	VLine2dE ql2 (q2,q3);
	LineInt (ql2,l->l,a0,a1,b0,b1);
	if (a0*a1 < 0. && b0*b1 < 0.)
	{
		return true;
	}
	VLine2dE ql3 (q3,q4);
	LineInt (ql3,l->l,a0,a1,b0,b1);
	if (a0*a1 < 0. && b0*b1 < 0.)
	{
		return true;
	}
	VLine2dE ql4 (q4,q1);
	LineInt (ql4,l->l,a0,a1,b0,b1);
	if (a0*a1 < 0. && b0*b1 < 0.)
	{
		return true;
	}
	return false;
}

bool Polyg::IsPointIn (const VPoint2d &p)
{
	int icount(0);
	VPoint2d oops(p.x() , maxyo );
	VLine2dE fakel (p,oops);
	int x = (int)p.x();
	int ys = (int)p.y();
	int ye = (int)maxyo;
	if (ys > ye)
		std::swap (ys,ye);
	++ye;
	if (x<0)
		x=0;
	if (ys<0)
		ys=0;
	if (ye>yc-1)
		ye=yc-1;

	for (int y=ys;y<=ye;++y)
	{
		PLR *plr = &(s[x][y]);
		do
		{
			if (plr->r)
			{
				if (IsCrossing (fakel,plr->r->l))
				{
					++icount;
				}
			}
			plr = plr->n;
		}while (plr);
	}
	if (icount % 2 == 1)
		return true;
	else
		return false;
}

bool Polyg::IsAbsPointIn (const VPoint2d &p)
{
	int icount(0);
	VPoint2d pt ( (p-m_min)*X);
	VPoint2d oops ( pt.x() , maxyo);
	VLine2dE fakel (pt,oops);
	int x = (int)pt.x();
	int ys = (int)pt.y();
	int ye = (int)maxyo;
	if (ys>ye)
		std::swap (ys,ye);
	if (x<0)
		x=0;
	if (ys<0)
		ys=0;
	if (ye>yc-1)
		ye=yc-1;
	std::list <PL*> mylist;
	for (int y=ys;y<=ye;++y)
	{
		PLR *plr = &(s[x][y]);
		do
		{
			if (plr->r)
			{
				bool isrepeat = false;
				std::list<PL *>::iterator li = mylist.begin();
				std::list<PL *>::iterator le = mylist.end();
				for (li;li!=le;++li)
				{
					if (*li == plr->r)
					{
						isrepeat = true;
						break;
					}
				}
				if (isrepeat == false)
				{
					if (IsCrossing(fakel,plr->r->l))
					{
						++icount;
						mylist.push_back (plr->r);
					}
				}
			}
			plr = plr->n;
		}while (plr);
	}
	if (icount % 2 == 1)
		return true;
	else
		return false;
}
