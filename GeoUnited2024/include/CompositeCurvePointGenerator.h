// CompositeCurvePointGenerator.h

#ifndef __CompositeCurvePointGenerator_h__
#define __CompositeCurvePointGenerator_h__

#include "CurvePointGenerator.h"

class IgesEntity;
class IgesEntitySet;

class CompositeCurvePointGenerator:public CurvePointGenerator
{
public:
	CompositeCurvePointGenerator(IgesEntity &ent , IgesEntitySet &allent);	
	virtual ~CompositeCurvePointGenerator();
	virtual VPoint GetPoint (const double &v)const;
	virtual void GetDefIntervals (DoubleVec &defint)const;
private:
	std::vector<CurvePointGenerator *>m_allcurves;
	double abs2mem(const double &absolutePar,int &curveNo)const;
	double mem2abs(const double &memPar,const int &curveNo)const;
	unsigned int m_curvecount;
	double m_singleRange;
};

#endif
