// TrimmedSurface.h

// Type 144
// PTS : pointer to directory entry of surface.
// N1 : (int) 0 = outer boundary is surface limits 1 = otherwise.
// N2 : (int) number of inner boundaries.
// PTO : pointer to DE of simple closed curve entity (curve on parametric surface)
//		 for outer boundary.
// PTI : pointers to DE of inner boundary curves.

#ifndef __TrimmedSurface_h__
#define __TrimmedSurface_h__

#include "IgesEntity.h"

class TrimmedSurface : public IgesEntity
{
public:
	TrimmedSurface();
	~TrimmedSurface();
	virtual void GetEntityIn( FileLines & mylines , int poffs);
	virtual void WriteEntityOut (std::vector <VStr> &strvec)const;
	virtual void GetChildren (std::vector<int> &childVec)const;
	virtual void AcceptVisitor (IgesEntityVisitor &visitor)const;
	//virtual bool IsPrimitiveEntity()const;
	//virtual double GetSortNo ()const;
	virtual IgesEntity * MakeCopyAndAddToSet (const IgesEntitySet &mainSet , IgesEntitySet &newSet)const;

//private:
	int PTS;
	int N1;
	int N2;
	int PTO;
	std::vector<int> PTI;

};

#endif // TrimmedSurface