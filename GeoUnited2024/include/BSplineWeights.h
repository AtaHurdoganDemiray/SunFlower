// BSplineWeights

#ifndef __BSplineWeights_h__
#define __BSplineWeights_h__

class BSplineWeights
{
public:
	BSplineWeights ( int _weightno , const std::vector<vreal> &_weights , bool _rational );
	BSplineWeights ( int _upointno , int _vpointno , const std::vector<vreal> &_weights , bool _rational ); 
	virtual ~BSplineWeights();
	const vreal & GetWeight ( int i ) const; 
	const vreal & GetWeight ( int iu , int iv ) const;
	int GetWeightNo() const;
	bool IsRational () const;

protected:
	int weightno;
	int upointno;
	std::vector<vreal> weights;
	bool rational;
};

#endif