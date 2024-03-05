// BspTreeToLineSet2d.h

#ifndef __BspTreeToLineSet2d_h__
#define __BspTreeToLineSet2d_h__

class LineSet2d;
struct VLine2d;
class SequentialLineSet2d;

namespace geo
{
	class BspTree;
	
	class BspTreeToLineSet2d
	{
	public:
		LineSet2d * Make (const BspTree &T); 
	private:
		void Add (const BspTree &T , LineSet2d &s);
		void AddLineIf (LineSet2d &s , std::list<const VLine2d*> &tmp,const VLine2d &l);

	};
};
#endif