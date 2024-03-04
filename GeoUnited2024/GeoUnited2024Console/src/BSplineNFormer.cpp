// BSplineNFormer.cpp

#include "stdafx.h"
#include "BSplineNFormer.h"
#include "BSplineN.h"
#include "BSplineKnots.h"


BSplineNFormer::BSplineNFormer ( const BSplineKnots & knots , const vreal &param )
:_knots (knots) , _param (param)
{
	BSplineN startN = GetStartN ();
	BSplineN newn1 ( startN , startN.Geti() , knots , param );
	BSplineN newn2 ( startN , startN.Geti()-1 , knots , param );
	nset.AddItem ( newn1 );
	nset.AddItem ( newn2 );
	nextformer = 0;
}

BSplineNFormer::BSplineNFormer ( const BSplineNFormer & parentformer )
: _knots ( parentformer._knots ) , _param ( parentformer._param )
{
	const BSplineN * currN = &(parentformer.nset[0]);
	BSplineN newN ( *currN , currN->Geti () , _knots , _param );
	nset.AddItem ( newN );
	const BSplineN * prevN = currN;

	for ( int i = 1; i < parentformer.nset.Size(); i++ )
	{
		currN = &(parentformer.nset[i]);

		BSplineN newN ( *currN , *prevN , _knots , _param );
		nset.AddItem ( newN );
		prevN = currN;	
	}
		
	BSplineN lastN ( *prevN , prevN->Geti()-1 , _knots , _param );
	nset.AddItem ( lastN );
	
	nextformer = 0;
}

BSplineNFormer::~BSplineNFormer ()
{
	if ( nextformer != 0 )
		delete nextformer;
}

const BSplineNSet & BSplineNFormer::Form ()
{
	BSplineNFormer * currFormer = this;

	for ( int i = 2; i < _knots.Getk(); i++ )
	{
		currFormer->nextformer = new BSplineNFormer ( *currFormer );
		currFormer = currFormer->nextformer;
	}

	return currFormer->nset;
}

BSplineN BSplineNFormer::GetStartN ()
{
	int starti = _knots.GetStarti ( _param );
	return ( BSplineN ( starti , _knots ) );
}
