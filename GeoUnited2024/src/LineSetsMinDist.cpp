// LineSetsMinDist.cpp

#include "stdafx.h"
#include "LineSetsMinDist.h"
#include "SequentialLineSet.h"
#include "PosCubes.h"
#include "LineLineMinDist.h"

void LineSetsMinDist::Calc (const SequentialLineSet &sl1 
							, const SequentialLineSet &sl2
							, vreal &mindist 
							, VLine &minline
							, int *set1i
							, int *set2i)
{
	LineLineMinDist calc;
	mindist = MAX_NUMBER_LIMIT;

	//PosCubes cubes (sl2);
	int size1 = sl1.Size();
	int size2 = sl2.Size();
	int s1i=-1;
	int s2i=-1;
	for (int i=0;i<size1;i++)
	{
		VLine currl = sl1[i];
		LongVec relind,nii;
		//cubes.SetRelatedIndiceListForLine (currl,relind,nii);
		if (relind.Size() == 0)
		{
			for (int j=0;j<size2;j++)
				relind.AddLong(j);
		}
		int relsize = relind.Size();
		for (int j=0;j<relsize;j++)
		{
			int curri = relind[j];
			VLine nextl = sl2[curri];
			vreal d;
			VLine l;
			calc.Calc(currl,nextl,d,l);
			if (d<mindist)
			{
				mindist =d;
				minline =l;
				s1i = i;
				s2i = curri;
			}
		}
	}
	if (set1i != 0)
	{
		*set1i = s1i; 
	}
	if (set2i != 0)
	{
		*set2i = s2i;
	}
}

void LineSetsMinDist::GetNearer (const SequentialLineSet &sl1
								, const SequentialLineSet &sl2
								//, std::vector<VPoint> &r1
								//, std::vector<VPoint> &r2
								, std::multimap<double,VLine> &r	// the double is the distance between 2 lines and line's first point is on sl1 and second point on sl2
								, double distthresholdmax )
{
	LineLineMinDist calc;

	//PosCubes cubes (sl2);
	int size1 = sl1.Size();
	int size2 = sl2.Size();
	double slLength = sl1.GetLength();
	std::map<int,double> distmap;
	std::map<int,VLine> witnessmap;
	double toint = slLength / distthresholdmax;
	for (int i=0;i<size1;i++)
	{
		VLine currl = sl1[i];
		//LongVec relind,nii;
		//cubes.SetRelatedIndiceListForLine (currl,relind,nii);
		//if (relind.Size() == 0)
		//{
		//	for (int j=0;j<size2;j++)
		//		relind.AddLong(j);
		//}
		//int relsize = relind.Size();
		for (int j=0;j<size2;j++)
		{
			//int curri = relind[j];
			VLine nextl = sl2 [j];//[curri];
			vreal d;
			VLine l;
			calc.Calc(currl,nextl,d,l);
			if (d<distthresholdmax)
			{
				//r1.push_back (l.GetP1());
				//r2.push_back (l.GetP2());
				double tonsl = sl1.GetT (l.GetP1() , i , slLength);
				int intpar = (int)(tonsl * toint);
				distmap.insert (std::pair<int,double> (intpar , d));
				witnessmap.insert (std::pair<int,VLine> (intpar , l));
			}
		}

	}	
	std::map<int,double>::iterator di = distmap.begin();
	std::map<int,double>::iterator de = distmap.end();
	std::map<int,VLine>::iterator wi = witnessmap.begin();
	for (di;di!=de;++di)
	{
		r.insert(std::pair<double,VLine> (di->second , wi->second));
		++wi;
	}
}

void LineSetsMinDist::GetNearer(const SequentialLineSet& sl1
	, const SequentialLineSet& sl2
	, std::list<std::pair<VPoint, VPoint>>& closePointPairsOnSl1Sl2
	, vreal distthresholdmax)
{
	LineLineMinDist calc;

	//PosCubes cubes (sl2);
	int size1 = sl1.Size();
	int size2 = sl2.Size();
	//double slLength = sl1.GetLength();
	//std::map<int, double> distmap;
	//std::map<int, VLine> witnessmap;
	//double toint = slLength / distthresholdmax;
	for (int i = 0; i < size1; i++)
	{
		VLine currl = sl1[i];
		//LongVec relind,nii;
		//cubes.SetRelatedIndiceListForLine (currl,relind,nii);
		//if (relind.Size() == 0)
		//{
		//	for (int j=0;j<size2;j++)
		//		relind.AddLong(j);
		//}
		//int relsize = relind.Size();
		for (int j = 0; j < size2; j++)
		{
			//int curri = relind[j];
			VLine nextl = sl2[j];//[curri];
			vreal d;
			VLine l;
			calc.Calc(currl, nextl, d, l);
			if (d < distthresholdmax)
			{
				////r1.push_back (l.GetP1());
				////r2.push_back (l.GetP2());
				//double tonsl = sl1.GetT(l.GetP1(), i, slLength);
				//int intpar = (int)(tonsl * toint);
				//distmap.insert(std::pair<int, double>(intpar, d));
				//witnessmap.insert(std::pair<int, VLine>(intpar, l));
				closePointPairsOnSl1Sl2.push_back(std::pair<VPoint, VPoint>(l.GetP1(),l.GetP2()));
			}
		}
	}
}