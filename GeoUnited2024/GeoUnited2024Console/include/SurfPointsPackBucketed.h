// SurfPointsPackBucketed.h

#ifndef __SurfPointsPackBucketed_h__
#define __SurfPointsPackBucketed_h__

#include "SurfPointsPack.h"

// Bucketing is done according to 2d set and min-max for 2d set is assumed to be from 0,0 to 1,1

class SurfPointsPackBucketed
{
public:
	SurfPointsPackBucketed (size_t bucketNoInOneDirection = 30);
	const SurfPointsPack &GetPack()const;
	SurfPointsPack &GetPack();
	void GetBucketIndicesForOnePoint (const VPoint2d &p , size_t &xi , size_t &yi) const; // returns false if one of the xi,yi is out of interval
	int InsertPoint (const VPoint2d &par,const VPoint &mod,const VPoint &nor,double coinTol);
	const std::list<int> & GetPointsAround (const VPoint2d &p)const;
private:
	SurfPointsPack m_pack;
	size_t m_bucketNoInOneDirection;
    std::vector<std::vector<std::list<int> > > m_buckets;
};

#endif
