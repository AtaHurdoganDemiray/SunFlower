// igesTransformationMatrix.h
// E124
//Index__      Name____     Type___   Description___
// 1           R11         Real      Top Row
// 2           R12         Real      .
// 3           R13         Real      .
// 4           T1           Real      .
// 5           R21         Real      Second Row
// 6           R22         Real      .
// 7           R23         Real      .
// 8           T2           Real      .
// 9           R31         Real      Third Row
//10           R32         Real      .
//11           R33         Real      .
//12           T3           Real      .

#ifndef __igesTransformationMatrix_h__
#define __igesTransformationMatrix_h__

#include "IgesEntity.h"

class igesTransformationMatrix : public IgesEntity 
{
public:
	igesTransformationMatrix ();
	//igesTransformationMatrix & operator = (const igesTransformationMatrix &);
	virtual void GetEntityIn ( FileLines & mylines , int poffs);
	virtual void WriteEntityOut (std::vector <VStr> &strvec)const;
	virtual void GetChildren (std::vector<int> &childVec)const;
	virtual void AcceptVisitor (IgesEntityVisitor &visitor)const;
	//virtual bool IsPrimitiveEntity()const;
	//virtual double GetSortNo ()const;
	virtual IgesEntity * MakeCopyAndAddToSet (const IgesEntitySet &mainSet , IgesEntitySet &newSet)const;
	virtual void OverrideBigZCoordinates (double zLimit , double zWork);
	bool IsEqualavent (const igesTransformationMatrix &toCompare)const;
	vreal mR11,mR12,mR13,mR21,mR22,mR23,mR31,mR32,mR33;
	vreal mT1,mT2,mT3;
	//int m_prev;
};

#endif