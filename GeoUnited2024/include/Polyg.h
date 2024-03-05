// Polyg.h

#ifndef __Polyg_h__
#define __Polyg_h__
#include "VLine2dE.h"
#include "SequentialLineSet2d.h"
class LineSet2d;
class Polyg
{
public:
	struct PL
	{
	public:
		PL (VLine2dE &myl):l(myl),prev(0),next(0){};
		// The line coordinates are transformed to local space
		VLine2dE l;
		PL *prev;
		PL *next;
	};
	struct PLR
	{
	public:
		PLR ():r(0),n(0){};
		~PLR ()
		{
			if (n)
				delete n;
		}
		void Add (PL *newr)
		{
			if (r==0)
			{
				r=newr;
				return;
			}
			PLR ** nn = &n;
			if (*nn)
			{
				for (;;)	
				{
					if ((*nn)->n == 0)
						break;
					else;
						nn = & ((*nn)->n);
				}
			
				PLR * nr = new PLR;
				nr->r = newr;
				(*nn)->n = nr;
			}
			else
			{
				PLR *nr = new PLR;
				nr->r = newr;
				n = nr;
			}
		};
			
			PL *r;
			PLR *n;
	};
	enum ConInf {in,on,out};
	Polyg ( const SequentialLineSet2d &sl
			,const VPoint2d &min
			,const VPoint2d &max
			,const vreal dstep);
	~Polyg ();
	bool IsPointIn (const VPoint2d &p);
	bool IsAbsPointIn (const VPoint2d &p);
	void PartitionLine (const VLine2d &l 
						, LineSet2d &pos 
						, LineSet2d &neg
						, LineSet2d &coi);
	void PartitionLine (const VLine2dE &l
						, std::vector<VLine2dE> &pos
						, std::vector<VLine2dE> &neg
						, std::vector<VLine2dE> &coi); 
	void Negate ();
	std::vector <Polyg *> Intersect (const Polyg &loop);
	std::vector <Polyg *> Unite (const Polyg &loop);
	std::vector <Polyg *> Subtract (const Polyg &loop);
	const VPoint2d & GetMin();
	const VPoint2d & GetMax();
	const vreal & GetStep();
	SequentialLineSet2d ConvertSL ();
	inline void LineInt (const VLine2dE &A,const VLine2dE &B
						,vreal &A0,vreal &A1
						,vreal &B0,vreal &B1)
	{
		A0 = B.GetNormal() % A.GetP1() - B.C();
		A1 = B.GetNormal() % A.GetP2() - B.C();
		B0 = A.GetNormal() % B.GetP1() - A.C();
		B1 = A.GetNormal() % B.GetP2() - A.C();
	}
	inline bool IsCrossing (VLine2dE &A , VLine2dE &B)
	{
		vreal A0,A1,B0,B1;
		LineInt (A,B,A0,A1,B0,B1);
		if (A0*A1<0. && B0*B1<0.)
			return true;
		return false;
	}
private:
	
	vreal m_dstep;
	VPoint2d m_min;
	VPoint2d m_max;
	vreal maxyo;

	PL *h;
	PLR ** s;
	// coordinates of min point used for localx = (px-bx)X , localy = (py-by)X
	vreal bx,by;
	// X = 1 / m_dstep
	vreal X;
	// cell count along x and y , xc = (int) (maxx - bx)X +1  
	int xc,yc;

	void PutToSpace (PL *nl);
	bool IsCellPLint (const int x,const int y,PL *l);
};

#endif