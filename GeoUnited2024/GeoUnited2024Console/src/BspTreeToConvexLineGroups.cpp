// BspTreeToConvexLineGroups.cpp

#include "stdafx.h"
#include "BspTreeToConvexLineGroups.h"
#include "BspTree.h"

namespace geo
{

	void BspTreeToConvexLineGroups::Convert(const BspTree &bsp
							, std::vector<std::vector<VLine2d> > &result)
	{
		const BspTree *posc = bsp.GetPosChild();
		if (posc)
		{
			std::vector<VLine2d> newVec;
			result.push_back(newVec);
			MakeOneLoop (true,*posc,result.back(),result);
		}
		const BspTree *negc = bsp.GetNegChild();
		if (negc)
		{
			std::vector<VLine2d> newVec;
			result.push_back(newVec);
			MakeOneLoop (false,*negc,result.back(),result);
		}
	}

	void BspTreeToConvexLineGroups::MakeOneLoop (bool isPos 
											, const BspTree &absp 
											, std::vector<VLine2d> &toAdd 
											, std::vector<std::vector<VLine2d>> &mainVec)
	{
		toAdd.push_back (absp.GetLine().L());
		if (isPos)
		{
			//for the positive child
			const BspTree *posc = absp.GetPosChild();
			if (posc)
			{
				MakeOneLoop(isPos,*posc,toAdd,mainVec);
			}
			// for the negative child
			const BspTree *negc = absp.GetNegChild();
			if (negc)
			{
				// start a new loop
				std::vector<VLine2d> newVec;
				mainVec.push_back (newVec);
				MakeOneLoop (!isPos , *negc , mainVec.back() , mainVec);
			}
		}
		else
		{
			// for the negative child
			const BspTree *negc = absp.GetNegChild();
			if (negc)
			{
				MakeOneLoop (isPos , *negc , toAdd , mainVec);
			}
			// for the positive child
			const BspTree *posc = absp.GetPosChild();	
			if (posc)
			{
				// start a new loop
				std::vector<VLine2d> newVec;
				mainVec.push_back (newVec);
				MakeOneLoop (!isPos , *posc , mainVec.back() , mainVec);
			}
		}
	}
};
