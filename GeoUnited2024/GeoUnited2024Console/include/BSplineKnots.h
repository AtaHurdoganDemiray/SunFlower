// BSplineKnots.h

#ifndef __BSplineKnots_h__
#define __BSplineKnots_h__

struct DoubleVec;

class BSplineKnots
{
public:
	BSplineKnots ( const int & _k , const int & _n 
		, const vreal & _V0 , const vreal & _V1 , const std::vector<vreal> &knotarray );
	virtual ~BSplineKnots();
	int GetKnotNo () const;
	const vreal & GetKnot ( const int & i ) const;
	int GetStarti ( const vreal & v ) const;
	const vreal & GetV0 () const;
	const vreal & GetV1 () const;
	const int & Getn () const;
	const int & Getk () const;
	void GetDefIntervals ( DoubleVec & defint ) const;
	vreal Reparametrize_Range0_1ToV0_V1 ( const vreal &v ) const;
	vreal Reparametrize_RangeV0_V1To0_1 ( const vreal &v ) const;
	int GetSegmentNo (const vreal &v)const;
	vreal GetNaturalParam (const vreal &realV)const;

private:
	std::vector<vreal> t;
	int k,n;
	int knotno;
	void CalculateKnots ();
	vreal V0;
	vreal V1;
};

#endif //__BSplineKnots_h__
