// PartitionLine.h

#ifndef __PartitionLine_h__
#define __PartitionLine_h__
	
class LineSet2d;	
struct VLine2d;

namespace geo
{
	class BspTree;

	void GetPartition (const BspTree &T, const VLine2d &E 
						, LineSet2d &Pos , LineSet2d &Neg 
						, LineSet2d &CoSame , LineSet2d &CoDiff);
	void GetPosPartition (const BspTree &T , const VLine2d &E 
						, LineSet2d &Pos , LineSet2d &Neg
						, LineSet2d &CoSame , LineSet2d &CoDiff);
	void GetNegPartition (const BspTree &T , const VLine2d &E
						, LineSet2d &Pos , LineSet2d &Neg
						, LineSet2d &CoSame , LineSet2d &CoDiff);
	void AddIntersecting (const VLine2d &l1 , const VLine2d &l2 , LineSet2d &CoSame , LineSet2d &CoDiff);
	
	void SubtractFromLine (const VLine2d &l , const LineSet2d &coincidentSet , LineSet2d &diff);
	void GetParameterForCoincidentLine (const VLine2d &l1 , const VLine2d &l2 , vreal &s1, vreal &s2);

};

#endif