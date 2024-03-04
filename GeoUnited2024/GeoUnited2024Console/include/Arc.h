// Arc.h
// E 100
//Index__      Name____     Type___   Description___
//1            ZT           Real      Parallel ZT displacement of arc from XT, YT plane
//2            X1           Real      Arc center abscissa
//3            Y1           Real      Arc center ordinate
//4            X2           Real      Start point abscissa
//5            Y2           Real      Start point ordinate
//6            X3           Real      Terminate point abscissa
//7            Y3           Real      Terminate point ordinate

#ifndef __Arc_h__
#define __Arc_h__
#include "IgesEntity.h"
class VArc;

class igesArc:public IgesEntity
{
public:
	igesArc();
	virtual void GetEntityIn ( FileLines & mylines , int poffs);
	virtual void WriteEntityOut (std::vector<VStr> &strvec)const;
	virtual void GetChildren (std::vector<int> &childVec)const;
	virtual void AcceptVisitor (IgesEntityVisitor &visitor)const;	
	//virtual bool IsPrimitiveEntity()const;
	//virtual double GetSortNo ()const;
	virtual IgesEntity * MakeCopyAndAddToSet (const IgesEntitySet &mainSet , IgesEntitySet &newSet)const;
	virtual void OverrideBigZCoordinates (double zLimit , double zWork);

	vreal mZT,mX1,mY1,mX2,mY2,mX3,mY3;
	//VArc GetUntransformedVArc()const;
};

#endif