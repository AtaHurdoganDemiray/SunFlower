// MultiPointGeneratorForCompositeCurve.h
#ifndef __MultiPointGeneratorForCompositeCurve_h__
#define __MultiPointGeneratorForCompositeCurve_h__
#include "CurvePointGenerator.h"

class IgesEntity;
class IgesEntitySet;

class MultiPointGeneratorForCompositeCurve
{
public:
	MultiPointGeneratorForCompositeCurve();
	virtual	~MultiPointGeneratorForCompositeCurve();
	void Add(IgesEntity *ent , IgesEntitySet &allent);
	void FinishAdding();
	bool IsSequential()const;
	bool IsDone()const;
	std::vector<Curve *>m_allcurves;
protected:
	bool CheckIfSeq (Curve *lastcurve , Curve *newcurve)const;
	bool m_sequential;
	bool m_done;
};

#endif
