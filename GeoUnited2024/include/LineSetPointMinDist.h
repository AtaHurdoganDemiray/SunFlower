// LineSetPointMinDist.h

#ifndef __LineSetPointMinDist_h__
#define __LineSetPointMinDist_h__

#include "PosCubes.h"
class LineSet;

struct VPoint;

class LineSetPointMinDist
{
public:
	LineSetPointMinDist ( const LineSet & newset );
	vreal MinDist ( const VPoint & p , VPoint & pointonset , int *seti = 0) const;
	
protected:
	const LineSet & set;
	PosCubes cubes;
};

class SequentialLineSetPointMinDist
{
public:
	SequentialLineSetPointMinDist (const SequentialLineSet &newset);
	vreal MinDist (const VPoint &p , VPoint &pointonset , int *seti = 0) const;

protected:
	const SequentialLineSet &set;
	PosCubes cubes;
};

#endif