// BSplineWeights.cpp

#include "stdafx.h"
#include "BSplineWeights.h"


BSplineWeights::BSplineWeights ( int _weightno , const std::vector<vreal> &_weights , bool _rational )
:weights (_weights)
{
	weightno = _weightno;
	upointno = weightno;
	//weights = new vreal [weightno];

	//for ( int i = 0; i < weightno; i++ )
	//{
	//	weights[i] = _weights[i];
	//}

	rational = _rational;
}

BSplineWeights::BSplineWeights ( int _upointno , int _vpointno , const std::vector<vreal> &_weights , bool _rational )
:weights(_weights)
{
	weightno = _upointno * _vpointno;
	upointno = _upointno;
	
	//weights = new vreal [weightno];

	//for ( int i = 0; i < weightno; i++ )
	//{
	//	weights[i] = _weights[i];
	//}

	rational = _rational;
}

BSplineWeights::~BSplineWeights ()
{
	//delete [] weights;
}

int BSplineWeights::GetWeightNo() const
{
	return weightno;
}

const vreal & BSplineWeights::GetWeight ( int i ) const
{
	if ( i < 0 || i >= weightno )
		throw ( VException ( ( IDS_BSpExc5) , 1 ) );

	return (weights[i]);
}

bool BSplineWeights::IsRational () const
{
	return rational;
}

const vreal & BSplineWeights::GetWeight ( int iu , int iv ) const
{
	int no = iv * upointno + iu;

	return ( GetWeight ( no ) );
}
