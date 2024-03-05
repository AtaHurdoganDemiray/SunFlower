// LoopSort.h

#ifndef __LoopSort_h__
#define __LoopSort_h__

class LoopSort
{
public:
	// inline implementation , template loop sort

	template <class T> static void Remove(std::vector<T *> &selCrvVec1, T *toRemove)
	{
		std::vector<T*> temp;
		int mysize = (int)selCrvVec1.size();
		for (int i = 0; i < mysize; ++i)
		{
			T*curt = selCrvVec1.at(i);
			if (curt != toRemove)
				temp.push_back(curt);
		}
		selCrvVec1 = temp;
	};

	template <class T> static double GetClosestCrvT(std::vector<T *> &cVec
		, const VPoint &p, const T *curCurv
		, T ** closeCrv
		, bool &isReversed, const double coinTol, int &coinCount)
	{
		double coinTol2 = coinTol * coinTol;
		coinCount = 0;
		std::vector<T *>::iterator i = cVec.begin();
		std::vector<T *>::iterator e = cVec.end();
		*closeCrv = *i;
		double minDist2 = MAX_NUMBER_LIMIT;
		for (i; i != e; ++i)
		{
			double db_2 = (p - *(*i)->GetLines().Start()).GetLength2();
			double	de_2 = (p - *(*i)->GetLines().End()).GetLength2();
			if (db_2 < minDist2)
			{
				//if (coinCount == 0) // if there are more than one coincident points, the first one in vector is selected as closest
				{
					*closeCrv = *i;
					isReversed = false;
					minDist2 = db_2;
				}
			}
			//if(db_2 < coinTol2)
			//	coinCount++;
			if (de_2 < minDist2)
			{
				//if (coinCount == 0) // if there are more than one coincident points, the first one in vector is selected as closest
				{
					*closeCrv = *i;
					isReversed = true;
					minDist2 = de_2;
				}
			}
			//if (de_2 < coinTol2)
			//	coinCount++;
			if (minDist2 < 1.e-20)
				break;
		}
		return sqrt(minDist2);
	};

	template <class T> static VPoint MakeContoursFromCurvesNGT(VPoint startPoint
		, std::vector<T *> &selCrvVec1	// The curves added to contourVec will be erased from vector
		, std::vector<std::vector<T *>> &contourVec
		, std::vector<bool> &isClosedVec
		, const double coinTol
		, const double sewMaxDist)
	{
		if (selCrvVec1.size() == 0)
			return startPoint;
		VPoint myep = startPoint;
		T * bcItr = 0;
		std::vector<T *> ordered;
		bool isRev;
		int coinCount(0);
		double mydist;
		mydist = GetClosestCrvT<T>(selCrvVec1, myep, 0, &bcItr, isRev, coinTol, coinCount);
		//mydist = GetClosestCrvNew (selCrvVec1 , myep , myep , UZ , bcItr , isRev , cointol , coinCount , UZ);

		if (isRev)
			bcItr->Reverse();
		ordered.push_back(bcItr);
		myep = *bcItr->GetLines().End();
		T *curCurv = bcItr;
		Remove (selCrvVec1 , bcItr);

		while (selCrvVec1.size())
		{
			bool isRev;
			int coinCount;
			mydist = GetClosestCrvT<T>(selCrvVec1, myep, curCurv, &bcItr, isRev, coinTol, coinCount);
			if (isRev)
				bcItr->Reverse();

			// now analyze 
			// possibilities:
			// 1- nearest curve first point is 
			// a - coincident with the last end point 
			//or b- within sewdist 
			//or c- not close but far
			bool isCaseC = false;

			//if (mydist < cointol)
			//{
			//	// case a , todo:add to contour and continue with other if statements 
			//	ordered.push_back(*bcItr);
			//	myep = *(*bcItr)->GetLines().End();
			//	curCurv = *bcItr;
			//	selCrvVec1.erase(bcItr);
			//}
			//else 
			if (mydist < sewMaxDist)
			{
				// case b , todo
				//AddSewLine(myep, *(*bcItr)->GetLines().Start(), ordered, ap);
				ordered.push_back(bcItr);
				myep = *bcItr->GetLines().End();
				curCurv = bcItr;
				Remove(selCrvVec1,bcItr);
			}
			else
			{
				// case c , the curve is far so add ordered as a contour but it should be open
				isCaseC = true;
				if (ordered.size())
				{
					VPoint mysp1 = *ordered.front()->GetLines().Start();
					VPoint myep1 = *ordered.back()->GetLines().End();
					double myclosedist = (mysp1 - myep1).GetLength();
					//if (myclosedist < cointol)
					//{
					//	isClosedVec.push_back(true);
					//}
					//else 
					if (myclosedist < sewMaxDist)
					{
						//AddSewLine(myep1, mysp1, ordered, ap);
						isClosedVec.push_back(true);
					}
					else
					{
						isClosedVec.push_back(false);
					}
					contourVec.push_back(ordered);
				}
				ordered.clear();
				ordered.push_back(bcItr);
				myep = *bcItr->GetLines().End();
				curCurv = bcItr;
				Remove(selCrvVec1,bcItr);
			}

			//if (isCaseC == false) // this if is wrong the new ordered may have only one curve but closed!
			//{
			// 2- nearest curve has closed the loop  
			// a-exact coincident 
			// b-near within sew dist
			// c-not closed the loop yet
			VPoint mysp = *ordered.front()->GetLines().Start();
			double sed = (myep - mysp).GetLength();
			//if (sed < cointol)
			//{
			//	// case a , loop closed add a closed contour and continue
			//	contourVec.push_back(ordered);
			//	isClosedVec.push_back(true);
			//	ordered.clear();
			//	ordered.push_back(*bcItr);
			//	myep = *(*bcItr)->GetLines().End();
			//	curCurv = *bcItr;
			//	selCrvVec1.erase(bcItr);
			//}
			//else 
			if (sed < sewMaxDist)
			{
				// case b , loop will be closed after adding sew line
				//AddSewLine(myep, mysp, ordered, ap);
				contourVec.push_back(ordered);
				isClosedVec.push_back(true);
				ordered.clear();
				if (selCrvVec1.size() > 0)
				{
					mydist = GetClosestCrvT(selCrvVec1, myep, curCurv, &bcItr, isRev, coinTol, coinCount);
					if (isRev)
						bcItr->Reverse();
					ordered.push_back(bcItr);
					myep = *bcItr->GetLines().End();
					curCurv = bcItr;
					Remove(selCrvVec1,bcItr);
				}
			}
			//else  // loop is open yet
			//{
			//	// case c
			//	// do nothing continue loop with the current myep
			//}
			// continue while
		}

		if (selCrvVec1.size() == 0 && ordered.size() > 0)
		{
			isClosedVec.push_back(false);
			VPoint mysp = *ordered.front()->GetLines().Start();
			double ld = (myep - mysp).GetLength();
			//if (ld < cointol)
			//	isClosedVec.back() = true;
			//else 
			if (ld < sewMaxDist)
			{
				//AddSewLine(myep, mysp, ordered, ap);	// Adds a line to the end to close	
				isClosedVec.back() = true;
			}
			contourVec.push_back(ordered);
		}

		//myep = MergeOpenContours(startPoint, contourVec, isClosedVec, cointol, sewMaxDist);
		myep = *contourVec.back().back()->GetLines().End();
		return myep;
	};
};

#endif