// CompositeCurve.h , entity type 102
//------------------------------------------------------------------------------------------
// index	name	type	description
// ------	----	----	----------------------------------------------------------------
// 1	|	N	  |	int		|number of entities
//------------------------------------------------------------------------------------------								
// 2	|	DE1	  |	pointer	|pointer to the DE of the first constiuent entity.
//------------------------------------------------------------------------------------------
// 3	|	...	  |	...		|...		
//------------------------------------------------------------------------------------------
// 1+N	|	DEN	  |pointer  |pointer to DE of the last constiuent entity.
//------------------------------------------------------------------------------------------

#ifndef __CompositeCurve_h__
#define __CompositeCurve_h__

#include "IgesEntity.h"

class CompositeCurve : public IgesEntity
{
public:
	CompositeCurve();
	~CompositeCurve();
	virtual void GetEntityIn( FileLines & mylines , int poffs);
	virtual void WriteEntityOut (std::vector <VStr> &strvec )const;
	virtual void GetChildren (std::vector<int> &childVec)const;
	virtual void AcceptVisitor (IgesEntityVisitor &visitor)const;
	//virtual bool IsPrimitiveEntity()const;
	//virtual double GetSortNo ()const;
	virtual IgesEntity * MakeCopyAndAddToSet (const IgesEntitySet &mainSet , IgesEntitySet &newSet)const;

//private:
	int N;
	std::vector<int> mDE;

};

#endif // CompositeCurve