// SurfaceQuadTrimmer2.cpp

#include "stdafx.h"
#include "SurfaceQuadTrimmer2.h"
#include "SurfaceQuadTrimmer.h"
#include "SequentialLineSet2d.h"
#include "VRefTriangleMesh.h"
#include "UntrimmedSurface.h"
#include "dSurface.h"
#include "VPolygon.h"
#include "GridIntersect.h"

SurfaceQuadTrimmer2::SurfaceQuadTrimmer2(const std::vector<vreal> &ugrid
										 , const std::vector<vreal> &vgrid
										 , UntrimmedSurface &surf
										 , const SequentialLineSet2d *outerLoop
										 , const std::vector<SequentialLineSet2d*> &innerLoopVec
										 , dSurface &dsurf)
										 :m_ugrid(ugrid)
										,m_vgrid(vgrid)
										,m_surf(surf)
										,m_outerLoop(outerLoop)
										,m_innerLoopVec(innerLoopVec)
										,m_dsurf(dsurf)
										,m_ub(ugrid.begin())
										,m_ue(ugrid.end())
										,m_vb(vgrid.begin())
										,m_ve(vgrid.end())
										,m_qcub(VPoint2d(0.,0.),VPoint2d(1.,1.))
										,m_fa(-1.0)
{
	m_uno = (int)ugrid.size();
	m_vno = (int)vgrid.size();
	m_quno = m_uno-1;
	m_qvno = m_vno-1;
	for (int v=0;v<m_qvno;++v)
	{
		for (int u=0;u<m_quno;++u)
		{
			VLine2d tmp(GetQLowerLeft(u,v),GetQUpperRight(u,v));
			m_qcub.AddItem (tmp,GetQIndex(u,v));
		}
	}
	m_qi.resize (m_quno * m_qvno);
	m_qv.resize (m_quno * m_qvno);
	m_ql.resize (m_quno * m_qvno);

}

VRefTriangleMesh * SurfaceQuadTrimmer2::Triangulate()
{
	m_res = new VRefTriangleMesh (m_uno * m_vno);
	AddQPointsIf ();
	AddLoop (m_outerLoop);
	int mysize = (int)m_innerLoopVec.size();
	for (int i=0;i<mysize;++i)
	{
		SequentialLineSet2d * currloop = m_innerLoopVec[i];
		AddLoop (currloop);
	}
	
	int qcount = m_quno*m_qvno;
	for (int i=0;i<qcount;++i)
	{
		TriangulateQuad (i);
	}

	int mysize2 = m_res->Size();
	for (int k=0; k<mysize2; ++k)
	{
		RefTriangle currrt = m_res->GetRefTri (k);
		const VPoint2d *p1 = m_res->surfpack.parampoints[currrt.ref0];
		VPoint *m1 = m_res->surfpack.modelpoints[currrt.ref0];
		const VPoint2d *p2 = m_res->surfpack.parampoints[currrt.ref1];
		VPoint *m2 = m_res->surfpack.modelpoints[currrt.ref1];
		const VPoint2d *p3 = m_res->surfpack.parampoints[currrt.ref2];
		VPoint *m3 = m_res->surfpack.modelpoints[currrt.ref2];

		*m1 = m_surf.GetPoint (p1->x(),p1->y());
		*m2 = m_surf.GetPoint (p2->x(),p2->y());
		*m3 = m_surf.GetPoint (p3->x(),p3->y());
	}
	m_res->CalculateNormals ();

	return m_res;
}

int SurfaceQuadTrimmer2::GetQIndex (const int quno , const int qvno)const
{
	return quno + m_quno*qvno;
}
void SurfaceQuadTrimmer2::GetQCoord(const int qi, int &quno, int &qvno) const
{
	qvno = (int)qi / m_quno;
	quno = qi - (qvno * m_quno);
}
int SurfaceQuadTrimmer2::CastUCoord (const vreal ucoord)const
{
	vreal u = ucoord + EPSILON;
	Itr j=m_ub;
	++j;
	int counter = 0;
	for (Itr i=m_ub;j!=m_ue;++i,++j)
	{
		if (u >= *i && u < *j)
			return counter;
		++counter;
	}
	return counter;
}

int SurfaceQuadTrimmer2::CastVCoord(const vreal vcoord) const
{
	vreal v = vcoord + EPSILON;
	Itr j=m_vb;
	++j;
	int counter =0;
	for (Itr i=m_vb;j!=m_ve;++i,++j)
	{
		if (v >= *i && v < *j)
			return counter;
		++counter;
	}
	return counter;
}

VLine2d SurfaceQuadTrimmer2::GetQLeft(const int quno, const int qvno) const
{
	VLine2d tmp(GetQUpperLeft(quno,qvno),GetQLowerLeft(quno,qvno));
	return tmp;
}

VLine2d SurfaceQuadTrimmer2::GetQRight(const int quno, const int qvno) const
{
	VLine2d tmp(GetQLowerRight(quno,qvno),GetQUpperRight(quno,qvno));
	return tmp;
}

VLine2d SurfaceQuadTrimmer2::GetQBottom(const int quno, const int qvno) const
{
	VLine2d tmp(GetQLowerLeft(quno,qvno),GetQLowerRight(quno,qvno));
	return tmp;
}

VLine2d SurfaceQuadTrimmer2::GetQTop(const int quno, const int qvno) const
{
	VLine2d tmp(GetQUpperRight(quno,qvno),GetQUpperLeft(quno,qvno));
	return tmp;
}

VPoint2d SurfaceQuadTrimmer2::GetQLowerLeft(const int quno, const int qvno) const
{
	VPoint2d tmp(m_ugrid[quno],m_vgrid[qvno]);
	return tmp;
}

VPoint2d SurfaceQuadTrimmer2::GetQLowerRight(const int quno, const int qvno)const
{
	VPoint2d tmp(m_ugrid[quno+1],m_vgrid[qvno]);
	return tmp;
}

VPoint2d SurfaceQuadTrimmer2::GetQUpperLeft(const int quno, const int qvno)const
{
	VPoint2d tmp(m_ugrid[quno],m_vgrid[qvno+1]);
	return tmp;
}

VPoint2d SurfaceQuadTrimmer2::GetQUpperRight(const int quno, const int qvno)const
{
	VPoint2d tmp(m_ugrid[quno+1],m_vgrid[qvno+1]);
	return tmp;
}

bool SurfaceQuadTrimmer2::IsInsideQ(const int quno, const int qvno, const VPoint2d &p) const
{
	VPoint2d lowleft = GetQLowerLeft (quno,qvno);
	VPoint2d upright = GetQUpperLeft (quno,qvno);
	if (p.x() >= lowleft.x() && p.y() >= lowleft.y()
		&& p.x() <= upright.x() && p.y()<=upright.y()) 
		return true;
	else
		return false;
}

void SurfaceQuadTrimmer2::AddLoop(const SequentialLineSet2d *loop)
{
	int mysize = loop->Size();
	for (int i=0;i<mysize;++i)
	{
		VLine2d l = loop->operator [](i);
		LongVec relind,nii;
		m_qcub.SetRelatedIndiceListForLine (l,relind,nii);
		int mysize2= relind.Size();
		for (int j=0;j<mysize2;++j)
		{
			int currqi = relind[j];
			int currqu , currqv;
			GetQCoord (currqi , currqu , currqv);
			VLine2d ql1 = GetQLeft (currqu,currqv);
			VLine2d ql2 = GetQTop (currqu,currqv);
			VLine2d ql3 = GetQRight (currqu,currqv);
			VLine2d ql4 = GetQBottom (currqu,currqv);

			VPoint2d intp;
			bool isint1 = SurfaceQuadTrimmer::RegularLineInt (l,ql1,intp);
			if (isint1)
			{
				int qi_second;
				//if (currqu != 0)
				//	qi_second = currqi -1;
				//else
					qi_second = -1;
				AddIntPoint (l,intp,currqu,currqv,currqi,qi_second);
			}
			bool isint2 = SurfaceQuadTrimmer::RegularLineInt (l,ql2,intp);
			if ( isint2)
			{
				int qi_second;
				//if (currqv != m_qvno-1)
				//	qi_second = currqi + m_quno;
				//else
					qi_second = -1;
				AddIntPoint (l,intp,currqu,currqv,currqi,qi_second);
			}
			bool isint3 = SurfaceQuadTrimmer::RegularLineInt (l,ql3,intp);
			if (isint3)
			{
				int qi_second;
				//if (currqu != m_quno-1)
				//	qi_second = currqi + 1;
				//else
					qi_second = -1;
				AddIntPoint (l,intp,currqu,currqv,currqi,qi_second);
			}
			bool isint4 = SurfaceQuadTrimmer::RegularLineInt (l,ql4,intp);
			if (isint4)
			{
				int qi_second;
				//if (currqv != 0)
				//	qi_second = currqi - m_quno;
				//else
					qi_second = -1;
				AddIntPoint (l,intp,currqu,currqv,currqi,qi_second);
			}

			// Intersection points added what if line start point is in quad
			if ( IsInsideQ (currqu,currqv,l.GetP1()) )
			{
				// point is in quad so add to quad points. 
				int pi = m_res->surfpack.InsertPoint (l.GetP1(),m_fa,m_fa);
				m_ql[currqi].push_back (pi);
			}

			// If last loop point and in quad add to quad 
			//if (i == mysize-1)
			//{
			//	// last loop point
			//	if ( IsInsideQ(currqu,currqv,l.GetP2()) )
			//	{
			//		int pi = m_res->surfpack.InsertPoint (l.GetP2(),m_fa,m_fa);
			//		m_qi[currqi].push_back(pi);
			//		m_qi[currqi].push_back (-1);
			//	}
			//}
		}
	}
}

void SurfaceQuadTrimmer2::AddQPointsIf ()
{
	for (int v=0;v<m_qvno;++v)
	{
		for (int u=0;u<m_quno;++u)
		{
			VPoint2d lowleft = GetQLowerLeft (u,v);
			if ( !m_dsurf.IsPointTrimmed (lowleft))
			{
				AddQVertex (lowleft,u,v);
			}
			
			if (u==m_quno-1)
			{
				VPoint2d lowright = GetQLowerRight (u,v);
				if ( !m_dsurf.IsPointTrimmed(lowright))
				{
					AddRightQVertex (lowright,u,v);
				}
			}

			if (v==m_qvno-1)
			{
				VPoint2d upleft = GetQUpperLeft (u,v);
				if (!m_dsurf.IsPointTrimmed (upleft))
				{
					AddUpQVertex (upleft,u,v);
				}
			}
			if (u==m_quno-1 && v==m_qvno-1)
			{
				VPoint2d upright = GetQUpperRight (u,v);
				if (!m_dsurf.IsPointTrimmed(upright))
				{
					AddUpRightQVertex (upright,u,v);
				}
			}
		}
	}
}

void SurfaceQuadTrimmer2::AddQVertex(const VPoint2d &p, const int u, const int v)
{
	int pi = m_res->surfpack.InsertPoint(p,m_fa,m_fa);	
	int i=GetQIndex (u,v);
	m_qv[i].push_back (pi);

	if (v!=0)
	{
		m_qv[i-m_quno].push_back(pi);
	}
	if (u!=0)
	{
		m_qv[i-1].push_back(pi);
	}
	if (u!=0 && v!=0)
	{
		m_qv[i-(m_quno+1)].push_back (pi);
	}
}

void SurfaceQuadTrimmer2::AddRightQVertex (const VPoint2d &p,const int u,const int v)
{
	int pi = m_res->surfpack.InsertPoint(p,m_fa,m_fa);
	int i=GetQIndex (u,v);
	m_qv[i].push_back(pi);

	if (v!=0)
	{
		m_qv[i-m_quno].push_back (pi);
	}
}

void SurfaceQuadTrimmer2::AddUpQVertex (const VPoint2d &p,const int u,const int v)
{
	int pi = m_res->surfpack.InsertPoint (p,m_fa,m_fa);
	int i=GetQIndex (u,v);
	m_qv[i].push_back (pi);

	if (u!=0)
	{
		m_qv[i-1].push_back (pi);
	}
}

void SurfaceQuadTrimmer2::AddUpRightQVertex (const VPoint2d &p,const int u,const int v)
{
	int pi=m_res->surfpack.InsertPoint (p,m_fa,m_fa);
	int i=GetQIndex (u,v);
	m_qv[i].push_back(pi);
}

bool SurfaceQuadTrimmer2::IsExiting(const VLine2d &l, const VPoint2d &intp, const int qu, const int qv)const
{
	VPoint2d aft = intp + l.GetV().Unit() * 1.e-6;
	if (IsInsideQ(qu,qv,aft))
		return false;
	else
		return true;

}

void SurfaceQuadTrimmer2::AddIntPoint ( const VLine2d &l
									   , const VPoint2d &intp
									   , const int currqu
									   , const int currqv
									   , const int currqi
									   , const int qi_second)
{
	int pi = m_res->surfpack.InsertPoint (intp,m_fa,m_fa);
	m_qi[currqi].push_back (pi);
	if (qi_second != -1)
		m_qi[qi_second].push_back (pi);

	if ( IsExiting (l,intp,currqu,currqv))
	{
		m_qi[currqi].push_back (-1);
	}
	else 
	{
		if (qi_second != -1)
		{
			m_qi[qi_second].push_back(-2);
		}
	}
}

bool mysortfunc (SurfaceQuadTrimmer2::SortStruct &s1 , SurfaceQuadTrimmer2::SortStruct &s2)
{
	if (s1.m_teta < s2.m_teta)
		return true;	
	else
		return false;
}

//void SurfaceQuadTrimmer2::TriangulateQuad(const int qi)
//{
//
//	int qu,qv;
//	GetQCoord (qi,qu,qv);
//	VPoint2d qmid = GetQMid (qu,qv);
//	std::vector<SortStruct> tosrt;
//	std::vector<int>::const_iterator i = m_qi[qi].begin();
//	std::vector<int>::const_iterator e = m_qi[qi].end();
//	SortStruct toadd;
//	
//	// add intersections
//	for (i;i!=e;++i)
//	{
//		if (toadd.m_rvec.size() == 0)
//		{
//			toadd.SetTeta (*m_res->surfpack.parampoints[*i],qmid);
//		}
//		if (*i == -1)
//		{
//			tosrt.push_back (toadd);
//			toadd.m_rvec.clear();
//		}
//		else
//			toadd.m_rvec.push_back (*i);
//	}
//
//	i = m_qv[qi].begin();
//	e = m_qv[qi].end();
//
//	// add qcorners
//	for (i;i!=e;++i)
//	{
//		toadd.m_rvec.clear();		
//		toadd.SetTeta (*m_res->surfpack.parampoints[*i],qmid);
//		toadd.m_rvec.push_back (*i);
//		tosrt.push_back(toadd);
//	}
//	
//	std::sort (tosrt.begin(),tosrt.end(),mysortfunc);
//
//	std::vector <int> polygonPoints;
//	int mysize = (int)tosrt.size();
//	for (int w=0;w<mysize;++w)
//	{
//		SortStruct &st = tosrt[w];
//		int mysize2 = (int)st.m_rvec.size();
//		for (int k = 0; k<mysize2; ++k)
//		{
//			polygonPoints.push_back (st.m_rvec[k]);
//		}
//	}
//	VPolygon tt (m_res->surfpack.parampoints.GetVec(),polygonPoints);
//	std::vector<VPolygon> qtri;
//	tt.Triangulate (qtri);
//	// add to result refmesh
//	std::vector<VPolygon>::iterator ii = qtri.begin();
//	std::vector<VPolygon>::iterator ee = qtri.end();
//	for (ii;ii!=ee;++ii)
//	{
//		RefTriangle rt (ii->m_polygonPoints[0]
//						,ii->m_polygonPoints[1]
//						,ii->m_polygonPoints[2]);
//		m_res->AddTriangle (rt);
//	}
//}

void SurfaceQuadTrimmer2::TriangulateQuad(const int qi)
{
	std::vector<VPoint2d> &pointPool = m_res->surfpack.parampoints.GetVec();
	std::vector<int>ref;
	
	if (m_qv[qi].size() != 4)
	{
		if (m_qi[qi].size() == 0 )
			return;
		GridIntersect gi;
		gi.SortedPolygon (m_qi[qi],m_ql[qi],m_qv[qi],pointPool,ref);
		VPolygon pol (pointPool,ref);
		std::vector<VPolygon> qtri;
		pol.Triangulate (qtri);
		// add to result refmesh
		std::vector<VPolygon>::iterator ii = qtri.begin();
		std::vector<VPolygon>::iterator ee = qtri.end();
		for (ii;ii!=ee;++ii)
		{
			RefTriangle rt (ii->m_polygonPoints[0]
							,ii->m_polygonPoints[1]
							,ii->m_polygonPoints[2]);
			m_res->AddTriangle (rt);
		}
	}
	else
	{
		if (m_qv[qi].size() == 0 || m_qi[qi].size() != 0 || m_ql[qi].size() != 0)
		{
			//throw VException (("Unexpected quad"),1);
		}
		RefTriangle rt1 (m_qv[qi][0] , m_qv[qi][1],m_qv[qi][2]);
		RefTriangle rt2 (m_qv[qi][2] , m_qv[qi][3],m_qv[qi][0]);
		m_res->AddTriangle (rt1);
		m_res->AddTriangle (rt2);
	}
}

void SurfaceQuadTrimmer2::SortStruct::SetTeta (const VPoint2d &p , const VPoint2d &qmid)
{
	VPoint2d v = p - qmid;
	m_teta = acos (fabs(v.x()) / v.GetLength());
	if (v.y() > 0.0 && v.x() < 0.0)
		m_teta = PI - m_teta;
	if (v.y() < 0.0 && v.x()< 0.0)
		m_teta += PI;
	if (v.y() < 0.0 && v.x() > 0.0)
		m_teta = 2. * PI - m_teta;
}

VPoint2d SurfaceQuadTrimmer2::GetQMid(const int quno, const int qvno) const
{
	return (GetQLowerLeft (quno,qvno) + GetQUpperRight (quno , qvno)) * 0.5;
}

void SurfaceQuadTrimmer2::FindLoopQuadIntPoints()
{
	// 1st hor

	// 2nd ver
}

void SurfaceQuadTrimmer2::FindLoopQuadIntHor()
{
	VPoint fnor (-1.,-1.,-1.);
	// all v lines
	int mysize = (int)(m_vgrid.size());
	std::vector<VPoint2d> svec;
	for (int i=0;i<mysize;++i)
	{
		const vreal &currv = m_vgrid[i];
		const int qv = CastVCoord (currv);
		VLine2d l ( VPoint2d (0.0,currv) , VPoint2d (1.0,currv));
		m_dsurf.AllLoopsLineInt (l,svec);
		// we have the intersection points , so put them all in appr quads
		int mysize2 = (int) (svec.size());
		for (int j=0;j<mysize2;++j)
		{
			const VPoint2d &intp=svec[j];
			vreal intu = intp.x();
			const int qu = CastUCoord (intu);
			VPoint2d p (intu,currv);
			VPoint m = m_surf.GetPoint (intu , currv);
			int qi = this->GetQIndex (qu,qv);
			// add par to qi
			int si = m_res->surfpack.InsertPoint (p,m,fnor);
			m_qi[qi].push_back(si);
		}
		CollectUntrimmedQVertexHor (svec);
		svec.clear();
	}
}

void SurfaceQuadTrimmer2::FindLoopQuadIntVer()
{

}

void SurfaceQuadTrimmer2::CollectUntrimmedQVertexHor (const std::vector<VPoint2d> &svec)
{
	vreal currv = svec[0].y();
	int qv = CastVCoord (currv);

	int mysize = svec.size();
	for (int i=0; i<mysize; i=i+2)
	{
		const VPoint2d &prevp = svec[i];
		const VPoint2d &currp = svec[i+1];
		int qu1 = CastUCoord(prevp.x());
		int qu2 = CastUCoord (currp.x());
		for (int qu=qu1 +1;qu<qu2;++qu)
		{	
			//AddRelQLowerLeft (j)
				// calculate point for vertex of the main quad
						// 2 branches 1st regular case   2nd line is on v == 1.
								
				// add the point to result pool and get an index
				
				// get related quads for qu,qv

				// if quad is not out of bounds add the index to qv

				
		}
	}
}
