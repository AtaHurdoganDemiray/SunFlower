// BspTreeConstructor.h

#ifndef __BspTreeConstructor_h__
#define __BspTreeConstructor_h__

class LineSet2d;
struct VLine2d;
class SequentialLineSet2d;

namespace geo
{
	class BspTree;

	class BspTreeConstructor
	{
	public:
		BspTree * ConstructTree (LineSet2d &L);
		BspTree * ConstructTree (const SequentialLineSet2d &SL);
		BspTree * ConstructTree (std::list <VLine2d> &L);	
		BspTree * MakeInfiniteSetBSP ();
	private:
		BspTree * ConstructTree (std::list <VLine2d *> &L);
		std::list<VLine2d> m_divided;
	};
};
#endif