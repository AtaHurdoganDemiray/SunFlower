// SurfacePointGenerator.cpp

#include "stdafx.h"
#include "SurfacePointGenerator.h"
#include "VDelaunay2d.h"
#include "VRefTriangleMesh.h"
#include "IgesEntity.h"
#include "DoubleVec.h"
#include "LinePointMinDist.h"

SurfacePointGenerator::SurfacePointGenerator ( const vreal & _u0 , const vreal & _u1 , const vreal & _v0 , const vreal & _v1 )
: u0 ( _u0 ) , u1 ( _u1 ) , v0 ( _v0 ) , v1 ( _v1 ) //, m_uParShift(0.0) , m_vParShift(0.0)
{
	deltau = u1 - u0;
	deltav = v1 - v0;
	m_initialDigitiseSegmentNo = 20;
	m_isPeriodicInU=false;
	m_isPeriodicInV=false;
	m_uPeriod=1.;
	m_vPeriod=1.;
}

void SurfacePointGenerator::GetPointAndNormal (const vreal &u , const vreal &v , VPoint &m , VPoint &n) const
{
	m = GetPoint(u, v);
	n = GetNormal(u, v, m);
}

const vreal & SurfacePointGenerator::U0 () const
{
	return u0;
}

const vreal & SurfacePointGenerator::U1 () const
{
	return u1;
}

const vreal & SurfacePointGenerator::V0 () const
{
	return v0;
}

const vreal & SurfacePointGenerator::V1 () const
{
	return v1;
}

SurfacePointGenerator::~SurfacePointGenerator()
{

}

void SurfacePointGenerator::FormRefTriangles (VRefTriangleMesh *rm)const
{
	VDelaunay2d triangulator;
	triangulator.Calculate(rm->surfpack.parampoints,rm->GetRefTriangleMesh());
}

//void SurfacePointGenerator::GetSnapEnt (const IgesEntity *current , const IgesEntitySet *entset , PointSet &snapp , LineSet &snapl)
//{
//	int igesetype = current->entitytype;
//	switch (igesetype)
//	{
//	case (144)://Trimmed Surface
//
//		break;
//	case (143)://Bounded Surface
//		
//		break;
//	case(128)://Nurbs Surface
//
//		break;
//	case(122)://Tabulated Cylinder
//
//		break;
//	case(120)://Surface of Revolution
//
//		break;
//	case (118)://Ruled Surface
//
//		break;
//	case (140)://Offset Surface
//
//		break;
//	case (108)://Plane
//
//		break;
//	}
//}

// dummy function for reducing numeric errors
//void DoNothing ( const vreal & tmp )
//{
//
//}
VPoint SurfacePointGenerator::GetNormalAppr (const vreal &u,const vreal &v,const VPoint &surfpoint)const
{
	VPoint uderv = GetDervAprAlongU (u,v,surfpoint);
	VPoint vderv = GetDervAprAlongV (u,v,surfpoint);
	return (uderv ^ vderv).Unit();
}
//VPoint SurfacePointGenerator::GetNormalAppr(const vreal &u, const vreal &v, const VPoint &surfpoint) const
//{
//	// for reducing numeric errors
//	vreal temp = u + VGlobalValues::MYDERV_DIFF;
//	DoNothing (temp);
//	vreal myderv_diff = temp - u;	
//	
//	VPoint uderv , vderv;
//	vreal ubef = u - myderv_diff;
//	vreal vbef = v - myderv_diff;
//	//vreal unext;
//
//	if ( ubef > 0.0 )
//	{
//		//unext = ubef;
//		VPoint dp = GetPoint ( ubef , v );
//		uderv = surfpoint - dp;
//		uderv.NormThis();
//		//uderv *= ( 1.0 / static_cast<vreal>(VGlobalValues::DERV_DIFF) );
//	}
//	else
//	{
//		vreal uaft = u + myderv_diff;
//		//unext = uaft;
//		VPoint dp = GetPoint ( uaft , v );
//		uderv = dp-surfpoint;
//		uderv.NormThis();
//		//uderv *= ( 1.0 / static_cast<vreal>(VGlobalValues::DERV_DIFF) );
//	}
//
//	if ( vbef > 0.0 )
//	{
//		VPoint dp = GetPoint ( u /*unext*/ , vbef );
//		vderv = surfpoint - dp;
//		vderv.NormThis();
//		//vderv *= ( 1.0 / static_cast<vreal>(VGlobalValues::DERV_DIFF) );
//	}
//	else
//	{
//		vreal vaft = v + myderv_diff;
//		VPoint dp = GetPoint ( u /*unext*/ , vaft );
//		vderv =  dp-surfpoint;
//		vderv.NormThis();
//		//vderv *= ( 1.0 / static_cast<vreal>(VGlobalValues::DERV_DIFF) );
//	}
//
//	VPoint result = uderv ^ vderv;
//	result.NormThis();
//
//	//if ( vbef <= 0.0 && ubef <= 0.0 )
//	//{
//	//	result *= -1;
//	//}
//
//	return result;
//}

void SurfacePointGenerator::DivideSpans (const DoubleVec &spans , DoubleVec &divided , int divideNo)
{
	int mysize = spans.Size() - 1;
	for (int i = 0; i<mysize; i++)
	{
		const vreal &v0 = spans[i];
		const vreal &v1 = spans[i+1];
		vreal delt = (v1 - v0) / divideNo;
		for (int j=0; j<divideNo; j++)
		{
			vreal currp = v0 + j * delt;
			divided.Addvreal (currp);
		}
		if (i == mysize-1)
			divided.Addvreal (v1);
	}
}

//VRefTriangleMesh * SurfacePointGenerator::GenericDigitise (const vreal &tol)const
//{
//	//DoubleVec uDefInt1;
//	//DoubleVec vDefInt1;
//	DoubleVec uDefInt2;
//	DoubleVec vDefInt2;
//	GetUDefIntervals (uDefInt2);
//	GetVDefIntervals (vDefInt2);
//
//	//InsertSegments (tol,surf,uDefInt1,uDefInt2,vDefInt1,vDefInt2);
//	
//	int uSize = uDefInt2.Size();
//	int vSize = vDefInt2.Size();
//	int totalSize = uSize * vSize;
//
//	VRefTriangleMesh *result = new VRefTriangleMesh(totalSize);
//	SurfPointsPack &surfpack = result->surfpack;
//	
//	vreal myu , myv;
//	VPoint newp,newn(-1,-1,-1);
//
//	for ( int i=0; i < uSize; i++ )
//	{
//		myu = uDefInt2[i];
//
//		for ( int j=0; j < vSize; j++ )
//		{
//			myv = vDefInt2[j];
//			newp = GetPoint (myu , myv);
//			//newn = GetNormal (myu , myv , newp);
//			VPoint2d par (myu , myv);
//			surfpack.parampoints.InsertPoint(par);
//			surfpack.modelpoints.InsertPoint(newp);
//			surfpack.normals.InsertPoint(newn);
//		}
//	}
//
//	for (int i=0; i < uSize-1; i++)
//	{
//		for (int j=0; j< vSize-1; j++)
//		{
//			int lowerLeft = i*vSize + j;
//			int lowerRight = (i+1) * vSize + j;
//
//			RefTriangle r1 (lowerLeft , lowerRight , lowerRight + 1);
//			RefTriangle r2 (lowerRight +1 , lowerLeft + 1 , lowerLeft);
//			result->AddTriangle (r1);
//			result->AddTriangle (r2);
//		}		
//	}
//
//	return result;
//}

VRefTriangleMesh * SurfacePointGenerator::GenericDigitise(const vreal &tol) const
{
	std::vector<vreal> ugrid;
	std::vector<vreal> vgrid;
	vreal dp=1./m_initialDigitiseSegmentNo;

	for (int i=0;i<m_initialDigitiseSegmentNo;++i)
	{
		vreal mypar=i*dp;
		vreal nextpar = (i+1)*dp;
		int uadd =
			CalculateDivideCountForIntervalAlongU (mypar
												,nextpar
												,0.5
												,tol);

		InsertSegments (uadd,mypar,nextpar,ugrid);
		int vadd =
			CalculateDivideCountForIntervalAlongV (mypar
												,nextpar
												,0.5
												,tol);
		InsertSegments (vadd,mypar,nextpar,vgrid);
		//ugrid[i]=mypar;
		//vgrid[i]=mypar;
	}
	ugrid.push_back (1.);
	vgrid.push_back(1.);

	int usize = (int)ugrid.size();
	int vsize = (int)vgrid.size();
	int totalsize = 2 * usize * vsize;
	VRefTriangleMesh *mymesh = new VRefTriangleMesh (totalsize);

	std::vector<vreal>::const_iterator myue = ugrid.end();
	std::vector<vreal>::const_iterator myv = vgrid.begin();
	std::vector<vreal>::const_iterator myve = vgrid.end();

	for (myv;myv!=myve;++myv)
	{
		std::vector<vreal>::const_iterator myu = ugrid.begin();
		for (myu;myu!=myue;++myu)
		{
			VPoint2d p(*myu,*myv);
			VPoint m,n;
			GetPointAndNormal(*myu,*myv,m,n);
			mymesh->surfpack.InsertPoint (p,m,n);
		}
	}

	// adding quads
	for (int i=0; i < vsize-1; i++)
	{
		for (int j=0; j< usize-1; j++)
		{
			int lowerLeft = i*usize + j;
			int upperLeft = (i+1) * usize + j;
			
			RefTriangle r1 (lowerLeft , lowerLeft+1 , upperLeft + 1);
			RefTriangle r2 ( upperLeft +1 , upperLeft  , lowerLeft);
			mymesh->AddTriangle (r1);
			mymesh->AddTriangle (r2);
		}		
	}

	return mymesh;
}

VPoint SurfacePointGenerator::GetDervAprAlongU(const vreal &u,const vreal &v,const VPoint &p)const
{
	vreal temp = u + 1.e-5;//VGlobalValues::MYDERV_DIFF;
	//DoNothing (temp);
	vreal myderv_diff = temp - u;	
	
	VPoint dp;
	vreal ubef = u - myderv_diff;

	if ( ubef > 0.0)
	{
		VPoint pp = GetPoint (ubef,v);
		dp = p - pp;
	}
	else
	{
		vreal uaft = u + myderv_diff;
		VPoint np = GetPoint ( uaft,v);
		dp = np-p;
	}
	vreal inv_derv_diff = 1. / myderv_diff;
	return dp * inv_derv_diff;
}
VPoint SurfacePointGenerator::GetDervDervAprAlongU (const vreal &u,const vreal &v,const VPoint &p,const VPoint &derv)const
{
	vreal temp = u + 1.e-5;//VGlobalValues::MYDERV_DIFF;
	//DoNothing (temp);
	vreal myderv_diff = temp - u;	
	
	VPoint ddp;
	vreal ubef = u - myderv_diff;

	if ( ubef > 0.0)
	{
		VPoint pp = GetPoint (ubef,v);
		VPoint pd = GetDervAprAlongU (ubef,v,pp);
		ddp = derv - pd;
	}
	else
	{
		vreal uaft = u + myderv_diff;
		VPoint np = GetPoint ( uaft,v);
		VPoint nd = GetDervAprAlongU (uaft,v,np);
		ddp = nd-derv;
	}
	vreal inv_derv_diff = 1. / myderv_diff;
	return ddp * inv_derv_diff;
}
vreal SurfacePointGenerator::GetCurvatureAprAlongU (const vreal &u,const vreal &v,VPoint &derv,VPoint &dervderv)const
{
	// K = | derv x dervderv | / |derv^3|
	VPoint p = GetPoint (u,v);
	derv = GetDervAprAlongU (u,v,p);
	dervderv = GetDervDervAprAlongU (u,v,p,derv);
	VPoint cros = derv ^ dervderv;
	double denu = derv.GetLength();
	return cros.GetLength() / (denu*denu*denu);
}

VPoint SurfacePointGenerator::GetDervAprAlongV(const vreal &u,const vreal &v,const VPoint &p)const
{
	vreal temp = v + 1.0e-5;//+ VGlobalValues::MYDERV_DIFF;
	//DoNothing (temp);
	vreal myderv_diff = temp - v;	
	
	VPoint dp;
	vreal vbef = v - myderv_diff;

	if ( vbef > 0.0)
	{
		VPoint pp = GetPoint (u,vbef);
		dp = p - pp;
	}
	else
	{
		vreal vaft = v + myderv_diff;
		VPoint np = GetPoint ( u,vaft);
		dp = np-p;
	}
	vreal inv_derv_diff = 1. / myderv_diff;
	return dp * inv_derv_diff;
}
VPoint SurfacePointGenerator::GetDervDervAprAlongV (const vreal &u,const vreal &v,const VPoint &p,const VPoint &derv)const
{
	vreal temp = v + 1.0e-5;//VGlobalValues::MYDERV_DIFF;
	//DoNothing (temp);
	vreal myderv_diff = temp - v;	
	
	VPoint ddp;
	vreal vbef = v - myderv_diff;

	if ( vbef > 0.0)
	{
		VPoint pp = GetPoint (u,vbef);
		VPoint pd = GetDervAprAlongV (u,vbef,pp);
		ddp = derv - pd;
	}
	else
	{
		vreal vaft = v + myderv_diff;
		VPoint np = GetPoint ( u,vaft);
		VPoint nd = GetDervAprAlongV (u,vaft,np);
		ddp = nd-derv;
	}
	vreal inv_derv_diff = 1. / myderv_diff;
	return ddp * inv_derv_diff;
}
vreal SurfacePointGenerator::GetCurvatureAprAlongV (const vreal &u,const vreal &v,VPoint &derv
													,VPoint &dervderv)const
{
	// K = | derv x dervderv | / |derv^3|
	VPoint p = GetPoint (u,v);
	derv = GetDervAprAlongV (u,v,p);
	dervderv = GetDervDervAprAlongV (u,v,p,derv);
	VPoint cros = derv ^ dervderv;
	double denu = derv.GetLength();
	return cros.GetLength() / (denu*denu*denu);
}

VRefTriangleMesh * SurfacePointGenerator::DigitiseForModelSpaceStep (const vreal &modelSpaceStep)const
{
	VRefTriangleMesh *newmesh = new VRefTriangleMesh (400);
	DoubleVec udefint,vdefint;
	GetUDefIntervals (udefint);
	GetVDefIntervals (vdefint);
	int myusize = udefint.Size();
	int myvsize = vdefint.Size();
	
	vreal upb = 0.0;
	vreal vpb = 0.0;
	
	for (int v=1; v<myvsize; v++)
	{
		vreal vp = vdefint[v];
		for (int u=1;u<myusize; u++)
		{
			vreal up = udefint[u];
			DigitiseIntervalForModelSpaceStep (upb,up,vpb,vp,modelSpaceStep,newmesh);
			upb = up;
		}
		upb = 0.0;
		vpb = vp;
	}
	newmesh->CalculateNormals();
	return newmesh;
}

void SurfacePointGenerator::DigitiseIntervalForModelSpaceStep (const vreal &u1
															   ,const vreal &u2
															   ,const vreal &v1
															   ,const vreal &v2
															   ,const vreal &modelSpaceStep
															   ,VRefTriangleMesh *newmesh)const
{
	vreal umid = (u1+u2)*0.5;
	vreal vmid = (v1+v2)*0.5;
	vreal du = u2 - u1;
	vreal dv = v2 - v1;

	VPoint p1 = GetPoint (umid , vmid );
	VPoint p2 = GetPoint (umid + du * 0.1 , vmid );
	VPoint p3 = GetPoint (umid , vmid + dv * 0.1 );
	vreal apprModelSpaceChangePerU = (p2 - p1).GetLength() * du / (du * 0.1);
	vreal apprModelSpaceChangePerV = (p3 - p1).GetLength() * dv / (dv * 0.1);
	int ustepno = ((int) (apprModelSpaceChangePerU / modelSpaceStep)) + 1;
	int vstepno = ((int) (apprModelSpaceChangePerV / modelSpaceStep)) + 1;
	//if segment is linear no steps needed
	VPoint derv,dervderv;
	vreal curvU = this->GetCurvatureAprAlongU (umid,vmid,derv,dervderv);
	vreal curvV = this->GetCurvatureAprAlongV (umid,vmid,derv,dervderv);
	if (fabs(curvU) < 1.0e-7)
		ustepno = 1;
	if (fabs(curvV) < 1.0e-7)
		vstepno = 1;
	du /= ustepno;
	dv /= vstepno;
	vreal up = u1;
	vreal vp = v1;
	int refOffset = newmesh->surfpack.parampoints.Size();
	for (int v = 0; v <= vstepno; v++)
	{
		//if (v==vstepno)
		//	if ( fabs (vp - 1.0) > 1.0e-7)
		//		continue;

		for (int u=0; u <= ustepno; u++)
		{
			//if (u==ustepno)
			//	if (fabs (up - 1.0) > 1.0e-7)
			//		continue;

			VPoint2d p(up,vp);
			VPoint m = GetPoint (up,vp);
			VPoint n /*= GetNormal (up,vp,m);*/ (-1.,-1.,-1.);
			
			int myi = newmesh->surfpack.InsertPoint (p,m,n);
			// Add reftriangle
			if ( u != 0 && v != 0)
			{
				RefTriangle r1 (myi , myi -1 , (myi - 1) - (ustepno + 1));
				RefTriangle r2 ((myi - 1) - (ustepno + 1) , myi - (ustepno + 1), myi);
				newmesh->AddTriangle (r1);
				newmesh->AddTriangle (r2);
			}
			
			up += du;
		}
		up = u1;
		vp += dv;
	}
}

int SurfacePointGenerator::m_divideCountCalcMaxSegmentLimit = 10000;
int SurfacePointGenerator::m_divideCountCalcMaxSegmentNo = 32;

int SurfacePointGenerator::CalculateDivideCountForIntervalAlongU (const vreal u1 
															, const vreal u2
															, const vreal v
															,const vreal tol)const
{
	return NewCalcDivideCountForIntervalAlongU(u1,u2,v,tol);
	// linear calculation
	vreal umid = (u1+u2)*0.5;
	VPoint nominalPoint = GetPoint (umid,v);
	VPoint p1 = GetPoint (u1,v);
	VPoint p2 = GetPoint (u2,v);
	VPoint apprPoint = (p1+p2)*0.5;
	vreal err = (nominalPoint-apprPoint).GetLength();
	if (fabs(err) < EPSILON )
		return 1;
	vreal totalLength = fabs(u2-u1);
	vreal subLength = tol * totalLength / err;
	int segmentCount = (int)(totalLength / subLength);
	if (segmentCount > m_divideCountCalcMaxSegmentNo)
		segmentCount = m_divideCountCalcMaxSegmentNo;
	return segmentCount+1;

	//  making a circle approximation for interval of [p1 p2]
	//vreal mu = (u1+u2)*0.5;
	//VPoint derv,dervderv;
	//vreal K = GetCurvatureAprAlongU (mu,v,derv,dervderv);;
	//if (fabs(K) < 1.e-8)//allmost linear
	//	return 1;	
	//vreal rc = 1. / K;
	//vreal teta = (derv.GetLength() / rc) * (u2-u1);
	//// this interval is modelled as arc of radius rc and arc angle teta
	//vreal tmp = 1. - tol / rc;
	//if (tol > rc)
	//	tmp = 0.5;
	//vreal divideTeta = 2. * acos ( tmp);
	////int res = (int)(teta / divideTeta + 0.5);
	//int res = (int)(TWO_PI * (u2 - u1) / divideTeta + 0.5);
	//if (res == 0)
	//	++res;
	//return res;
}
int SurfacePointGenerator::CalculateDivideCountForIntervalAlongV (const vreal v1 
															, const vreal v2
															, const vreal u
															,const vreal tol)const
{
	return NewCalcDivideCountForIntervalAlongV(v1,v2,u,tol);
	// linear calculation
	vreal vmid = (v1+v2)*0.5;
	VPoint nominalPoint = this->GetPoint (u,vmid);
	VPoint p1 = GetPoint (u,v1);
	VPoint p2 = GetPoint (u,v2);
	VPoint apprPoint = (p1+p2)*0.5;
	vreal err = (nominalPoint-apprPoint).GetLength();
	if (fabs(err)<EPSILON)
		return 1;
	vreal totalLength = fabs (v2-v1);
	vreal subLength = tol * totalLength / err;
	int segmentCount = (int)(totalLength / subLength);
	if (segmentCount > m_divideCountCalcMaxSegmentNo)
		segmentCount = m_divideCountCalcMaxSegmentNo;
	return segmentCount+1;
	
	
	//  making a circle approximation for interval of [p1 p2]
	//vreal mv = (v1+v2)*0.5;
	//VPoint derv,dervderv;
	//vreal K = GetCurvatureAprAlongU (mv,u,derv,dervderv);;
	//if (fabs(K) < 1.e-8)//allmost linear
	//	return 1;	
	//vreal rc = 1. / K;
	//vreal teta = (derv.GetLength() / rc) * (v2-v1);
	//// this interval is modelled as arc of radius rc and arc angle teta
	//vreal tmp = 1. - tol / rc;
	//if (tol > rc)
	//	tmp = 0.5;
	//vreal divideTeta = 2. * acos ( tmp);
	//int res = (int)(TWO_PI *(v2-v1) / divideTeta + 0.5);
	//
	////int res = (int)(teta / divideTeta + 0.5);
	//if (res == 0)
	//	++res;
	//return res;
}

int SurfacePointGenerator::MAX_DIVIDE_COUNT = 400;
int DIVIDE_COUNT = 800;
int SurfacePointGenerator::DIVIDE_COUNT_FOR_ERROR = 10;

int SurfacePointGenerator::NewCalcDivideCountForIntervalAlongU (vreal u1,vreal u2,vreal v, vreal tol)const
{
	//vreal du = u2 - u1;
	//vreal ddu = du / DIVIDE_COUNT;
	//vreal prevu = u1;
	//vreal maxerr = MIN_NUMBER_LIMIT;
	//for (int i=1;i<DIVIDE_COUNT+1;++i)
	//{
	//	vreal curu= u1 + i*ddu;
	//	vreal curerr = GetApproximationErrorAlongU(prevu , curu , v);
	//	if (curerr >maxerr)
	//		maxerr = curerr;
	//	prevu = curu;
	//}
	//if (maxerr == MIN_NUMBER_LIMIT)
	//	throw VException (("SurfacePointGenerator::NewCalcDivideCount exception , maxerr is not filled!"),1);
	//if (maxerr > tol)
	//	return 1;
	//int dc = (int) ( (DIVIDE_COUNT * maxerr / tol) + 1);
	//if (dc > MAX_DIVIDE_COUNT)
	//	throw VException (("SurfacePointGenerator::NewCalcDivideCount exception , divide count exceeds max divide count!"),1);
	//return dc;
	int dividecount = 1;
	while (true)
	{
		vreal du = (u2 - u1) / dividecount;
		bool allpass = true;
		vreal maxerr = MAX_NUMBER_LIMIT;
		for (vreal u=u1;u<(u2-du)+1.e-7; u+=du)
		{
			maxerr = GetApproximationErrorAlongU(u,u+du,v);
			if (maxerr > tol)
			{
				allpass = false;
				break;
			} 
		}
		if (allpass)
			return dividecount;
		if (dividecount > MAX_DIVIDE_COUNT)
		{
			CString temp;
			temp.Format("Max approximation error along v is : %.4f", maxerr);
			LogFileWriter::WriteUnknownExceptionLog("SurfacePointGenerator::NewCalcDivideCountForInterval U max divide count exceed ", (LPCTSTR)temp);
			return DIVIDE_COUNT_FOR_ERROR;// throw VException((IDS_SurNewExcDivCou), 1);
		}
		++dividecount;
	}

}
int SurfacePointGenerator::NewCalcDivideCountForIntervalAlongV (vreal v1 , vreal v2 , vreal u , vreal tol)const
{
	int dividecount = 1;
	while (true)
	{
		vreal dv = (v2-v1) / dividecount;
		bool allpass = true;
		vreal maxerr = MAX_NUMBER_LIMIT;
		for (vreal v=v1;v<(v2-dv)+1.e-7; v+=dv)
		{
			maxerr = GetApproximationErrorAlongV(v,v+dv,u);
			if (maxerr > tol)
			{
				allpass = false;
				break;
			}
		}
		if (allpass)
			return dividecount;
		if (dividecount > MAX_DIVIDE_COUNT)
		{
			CString temp;
			temp.Format("Max approximation error along v is : %.4f",maxerr);
			LogFileWriter::WriteUnknownExceptionLog("SurfacePointGenerator::NewCalcDivideCountForInterval V max divide count exceed ",(LPCTSTR)temp);
			return DIVIDE_COUNT_FOR_ERROR;//throw VException (( IDS_SurNewAloVExcDi),1);
		}
		++dividecount;
	}
	//vreal dv = v2 - v1;
	//vreal ddv = dv / DIVIDE_COUNT;
	//vreal prevv = v1;
	//vreal maxerr = MIN_NUMBER_LIMIT;
	//for (int i=1;i<DIVIDE_COUNT+1;++i)
	//{
	//	vreal curv = v1 + i*ddv;
	//	vreal currerr = GetApproximationErrorAlongV(prevv , curv , u);
	//	if (currerr > maxerr)
	//		maxerr = currerr;
	//	prevv = curv;
	//}
	//if (maxerr == MIN_NUMBER_LIMIT)
	//	throw VException (("SurfacePointGenerator::NewCalcDivideCount exception , maxerr is not filled , v"),1 );
	//if (maxerr >= tol)
	//	return 1;
	//int dc = (int) ((DIVIDE_COUNT * maxerr / tol ) + 1);
	//if (dc > MAX_DIVIDE_COUNT)
	//	throw VException (("SurfacePointGenerator::NewCalcDivideCountAlongV exception , divide count exceeds max divide count!"),1);
	//return dc;
}
int APPR_DIGITISE_STEP = 6;
double SurfacePointGenerator::GetApproximationErrorAlongU (vreal u1 , vreal u2 , vreal v)const
{
	VPoint p1 = GetPoint (u1,v);
	VPoint p2 = GetPoint (u2,v);
	VLine l (p1,p2);
	LinePointMinDist mdc;
	vreal maxerr = MIN_NUMBER_LIMIT;
	VPoint temp;
	vreal du = (u2 - u1) / APPR_DIGITISE_STEP; 
	for (vreal ue=u1+du; ue<u2-EPSILON ;ue+=du)
	{
		VPoint pe = GetPoint (ue,v);
		vreal cd = mdc.MinDist (l,pe,temp);
		if (cd > maxerr)
			maxerr= cd;
	}
	return maxerr;
}

double SurfacePointGenerator::GetApproximationErrorAlongV (vreal v1 , vreal v2 , vreal u)const
{
	//vreal vmid = (v1+v2)*0.5;
	//VPoint nominalPoint = this->GetPoint (u,vmid);
	//VPoint p1 = GetPoint (u,v1);
	//VPoint p2 = GetPoint (u,v2);
	//VPoint apprPoint = (p1+p2)*0.5;
	//vreal err = (nominalPoint-apprPoint).GetLength();
	//return err;
	VPoint p1 = GetPoint(u,v1);
	VPoint p2 = GetPoint(u,v2);
	VLine l (p1,p2);
	LinePointMinDist mdc;
	vreal maxerr = MIN_NUMBER_LIMIT;
	VPoint temp;
	vreal dv = (v2- v1) / APPR_DIGITISE_STEP;
	for (vreal ve = v1 + dv; ve<v2-EPSILON; ve+=dv)
	{
		VPoint pe = GetPoint(u,ve);
		vreal cd = mdc.MinDist(l,pe,temp);
		if (cd > maxerr)
			maxerr = cd;
	}
	return maxerr;
}

void SurfacePointGenerator::InsertSegments (const int segNo 
											, const vreal p1 
											, const vreal p2
											, std::vector<vreal> &param2)const
{
	vreal delta = p2-p1;
	vreal myStep = delta / segNo;
	vreal newP2 = p1;

	for (int j=0; j<segNo; j++)
	{
		param2.push_back (newP2);
		newP2 += myStep;
	}
}

//void SurfacePointGenerator::SetUParShift (vreal uParShift)
//{
//	m_uParShift = uParShift;
//}
//
//vreal SurfacePointGenerator::GetUParShift ()const
//{
//	return m_uParShift;
//}
//
//void SurfacePointGenerator::SetVParShift (vreal vParShift)
//{
//	m_vParShift = vParShift;
//}
//
//vreal SurfacePointGenerator::GetVParShift ()const
//{
//	return m_vParShift;
//}

void SurfacePointGenerator::ShiftParIf (vreal u,vreal v,vreal &newu,vreal &newv)const
{
	//newu = u + m_uParShift;
	//newv = v + m_vParShift;
	newu = u;
	newv = v;
	if (u < -1.e-8 || u > 1.0+1.e-8)
	{
		if (m_isPeriodicInU)
		{
			newu = this->GetEquavalentInMainRange (u,m_uPeriod,0.0,1.0);
		}
		else
		{
			//throw VException ("SurfacePointGenerator::ShiftParIf exception , u parameter out of range",1);
			if (u<-1.e-8)
				u = 0.0;
			else
				u = 1.0;
		}
	}
	if (v < -1.e-8 || v > 1.0+1.e-8)
	{
		if (m_isPeriodicInV)
		{
			newv = this->GetEquavalentInMainRange (v,m_vPeriod,0.0,1.0);
		}
		else
		{
			//throw VException ("SurfacePointGenerator::ShiftParIf exception, v parameter out of range",1);
			if (v < -1.e-8)
				v=0.0;
			else
				v=1.0;
		}
	}
}

int SurfacePointGenerator::GetInitialDigitiseSegmentNo ()const
{
	return m_initialDigitiseSegmentNo;
}

void SurfacePointGenerator::SetInitialDigitiseSegmentNo(int initialDigitiseSegmentNo)
{
	m_initialDigitiseSegmentNo = initialDigitiseSegmentNo;
}
bool SurfacePointGenerator::GetIsPeriodicInU()const
{
	return m_isPeriodicInU;
}
void SurfacePointGenerator::SetIsPeriodicInU (bool isPeriodicInU)
{
	m_isPeriodicInU = isPeriodicInU;
}
bool SurfacePointGenerator::GetIsPeriodicInV()const
{
	return m_isPeriodicInV;
}
void SurfacePointGenerator::SetIsPeriodicInV (bool isPeriodicInV)
{
	m_isPeriodicInV = isPeriodicInV;
}
vreal SurfacePointGenerator::GetEquavalentInMainRange (vreal par,vreal period ,vreal t0, vreal t1)const
{
	vreal c1 = par / period;
	int c1i = (int)c1;
	vreal d1 = (c1-c1i);
	vreal c2 = t0 / period;
	int c2i = (int)c2;
	vreal d2 = (c2-c2i);
	vreal c3 = t1 / period;
	int c3i = (int)c3;
	vreal d3 = (c3-c3i);
	if (fabs(d3) < 1.e-7)
		d3 = period;
	if (d1 > d3 || d1 < d2)
	{
		throw VException ("SurfacePointGenerator::GetEquavalentInMainRange exception , par can not be in side given range!",1);
	}
	else
	{
		vreal newpar = (d1*period) + (c2i*period);
		return newpar;
	}
}

vreal SurfacePointGenerator::GetUPeriod()const
{
	return m_uPeriod;
}
vreal SurfacePointGenerator::GetVPeriod()const
{
	return m_vPeriod;
}
void SurfacePointGenerator::SetUPeriod(vreal uPeriod)
{
	m_uPeriod = uPeriod;
}
void SurfacePointGenerator::SetVPeriod(vreal vPeriod)
{
	m_vPeriod = vPeriod;
}

