// SurfaceQuadTrimmer.cpp

#include "stdafx.h"
#include "SurfaceQuadTrimmer.h"
#include "UntrimmedSurface.h"
#include "SequentialLineSet2d.h"
#include "VRefTriangleMesh.h"
#include "VPoint2d.h"
#include "PosCubes2d.h"
#include "LineIntersection2d.h"
#include "VDelaunay2d.h"
#include "RefTriangleSplitter.h"
#include "BspTreeConstructor.h"
#include "BspTree.h"
#include "IncDel.h"
#include "LoopLineInt.h"
#include "BspTree.h"
#include "BspTreeConstructor.h"
#include "BspTreeBooleanOp.h"
#include "TrimLoopVerifier.h"
#include "VPolygon.h"
#include "PolarCalc2d.h"
#include "stdio.h"

const int SUR_QUA_TRI_MESH_SIZE = 2000;  //900000
//VRefTriangleMesh * SurfaceQuadTrimmer::m_res = new VRefTriangleMesh (3000);
//SurfaceQuadTrimmer::Mesh SurfaceQuadTrimmer::m_res;
SurfaceQuadTrimmer::Mesh::Mesh()
{
	m_vec.resize (SUR_QUA_TRI_MESH_SIZE);
	m_rel.resize (SUR_QUA_TRI_MESH_SIZE);
	m_tri.resize (SUR_QUA_TRI_MESH_SIZE);
	m_temp.resize (SUR_QUA_TRI_MESH_SIZE);
	m_usedp = 0;
	m_usedtri = 0;
}

void SurfaceQuadTrimmer::Mesh::Clear()
{
	for (int i=0;i<m_usedp;++i)
		m_rel[i].clear();
	m_usedp = 0;
	m_usedtri = 0;
}

void SurfaceQuadTrimmer::Mesh::ExpandVector()
{
	size_t newsize = m_vec.size() * 2;
	size_t newsize2 = m_rel.size() * 2;
	size_t newsize3 = m_tri.size() * 2;
	size_t newsize4 = m_temp.size() * 2;
	m_vec.resize(newsize);
	m_rel.resize(newsize2);
	m_tri.resize(newsize3);
	m_temp.resize(newsize4);
}

int SurfaceQuadTrimmer::Mesh::AddPoint (const VPoint2d &p)
{
	if (m_usedp >= m_vec.size())
	{
		ExpandVector();	
	//	throw VException (("Max mesh size limit exceeded"),1);
	}
	m_vec[m_usedp] = p;
	return m_usedp++;
}

int SurfaceQuadTrimmer::Mesh::AddTri (const RefTriangle &t)
{
	if (m_usedtri >= m_tri.size())
	{
		ExpandVector();
		//throw VException (("Max mesh size limit exceeded for triangles"),1);
	}
	m_tri[m_usedtri] = t;
	m_rel[t.ref0].push_back (m_usedtri);
	m_rel[t.ref1].push_back (m_usedtri);
	m_rel[t.ref2].push_back (m_usedtri);
	++m_usedtri;
	return m_usedtri;
}

const std::vector<VPoint2d> & SurfaceQuadTrimmer::Mesh::Vec()const
{
	return m_vec;
}

std::vector<VPoint2d> & SurfaceQuadTrimmer::Mesh::GetVec()
{
	return m_vec;
}

VRefTriangleMesh * SurfaceQuadTrimmer::Mesh::GetRes(UntrimmedSurface &surf)
{
	// first eleminate unneeded points
	int remp(0);
	for (int i=0;i<m_usedp;++i)
	{
		if (m_rel[i].size())
		{
			m_temp[remp]=m_vec[i];
			if (remp!=i)
			{
				std::list<int>::iterator ii = m_rel[i].begin();
				std::list<int>::iterator ee = m_rel[i].end();
				for(ii;ii!=ee;++ii)
				{
					RefTriangle &rt = m_tri[*ii];
					if (rt.ref0 == i)
						rt.ref0 = remp;
					if (rt.ref1 == i)
						rt.ref1 = remp;
					if (rt.ref2 == i)
						rt.ref2 = remp;
				}
			}
			++remp;
		}
	}

	// Make res
	VRefTriangleMesh *res = new VRefTriangleMesh (remp,m_usedtri);
	VPoint m,n;
	for (int i=0;i<remp;++i)
	{
		surf.GetPointAndNormal (m_temp[i].x(),m_temp[i].y(),m,n);
		res->surfpack.InsertPoint (m_temp[i],m,n);
	}

	for (int i=0;i<m_usedtri;++i)
	{
		res->AddTriangle (m_tri[i]);
	}
	return res;
}

SurfaceQuadTrimmer::SurfaceQuadTrimmer (const std::vector<vreal> &ugrid
										,const std::vector<vreal> &vgrid
										,UntrimmedSurface &surf
										, SequentialLineSet2d *outerLoop
										, const std::vector<SequentialLineSet2d*> &innerLoopVec
										, PosCubes2d *cubesOuterLoop
										, std::vector<PosCubes2d *> &cubesInnerLoopVec
										,bool useposcubesfortrim
										,bool makeboundarycurve)
:m_ugrid(ugrid)
,m_vgrid(vgrid)
,m_surf(surf)
,m_outerLoop(outerLoop)
,m_innerLoopVec(innerLoopVec)
,m_cubesOuterLoop(cubesOuterLoop)
,m_cubesInnerLoopVec(cubesInnerLoopVec)
,m_useposcubesfortrim (useposcubesfortrim)
,m_lpointnoOffs(0.)
,m_makeboundarycurve (makeboundarycurve)
{
	if (!useposcubesfortrim)
		MakeBsp();
	
	m_uStartPar = m_ugrid.front();
	m_uEndPar = m_ugrid.back();
	m_vStartPar = m_vgrid.front();
	m_vEndPar = m_vgrid.back();
	
	m_ugrid.push_back (1.1);
	m_vgrid.push_back (1.1);

	m_uno = (int)(m_ugrid.size() -1);
	m_vno = (int)(m_vgrid.size() -1);
	m_qno = m_uno*m_vno;
	m_q = new std::list<rqp>[m_qno];
	m_qvrefmeshi = new RefQuad[m_qno];
	m_res.Clear();
	m_boundaryParanthesis = closed;
}

const vreal COIN_OFFS = 1.e-6;
const vreal HALF_COIN_OFFS = 5.e-7;

SurfaceQuadTrimmer::~SurfaceQuadTrimmer()
{
	delete [] m_q;
	delete [] m_qvrefmeshi;
	if (!m_useposcubesfortrim)
	{
		delete m_bspouter;
		int mysize=m_bspinnervec.size();
		for (int i=0;i<mysize;++i)
		{
			delete m_bspinnervec[i];
		}
	}
}

VRefTriangleMesh * SurfaceQuadTrimmer::TrimSurface()
{
	LoopQuadsInt (m_outerLoop,0);

	int mysize = (int) m_innerLoopVec.size();
	for (int i=0;i<mysize;i++)
	{
		SequentialLineSet2d *currl = m_innerLoopVec[i];
		LoopQuadsInt (currl,1+i);
	}
	AddQuadCorners();
	
	//std::vector<VPoint2d> &vec = m_res.Vec();
	int uno = m_uno-1;
	int vno = m_vno-1;
	for (int u=0;u<uno;u++)
	{
		m_boundaryParanthesis = closed;
		for (int v=0;v<vno;v++)
		{
			int myqindex = u+v*m_uno;
			int pno = m_q[myqindex].size();
			if ( pno > 0)
			{
				//Add quad corners
				//AddQDiagonalInt (u,v);
				bool mydone = SingleQTriangulate (myqindex);
				if (mydone == false)
				{
					DoItOldDelaunayWay (myqindex);
				}
				
			}
			else
			{
				//if (IsPointTrimmed ((GetQLowerLeft (u,v) + GetQUpperRight(u,v)) * 0.5) ==false)
				if (m_boundaryParanthesis == open)
				{
					RefQuad &rq = m_qvrefmeshi[myqindex];
					int t1 = AddMyTriangle (RefTriangle (rq.mi[0],rq.mi[1],rq.mi[2]));
					int t2 = AddMyTriangle (RefTriangle (rq.mi[2],rq.mi[3],rq.mi[0]));
				}
			}
		}
	}

	//m_res->surfpack.modelpoints.UpdateMinMax();
	VRefTriangleMesh * myresult = m_res.GetRes (m_surf);

	return myresult;
}

bool isboupointsmaller (const SurfaceQuadTrimmer::sortboupoints &p1 , const SurfaceQuadTrimmer::sortboupoints &p2)
{
	if (p1.par < p2.par)
		return true;
	else
		return false;
}

void SurfaceQuadTrimmer::LoopQuadsInt (const SequentialLineSet2d *loop,const int loopno)
{
	int mysize = loop->Size();
	m_lpointnoOffs = (vreal)mysize;
	int firstUno = CastUCoord (loop->GetPoint(0)->x());
	int firstVno = CastVCoord (loop->GetPoint(0)->y());
	m_loopBeginsQuadNo = firstUno + firstVno * m_uno;

	// Boundary curve
	if (m_makeboundarycurve)
	{
		std::vector<VPoint2d> vec;
		m_boundaryCurves.push_back (vec);
	}

	for (int i=0;i<mysize;i++)
	{
		VLine2d currl = loop->operator [](i);
		int uno1 = CastUCoord (currl.GetP1().x());
		int vno1 = CastVCoord (currl.GetP1().y());
		VPoint2d &p = currl.GetP1();
		VPoint m,n;
		
		VPoint2d cbl (GetQLowerLeft (uno1,vno1));
		VPoint2d cur (GetQUpperRight(uno1,vno1));
		if ( fabs (p.x()-cbl.x()) < EPSILON 
			|| fabs (p.x() - cur.x()) < EPSILON
			|| fabs (p.y() - cbl.y()) < EPSILON 
			|| fabs (p.y() - cur.y()) < EPSILON )
		{
			// the point is on quad , do not record if on a quad
			// line intersection will record it somehow
			if (i==0)
			{
				// this is first point of loop so it will no create exceptional flow in a quad
				m_lpointnoOffs = 0.;
			}
		}
		else
		{// point is in a quad record it to quad 
			int myref = m_res.AddPoint(p);// ->surfpack.InsertPoint(p,m,n);
			m_q[uno1+vno1*m_uno].push_back (rqp (loopno,(vreal)i+m_lpointnoOffs,myref,-1));
		}

		if (m_makeboundarycurve)
			m_localIntPar.clear();
		
		RecordQuadLinesInt(currl,loopno,i);
		
		if (m_makeboundarycurve)
		{
			// Adding intersections and line start to boundary curves
			std::sort (m_localIntPar.begin(),m_localIntPar.end(),isboupointsmaller);
			std::vector<sortboupoints>::iterator si = m_localIntPar.begin();
			std::vector<sortboupoints>::iterator se = m_localIntPar.end();
			m_boundaryCurves.back().push_back (p);
			for (si;si!=se;++si)
			{
				m_boundaryCurves.back().push_back (si->solidp);
			}
		}
	}
	if (m_makeboundarycurve)
		m_boundaryCurves.back().push_back (*loop->End());
}

int SurfaceQuadTrimmer::CastUCoord (const double &ucoord)
{
	vreal u = ucoord + EPSILON;
	if (u<0.)
		return 0;
	//if (u>1.)//m_uEndPar)
	//	return m_ugrid.size()-2;
	Itr j=m_ugrid.begin();
	++j;
	int counter = 0;
	Itr ue=m_ugrid.end();
	for (Itr i=m_ugrid.begin();j!=ue;++i,++j)
	{
		if (u >= *i && u < *j)
			return counter;
		++counter;
	}
	return counter-1;
}	
int SurfaceQuadTrimmer::CastVCoord (const double &vcoord)
{
	if (vcoord<0.)
		return 0;
	//if (vcoord>1.)//m_vEndPar)
	//	return m_vgrid.size() - 2;
	vreal v = vcoord + EPSILON;
	Itr j=m_vgrid.begin();
	++j;
	int counter =0;
	Itr ve = m_vgrid.end();
	for (Itr i=m_vgrid.begin();j!=ve;++i,++j)
	{
		if (v >= *i && v < *j)
			return counter;
		++counter;
	}
	return counter-1;
}
VLine2d SurfaceQuadTrimmer::GetQTop (int quno , int qvno)
{
	VLine2d res(GetQUpperRight(quno,qvno)
				,GetQUpperLeft(quno,qvno));
	return res;
}
VLine2d SurfaceQuadTrimmer::GetQBottom (int quno , int qvno)
{
	VLine2d res(GetQLowerLeft(quno,qvno) 
				,GetQLowerRight(quno,qvno));
	return res;
}
VLine2d SurfaceQuadTrimmer::GetQLeft (int quno , int qvno)
{
	VLine2d res(GetQUpperLeft(quno,qvno)
				,GetQLowerLeft(quno,qvno));
	return res;
}	
VLine2d SurfaceQuadTrimmer::GetQRight (int quno , int qvno)
{
	VLine2d res(GetQLowerRight(quno,qvno)
				,GetQUpperRight(quno,qvno));
	return res;
}
VPoint2d SurfaceQuadTrimmer::GetQUpperLeft (int quno,int qvno)
{
	VPoint2d p(m_ugrid[quno],m_vgrid[qvno+1]);
	return p;
}
VPoint2d SurfaceQuadTrimmer::GetQUpperRight (int quno,int qvno)
{
	VPoint2d p(m_ugrid[quno+1],m_vgrid[qvno+1]);
	return p;
}
VPoint2d SurfaceQuadTrimmer::GetQLowerLeft (int quno,int qvno)
{
	VPoint2d p(m_ugrid[quno],m_vgrid[qvno]);
	return p;
}
VPoint2d SurfaceQuadTrimmer::GetQLowerRight (int quno,int qvno)
{
	VPoint2d p(m_ugrid[quno+1],m_vgrid[qvno]);
	return p;
}
//bool SurfaceQuadTrimmer::RegularLineInt (const VLine2d &l1,const VLine2d &l2,VPoint2d &intp)
//{
//	bool ck = false;
//	VPoint2d l1normal = l1.GetNormal();
//	VPoint2d l2normal = l2.GetNormal();
//	vreal s1 = (l2.GetP1() - l1.GetP1()) % l1normal;
//	vreal s2 = (l2.GetP2() - l1.GetP1()) % l1normal;
//	if (s1 * s2 > 0.0)
//		return false;
//	vreal t1 = (l1.GetP1() - l2.GetP1()) % l2normal;
//	vreal t2 = (l1.GetP2() - l2.GetP1()) % l2normal;
//	if ( t1 * t2 > 0.0)
//		return false;
//
//	VPoint2d l1v = l1.GetV();
//	VPoint2d l2v = l2.GetV();
//	double div = l1v.x() * (-l2v.y()) - (l1v.y() * (-l2v.x()));
//	if (fabs(div) < EPSILON)
//		return false;
//	double tt2 = ( l1v.x() * (l2.GetP1().y() - l1.GetP1().y()) 
//					- (l1v.y() * (l2.GetP1().x() - l1.GetP1().x() ) ) ) / div;
//	if (tt2 < -EPSILON || tt2 > 1.0+EPSILON)
//		return false;
//
//	intp = l2.GetP1() + l2v * tt2;
//	return true;
//}

bool SurfaceQuadTrimmer::IsInQ (int quno,int qvno,const VPoint2d &p)
{
	VPoint2d p1 = GetQLowerLeft (quno,qvno);//(quno*m_du,qvno*m_dv);
	VPoint2d p2 = GetQUpperRight(quno,qvno);//((quno+1)*m_du,(qvno+1)*m_dv);
	if (p.x() >= p1.x() && p.y() >= p1.y()
		&& p.x() <= p2.x() && p.y() <= p2.y() )
		return true;
	else
		return false;
}

#define TEST_BSP_TRIM 0

bool SurfaceQuadTrimmer::IsPointTrimmed (const VPoint2d &p)
{
#if TEST_BSP_TRIM
	bool ispointtrimmed = false;
	if (LoopLineInt::IsPointInLoop (p , m_outerLoop , m_cubesOuterLoop ) == false)
	{
		ispointtrimmed = true;
	}
	if (!ispointtrimmed)
	{
		int mysize = (int)(m_innerLoopVec.size());
		for (int i=0;i<mysize;i++)
		{
			SequentialLineSet2d *currL = m_innerLoopVec[i];
			PosCubes2d *currC = m_cubesInnerLoopVec[i];
			if (LoopLineInt::IsPointInLoop (p , currL , currC) == true)
			{
				ispointtrimmed= true;
				break;
			}
		}
	}
#endif

	if (m_isinloopalgo == bsp)
	{
		if (m_bspouter->PointLocation(p) < 0)
		{
					#if TEST_BSP_TRIM
						if (!ispointtrimmed)
							bool conflict = true;
					#endif
			return true;
		}
		int mysize = (int)(m_bspinnervec.size());
		for (int i=0;i<mysize;i++)
		{
			if (m_bspinnervec[i]->PointLocation(p) < 0)
			{
						#if TEST_BSP_TRIM
							if (!ispointtrimmed)
								bool conflict = true;
						#endif
				return true;
			}
		}
		return false;
	}
	else 
	{
		if (LoopLineInt::IsPointInLoop (p , m_outerLoop , m_cubesOuterLoop ) == false)
		{
			return true;
		}

		int mysize = (int)(m_innerLoopVec.size());
		for (int i=0;i<mysize;i++)
		{
			SequentialLineSet2d *currL = m_innerLoopVec[i];
			PosCubes2d *currC = m_cubesInnerLoopVec[i];
			if (LoopLineInt::IsPointInLoop (p , currL , currC) == true)
			{
				return true;
			}
		}
		return false;
	}
}
bool SurfaceQuadTrimmer::IsQIndexValid (const int qi)const
{
	if (m_qno>qi && qi>-1)
		return true;
	else
		return false;
}

vreal SurfaceQuadTrimmer::GetLpointNo (const int lpointno , const vreal intp , const int qno )
{
	if (m_lpointnoOffs > 0.)
	{
		// start of a loop 
		if (m_loopBeginsQuadNo == qno)
		{
			//float resval = ((float)lpointno + m_lpointnoOffs + (float)intp + 1.0e-8f)*0.99999999f;
			vreal resval = (vreal) lpointno + m_lpointnoOffs + (vreal)intp;
			// loop goes out of quad so...
			m_lpointnoOffs = 0.;
			return resval;
		}
		else
		{
			//return ((float)lpointno + (float)intp + 1.0e-8f)*0.99999999f;
			return (vreal)lpointno + (vreal)intp;
		}
	}
	else
	{
		//return  ((float)lpointno + (float)intp + 1.0e-8f)*0.99999999f;
		return (vreal)lpointno + (vreal)intp;
	}
}
void SurfaceQuadTrimmer::AddSorted (std::list<rqp> &qlist,rqp &q)
{
	if (qlist.size())
	{
		rqp &lq = qlist.back();
		vreal diff = lq.lpointno-q.lpointno;
		if (lq.loopno== q.loopno && diff <= 1. && diff > 0.)
		{
			// q is int point of the same line with lq
			// and last point (lq) should come after current point q
			std::list<rqp>::iterator e=qlist.end();
			--e;
			qlist.insert (e,q);
		}
		else
		{
			qlist.push_back(q);
		}
	}
	else
	{
		qlist.push_back (q);
	}
}
void SurfaceQuadTrimmer::RecordQuadLinesInt(const VLine2dE &l
											,const int loopno
											,const int lpointno)
{
	const VPoint2d &p1 = l.GetP1();
	const VPoint2d &p2 = l.GetP2();
	int u1 = CastUCoord (p1.x());
	int u2 = CastUCoord (p2.x());
	int v1 = CastVCoord (p1.y());
	int v2 = CastVCoord (p2.y());
	
	VPoint2d intp;
	if ( fabs(p1.x()-p2.x()) > EPSILON) //u1 != u2)
	{
		// check if line is compeletely along a quad horizantal line
		bool dobreak (false);
		if (fabs (p1.y() - p2.y()) < EPSILON)
		{
			VPoint2d tmp1 (GetQLowerLeft (u1,v1));
			if (fabs (p1.y()-tmp1.y()) < EPSILON )//|| fabs(p1.y()- 1.)<EPSILON)
				dobreak = true;
		}
		if (!dobreak)
		{
			// at least crossing one quad vertical border.
			if (u1 > u2)
				std::swap (u1,u2);
			for (int u=u1;u<=u2;++u)
			{
				VPoint2d lb = this->GetQLowerLeft (u,0);
				VPoint2d le = this->GetQUpperLeft (u,m_vno-1);
				VLine2dE currl (lb,le);
				vreal fastck =  l.FastIntCalc (currl);
				if (fastck >= 0.0) //RegularLineInt (l,currl,intp))
				{//Lines intersects 
					intp = l.GetP (fastck);
					int myv=CastVCoord (intp.y());
					int myi=u+myv*m_uno;
					// Add point to surfpack and ref to quad
					VPoint m , n;
					int myref = m_res.AddPoint(intp);//->surfpack.InsertPoint (intp,m,n);
					AddSorted (m_q[myi], rqp (loopno,GetLpointNo(lpointno,fastck,myi),myref,1) );
					if (u!=0)
					{
						AddSorted (m_q[myi-1],rqp(loopno,GetLpointNo(lpointno,fastck,myi-1),myref,3));
					}
					
					if (m_makeboundarycurve)
					{
						// Adding intersection to boundary curves
						sortboupoints sbp;
						sbp.par = fastck;
						sbp.solidp = intp;
						m_localIntPar.push_back (sbp);
					}
				}
			}
		}
	}
	if (fabs(p1.y()-p2.y()) > EPSILON) //(v1!=v2)
	{
		bool dobreak (false);
		// ck if line is compeletely along a vertical quad line
		if (fabs(p1.x()-p2.x())< EPSILON)
		{
			VPoint2d tmp2 (GetQLowerLeft (u1,v1));
			if (fabs(p1.x() - tmp2.x()) < EPSILON  )
				dobreak = true;
		}
		if (!dobreak)
		{
			// at least crossing one quad horizantal border.
			if (v1 > v2)
				std::swap (v1,v2);
			for (int v=v1;v<=v2;++v)
			{
				VPoint2d lb = GetQLowerLeft (0,v);
				VPoint2d le = GetQLowerRight (m_uno-1,v);
				VLine2dE currl(lb,le);
				vreal fastck = l.FastIntCalc (currl);
				if (fastck >= 0.0)
				{//Lines intersect
					intp = l.GetP(fastck);
					int myu = CastUCoord (intp.x());
					int myi = myu + v*m_uno;
					VPoint m ,n;
					int myref = m_res.AddPoint(intp);//->surfpack.InsertPoint(intp,m,n);
					AddSorted (m_q[myi],rqp(loopno,GetLpointNo(lpointno,fastck,myi),myref,2));
					if (v!=0)
					{
						AddSorted (m_q[myi-m_uno],rqp(loopno,GetLpointNo(lpointno,fastck,myi-m_uno),myref,0));
					}
					if (m_makeboundarycurve)
					{
						// Adding intersection to boundary curves
						sortboupoints sbp;
						sbp.par = fastck;
						sbp.solidp = intp;
						m_localIntPar.push_back (sbp);
					}
				}
			}
		}
	}
}

void SurfaceQuadTrimmer::AddQuadCorners()
{
	int ulimit = m_uno -1;
	int vlimit = m_vno -1;
	VPoint m,n;
	for (int v=0;v<m_vno;++v)
	{
		for (int u=0;u<m_uno;++u)
		{
			int qno = u + v * m_uno;
			VPoint2d p=GetQLowerLeft(u,v);
			int myi = m_res.AddPoint(p);//->surfpack.InsertPoint(p,m,n);
			m_qvrefmeshi[qno].mi[2] = myi;
			if (IsQIndexValid (qno-1) && u!=0)
				m_qvrefmeshi[qno-1].mi[3] = myi;
			if (IsQIndexValid (qno - m_uno))
				m_qvrefmeshi[qno - m_uno].mi[1] = myi;
			if (IsQIndexValid (qno - m_uno - 1) && u!=0)
				m_qvrefmeshi[qno - m_uno - 1].mi[0] =myi;
		
			if (u==ulimit)
			{
				VPoint2d p2 = GetQLowerRight (u,v);
				VPoint m2,n2;
				int myi2 = m_res.AddPoint(p2);//->surfpack.InsertPoint (p2,m2,n2);
				m_qvrefmeshi[qno].mi[3] = myi2;
				if (IsQIndexValid (qno - m_uno))
					m_qvrefmeshi[qno-m_uno].mi[0] = myi2;
			}
			if (v==vlimit)
			{
				VPoint2d  p3 = GetQUpperLeft (u,v);
				VPoint m3 ,n3;
				int myi3 = m_res.AddPoint(p3);//->surfpack.InsertPoint(p3,m3,n3);
				m_qvrefmeshi[qno].mi[1]=myi3;
				if (IsQIndexValid (qno-1))
					m_qvrefmeshi[qno-1].mi[0] = myi3;
			}
		}
	}
	int lastq = ulimit + vlimit * m_uno;
	VPoint2d p4 = GetQUpperRight (ulimit,vlimit);
	VPoint m4 ,n4;
	int myi4 = m_res.AddPoint(p4);//->surfpack.InsertPoint (p4,m4,n4);
	m_qvrefmeshi[lastq].mi[0] = myi4;
}

void SurfaceQuadTrimmer::MakeBsp()
{
	m_bspouter=0;
	if (m_useposcubesfortrim)
	{
		m_isinloopalgo=poscubes;
		return;
	}
	m_isinloopalgo = bsp;
	if (!TrimLoopVerifier::IsInUnitSquare (*m_outerLoop,1.e-6) 
		|| !TrimLoopVerifier::IsClosed(*m_outerLoop,1.e-6))
	{
		m_isinloopalgo = poscubes;
		return;
		//throw VException (("trim loop exception"),1);
	}
	
	VPoint2d outp (2,2);
	geo::BspTreeConstructor myfactory;
	SequentialLineSet2d tempouter (m_outerLoop->GetPointSize());
	TrimLoopVerifier::RemoveDuplicatePoints (*m_outerLoop,tempouter,1.e-6);
	if (tempouter.Size() < 3)
		throw VException (( IDS_TrimLoopExc),1);
	m_bspouter = myfactory.ConstructTree (tempouter);
	if (m_bspouter->PointLocation (outp) > 0)
		geo::BspTreeBooleanOp::NegateThis (*m_bspouter);

	int mysize = (int)m_innerLoopVec.size();
	for (int i=0;i<mysize;++i)
	{
		SequentialLineSet2d * currLoop = m_innerLoopVec[i];
		if (!TrimLoopVerifier::IsInUnitSquare (*currLoop,1.e-6)
			|| !TrimLoopVerifier::IsClosed (*currLoop,1.e-6))
		{
			m_isinloopalgo=poscubes;
			return;
			//throw VException (("trim loop exception"),1);
		}
		SequentialLineSet2d tempinner (currLoop->GetPointSize());
		TrimLoopVerifier::RemoveDuplicatePoints (*currLoop,tempinner,1.e-6);
		if (tempinner.Size() < 3)
			throw VException (( IDS_TrimLoopExc),1);
		m_bspinnervec.push_back(myfactory.ConstructTree (tempinner) );
		geo::BspTree * currbsp = m_bspinnervec[i];
		if ( currbsp->PointLocation(outp) < 0)
			geo::BspTreeBooleanOp::NegateThis (*currbsp);
	}
}

int SurfaceQuadTrimmer::AddMyTriangle(const RefTriangle &rt)
{
	//VPoint tmp;
	//if (m_res->surfpack.normals[rt.ref0]->Compare(tmp,1.e-8))
	//{
	//	const VPoint2d *p1 = m_res->surfpack.parampoints[rt.ref0];	
	//	m_surf.GetPointAndNormal (p1->x(),p1->y()
	//							,*m_res->surfpack.modelpoints[rt.ref0]
	//							,*m_res->surfpack.normals[rt.ref0] );
	//}
	//if (m_res->surfpack.normals[rt.ref1]->Compare(tmp,1.e-8))
	//{
	//	const VPoint2d *p2 = m_res->surfpack.parampoints[rt.ref1];
	//	m_surf.GetPointAndNormal (p2->x(),p2->y()
	//							,*m_res->surfpack.modelpoints[rt.ref1]
	//							,*m_res->surfpack.normals[rt.ref1]);
	//}
	//if (m_res->surfpack.normals[rt.ref2]->Compare(tmp,1.e-8))
	//{
	//	const VPoint2d *p3 = m_res->surfpack.parampoints[rt.ref2];
	//	m_surf.GetPointAndNormal (p3->x(),p3->y()
	//						,*m_res->surfpack.modelpoints[rt.ref2]
	//						,*m_res->surfpack.normals[rt.ref2]);
	//}
	//m_res->AddTriangle (rt);
	//return m_res->GetRefTriangleMesh().Size()-1;
	return m_res.AddTri (rt);
}

bool rqpcomp (SurfaceQuadTrimmer::rqp &r1,SurfaceQuadTrimmer::rqp &r2)
{
	if (r1.loopno != r2.loopno)
		return r1.loopno < r2.loopno;
	else
		return r1.lpointno < r2.lpointno;
}

bool SurfaceQuadTrimmer::CacheQEdgePars (const int qno
										 ,std::vector<std::list<rqp>> &sequences
										 ,std::vector<vreal> &startQEdgeParVec
										 ,std::vector<vreal> &endQEdgeParVec)
{
	// This function calculates the quad edge parameters for start and end
	// points of every quad sequence and stores in startQEdgeParVec & endQEdgeParVec
	RefQuad &rq = m_qvrefmeshi[qno];
	const VPoint2d *myqc[4];
	myqc[0] = & m_res.Vec() [rq.mi[0]];
	myqc[1] = & m_res.Vec() [rq.mi[1]];
	myqc[2] = & m_res.Vec() [rq.mi[2]];
	myqc[3] = & m_res.Vec() [rq.mi[3]];

	int mysize = (int)sequences.size();
	for (int i=0;i<mysize;++i)
	{
		std::list<rqp> &currq = sequences[i];
		rqp &sp = currq.front();
		rqp &ep = currq.back();
		// check if going right
		if (sp.qeno == -1 || ep.qeno == -1)
			return false;
		
		// Calculating start and end parameters
		vreal startPar,endPar;
		VLine2d qes,qee;
		int i1 = sp.qeno;
		int i2 = ep.qeno;
		qes.SetP1 (*myqc[i1]);
		qes.SetP2 (*myqc[(i1+1)%4]);
		qee.SetP1 (*myqc[i2]);
		qee.SetP2 (*myqc[(i2+1)%4]);
		
		const VPoint2d *spp = & m_res.Vec()[sp.refi];
		const VPoint2d *epp = & m_res.Vec()[ep.refi];
		startPar = qes.GetPar01 (*spp)+i1;
		endPar = qee.GetPar01 (*epp)+i2;
		// Store in vec
		startQEdgeParVec.push_back (startPar);
		endQEdgeParVec.push_back (endPar);
	}

	return true;
}
int SurfaceQuadTrimmer::GetNearestSequence(const int qno
										   , const int curri
										   , const std::vector<vreal> &startQEdgeParVec
										   , const std::vector<vreal> &endQEdgeParVec
										   , int quadCornersBetween[]
										   , bool quadCornerExist[])
{
	vreal qp = endQEdgeParVec[curri];
	int nearesti = -1;
	vreal pardiff (MAX_NUMBER_LIMIT);
	int mysize = (int) startQEdgeParVec.size();
	for (int i=0;i<mysize;++i)
	{
		vreal currdif = startQEdgeParVec[i] - qp;
		if (currdif < 0.)
			currdif = 4.0+currdif;
		if (currdif < pardiff)
		{
			nearesti = i;
			pardiff =currdif;
		}
	}
	// add quad corners between 
	int si = (int)qp + 1;
	vreal way = pardiff - (si-qp);
	RefQuad &rq = m_qvrefmeshi[qno];
	int counter = 0;
	while (way > 0.)
	{
		quadCornersBetween[counter]=rq.mi[si%4];
		quadCornerExist[si%4]=true;
		++si;
		++counter;
		way-=1.;
	}
	
	return nearesti;
}
int SurfaceQuadTrimmer::GetSequence(const std::vector<bool> &usedVec)
{
	int mysize = (int)usedVec.size();
	for (int i=0;i<mysize;++i)
	{
		if (usedVec[i]==false)
			return i;
	}
	return -1;
}
void SurfaceQuadTrimmer::AddSequenceToPolygon(std::vector<std::list<rqp> > &sequences
											  , const int currs
											  , std::vector<int> &polyLoop)
{
	std::list<rqp> &l=sequences[currs];
	std::list<rqp>::iterator i=l.begin();
	std::list<rqp>::iterator e=l.end();
	for (i;i!=e;++i)
	{
		polyLoop.push_back(i->refi);
	}
}
void SurfaceQuadTrimmer::MakeSequences (const std::list<rqp> &ql
										,std::vector<std::list<rqp>> &sequences)
{
	std::list<rqp>::const_iterator i = ql.begin();
	std::list<rqp>::const_iterator pi = i;
	++i;
	std::list<rqp>::const_iterator e = ql.end();
	std::list<rqp>temp;
	sequences.push_back (temp);
	if (i!=e)
	{
		if (m_res.Vec()[i->refi].Compare(m_res.Vec()[pi->refi],1.e-10))
		{
			++pi;
			++i;
		}
	}
	sequences[0].push_back(*pi);
	
	
	for (i;i!=e;++i)
	{
		if (i->lpointno -  (int)pi->lpointno < 1.+EPSILON && i->loopno == pi->loopno)
		{
			// this is a sequence add to last list
			if (m_res.Vec()[i->refi].Compare (m_res.Vec()[pi->refi],1.e-10) == false)
			{
				// points are not coincident so record.
				sequences.back().push_back(*i);
			}
			//else
			//	sequences.back().push_back(*i);

		}
		else
		{
			if (m_res.Vec()[i->refi].Compare(m_res.Vec()[pi->refi],1.e-10) == false)
			{
				// this should be new sequence
				std::list<rqp> temp;
				sequences.push_back (temp);
				sequences.back().push_back (*i);
			}
		}
		pi = i;
	}
}
bool SurfaceQuadTrimmer::SingleQTriangulate (const int qno)
{
	//Get the refquad points and form sequences of them 
	std::vector<std::list<rqp>> sequences;
	std::list<rqp> &ql = m_q[qno];
	if (ql.size() == 1)
		return false;
	MakeSequences (ql,sequences);

	int mysize = (int)sequences.size();
	// Cache the quad edge parameters of each sequence start and end
	std::vector<vreal> startQEdgeParVec;
	std::vector<vreal> endQEdgeParVec;
	bool ck = CacheQEdgePars (qno,sequences,startQEdgeParVec,endQEdgeParVec);
	if (!ck)
	{
		sequences.clear();
		startQEdgeParVec.clear();
		endQEdgeParVec.clear();
		std::list<rqp> sortedql;
		GetSortedQListForQuad (qno,sortedql);
		MakeSequences (sortedql,sequences);
		mysize = (int)sequences.size();
		bool ck2 = CacheQEdgePars (qno,sequences,startQEdgeParVec,endQEdgeParVec);
		if (!ck2)
			return false;// some sequence start or end is not on quad edge
	}

	//Match the sequences until they form a closed loop
	// polygon loop is formed by finding the nearest parametered sequence to end parameter of previous sequence
	// until nearest sequence is start sequence
	std::vector<bool> usedVec(mysize,false);
	int starti = GetSequence (usedVec);
	if (starti!=-1)
		usedVec[starti] = true;
	int endi=starti;
	std::vector<int> poly;
	int counter = 0;
	bool quadCornerExist[4]={false,false,false,false};
	while (starti != -1)
	{
		if (counter>mysize)
			return false;
		AddSequenceToPolygon (sequences,endi,poly);
		int quadCornersBetween[4] = {-1,-1,-1,-1};
		endi = GetNearestSequence (qno,endi,startQEdgeParVec,endQEdgeParVec,quadCornersBetween,quadCornerExist);//gets sequence that start with nearest quad edge parameter to latest sequence end
		++counter;
		usedVec[endi]=true;
		// add the quad corners between previous and new sequence
		for (int w=0;w<4;++w)
		{
			if (quadCornersBetween[w] != -1)
				poly.push_back (quadCornersBetween[w]);
			else
				break;
		}

		if (endi == starti)
		{
			// loop is closed 
			TriangulateQuadLoop (poly);
			poly.clear();
			starti = GetSequence (usedVec);
			if (starti!=-1)
				usedVec[starti] = true;
			endi = starti;
		}
	}
	// do trimming related record for whole quads 
	if (quadCornerExist[0]  && quadCornerExist[1])
		m_boundaryParanthesis = open;
	else
		m_boundaryParanthesis = closed;

	return true;
}
void SurfaceQuadTrimmer::GetSortedQListForQuad(const int qno, std::list<rqp> &sortedl)
{
	std::vector<rqp> rv(m_q[qno].size());
	std::copy (m_q[qno].begin(),m_q[qno].end(),rv.begin());
	std::sort (rv.begin(),rv.end(),rqpcomp);
	std::vector<rqp>::iterator i=rv.begin();
	std::vector<rqp>::iterator e=rv.end();
	for(i;i!=e;++i)
	{
		sortedl.push_back(*i);
	}
}

void SurfaceQuadTrimmer::TriangulateQuadLoop (std::vector<int> &polygonloop)
{
	// this function gets ccw sorted sequences of loop points 
	// which are considered to form a ccw polygon
	// first polygon is formed
	// then it is triangulated by ear clipping
	// and triangles are improved by delaunay method
	// and finaly results are recorded to main triangle mesh
	VPolygon accwP (m_res.Vec(), polygonloop);
	// Triangulate polygon
	std::vector<VPolygon> clippedToTris;
	accwP.Triangulate (clippedToTris);
	// delaunay refinement
	std::vector<VPolygon>::iterator clippedi = clippedToTris.begin();
	std::vector<VPolygon>::iterator clippede = clippedToTris.end();
	DelRefMesh mydelmesh (m_res.Vec());
	//if (clippedToTris.size() == 0)
	//	int stp=1;
	for (clippedi;clippedi!=clippede;++clippedi)
	{
		int triref[3] = {clippedi->m_polygonPoints[0],clippedi->m_polygonPoints[1],clippedi->m_polygonPoints[2]};
		mydelmesh.Insert (triref);
	}
	std::list<DelRefMesh::tripair> swapstak;
	IncDel::BuildSwapStack (mydelmesh,swapstak);
	IncDel::SwapTrianglesTill (swapstak,mydelmesh);
	// add triangles to final mesh
	const std::vector<DelRefMesh::Tri> &tv=mydelmesh.GetTriMesh();
	std::vector<DelRefMesh::Tri>::const_iterator tvi = tv.begin();
	std::vector<DelRefMesh::Tri>::const_iterator tve = tv.end();
	for (tvi;tvi!=tve;++tvi)
	{
		RefTriangle reft (tvi->v);
		AddMyTriangle (reft);
	}
}

void SurfaceQuadTrimmer::DoItOldDelaunayWay (int myqindex)
{return;
	std::list<rqp> &pl = m_q[myqindex];
	std::list<int>mydellist;
	std::list <rqp>::iterator myi=pl.begin();
	std::list <rqp>::iterator mye =pl.end();
	for (myi;myi!=mye;++myi)
		mydellist.push_back (myi->refi);
	
	RefQuad & rq = m_qvrefmeshi[myqindex];
	mydellist.push_back (rq.mi[0]);
	mydellist.push_back (rq.mi[1]);
	mydellist.push_back (rq.mi[2]);
	mydellist.push_back (rq.mi[3]);
	RefTriangleMesh rrmesh (20);
	std::vector<VPoint2d> &vec = m_res.GetVec();
	IncDel::IncDel2d (vec,mydellist,rrmesh);
	int mysize = rrmesh.Size();
	
	for (int w=0;w<mysize;++w)
	{
		RefTriangle *curt = rrmesh[w];
		VTriangle2dE tock (vec[curt->ref0],vec[curt->ref1],vec[curt->ref2]);
		if (IsPointTrimmed (tock.GetCenterOfMass()) == false)
			AddMyTriangle (*curt);//m_res->AddTriangle (*curt);
	}
}

void SurfaceQuadTrimmer::IsLoopInsideQuad(const SequentialLineSet2d *loop
											, vreal &uToAdd, vreal &vToAdd)
{
	uToAdd = -1.;
	vToAdd = -1.;
	VPoint2d minp = loop->GetMin();
	VPoint2d maxp = loop->GetMax(); 

	IsIntervalInsideQuad (minp.x(),maxp.x(),m_ugrid,uToAdd);
	IsIntervalInsideQuad (minp.y(),maxp.y(),m_vgrid,vToAdd);
}

void SurfaceQuadTrimmer::IsIntervalInsideQuad (const vreal pmin , const vreal pmax
												, const std::vector<vreal> &grid
												, vreal &toAdd )
{
	int mysize = (int)grid.size() - 1;
	for (int i=0;i<mysize;++i)
	{
		if (pmin >= grid[i] && pmin <= grid[i+1]
				&& pmax >= grid[i] && pmax <= grid[i+1])
		{
			toAdd = (pmin + pmax) *0.5;
			break;
		}
	}	
}
