// BSplineSurfacePointGenerator.cpp

#include "stdafx.h"
#include "BSplineSurfacePointGenerator.h"
#include "BSplineSurface.h"
#include "BSplineNFormer.h"
#include "BSplineN.h"

#include "DoubleVec.h"
#include "VRefTriangleMesh.h"
#include "BSplineSurfacePointGenerator2.h"


bool IsCoin ( VPoint & p1 , VPoint & p2 )
{
	if ( fabs(p1.x()-p2.x()) < 1.e-3
		&& fabs(p1.y()-p2.y()) < 1.e-3
		&& fabs(p1.z()-p2.z()) < 1.e-3 )
		return true;
	
	return false;
}

BSplineSurfacePointGenerator::BSplineSurfacePointGenerator ( const BSplineSurface & surf )
:m_surf(surf) 
, SurfacePointGenerator ( surf.m_U0 , surf.m_U1 , surf.m_V0 , surf.m_V1 ) 
#if USENN
, m_NE (surf)
#else
, knots_u ( surf.GetDegreeOfBasisFunctionsU()+1 , surf.GetControlPointNoU()-1 , surf.m_U0 , surf.m_U1 , surf.m_S )
, knots_v ( surf.GetDegreeOfBasisFunctionsV()+1 , surf.GetControlPointNoV()-1 , surf.m_V0 , surf.m_V1 , surf.m_T )
, controlpoints ( surf.GetControlPointNoU() , surf.GetControlPointNoV() , surf.controlpoints )
, weights ( surf.GetControlPointNoU() , surf.GetControlPointNoV() , surf.m_W , surf.IsRational() )
#endif
{

}

VPoint BSplineSurfacePointGenerator::GetPoint ( const vreal & u , const vreal & v ) const
{
#if USENN
		vreal su,sv;
		ShiftParIf(u,v,su,sv);
		return m_NE.GetPoint (su,sv);
#else
	try
	{
	vreal myu,myv;
	
	if ( u > 1.0f )
		myu = 1.0f;
	else if ( u < 0.0f )
		myu = 0.0f;
	else
		myu = u;

	if ( v > 1.0f )
		myv = 1.0f;
	else if ( v < 0.0f )
		myv = 0.0f;
	else
		myv = v;

	vreal realu = knots_u.Reparametrize_Range0_1ToV0_V1 ( myu );
	vreal realv = knots_v.Reparametrize_Range0_1ToV0_V1 ( myv );
	BSplineNFormer former_u ( knots_u , realu );
	const BSplineNSet & nset_u = former_u.Form();
	BSplineNFormer former_v ( knots_v , realv );
	const BSplineNSet & nset_v = former_v.Form();
		
	vreal deno = 0.0f;
	
	vreal result[3] = { 0.0f , 0.0f , 0.0f };
	
	for ( int i = 0; i < nset_u.Size(); i++ )
	{
		const BSplineN &currNu = nset_u[i];
		
		for ( int j = 0; j < nset_v.Size(); j++ )
		{
			const BSplineN &currNv = nset_v[j];

			if ( weights.IsRational() == false )
			{
				result[0] += ( static_cast<vreal> (controlpoints.GetPoint ( currNu.Geti() , currNv.Geti() ).x() ) 
							* (currNu.GetValue()) 
							* (currNv.GetValue()) );
				result[1] += ( static_cast<vreal> (controlpoints.GetPoint ( currNu.Geti() , currNv.Geti() ).y() ) 
							* (currNu.GetValue()) 
							* (currNv.GetValue()) );
				result[2] += ( static_cast<vreal> (controlpoints.GetPoint ( currNu.Geti() , currNv.Geti() ).z() ) 
							* (currNu.GetValue()) 
							* (currNv.GetValue()) );

				deno += ( currNu.GetValue() * currNv.GetValue() );		
			}
			else
			{	
				result[0] += ( static_cast<vreal> ( controlpoints.GetPoint (currNu.Geti(),currNv.Geti()).x() ) 
							* (currNu.GetValue()) 
							* (currNv.GetValue()) 
							* (weights.GetWeight ( currNu.Geti() , currNv.Geti() )) );
				result[1] += ( static_cast<vreal> ( controlpoints.GetPoint (currNu.Geti(),currNv.Geti()).y() ) 
							* (currNu.GetValue()) 
							* (currNv.GetValue()) 
							* (weights.GetWeight ( currNu.Geti() , currNv.Geti() )) );
				result[2] += ( static_cast<vreal> ( controlpoints.GetPoint (currNu.Geti(),currNv.Geti()).z() ) 
							* (currNu.GetValue()) 
							* (currNv.GetValue()) 
							* (weights.GetWeight ( currNu.Geti() , currNv.Geti() )) );

				deno += ( ( currNu.GetValue() * currNv.GetValue() )		
							* weights.GetWeight ( currNu.Geti() , currNv.Geti() ) );
			
			}
		}
	}
	
	result[0] /= deno;
	result[1] /= deno;
	result[2] /= deno;

	VPoint re (result[0],result[1],result[2]);//( static_cast<vreal>(result[0]) , static_cast<vreal>(result[1]) , static_cast<vreal>(result[2]) );

#if TEST_GEN2
	VPoint mytest = TryGen2(myu,myv);
#endif

	return (re);
	
	}
	catch ( VException & e )
	{
		throw ( e );
		return ( VPoint ( 0,0,0 ) );
	}
#endif
}

VPoint BSplineSurfacePointGenerator::GetNormal ( const vreal & u , const vreal & v , const VPoint & surfpoint ) const
{
#if USENN
	vreal su,sv;
	ShiftParIf(u,v,su,sv);
	VPoint np , nn;
	m_NE.GetPointAndNormal (u,v,np,nn);
	return nn;
#else

	VPoint n = GetNormalAppr(u,v,surfpoint);
	// for debug
	//static NurbsSurfEval myeval (m_surf);
	//VPoint re2,rn2;
	//myeval.GetPointAndNormal (u,v,re2,rn2);
	//if (surfpoint.Compare (re2,0.0001) == false)
	//{
	//	throw VException (("eyvahlar") , 2);
	//}
	//if (n.Compare (rn2,0.0001)==false)
	//	throw VException (("eyvah"),3);
	return n;
#endif
}
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

void BSplineSurfacePointGenerator::GetUDefIntervals ( DoubleVec & defint ) const
{
#if USENN
	m_NE.GetUKnots().GetDefIntervals (defint);
#else
	knots_u.GetDefIntervals ( defint );
#endif
	
	if ( defint.Size() < 4 )
	{
		vreal compTol=VGlobalValues::ONEMICRON_TO_VUNIT * 10;
		if (defint.IsValueARepeat(0.25,compTol)==false)
			defint.Addvreal ( 0.25 );
		if(defint.IsValueARepeat(0.5,compTol)==false)
			defint.Addvreal ( 0.5 );
		if(defint.IsValueARepeat(0.75,compTol)==false)
			defint.Addvreal ( 0.75 );
		defint.Sort();
	}
}

void BSplineSurfacePointGenerator::GetVDefIntervals ( DoubleVec & defint ) const
{
#if USENN
	m_NE.GetVKnots().GetDefIntervals (defint);
#else
	knots_v.GetDefIntervals ( defint );
#endif
	if ( defint.Size() < 4 )
	{
		vreal compTol=VGlobalValues::ONEMICRON_TO_VUNIT * 10;
		if(defint.IsValueARepeat(0.25,compTol)==false)
			defint.Addvreal ( 0.25 );
		if(defint.IsValueARepeat(0.5,compTol)==false)		
			defint.Addvreal ( 0.5 );
		if(defint.IsValueARepeat(0.75,compTol)==false)
			defint.Addvreal ( 0.75 );
		defint.Sort();
	}
}

#if TEST_GEN2
VPoint BSplineSurfacePointGenerator::TryGen2 (const vreal &u , const vreal &v)const
{
	BSplineSurfacePointGenerator2 gen2 (m_surf);
	BSplineCurve *crv = gen2.MakeCurve (v);
	BSplineCurvePointGenerator crvgen (*crv);
	return crvgen.GetPoint(u);
}
#endif
#if USENN
VRefTriangleMesh * BSplineSurfacePointGenerator::Digitise (const vreal &tol)const
{
	return this->GenericDigitise(tol);
}
#else
VRefTriangleMesh * BSplineSurfacePointGenerator::Digitise(const vreal &tol)const
{
	DoubleVec uDefInt1;
	DoubleVec vDefInt1;
	GetUDefIntervals (uDefInt1);
	GetVDefIntervals (vDefInt1);
	int uSize = uDefInt1.Size()*4;
	int vSize = vDefInt1.Size()*4;
	int totalSize = uSize * vSize;
	VRefTriangleMesh *result = new VRefTriangleMesh(totalSize);
	SurfPointsPack &surfpack = result->surfpack;
	BSplineSurfacePointGenerator2 *gen = new BSplineSurfacePointGenerator2 (m_surf);
	gen->NetDivide (4,4,surfpack.modelpoints,surfpack.normals,surfpack.parampoints);
	delete gen;
	FormRefTriangles(result);
	return result;
}
#endif

void BSplineSurfacePointGenerator::GetPointAndNormal(const vreal &u
													 , const vreal &v
													 , VPoint &m
													 , VPoint &n) const
{
	vreal su,sv;
	ShiftParIf(u,v,su,sv);
	m_NE.GetPointAndNormal (su,sv,m,n);
}

NurbsSurfEval & BSplineSurfacePointGenerator::GetNE()
{
	return m_NE;
}

//bool BSplineSurfacePointGenerator::Unproject(const VPoint &mp, vreal &u, vreal &v , vreal tol)
//{
	//return SurfacePointGenerator::GenericUnproject(mp, u, v , tol);
//}

IgesEntity * BSplineSurfacePointGenerator::GetIgesEntity()
{
	return ((IgesEntity *)&m_surf);
}