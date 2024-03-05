// Line.h
// E 110
//Parameter Data
//
//
//Index__      Name____     Type___   Description___
//1            X1           Real      Start Point P 1
//2            Y1           Real
//3            Z1           Real
//4            X2           Real      Terminate Point P 2
//5            Y2           Real
//6            Z2           Real

#ifndef __Line_h__
#define __Line_h__

#include "IgesEntity.h"

class Line : public IgesEntity
{
public:
	Line ();
	virtual void GetEntityIn( FileLines & mylines , int poffs);
	virtual void WriteEntityOut (std::vector<VStr> &strvec)const;
	virtual void GetChildren (std::vector<int> &childVec)const;
	virtual void AcceptVisitor (IgesEntityVisitor &visitor)const;
	//virtual bool IsPrimitiveEntity()const;
	//virtual double GetSortNo ()const;
	virtual IgesEntity * MakeCopyAndAddToSet (const IgesEntitySet &mainSet , IgesEntitySet &newSet)const;
	virtual void OverrideBigZCoordinates (double zLimit , double zWork);

	vreal X1 , Y1 , Z1; // start point
	vreal X2 , Y2 , Z2; // terminate point

};

#endif