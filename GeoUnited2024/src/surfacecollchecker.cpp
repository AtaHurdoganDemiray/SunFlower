// SurfaceCollChecker.cpp

#include "stdafx.h"
#include "SurfaceCollChecker.h"
#include "VSurface.h"
#include "MultiTriangles.h"
#include "TrimmedSurfaceDigitiserByLength.h"

SurfaceCollChecker::SurfaceCollChecker (const std::vector<VSurface *> &surfVec
										,const vTool <float> &tool )
										:m_tool (tool)
{
	float mystep1 = 1.f;
	std::vector<const Triangles *> vrtriVec;
	std::vector<VSurface *>::const_iterator i=surfVec.begin();
	std::vector<VSurface *>::const_iterator e=surfVec.end();
	for (i;i!=e;++i)
	{
		//vrtriVec.push_back ( (const Triangles *)&((*i)->GetTriangles()));
		std::vector<sVector<float,3>> pvec;
		std::vector<sVector<float,3>> nvec;
		std::vector<float> avec;
		TrimmedSurfaceDigitiserByLength di;	
		di.Digitise (mystep1 , * (*i)->GetDSurf() , pvec , nvec , avec);
		std::vector<sVector<float,3>>::iterator ii = pvec.begin();
		std::vector<sVector<float,3>>::iterator ee = pvec.end();
		for (ii;ii!=ee;++ii)
		{
			m_pvec.push_back (*ii);
		}
	}
	PointSet tmp ((int)m_pvec.size() +1);
	int mysize = (int)m_pvec.size();
	for (int j=0;j<mysize;++j)
	{
		sVector<float,3> &cp = m_pvec[j];
		VPoint p;
		p.x(static_cast<double>(cp[0]));
		p.y(static_cast<double>(cp[1]));
		p.z(static_cast<double>(cp[2]));
		tmp.InsertPoint(p);
	}
	//m_tri = new MultiTriangles (vrtriVec);
	vreal mystep2 = 20.0;
	//m_cubes = new PosCubes (*m_tri , mystep);
	m_cubes=new PosCubes (tmp);
}

SurfaceCollChecker::~SurfaceCollChecker()
{
	delete m_cubes;
	//delete m_tri;
}

bool SurfaceCollChecker::Check(VPoint &pos)
{
	float tf[3] = {
					static_cast<float>(pos.x())
					,static_cast<float>(pos.y())
					,static_cast<float>(pos.z())
				};
	p3f sp (tf);
	int mysize=m_tool.Size();
	for(int i=0;i<mysize;++i)
	{
		const ToolPrim<float> *currt = m_tool[i];
		float maxRad = currt->GetMaxRadius();
		VPoint offs (static_cast<double>(maxRad));
		VPoint boxmin = pos - offs;
		VPoint boxmax = pos + offs;
		LongVec relind,nii;
		m_cubes->SetRelatedIndiceListForBox (boxmin,boxmax,relind,nii);
		int mysize = relind.Size();
		//for all related triangles
		for (int j=0;j<mysize;++j)
		{
			int curri = relind[j];
			sVector<float,3> &cp = m_pvec[curri];
			p3f diff = cp - sp;
			float pp[2] = {sqrt (diff[0]*diff[0]+diff[1]*diff[1]),diff[2]};
			p2f ppp (pp);
			if (currt->IsIncluding (ppp))
				return true; // there is collision
			//VTriangleE currtri = m_tri->operator [](curri);
			//bool myres = currt->IsIncluding( m_tri->GetPoints());
			//if (myres==true)
			//	return myres;
		}
	}
	// no collision detected
	return false;
}

//bool SurfaceCollChecker::Check(VPoint &posb, VPoint &pose)
//{return false;
	//int mysize=m_tool.Size();
	//for (int i=0;i<mysize;++i)
	//{
	//	const ToolPrim<float> *currt = m_tool[i];
	//	float maxRad = currt->GetMaxRadius();
	//	VPoint offs (static_cast<double> (maxRad));
	//	VPoint boxmin (posb);
	//	VPoint boxmax (posb);
	//	boxmin.ReplaceMin(pose);
	//	boxmax.ReplaceMax (pose);
	//	boxmin-=offs;
	//	boxmax+=offs;
	//	LongVec relind,nii;
	//	m_cubes->SetRelatedIndiceListForBox (boxmin,boxmax,relind,nii);
	//	int mysize = relind.Size();
	//	for (int j=0;j<mysize;++j)
	//	{
	//		int curri = relind[j];
	//		VTriangleE tri = m_tri->operator [](curri);
	//		bool res = Check (tri,posb,pose,*currt);
	//		if (res)
	//			return res;
	//	}
	//}

	//bool res (false);
	//return res;
//}

//bool SurfaceCollChecker::Check(const VTriangleE &tri,const VPoint &pos,const ToolPrim<float> &prim)
//{
//	vreal mystep = 0.2;
//	std::vector<VPoint>pvec;
//	tri.Digitise (mystep,pvec);
//	std::vector<VPoint>::const_iterator i=pvec.begin();
//	std::vector<VPoint>::const_iterator e=pvec.end();
//	for (i;i!=e;++i)
//	{
//		VPoint dif = (*i)-pos;
//		float x = static_cast<float>(dif.x());
//		float y = static_cast<float>(dif.y());
//		float myp[2] = {sqrt (x*x+y*y),static_cast<float> (dif.z())};
//		if (prim.IsIncluding(myp))
//			return true;
//	}
//	return false;
//}
//
//bool SurfaceCollChecker::Check(const VTriangleE &tri, const VPoint &posb,const VPoint &pose,const ToolPrim<float> &prim)
//{
//	VPoint v = pose - posb;
//	const VPoint &c1 = tri.GetCorner1();
//	const VPoint &c2 = tri.GetCorner2();
//	const VPoint &c3 = tri.GetCorner3();
//	VTriangleE e1 (c1 , c2 , c1 - v);
//	VTriangleE e2 (c1-v , c2-v , c2);
//	VTriangleE e3 (c1 , c3 , c1 - v);
//	VTriangleE e4 (c1-v , c3-v , c3 );
//	VTriangleE e5 (c2 , c3 , c2 - v);
//	VTriangleE e6 (c2-v , c3-v , c3);
//	VTriangleE e7 (c1-v,c2-v,c3-v);
//		 
//	if (Check (tri,posb,prim))
//		return true;
//	if (Check (e1,posb,prim))
//		return true;
//	if (Check(e2,posb,prim))
//		return true;
//	if (Check(e3,posb,prim))
//		return true;
//	if (Check(e4,posb,prim))
//		return true;
//	if (Check(e5,posb,prim))
//		return true;
//	if (Check(e6,posb,prim))
//		return true;
//	if (Check(e7,posb,prim))
//		return true;
//	return false;
//}
