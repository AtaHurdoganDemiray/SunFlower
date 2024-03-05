// LoopsLinker.cpp

#include "stdafx.h"
#include "LoopsLinker.h"
#include "SequentialLineSet2d.h"
#include "PartitionLine.h"
#include "LineSet2d.h"


void LoopsLinker::Link(SequentialLineSet2d *outerLoop
					   , std::vector<SequentialLineSet2d*> &innerLoops
					   , geo::BspTree *allLoops
					   , SequentialLineSet2d *sum)
{
	// For each inner loop find a diagonal to outer loop with all possibility loop
	int mysize = (int)innerLoops.size();
	std::vector<linepoints> lpVec(mysize);
	for (int i=0;i<mysize;++i)
	{
		SequentialLineSet2d *currinnerloop = innerLoops[i];
		lpVec[i].innerloopno =i;
		bool ck=FindAdiagonal (outerLoop , currinnerloop,allLoops,lpVec[i]);
		if (!ck)
			throw VException (( IDS_NoDiagonal),1);
	}

	std::sort (lpVec.begin(),lpVec.end(),IsFirstSmaller);

	int starti (0),endi(lpVec[0].oi);
	for (int i=0;i<mysize;++i)
	{
		linepoints &lp = lpVec[i];
		// Add outer part first
		for (int j = starti;j<=endi; ++j)
		{
			sum->InsertPoint(*outerLoop->GetPoint(j));
		}
		starti = endi;
		endi = lpVec[(i+1) % mysize].oi;
		
		// Add inner loop
		SequentialLineSet2d *currl = innerLoops[lpVec[i].innerloopno];
		int istart = lpVec[i].ii;
		int pointno = currl->GetPointSize();
		int iend = istart + pointno;
		for (int w=istart;w<=iend;w++)
		{
			int ri = w % pointno;
			sum->InsertPoint (*currl->GetPoint(ri));
		}
	}
	// Add last portion of outerloop
	int outerLoopSize = outerLoop->GetPointSize();
	int mystarti = lpVec[mysize-1].oi;
	for (int i=mystarti;i<outerLoopSize;++i)
	{
		sum->InsertPoint (*outerLoop->GetPoint(i));
	}

}

bool LoopsLinker::FindAdiagonal(SequentialLineSet2d *outerloop
								, SequentialLineSet2d *innerloop
								, geo::BspTree *allLoops
								, linepoints &lp)
{
	//  big loop for all lines of inner loop and outer loop

	int outersize = outerloop->GetPointSize();
	int innersize = innerloop->GetPointSize();

	for (int i=0;i<innersize;++i)
	{
		VPoint2d *ip = innerloop->GetPoint(i);
		for (int j=5;j<outersize;++j)
		{
			VPoint2d *op = outerloop->GetPoint (j);
			VLine2d myline (*op,*ip);
			LineSet2d Pos(2),Neg(2),CoSame(2),CoDiff(2);
			geo::GetPartition (*allLoops,myline,Pos,Neg,CoSame,CoDiff);			
			if ( (CoSame.Size()+CoDiff.Size()+Neg.Size()) == 0 
				&& Pos.Size() == 1)
			{
				lp.oi = j;
				lp.ii = i;
				return true;
			}
		}
	}
	return false;
}

bool IsFirstSmaller (const LoopsLinker::linepoints &lp1 , const LoopsLinker::linepoints &lp2)
{
	if (lp1.oi<lp2.oi)
		return true;
	else
		return false;
}
