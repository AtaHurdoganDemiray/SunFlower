// SurfaceQuadBspTrimmer.cpp

#include "stdafx.h"
#include "SurfaceQuadBspTrimmer.h"
#include "SequentialLineSet2d.h"
#include "BspTreeConstructor.h"
#include "TrimLoopVerifier.h"
#include "BspTreeBooleanOp.h"
#include "BspTreeToLineSet2d.h"
#include "LineSet2d.h"
#include "LineSet2d2SequentialLineSet2d.h"
#include "IncDel.h"
#include "VRefTriangleMesh.h"
#include "UntrimmedSurface.h"

std::vector<VPoint2d> SurfaceQuadBspTrimmer::m_pointpool (30000);
int SurfaceQuadBspTrimmer::m_pointcount (0);
SequentialLineSet2d SurfaceQuadBspTrimmer::m_unitSquare (5);

SurfaceQuadBspTrimmer::SurfaceQuadBspTrimmer (const std::vector<vreal> &ugrid
											  ,const std::vector<vreal> &vgrid		
											  ,const SequentialLineSet2d *outerLoop
											  ,const std::vector<SequentialLineSet2d *> &innerLoopVec
											  ,const UntrimmedSurface &surf)
											  :m_outerLoop (outerLoop)
											  ,m_innerLoopVec(innerLoopVec)
											  ,m_surf(surf)
{
	if (!m_unitSquare.Size())
	{
		m_unitSquare.InsertPoint (VPoint2d(0,0));
		m_unitSquare.InsertPoint (VPoint2d(1,0));
		m_unitSquare.InsertPoint (VPoint2d (1,1));
		m_unitSquare.InsertPoint (VPoint2d (0,1));
		m_unitSquare.InsertPoint (VPoint2d (0,0));
	}
	//resize pointpool
	m_pointcount = 0;
	if (m_outerLoop == 0)
	{
		m_outerLoop = &m_unitSquare;
	}
	MakeBsp();
	int us = (int)ugrid.size()-1;
	int vs = (int)vgrid.size()-1;
	SequentialLineSet2d currquad(5);
	for (int i=0;i<5;++i)
		currquad.InsertPoint(VPoint2d (i,i));
	std::vector<VPoint2d> &qv = currquad.GetPointSet().GetVec();

	for (int v=0;v<vs;++v)
	{
		const vreal &vp = vgrid[v];
		const vreal &vpn = vgrid[v+1];
		for (int u=0;u<us;++u)
		{
			const vreal &up = ugrid[u];
			const vreal &upn = ugrid[u+1];
			qv[0] = qv[4] = VPoint2d (up,vp);
			qv[1] = VPoint2d (upn,vp);
			qv[2] = VPoint2d (upn,vpn);
			qv[3] = VPoint2d (up,vpn);
			TrimAndTriangulateSingleQuad (currquad);
		}
	}
	PointSet2d tmp (m_pointcount);
	RefTriangleMesh refmesh (m_pointcount/3);
	bool isexist1(false),isexist2(false),isexist3(false);
	for (int w=0;w<m_pointcount;)
	{
		const VPoint2d &p1 = m_pointpool[w++];
		const VPoint2d &p2 = m_pointpool[w++];
		const VPoint2d &p3 = m_pointpool[w++];
		int r1 = tmp.InsertPointIf (p1,isexist1);
		int r2 = tmp.InsertPointIf (p2,isexist2);
		int r3 = tmp.InsertPointIf (p3,isexist3);
		refmesh.AddTriangle (RefTriangle (r1,r2,r3));
	}
	int realpointcount = tmp.Size();
	m_result = new VRefTriangleMesh (realpointcount);
	VPoint m,n;
	for (int i=0;i<realpointcount;++i)
	{
		const VPoint2d *p = tmp[i];
		m_surf.GetPointAndNormal (p->x(),p->y(),m,n);
		m_result->surfpack.InsertPoint (*p,m,n);
	}
	int trisize = refmesh.Size();
	for (int w=0;w<trisize;++w)
	{
		RefTriangle *reftri = refmesh[w];
		m_result->AddTriangle (*reftri);
	}
}

SurfaceQuadBspTrimmer::~SurfaceQuadBspTrimmer()
{
	delete m_bspouter;
	int mysize=(int)m_bspinnervec.size();
	for (int i=0;i<mysize;++i)
	{
		geo::BspTree *currbsp = m_bspinnervec[i];
		delete currbsp;
	}
}

void SurfaceQuadBspTrimmer::TrimAndTriangulateSingleQuad (SequentialLineSet2d &q)
{
	VPoint tmp;
	geo::BspTreeConstructor myfactory;
	//quad larýn loop intersection'ý sonucu bir bsp 
	geo::BspTree *qbsp = myfactory.ConstructTree (q);

	geo::BspTree *i1 = geo::BspTreeBooleanOp::Intersect (*qbsp,*m_bspouter);
	delete qbsp;
	if (i1->IsEmptySet())
	{//quad is totally out of surface region
		delete i1;
		return;
	}
	int mysize = (int)m_bspinnervec.size();
	for (int i=0;i<mysize;++i)
	{
		geo::BspTree *currbsp = m_bspinnervec[i];
		geo::BspTree *tmp = i1;
		i1 = geo::BspTreeBooleanOp::Subtract (*i1,*currbsp);
		if (i1->IsEmptySet())
		{//quad is totally out of surface region
			delete tmp;
			delete i1;
			return;
		}
		delete tmp;
	}
	//bsp->  std::set<VPoint2d>
	geo::BspTreeToLineSet2d myconverter;
	LineSet2d *ils = myconverter.Make (*i1);
	LineSet2d2SequentialLineSet2d tosl;
	std::vector<SequentialLineSet2d *> slvec = tosl.Make (*ils);
	delete ils;

	int mysize3 = (int)slvec.size();
	for (int i=0;i<mysize3;++i)
	{
		RefTriangleMesh refmesh(50);
		SequentialLineSet2d *currp = slvec[i];
		std::vector<VPoint2d> &vec = currp->GetPointSet().GetVec();
		int mysize4=currp->GetPointSize()-1;
		std::list<int> refl;
		for (int j=0;j<mysize4;++j)
		{
			refl.push_back(j);
		}
		IncDel::IncDel2d (vec,refl,refmesh);
		
		int mysize5 = refmesh.Size();
		for (int w=0;w<mysize5;++w)
		{
			const RefTriangle *myrt = refmesh.operator [](w);
			//deltrianglemesh  for each tri bsp intersection
			AddTriToPool ( i1,vec[myrt->ref0],vec[myrt->ref1],vec[myrt->ref2] );
		}
		delete currp;
	}
	delete i1;	
}

void SurfaceQuadBspTrimmer::MakeBsp()
{
	//if (!TrimLoopVerifier::IsInUnitSquare (*m_outerLoop,1.e-4) 
	//	|| !TrimLoopVerifier::IsClosed(*m_outerLoop,1.e-4))
	//{
	//	throw VException (("trim loop exception"),1);
	//}
	
	VPoint2d outp (2.,2.);
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
		const SequentialLineSet2d * currLoop = m_innerLoopVec[i];
		if (!TrimLoopVerifier::IsInUnitSquare (*currLoop,1.e-6)
			|| !TrimLoopVerifier::IsClosed (*currLoop,1.e-6))
		{
			throw VException (( IDS_TriLooExc),1);
		}
		SequentialLineSet2d tempinner (currLoop->GetPointSize());
		TrimLoopVerifier::RemoveDuplicatePoints (*currLoop,tempinner,1.e-6);
		if (tempinner.Size() < 3)
			throw VException (( IDS_TrimLoopExc),1);
		m_bspinnervec.push_back(myfactory.ConstructTree (tempinner) );
		geo::BspTree * currbsp = m_bspinnervec[i];
		if ( currbsp->PointLocation(outp) > 0)
			geo::BspTreeBooleanOp::NegateThis (*currbsp);
	}
}

VRefTriangleMesh * SurfaceQuadBspTrimmer::GetResult()
{
	return m_result;
}

void SurfaceQuadBspTrimmer::AddTriToPool(const geo::BspTree *myloop, const VPoint2d &c1, const VPoint2d &c2, const VPoint2d &c3)
{
//	if (myloop->PointLocation(c1) < 0 || myloop->PointLocation(c2) < 0 || myloop->PointLocation(c3) < 0)
//		return;
//	else
//	{
		m_pointpool[m_pointcount++] = c1;
		m_pointpool[m_pointcount++] = c2;
		m_pointpool[m_pointcount++] = c3;
//	}

	//SequentialLineSet2d tmp (4);
	//tmp.InsertPoint (c1);tmp.InsertPoint(c2);tmp.InsertPoint(c3);tmp.InsertPoint(c1);
	//geo::BspTreeConstructor myfactory;
	//geo::BspTree *mytri = myfactory.ConstructTree (tmp);
	//geo::BspTree *myreg = geo::BspTreeBooleanOp::Intersect (*mytri,*myloop);
	//if (myreg->IsEmptySet())
	//	return;
	//geo::BspTreeToLineSet2d myx;
	//LineSet2d *ls = myx.Make (*myreg);
	//LineSet2d2SequentialLineSet2d myx2;
	//std::vector<SequentialLineSet2d *> slvec = myx2.Make (*ls);
	//delete ls;
	//int mysize = (int)slvec.size();
	//if (mysize == 1 && slvec[0]->operator ==(tmp) )
	//{
	//	m_pointpool[m_pointcount++] = c1;
	//	m_pointpool[m_pointcount++] = c2;
	//	m_pointpool[m_pointcount++] = c3;
	//	delete slvec[0];
	//}
	//else
	//{
		//for (int i=0;i<mysize;++i)
		//{
		//	RefTriangleMesh rm(4);
		//	std::list<int> reflist;
		//	int slsize = slvec[i]->GetPointSize ()-1;
		//	for (int j=0;j<slsize;++j)
		//	{
		//		std::vector<VPoint2d> &vec = slvec[i]->GetPointSet().GetVec();
		//		IncDel::IncDel2d (vec,reflist,rm);
		//		int tric = rm.Size();
		//		for (int k=0;k<tric;++k)
		//		{
		//			RefTriangle *rt = rm[k];
		//			m_pointpool[m_pointcount++]=vec[rt->ref0];
		//			m_pointpool[m_pointcount++]=vec[rt->ref1];
		//			m_pointpool[m_pointcount++]=vec[rt->ref2];
		//		}
		//	}
		//}
	//}
}
