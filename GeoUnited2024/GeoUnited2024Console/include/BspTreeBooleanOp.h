// BspTreeBooleanOp.h

#ifndef __BspTreeBooleanOp_h__
#define __BspTreeBooleanOp_h__

class LineSet2d;
struct VLine2d;

namespace geo
{
	class BspTree;
	class BspTreeBooleanOp
	{
	public:
		static BspTree * Unite (const BspTree &first , const BspTree &second);
		static BspTree * Subtract (const BspTree &first , const BspTree &second);
		static BspTree * Intersect (const BspTree &first , const BspTree &second);
		static BspTree * Negate (const BspTree &t);
		static void NegateThis (BspTree &t);
		static BspTree * ExclusiveOr (const BspTree &first , const BspTree &second);
		static void CheckIfEmpty (BspTree &t);
	private:
		static void AddIntersection(const BspTree &T , const VLine2d &l , LineSet2d &intr);
		static void Intersect (const BspTree &first , const BspTree &second , LineSet2d &intr);
	
	};
};
#endif