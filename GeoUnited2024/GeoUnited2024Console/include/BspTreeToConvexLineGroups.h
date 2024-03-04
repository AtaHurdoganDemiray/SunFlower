// BspTreeToConvexLineGroups.h

#ifndef __BspTreeToConvexLineGroups_h__
#define __BspTreeToConvexLineGroups_h__

struct VLine2d;

namespace geo
{
	class BspTree;

	class BspTreeToConvexLineGroups
	{
	public:
		static void Convert (const BspTree &bsp 
			, std::vector < std::vector <VLine2d> > &result);
	private:
		static void MakeOneLoop (bool isPos 
							, const BspTree &absp 
							, std::vector<VLine2d> &toAdd 
							, std::vector<std::vector<VLine2d>> &mainVec);

	};
};
#endif