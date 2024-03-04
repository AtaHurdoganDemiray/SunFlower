// SurfaceBspTrimmer.cpp

#include "stdafx.h"
#include "SurfaceBspTrimmer.h"
#include "BspTree.h"
#include "VTriangle2d.h"
#include "BspTreeBooleanOp.h"
#include "BspTreeToConvexLineGroups.h"
#include "LineGroupsToPolygons.h"
#include "VLine2dE.h"
#include "VLine2dE.h"
#include "SequentialLineSet2d.h"
#include "dSurface.h"
#include "LoopLineInt.h"

SurfaceBspTrimmer::SurfaceBspTrimmer (dSurface &surf
									  ,const SequentialLineSet2d &outerLoop
									  , const std::vector<SequentialLineSet2d *> &innerLoopVec
									  )
									  :m_outerLoop (outerLoop)
									  ,m_innerLoopVec(innerLoopVec)
									  ,m_surf (surf)
{
	MakeLineVec ();
}
SurfaceBspTrimmer::~SurfaceBspTrimmer()
{

}

std::vector<VTriangle2d> * SurfaceBspTrimmer::Trim ()
{
	std::vector<VTriangle2d> *res = new std::vector<VTriangle2d>;
	std::vector<std::vector<VLine2dE>> linegroups;
	DivideUntil (m_lineVec , linegroups);

	std::vector<std::vector<VPoint2d>> polygons;
	MakeConvexPolygons (linegroups , polygons);
	FillTriangleVec (polygons , *res);
	return res;
}

void SurfaceBspTrimmer::MakeConvexPolygons (const std::vector<std::vector<VLine2dE>> &linegroups
											,std::vector<std::vector<VPoint2d>> &polygons)
{
	LineGroupsToPolygons::Convert (linegroups,polygons);
}

void SurfaceBspTrimmer::FillTriangleVec(const std::vector<std::vector<VPoint2d> > &polygons
										, std::vector<VTriangle2d> &triVec)
{
	std::vector<std::vector<VPoint2d>>::const_iterator i = polygons.begin();
	std::vector<std::vector<VPoint2d>>::const_iterator e = polygons.end();
	for (i;i!=e;++i)
	{
		AddSinglePolygon (*i,triVec);
	}
}

void SurfaceBspTrimmer::AddSinglePolygon (const std::vector<VPoint2d> &polygon
										  , std::vector<VTriangle2d> &triVec )
{
	std::vector<VPoint2d>::const_iterator b = polygon.begin();
	std::vector<VPoint2d>::const_iterator i = b;
	++i;
	std::vector<VPoint2d>::const_iterator ip = b;
	std::vector<VPoint2d>::const_iterator e = polygon.end();

	for (i;i!=e;)
	{
		VTriangle2d newtri(*b,*ip,*i);
		// Check if line
		// if (NOLINE)	
			triVec.push_back (newtri);
		++ip;
		++i;
	}
}

void SurfaceBspTrimmer::DivideUntil (const std::vector<VLine2dE> &toDivide 
									, std::vector<std::vector<VLine2dE>> &linegroups)
{
	std::vector<VLine2dE>::const_iterator i = toDivide.begin();
	std::vector<VLine2dE>::const_iterator e = toDivide.end();
	std::vector<VLine2dE> posChildVec;
	std::vector<VLine2dE> negChildVec;
	bool isDivided;	 
	for (i;i!=e;++i)
	{
		posChildVec.clear();
		negChildVec.clear();
		
		isDivided = Divide (i , toDivide , posChildVec , negChildVec);	
		if (isDivided)
		{// add line it self to positive and negative lists
			std::vector<VPoint2d> sortedVec;
			
			// Extend line so that it gets out of all loops
			VLine2d tmpl (i->L());
			VPoint2d tmpv = tmpl.GetV();
			tmpv.NormThis();
			VLine2d tmpl2 (tmpl.GetP1() - tmpv , tmpl.GetP2() + tmpv);
			
			m_surf.AllLoopsLineInt (tmpl2 , sortedVec);
			int mysize = (int)sortedVec.size();
			for (int w=1;w<mysize;++w)
			{
				VPoint2d &p1 = sortedVec[w-1];
				VPoint2d &p2 = sortedVec[w];
				if (w%2 == 1)
				{
					//untrimmed segment
					VLine2dE toAdd(p1,p2);
					if ( toAdd.GetV() % i->GetV() < 0.0)
					{
						// reverse and add to positive
						posChildVec.push_back (toAdd.GetReversed());
						// add directly to negative
						negChildVec.push_back (toAdd);
					}
					else
					{
						// add directly to positive
						posChildVec.push_back (toAdd);
						// reverse and add to negative
						negChildVec.push_back (toAdd.GetReversed());
					}
				}
				//else trimmed segment
			}
			DivideUntil (posChildVec , linegroups);
			DivideUntil (negChildVec , linegroups);
			break;
		}
	}
	if (isDivided == false)
	{
		linegroups.push_back (toDivide);
	}
}

bool SurfaceBspTrimmer:: Divide (const std::vector<VLine2dE>::const_iterator &l 
									, const std::vector<VLine2dE> &toDivide
									, std::vector <VLine2dE> &posChildVec 
									, std::vector <VLine2dE> &negChildVec)
{
	std::vector<VLine2dE>::const_iterator i = toDivide.begin();
	std::vector<VLine2dE>::const_iterator e = toDivide.end();
	VLine2dE posChild , negChild;
	for (i;i!=e;++i)
	{
		Type ck = Classify (*l , *i , posChild , negChild);
		if (ck == CROSSING)
		{
			posChildVec.push_back (posChild);
			negChildVec.push_back (negChild);
		}
		else if (ck == POSITIVE)
		{
			posChildVec.push_back (*i);
		}
		else if (ck == NEGATIVE)
		{
			negChildVec.push_back (*i	);
		}
		// IF COINCIDENT IT IS NOT ADDED
	}
	if (posChildVec.size() && negChildVec.size())
		return true;
	else
		return false;
}

SurfaceBspTrimmer::Type SurfaceBspTrimmer::Classify(const VLine2dE &l1
												, const VLine2dE &l2
												, VLine2dE &posChild
												, VLine2dE &negChild)
{
	double c = l1.C();
	double c1 = l1.N() % l2.GetP1();
	double c2 = l1.N() % l2.GetP2();
	double d1 = c1 - c;
	double d2 = c2 - c;
	if (fabs(d1)<EPSILON)
		d1 = 0.0;
	if (fabs(d2)<EPSILON)
		d2 = 0.0;
	if (d1*d2 < 0.0)
	{
		// Find intersection point 
		double co = l2.C();
		double det = l2.N().x() * l1.N().y() - l1.N().x() * l2.N().y();
		if ( fabs(det) > EPSILON)
		{
			double px = (l2.N().y() * c - co * l1.N().y()) / (-det);			
			double py = (c * l2.N().x() - co * l1.N().x()) / det;
			VPoint2d intp (px,py);
			if (d1 > 0)
			{
				VLine2dE tmp1 (l2.GetP1(),intp);
				posChild = tmp1;
				VLine2dE tmp2 (intp,l2.GetP2());
				negChild = tmp2;
			}
			else
			{
				VLine2dE tmp1 (intp,l2.GetP2());
				posChild = tmp1;
				VLine2dE tmp2 (l2.GetP1() , intp);
				negChild = tmp2;
			}
		}
		else
		{
			throw VException (( IDS_SurClaExcDet),1);
		}
		return CROSSING;
	}
	if (d1 > 0.0 || d2 > 0.0)
		return POSITIVE;
	if (d1 < 0.0 || d2 < 0.0)
		return NEGATIVE;

	return COINCIDENT;
}

void SurfaceBspTrimmer::FillMyLineVec (const SequentialLineSet2d *sl,bool rev)
{
	int mysize = sl->GetPointSize();
	const VPoint2d *prevp = sl->GetPoint(0);
	for (int j=1;j<mysize;++j)
	{
		const VPoint2d *currp = sl->GetPoint(j);
		if (rev == false)
		{
			VLine2dE newl (*prevp,*currp);
			if (j != 1)
			{
				if (newl.N().Compare (m_lineVec.back().N() , EPSILON) == false)
					m_lineVec.push_back (newl);
				else
					m_lineVec.back().SetP2 (newl.GetP2());
			}
			else
				m_lineVec.push_back (newl);
		}
		else
		{
			VLine2dE newl (*currp , *prevp);
			if (j!=1)
			{
				if (newl.N().Compare (m_lineVec.back().N() , EPSILON) == false)
					m_lineVec.push_back (newl);
				else
					m_lineVec.back().SetP1 (newl.GetP1());
			}
			else
				m_lineVec.push_back (newl);
		}
		prevp = currp;
	}
}
void SurfaceBspTrimmer::MakeLineVec ()
{
	std::vector<SequentialLineSet2d *>::const_iterator i = m_innerLoopVec.begin();
	std::vector<SequentialLineSet2d *>::const_iterator e = m_innerLoopVec.end();
	int counter = 0;
	for (i;i!=e;++i)
	{
		bool rev = ShouldIReverseInnerLoop (*i,counter);
		FillMyLineVec (*i,rev);
		++counter;
	}
	bool rev = ShouldIReverseOuterLoop (&m_outerLoop);
	FillMyLineVec (&m_outerLoop,rev);
}

const vreal LOOP_SMALL_DIST = 0.05;

bool SurfaceBspTrimmer::ShouldIReverseOuterLoop(const SequentialLineSet2d *sl) const
{
	VLine2d l = sl->operator [] (0);
	VLine2dE tmp (l.GetP1(),l.GetP2());
	VPoint2d innerp = l.GetMidPoint () + tmp.N() * LOOP_SMALL_DIST;
	if (LoopLineInt::IsPointInLoop (innerp , sl , m_surf.cubesOuterLoop) )
		return false;
	else
		return true;
}

bool SurfaceBspTrimmer::ShouldIReverseInnerLoop(const SequentialLineSet2d *sl,int i) const
{
	VLine2d l = sl->operator [] (0);
	VLine2dE tmp (l.GetP1(),l.GetP2());
	VPoint2d innerp = l.GetMidPoint() - tmp.N() * LOOP_SMALL_DIST;
	if (LoopLineInt::IsPointInLoop (innerp , sl , m_surf.cubesInnerLoopVec[i]))
		return false;
	else
		return true;
}
