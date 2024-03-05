// OrderSequentialLineSets.h

#ifndef __OrderSequentialLineSets_h__
#define __OrderSequentialLineSets_h__

template <class S , class T> class OrderSequentialLineSets
{
public:
	S* MakeOrderedUnion (std::vector <S> &slVec)
	{
		S * res = MakeSet(slVec);

		int mysize = (int)(slVec.size());
		std::vector<bool> usedFlag (mysize,false);
	
		int i=0;
		S &currSL = slVec[i];
		while (currSL.GetPointSize()<2)
		{
			usedFlag[i] = true;
			i++;
			if (i == mysize)
				throw VException ("OrderSequentialLineSets exception" , 1);			
			currSL = slVec[i];
		};
		
		int nextNearest = i;
		bool reverseSL = false;

		while (nextNearest != -1)
		{	
				S &mynext = slVec[nextNearest];
				if (reverseSL)
					mynext.Reverse();
				res->AddBack(mynext);
				usedFlag[nextNearest] = true;
				const T *currEP = mynext.End();

			vreal minD = MAX_NUMBER_LIMIT;
			nextNearest = -1;
			reverseSL = false;
			
				for (int j=1;j<mysize;j++)
				{
					if (usedFlag[j] == true)
						continue;
				
					S &nextSL = slVec[j];
					const T *nextBP = nextSL.Start();
					const T *nextEP = nextSL.End();
					vreal d1 = (*currEP - *nextBP).GetLength();
					if (d1 < minD)
					{
						minD = d1;
						nextNearest = j;
						reverseSL = false;
					}
					vreal d2 = (*currEP - *nextEP).GetLength();
					if (d2 < minD)
					{
						minD = d2;
						nextNearest = j;
						reverseSL = true;
					}
					if (fabs(minD) < EPSILON)
						break;
				}// for all sl
		}	// while
		return res;
	};

private:
	S *MakeSet (std::vector<S> &slVec)
	{
		int mysize = (int)(slVec.size());
		int totalCount = 0;
		for (int i=0;i<mysize;i++)
		{
			const S &currsl = slVec[i];
			totalCount += currsl.Size();
		}
		S *newsl = new S (totalCount);
		return newsl;
	};

};

#endif