// LineSetsMinDist.h

#ifndef __LineSetsMinDist_h__
#define __LineSetsMinDist_h__

#include <map>

struct VLine;
class SequentialLineSet;
struct VPoint;

class LineSetsMinDist
{
public:
	static void Calc (const SequentialLineSet &sl1 
					, const SequentialLineSet &sl2
					, vreal &mindist 
					, VLine &minline
					, int *set1i = 0
					, int *set2i = 0);
	static void GetNearer (const SequentialLineSet &sl1
							, const SequentialLineSet &sl2
							//, std::vector<VPoint> &r1
							//, std::vector<VPoint> &r2
							, std::multimap<double,VLine> &r		// the double is the distance between 2 lines and line's first point is on sl1 and second point on sl2
							, double distthresholdmax);
	static void GetNearer(const SequentialLineSet& sl1
		, const SequentialLineSet& sl2
		, std::list<std::pair<VPoint, VPoint>>& closePointPairsOnSl1Sl2
		, vreal distthresholdmax);
private:
	

};

#endif