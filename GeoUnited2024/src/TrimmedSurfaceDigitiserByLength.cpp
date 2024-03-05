// TrimmedSurfaceDigitiserByLength.cpp

#include "stdafx.h"
#include "TrimmedSurfaceDigitiserByLength.h"
#include "dSurface.h"
#include "VPoint2d.h"
#include "VPoint.h"

void TrimmedSurfaceDigitiserByLength::Digitise(const vreal &step
											   , dSurface &surf
											   , std::vector<sVector<float,3>> &pvec
											   ,std::vector<sVector<float,3>> &nvec
											   ,std::vector<float> &avec)
{
	const UntrimmedSurface &s = surf.GetSurface();
	vreal ustep,vstep;
	int ucount,vcount;
	GetUVSteps (surf,step,ustep,vstep,ucount,vcount);

	// First digitise then trim
	vreal up=0.;
	vreal vp = 0.;
	std::vector<sVector<float,3>> tmpvec;
	for (int u=0; u<=ucount; u++)
	{
		DoVLoop ( vcount , up , vp , vstep , s , tmpvec);
		vp=0.;
		up+=ustep;
	}
	//the last u
	vp = 0.;
	DoVLoop (vcount , 1. , vp , vstep , s, tmpvec);

	up = 0.;
	vp = 0.;
	// trim and calculate normals
	for (int u=0;u<=ucount;u++)
	{
		DoVLoop2(u,ucount,vcount,vcount+2,up,vp,vstep,surf,tmpvec,pvec,nvec,avec);
		vp = 0.;
		up += ustep;
	}
	up = 1.;
	vp = 0.;
	DoVLoop2 (ucount+1,ucount,vcount,-(vcount+2),up,vp,vstep,surf,tmpvec,pvec,nvec,avec);
}

int TrimmedSurfaceDigitiserByLength::GetIndex (int uno , int vno
											   , int u , int v)
{
	return v + vno * u;
}

void TrimmedSurfaceDigitiserByLength::DoVLoop (int vcount 
											   , const vreal &up
											   , vreal &vp
											   , const vreal &vstep
											   , const UntrimmedSurface &s
											   , std::vector<sVector<float,3>> &tmpvec
											   )
{
	for (int v=0;v<=vcount;v++)
	{
		VPoint myp = s.GetPoint(up,vp);
		float fp[3] = {
						static_cast<float> (myp.x())
						,static_cast<float> (myp.y())
						,static_cast<float> (myp.z())
					};
		tmpvec.push_back (fp);
		vp+=vstep;
	}
	VPoint lp = s.GetPoint (up,1.);
	float lfp[3] = {
						static_cast<float> (lp.x())
						,static_cast<float> (lp.y())
						,static_cast<float> (lp.z())
					};
	tmpvec.push_back (lfp);
}

//sVector<float,3> TrimmedSurfaceDigitiserByLength::CalcNormalAndGetPoint 
//											(int u
//											,int v
//											,int ucount
//											,int vcount
//											,std::vector<sVector<float,3>> &tmpvec
//											,sVector<float,3> &mp
//											)
//{
//	int ind = GetIndex (ucount + 2 , vcount +2 , u , v);
//	mp = tmpvec[ind];
//	const sVector<float,3> &vinc = tmpvec[ind+1];
//	const sVector<float,3> &uinc = tmpvec[ind+vcount+1];
//	sVector<float,3> n = cross3d ( (uinc - mp) , (vinc - mp));
//	n.NormThis();
//	return n;
//}
void TrimmedSurfaceDigitiserByLength::CalcNormalAndPointAndArea (int u
																,int v
																,int ucount
																,int vcount
																,int neighborUOfs
																,int neighborVOfs
																,std::vector<sVector<float,3>> &tmpvec
																,sVector<float,3> &mp
																,sVector<float,3> &n
																,float &a
																)
{
	// number of points is (ucount + 2) x (vcount + 2)
	int ind = GetIndex (ucount + 2 , vcount +2 , u , v);
	mp = tmpvec[ind];
	const sVector<float,3> &vinc = tmpvec[ind+neighborVOfs];
	const sVector<float,3> &uinc = tmpvec[ind+neighborUOfs];
	sVector<float,3> d1 = uinc - mp;
	sVector<float,3> d2 = vinc - mp;
	n = cross3d ( d1 , d2);
	n.NormThis();
	// sign issues
	if ( neighborUOfs * neighborVOfs < 0)
		n *= -1.;
	if (neighborUOfs < 0 || neighborVOfs < 0)
		a = 0.f;
	else
		a = d1.GetLength() * d2.GetLength();
}
void TrimmedSurfaceDigitiserByLength::DoVLoop2
												(int u
												 ,int ucount
												 ,int vcount
												 ,int neighborUOfs
												 ,vreal &up
												 ,vreal &vp
												 ,const vreal &vstep 
												 ,dSurface &surf
												 ,std::vector<sVector<float,3>> &tmpvec
												 ,std::vector<sVector<float,3>> &pvec
												 ,std::vector<sVector<float,3>> &nvec
												 ,std::vector<float> &avec
												 )
{
	for (int v=0;v<=vcount;v++)
	{
		VPoint2d pp(up,vp);
		if (surf.IsPointTrimmed (pp) == false)
		{
			sVector<float,3> mp;
			sVector<float,3> n;
			float a;
			CalcNormalAndPointAndArea
				(u,v,ucount,vcount,neighborUOfs,1,tmpvec,mp,n,a);

			// Add point
			pvec.push_back (mp);
			nvec.push_back (n);
			avec.push_back(a);
		}
		vp += vstep;
	}
	VPoint2d lpp (up , 1.);
	if (surf.IsPointTrimmed (lpp) == false)
	{
		sVector <float,3> mp;
		sVector <float,3> n;
		float a;
		CalcNormalAndPointAndArea (u,vcount+1,ucount,vcount, neighborUOfs , -1,tmpvec,mp,n,a);

		pvec.push_back(mp);
		nvec.push_back(n);
		avec.push_back(a);
	}
}

void TrimmedSurfaceDigitiserByLength::Digitise (const vreal &step
												, dSurface &surf 
												, std::vector<VPoint2d> &pvec
												, std::vector<VPoint> &mvec
												, std::vector<VPoint> &nvec)
{
	vreal ustep,vstep;
	int ucount,vcount;
	GetUVSteps (surf,step,ustep,vstep,ucount,vcount);
	vreal up=0.;
	vreal vp = 0.;
	for (int u=0; u<=ucount; u++)
	{
		DoVLoop3 ( vcount , up , vp , vstep , surf , pvec , mvec , nvec);
		vp=0.;
		up+=ustep;
	}
	//the last u
	vp = 0.;
	DoVLoop3 (vcount , 1. , vp , vstep , surf, pvec,mvec,nvec);

}

void TrimmedSurfaceDigitiserByLength::GetUVSteps (dSurface &surf
												  , const vreal &step
												  , vreal &ustep
												  , vreal &vstep
												  , int &ucount
												  , int &vcount)
{
	const UntrimmedSurface &s = surf.GetSurface();
	VPoint p1 = s.GetPoint (0.5,0.5);
	VPoint vconst = s.GetPoint (0.55 , 0.5);
	VPoint uconst = s.GetPoint (0.5 , 0.55);
	vreal ulen = (vconst - p1).GetLength() / 0.05;
	vreal vlen = (uconst - p1).GetLength() / 0.05;
	ustep = step / ulen;
	vstep = step / vlen;
	ucount = (int) (1. / ustep);
	vcount = (int) (1. / vstep);
}

void TrimmedSurfaceDigitiserByLength::DoVLoop3 (int vcount 
											 , const vreal &up
											 , vreal &vp
											 , const vreal &vstep
											 , dSurface &surf
			 								 , std::vector<VPoint2d> &pvec
											 , std::vector<VPoint> &mvec
											 , std::vector<VPoint> &nvec
											 )
{
	UntrimmedSurface &s = surf.GetSurface();
	for (int v=0;v<=vcount;v++)
	{
		VPoint2d p (up,vp);
		if (surf.IsPointTrimmed(p) == false)
		{
			VPoint m = s.GetPoint(up,vp);
			VPoint n = s.GetNormal (up,vp,m);
			pvec.push_back(p);
			mvec.push_back(m);
			nvec.push_back(n);
		}		
		vp+=vstep;
	}
	VPoint2d lp (up,1.0);
	if (surf.IsPointTrimmed (lp)==false)
	{
		VPoint lm = s.GetPoint (up,1.0);
		VPoint ln = s.GetNormal (up,1.0,lm);
		pvec.push_back (lp);	
		mvec.push_back (lm);	
		nvec.push_back (ln);	
	}
}
