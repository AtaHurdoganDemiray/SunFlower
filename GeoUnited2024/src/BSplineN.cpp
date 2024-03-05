// BsplineN.cpp

#include "stdafx.h"
#include "BSplineN.h"
#include "BSplineKnots.h"

#include "BasicDefinitions.h"

BSplineN::BSplineN ( const BSplineN &parent1 , const BSplineN & parent2
					, const BSplineKnots & knots , const vreal & param ) 
{
	Seti ( parent1 , parent2 );
	Setk ( parent1 , parent2 );
	SetValue ( parent1 , parent2 , knots, param );
}

BSplineN::BSplineN ( const BSplineN & singleparent , int _i , const BSplineKnots & knots , const vreal & param ) 
{
	Seti ( singleparent , _i );
	Setk ( singleparent );
	SetValue ( singleparent , knots , param );
}

BSplineN::BSplineN ( int _i , const BSplineKnots & knots )
{
	Seti ( _i );
	Setk ( 1 );
	SetValue ( 1.0 );
}

const int & BSplineN::Geti () const
{
	return i; 
}

const int & BSplineN::Getk () const 
{
	return k;
}

const vreal & BSplineN::GetValue () const
{
	return value;
}

void BSplineN::Seti ( const BSplineN & parent1 , const BSplineN & parent2 )
{
	if ( (parent1.Geti() != parent2.Geti()+1) 
		&& (parent1.Geti()+1 != parent2.Geti()) )
		throw ( VException ( ( IDS_BSpExc2) , 0 ) );
	
	if ( parent1.Geti() == parent2.Geti()-1 )
		i = parent1.Geti();
	else if ( parent2.Geti() == parent1.Geti()-1 )
		i = parent2.Geti();
}

void BSplineN::Seti ( const BSplineN & singleparent , int _i )
{
	if ( singleparent.Geti() - 1 != _i && singleparent.Geti() != _i )
		throw ( VException ( ( IDS_BSpExc3) , 0 ) );

	i = _i;
}

void BSplineN::Seti ( int _i )
{
	i = _i;
}

void BSplineN::Setk ( const BSplineN & parent1 , const BSplineN & parent2 )
{
	if ( parent1.Getk() != parent2.Getk() )
		throw ( VException ( ( IDS_BSpExc4) , 0 ) );

	k = parent1.Getk() + 1;
}

void BSplineN::Setk ( const BSplineN & singleparent ) 
{
	k = singleparent.Getk() + 1;
}

void BSplineN::Setk ( int _k )
{
	k = _k;
}

void BSplineN::SetValue ( const BSplineN & parent1 , const BSplineN & parent2 
						 , const BSplineKnots & knots , const vreal & param )
{
	vreal ti = knots.GetKnot ( i );
	int index1 = i + k - 1;
	vreal tindex1 = knots.GetKnot ( index1 );
	
	vreal tinext = knots.GetKnot ( i+1 );
	int index2 = i + k;
	vreal tindex2 = knots.GetKnot ( index2 );

	vreal Ni , Ninext;
	
	if ( parent1.Geti() == i )
	{
		Ni = parent1.GetValue();
		Ninext = parent2.GetValue();
	}
	else
	{
		Ni = parent2.GetValue();
		Ninext = parent1.GetValue();
	}
	
	vreal val1,val2;
	
	vreal numr = (param-ti)*Ni;
	vreal denumr = tindex1-ti;
	if ( fabs(numr)<F_EPSILON && fabs(denumr)<EPSILON/*F_EPSILON*/ )
		val1 = Ni;
	else
		val1 = numr / denumr;
	
	numr = (tindex2-param)*Ninext;
	denumr = (tindex2-tinext);
	if ( fabs(numr)<F_EPSILON && fabs(denumr)<EPSILON/*F_EPSILON*/ ) 
		val2 = Ninext;
	else
		val2 = numr / denumr;
	
	value = val1+val2;   
	
}

void BSplineN::SetValue ( const BSplineN & singleparent  
						 , const BSplineKnots & knots , const vreal & param )
{
	if ( i == singleparent.Geti() - 1 )
	{
		vreal tinext = knots.GetKnot ( i+1 );
		int index2 = i + k;
		vreal tindex2 = knots.GetKnot ( index2 );
		vreal Ninext = singleparent.GetValue();
		
		vreal numr = (tindex2-param)*Ninext; 
		vreal denumr = (tindex2-tinext);
		if ( fabs(numr)<F_EPSILON && fabs(denumr)<EPSILON/*F_EPSILON*/ )
			value = Ninext;
		else
			value = numr / denumr;  
	}
	else if ( i == singleparent.Geti () )
	{
		vreal ti = knots.GetKnot ( i );
		int index1 = i + k - 1;
		vreal tindex1 = knots.GetKnot ( index1 );
		vreal Ni = singleparent.GetValue();
		
		vreal numr = (param-ti)*Ni;
		vreal denumr = (tindex1-ti);
		if ( fabs(numr)<F_EPSILON && fabs(denumr)<EPSILON/*F_EPSILON*/ )
			value = Ni;
		else
			value = numr / denumr;
	}

}

void BSplineN::SetValue ( const vreal & newvalue )
{
	value = newvalue;
}



