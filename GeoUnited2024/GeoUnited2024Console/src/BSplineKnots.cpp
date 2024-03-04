// BSplineKnots.cpp

#include "stdafx.h"
#include "BSplineKnots.h"

#include "DoubleVec.h"
#include "BasicDefinitions.h"

BSplineKnots::BSplineKnots ( const int & _k , const int & _n 
							, const vreal & _V0 , const vreal & _V1 , const std::vector<vreal> &knotarray )
							:t(knotarray)
{
	k = _k;
	n = _n;
	knotno = n + k + 1;
	V0 = _V0;
	V1 = _V1;
	//bool doRepar = false;
	//if (V0 - t.at(0) > 1.e-4 || t.at(knotno - 1) - V1  > 1.e-4)
	//{
	//	doRepar = true;
	//}
	//if (doRepar)
	//{
	//	vreal deltapar = V1 - V0;
	//	for (int i = 0; i < knotno; ++i)
	//	{
	//		vreal curpar = t[i];
	//		vreal newpar = V0 + curpar*deltapar;
	//		t[i] = newpar;
	//	}
	//}
	////t = new vreal [knotno];
	
	////for ( int i = 0; i < knotno; i++ )
	////{
	////	t[i] = knotarray[i];
	////}
}

//#############################################################################

BSplineKnots::~BSplineKnots()
{
	//delete [] t;
}

//#############################################################################

void BSplineKnots::CalculateKnots ()
{
	if ( knotno <= 0 )
		throw ( VException ( ( IDS_BSpExc) , 0 ) );

	//t = new vreal [knotno];
	t.resize(knotno,0.0);

	for ( int i = 0; i < knotno; i++ )
	{
		if( i < k )
		{
			t[i] = 0;
		}
		else if ( i >= k  && i <= n )
		{
			t[i] = static_cast<vreal>(i - k + 1);
		}
		else if ( i > n )
		{
			t[i] = static_cast<vreal>(n - k + 2);
		}
	}

}

int BSplineKnots::GetStarti ( const vreal & v ) const 
{
	//if ( v < V0-1.E-4/*EPSILON*/ || v > V1+1.E-7/*EPSILON*/ )
	//{
	//	VOStrStream  mystream;
	//	mystream<<V0<<("<-V0 ")<<V1<<("<-V1 ")<< v<<("<-v")<<(" BSplineKnots::GetStarti exception");
	//	throw ( VException ( mystream.str() , 0 ) );
	//}

	//int baseknot = static_cast <int> (v);
	
	// ti = i - k + 1 if k<=i<=n    ti = baseknot
	//int result = baseknot + k - 1;

//	int result;

	if ( fabs ( v - V0 ) < 1.E-4/*EPSILON*//*F_EPSILON*/ )
		return (k-1);

	//if (fabs(v - V1) < 1.E-4/*EPSILON*//*F_EPSILON*/)
	//{
		//return n;
		//return n - 1;
	//}
	
	vreal prevKnot = t[0];
	
	for ( int i = 1; i < knotno; i++ )
	{
		vreal currKnot = t[i];
		
		if ( v <= currKnot && v >= prevKnot )
			return (i-1);	

		prevKnot = currKnot;
	}

	throw (VException ( ( IDS_BSpExc1) , 0 ) );
}

const vreal & BSplineKnots::GetKnot ( const int & i ) const
{
//	if ( i >= knotno )
//		throw ( VException ( ("BSplineKnots exception 3") , 0 ) );

	return t[i];
}

int BSplineKnots::GetKnotNo () const
{
	return knotno;
}

const vreal & BSplineKnots::GetV0 () const
{
	return V0;
}

const vreal & BSplineKnots::GetV1 () const
{
	return V1;
}

vreal BSplineKnots::Reparametrize_Range0_1ToV0_V1 ( const vreal & v ) const
{
	vreal newv = V0 + v * ( V1 - V0 );
	if (newv < V0)
		newv=V0;
	if (newv>V1)
		newv=V1;
	return newv;
}

vreal BSplineKnots::Reparametrize_RangeV0_V1To0_1 ( const vreal & v ) const
{
	vreal newv = ( v - V0 ) / ( V1 - V0 );
	return newv;
}

const int & BSplineKnots::Getk () const
{
	return k;
}

const int & BSplineKnots::Getn () const
{
	return n;
}

void BSplineKnots::GetDefIntervals ( DoubleVec & defint ) const
{
	int contp_no = n + 1;
	int degree = k - 1;
	vreal prevdefpoint = -1.0;
	for ( int i=degree; i <= contp_no; i++ )
	{
		vreal tmp = GetKnot ( i );
		if (i == degree)
			tmp = V0;
		if (i == contp_no)
			tmp = V1;
		vreal defintpoint = Reparametrize_RangeV0_V1To0_1 (tmp);
		if ( fabs(defintpoint-prevdefpoint) > 1.0e-6f )
			defint.Addvreal (defintpoint);
		prevdefpoint = defintpoint;
	}
}

int BSplineKnots::GetSegmentNo (const vreal &v)const
{
	return (GetStarti (v) - (k-1));
}

vreal BSplineKnots::GetNaturalParam (const vreal &realV)const
{
	int si = GetStarti (realV);
	vreal delta = (V1-V0)/(n+1);
	vreal diff =  realV - si * delta;
	vreal rediff = diff / delta;
	vreal result = rediff + (vreal)(si);
	return result;
}
