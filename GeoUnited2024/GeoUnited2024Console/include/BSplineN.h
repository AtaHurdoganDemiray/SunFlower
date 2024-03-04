// BSplineN.h

#ifndef __BSplineN_h__
#define __BSplineN_h__

class BSplineKnots;

class BSplineN
{
public:

	BSplineN ( const BSplineN & parent1 , const BSplineN & parent2 
				, const BSplineKnots & knots , const vreal & param );

	BSplineN ( const BSplineN & singleparent , int _i
				, const BSplineKnots & knots , const vreal & param );

	BSplineN ( int _i , const BSplineKnots & knots );

	const int & Geti () const;
	const int & Getk () const;
	const vreal & GetValue () const;

protected:
	int i , k;
	vreal value;
		
	void Seti ( const BSplineN & parent1 , const BSplineN & parent2 );
	void Seti ( const BSplineN & singleparent , int _i );
	void Seti ( int _i );
	void Setk ( const BSplineN & parent1 , const BSplineN & parent2 );
	void Setk ( const BSplineN & singleparent );
	void Setk ( int _k );
	void SetValue ( const BSplineN & parent1 , const BSplineN & parent2 
					, const BSplineKnots & knots , const vreal & param );
	void SetValue ( const BSplineN & singleparent 
					, const BSplineKnots & knots , const vreal & param );
	void SetValue ( const vreal & param );
};

#endif