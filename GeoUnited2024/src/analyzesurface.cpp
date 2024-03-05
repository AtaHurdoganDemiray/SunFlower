// AnalyzeSurface.cpp

#include "stdafx.h"
#include "AnalyzeSurface.h"
#include "UntrimmedSurface.h"
#include "SurfacePointGenerator.h"
#include "VPoint.h"
#include "VArc.h"
#include "PolarCalc2d.h"
#include "UntypedException.h"
#include "ExceptionLogger.h"
#include <stdexcept>

AnalyzeSurface::SurfaceType
	AnalyzeSurface::IsPlanarOrCylinder (const UntrimmedSurface &surf 
										 , const double &u0
										, const double &u1
										, const double &v0
										, const double &v1
										, double &radius0
										, double &radius1
										, double &h
										, VPoint &center
										, VPoint &ax 
										, bool &isClosedCylinder
										, vreal tol)
{
	try
	{
		isClosedCylinder = false;
		// calculate curvature values
		vreal uc[6];
		vreal vc[6];
		vreal u = 0.0 + 1.e-4;//u0 +1.e-4;
		vreal v = 0.0 + 1.e-4;//v0 +1.e-4;
		vreal du = (1.0 - 2.e-4) / 5.;//(u1-u0-2.e-4)/5.;
		vreal dv = (1.0 - 2.e-4) / 5.;//(v1-v0-2.e-4)/5.;
		for (int i=0;i<6;i++)
		{
			VPoint derv,dervderv;
			uc[i] = surf.GetDigitiser().GetCurvatureAprAlongU (u,v,derv,dervderv);
			vc[i] = surf.GetDigitiser().GetCurvatureAprAlongV (u,v,derv,dervderv);
			u+=du;
			v+=dv;
		}
		// Is radius of curvature constant along u and oo along v
		bool cylinderAxAlongV = true;
		bool cylinderAxAlongU = true;
		for (int i=0;i<6;i++)
		{
			if (fabs(vc[i]) > tol)
			{
				cylinderAxAlongV = false;
			}
		}
		// Is radius of curvature constant along v and oo along u
		for (int i=0;i<6;i++)
		{
			if (fabs (uc[i]) > tol)
			{
				cylinderAxAlongU = false;
			}
		}
		// if not cylinder 
		if (cylinderAxAlongV == false && cylinderAxAlongU == false)
		{
			radius0 = 0.;
			radius1 = 0.;
			h = 0.;
			VPoint o(0.0,0.0,0.0);
			center = o;
			ax = o;
			return none;
		}
		if ( cylinderAxAlongV == true && cylinderAxAlongU == true)
		{
			radius0 = MAX_NUMBER_LIMIT;
			radius1 = MAX_NUMBER_LIMIT;
			center = surf.GetPoint(0.5,0.5);//surf.GetPoint ( (u0+u1)*0.5 , (v0+v1)*0.5 );
			ax = surf.GetNormal(0.5,0.5,center);//surf.GetNormal ( (u0+u1)*0.5 , (v0+v1)*0.5 , center);
			return planar; //surface is planar
		}
		
		if (cylinderAxAlongU)
		{
			VPoint p1 = surf.GetPoint(0.0, 0.0);//surf.GetPoint (u0,v0);
			VPoint p2 = surf.GetPoint(0.0,0.5);//surf.GetPoint (u0,(v0+v1) * 0.5);
			VPoint p3 = surf.GetPoint(0.0, 0.75);//surf.GetPoint (u0,(v0+v1) *0.75);
			VPoint p4 = surf.GetPoint(1.0,0.75);//surf.GetPoint (u1,(v0+v1) * 0.75);
			VPoint pck = surf.GetPoint(1.0,0.0);//surf.GetPoint (u0,v1);
			VPoint pck2 = surf.GetPoint(1.0,0.5);
			VArc a(p1, p2, p3);
			VArc a2(pck, pck2, p4);
			//radius0 = 1.0 / vc[0];
			radius0 = a.GetRadius();
			radius1 = a2.GetRadius();
			//ax = p4 - p3;
			ax = a2.GetCenter() - a.GetCenter();
			h = ax.GetLength();
			ax/=h;
			//if (fabs(a.GetRadius() - radius0)>tol)
			//{
			//	LogFileWriter::WriteUnknownExceptionLog("AnalyzeSurface::IsPlanarOrCylinder exception ","Arc radius at 0 and radius of curvature are different");
			//}
			center = a.GetCenter();
			if (pck.Compare(p1, tol))// 1.e-6))
				isClosedCylinder = true;
		}
		else
		{
			VPoint p1 = surf.GetPoint(0.0,0.0);//surf.GetPoint (u0,v0);
			VPoint p2 = surf.GetPoint(0.5,0.0);//surf.GetPoint ((u0+u1)*0.5,v0);
			VPoint p3 = surf.GetPoint(0.75,0.0);//surf.GetPoint ((u0+u1)*0.75,v0);
			VPoint p4 = surf.GetPoint(0.75,1.0);//surf.GetPoint ((u0+u1)*0.75,v1);
			VPoint pck = surf.GetPoint(1.0,0.0);//surf.GetPoint (u1,v0);
			VPoint pck2 = surf.GetPoint(1.0,0.5);
			VArc a(p1, p2, p3);
			VArc a2(pck, pck2, p4);
			//radius0 = 1.0/uc[0];
			radius0 = a.GetRadius();
			radius1 = a2.GetRadius();
			//ax =p4-p3;
			ax = a2.GetCenter() - a.GetCenter();
			h=ax.GetLength();
			ax/=h;
			center = a.GetCenter();
			if (pck.Compare (p1,tol))//1.e-6))
				isClosedCylinder = true;
		}
		SurfaceType res = fabs(radius1-radius0) < tol ? cylindirical : conical;
		return res;
	}
	catch ( exception &exc)
	{
		ExceptionLogger::log(exc, __FILE__, __LINE__);
		return none;
	}
	catch (...)
	{		
		UntypedException uexc(( IDS_UnkExcInAnaIsP),999);
		ExceptionLogger::log(uexc, __FILE__, __LINE__);
	}
}

int AnalyzeSurface::GetUStep (const UntrimmedSurface &surf
								, const double &tol )
{
	VPoint derv,dervderv;
	vreal uc1 = surf.GetDigitiser().GetCurvatureAprAlongU(0.1,0.9,derv,dervderv);
	vreal uc2 = surf.GetDigitiser().GetCurvatureAprAlongU(0.6,0.6,derv,dervderv);
	int ustep=20;
		
	// step = PI / acos (1.0 - tol/r)
		
	if (fabs(uc1) < EPSILON && fabs(uc2) < EPSILON)
		ustep = 6;
	else
	{
		double minr = fabs(uc1) > fabs(uc2) ? 1./uc1 : 1./uc2;
		ustep = (int)(PI / vp_acos (1.0 - tol / minr));//0.4->visualTolerance
		//ustep += 6;
	}
	return ustep;
}
	
int AnalyzeSurface::GetVStep (const UntrimmedSurface &surf
								, const double &tol)
{
	VPoint derv,dervderv;
	vreal vc1 = surf.GetDigitiser().GetCurvatureAprAlongV(0.1,0.1,derv,dervderv);
	vreal vc2 = surf.GetDigitiser().GetCurvatureAprAlongV(0.5,0.6,derv,dervderv);
	int vstep=20;

	// step = PI / acos (1.0 - tol/r)

	if (fabs(vc1)<EPSILON && fabs (vc2)<EPSILON)
		vstep = 6;
	else
	{
		double minr = fabs(vc1) > fabs(vc2) ? 1./vc1 : 1./vc2;
		vstep = (int)(PI / vp_acos (1.0 - tol / minr));
		//vstep += 6;
	}
	return vstep;
}

vreal AnalyzeSurface::GetParStepForU (const UntrimmedSurface &surf,const vreal u,const vreal v,const vreal tol)
{
	//VPoint derv,dervderv;
	//vreal vc1 = surf.GetDigitiser().GetCurvatureAprAlongU(u,v,derv,dervderv);
	//vreal mystep;
	//if (fabs(vc1)<EPSILON)
	//	mystep = 1.0;
	//else
	//{
	//	double r = 1./vc1;
	//	mystep = acos (1.0 - tol / r) / PI;
	//}
	//return mystep;
	vreal u0,u1;
	if (u + 0.1 > 1.)
	{
		u0 = u - 0.1;
		u1 = u;
	}
	else
	{
		u0 = u;
		u1 = u + 0.1;
	}
	VPoint p0 = surf.GetPoint (u0,v);
	VPoint p1 = surf.GetPoint (u1,v);
	VPoint pm = surf.GetPoint ( (u0+u1)*0.5,v);
	vreal err = (pm - (p1+p0)*0.5).GetLength();
	vreal res = 0.1*tol/err;
	if (res > 1.)
		res = 1.;
	return res;
}

vreal AnalyzeSurface::GetParStepForV (const UntrimmedSurface &surf,const vreal u,const vreal v,const vreal tol)
{
	//VPoint derv,dervderv;
	//vreal vc1 = surf.GetDigitiser().GetCurvatureAprAlongV(u,v,derv,dervderv);
	//vreal mystep;
	//if (fabs(vc1)<EPSILON)
	//	mystep = 1.0;
	//else
	//{
	//	double r = 1./vc1;
	//	mystep = acos (1.0 - tol / r) / PI;
	//	if (mystep > 1.)
	//		mystep = 1.;
	//}
	//return mystep;
	vreal v0,v1;
	if (v + 0.1 > 1.)
	{
		v0 = v - 0.1;
		v1 = v;
	}
	else
	{
		v0 = v;
		v1 = v + 0.1;
	}
	VPoint p0 = surf.GetPoint (u,v0);
	VPoint p1 = surf.GetPoint (u,v1);
	VPoint pm = surf.GetPoint (u, (v0+v1)*0.5);
	vreal err = (pm - (p1+p0)*0.5).GetLength();
	vreal res = 0.1*tol/err;
	if (res > 1.)
		res = 1.;
	return res;
}

vreal ADELTAU = 0.02;
vreal ADELTAV = 0.02;

vreal AnalyzeSurface::GetAprModelSpacePerimeterAlongU(const UntrimmedSurface &surf)
{
	VPoint p1 = surf.GetPoint (0.5,0.5);
	VPoint p2 = surf.GetPoint (0.5 + ADELTAU,0.5);
	VPoint d = p2 - p1;
	return d.GetLength() / ADELTAU;
}

vreal AnalyzeSurface::GetAprModelSpacePerimeterAlongV(const UntrimmedSurface &surf)
{
	VPoint p1 = surf.GetPoint (0.5,0.5);
	VPoint p2 = surf.GetPoint (0.5,0.5+ADELTAV);
	VPoint d = p2 - p1;
	return d.GetLength() / ADELTAV;
}
