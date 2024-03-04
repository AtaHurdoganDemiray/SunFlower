// BSplineNFormer.h

#ifndef __BSplineNFormer_h__
#define __BSplineNFormer_h__

#include "BSplineNSet.h"

class BSplineN;
class BSplineKnots;

class BSplineNFormer
{
public:
	BSplineNFormer ( const BSplineKnots & knots , const vreal & param );
	BSplineNFormer ( const BSplineNFormer & parentformer );	
	virtual ~BSplineNFormer ();
	const BSplineNSet & Form ();

protected:
	BSplineNSet nset;
	const BSplineKnots & _knots;
	vreal _param;
	BSplineNFormer * nextformer;
	
	BSplineN GetStartN();

};

#endif
