// CurveOnParametricSurface.h
//------------------------------------------------------------------------------------------
// CURVE ON A PARAMETRIC SURFACE ENTITY (TYPE 142)
// index	name	type	description
// ------	----	----	----------------------------------------------------------------
// 1	|	CRTN  |	int		|indicates the way the curve on surface has been created
//		|		  |			|0=unspecified 1=projection of a given curve on surface
//		|		  |			|2=intersection of 2 surfaces 3=isoparametric u or v param curve	
//------------------------------------------------------------------------------------------								
// 2	|	SPTR  |	pointer	|pointer to the DE of surface on which the curve lies.
//------------------------------------------------------------------------------------------
// 3	|	BPTR  |	pointer	|pointer to the DE of the entity that contains the definition of
//		|		  |			|the curve B in parametric space (u,v) of the surface.			
//------------------------------------------------------------------------------------------
// 4	|	CPTR  |pointer  |pointer to DE ofcurve C
//------------------------------------------------------------------------------------------
// 5	|   PREF  |	int		|indicates the preffered representation in the sending system.
//		|		  |			|0=unspecified 1=SOB is preffered 2=C is preffered 3=C and SOB
//		|		  |			|equally preffered.
//------------------------------------------------------------------------------------------

#ifndef __CurveOnParametricSurface_h__
#define __CurveOnParametricSurface_h__

#include "IgesEntity.h"

class CurveOnParametricSurface : public IgesEntity
{
public:
	CurveOnParametricSurface();
	~CurveOnParametricSurface();
	virtual void GetEntityIn( FileLines & mylines , int poffs);
	virtual void WriteEntityOut (std::vector <VStr> &strvec)const; 
	virtual void GetChildren (std::vector<int> &childVec)const;
	virtual void AcceptVisitor (IgesEntityVisitor &visitor)const;
	//virtual bool IsPrimitiveEntity()const;
	//virtual double GetSortNo ()const;
	virtual IgesEntity * MakeCopyAndAddToSet (const IgesEntitySet &mainSet , IgesEntitySet &newSet)const;

//private:
	int CRTN;
	long SPTR;
	long BPTR;	
	long CPTR;
	int PREF;

};

#endif // CurveOnParametricSurface
