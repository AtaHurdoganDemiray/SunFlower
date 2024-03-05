// LoopsLinker.h

#ifndef __LoopsLinker_h__
#define __LoopsLinker_h__

#include "BspTree.h"

class SequentialLineSet2d;


class LoopsLinker
{
public:
	class linepoints 
	{
	public:
		int oi,ii,innerloopno;
	};
	static void Link  (SequentialLineSet2d *outerLoop
						,std::vector<SequentialLineSet2d *> &innerLoops
						,geo::BspTree *allLoops
						,SequentialLineSet2d *sum
						);
	static bool FindAdiagonal (SequentialLineSet2d *outerloop
								,SequentialLineSet2d *innerloop
								,geo::BspTree *allLoops
								,linepoints &lp);

};

bool IsFirstSmaller (const LoopsLinker::linepoints &lp1 , const LoopsLinker::linepoints &lp2);

#endif