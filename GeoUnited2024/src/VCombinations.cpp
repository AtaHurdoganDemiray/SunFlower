// VCombinations.cpp

#include "stdafx.h"
#include "VCombinations.h"

void VCombinations::CollectCombinations(const std::vector<int> &s
	, std::list<std::vector<int>> &combinations
	, vreal k
	, std::vector<int> &t
	, vreal q, vreal r)
{
	vreal p = (vreal)s.size();
	if (q == k)
	{
		combinations.push_back(std::vector<int>((size_t)(k+0.1)));
		for (vreal ci = 0; ci < k; ++ci)
			combinations.back().operator[]((size_t)(ci+0.1)) = t[(size_t)(ci+0.1)];
	}
	else
	{
		for (vreal i = r; i<p; i++)
		{
			t[(size_t)(q+0.1)] = s[(size_t)(i+0.1)];
			CollectCombinations (s, combinations , k ,  t , q + 1, i + 1);
		}
	}
}

void VCombinations::TestCollectCombinations(int setSize , int k)
{
	std::vector<int> set(setSize);
	for (int si = 0; si < setSize; ++si)
		set[si] = si;
	std::list<std::vector<int>> combinations;
	//int k = 4;
	std::vector<int> t(12);
	VCombinations::CollectCombinations(set, combinations, k, t);
	std::list<std::vector<int>>::iterator li = combinations.begin();
	std::list<std::vector<int>>::iterator le = combinations.end();
	VOStrStream str2;
	str2 << combinations.size();
	LogFileWriter::WriteUnknownExceptionLog("C size:", str2.str().c_str());
	for (li; li != le; ++li)
	{
		int mysize = (int)li->size();
		VOStrStream str;
		for (int vi = 0; vi < mysize; ++vi)
		{
			str << li->at(vi) << ";";
		}
		LogFileWriter::WriteUnknownExceptionLog(str.str().c_str());
	}
}

void VCombinations::PopulateRotationsOneBasedi(int setCount, std::list<std::vector<int>> &allCombinations , const std::vector<bool> &isRotateble)
{
	std::vector<int> v(setCount);
	for (int vi = 0; vi < setCount; ++vi)
	{
		v[vi] = (vi + 1); // one based index
	}
	allCombinations.push_back(v);
	int cursor = 0;
	while (cursor < setCount)
	{
		if (isRotateble.at(cursor))
		{
			std::list<std::vector<int>>::iterator li = allCombinations.begin();
			std::list<std::vector<int>>::iterator le = allCombinations.end();
			std::list<std::vector<int>> tmp;
			for (li; li != le; ++li)
			{
				std::vector<int> curC = *li;
				curC.at(cursor) = curC.at(cursor) * -1;
				tmp.push_back(curC);
			}
			allCombinations.insert(allCombinations.end(), tmp.begin(), tmp.end());
		}
		++cursor;
	}
}
void VCombinations::PopulateRotationsOneBasedi(const std::vector<int> &zeroBasedVec , std::list<std::vector<int>> &allCombinations, const std::vector<bool> &isRotateble)
{
	int setCount = (int)zeroBasedVec.size();
	std::vector<int> v(setCount);
	for (int vi = 0; vi < setCount; ++vi)
	{
		v[vi] = (zeroBasedVec[vi] + 1); // one based index
	}
	allCombinations.push_back(v);
	int cursor = 0;
	while (cursor < setCount)
	{
		if (isRotateble.at(cursor))
		{
			std::list<std::vector<int>>::iterator li = allCombinations.begin();
			std::list<std::vector<int>>::iterator le = allCombinations.end();
			std::list<std::vector<int>> tmp;
			for (li; li != le; ++li)
			{
				std::vector<int> curC = *li;
				curC.at(cursor) = curC.at(cursor) * -1;
				tmp.push_back(curC);
			}
			allCombinations.insert(allCombinations.end(), tmp.begin(), tmp.end());
		}
		++cursor;
	}
}
void VCombinations::TestPopulateRotations()
{
	std::list<std::vector<int>> combinations;
	std::vector<bool> isRotateble(4, true);
	PopulateRotationsOneBasedi(4,combinations,isRotateble);
	
	std::list<std::vector<int>>::iterator li = combinations.begin();
	std::list<std::vector<int>>::iterator le = combinations.end();
	VOStrStream str2;
	str2 << combinations.size();
	LogFileWriter::WriteUnknownExceptionLog("C size:", str2.str().c_str());
	for (li; li != le; ++li)
	{
		int mysize = (int)li->size();
		VOStrStream str;
		for (int vi = 0; vi < mysize; ++vi)
		{
			str << li->at(vi) << ";";
		}
		LogFileWriter::WriteUnknownExceptionLog(str.str().c_str());
	}
}

// for C (5,3)  = 5! / (3!x2!)   call Comb (5,3)
vreal VCombinations::Comb(vreal n, vreal i)
{
	if (i > n)
		throw VException ("VCombinations::Comb exception n < i",1);
	if (i == n || i == 0)
		return 1;
	vreal di = n - i;
	if (di > i)
	{
		vreal uf = Factorial(n, i);
		vreal lf = Factorial(i, i);
		return uf / lf;
	}
	else
	{
		vreal uf = Factorial(n, di);
		vreal lf = Factorial(di, di);
		return uf / lf;
	}
}

// for 5! / 3!  Factorial (5,2)  should be called  , for 5!  Factorial (5,5)  need to be called
vreal VCombinations::Factorial(vreal n, vreal i)
{
	vreal ln = n - i;
	vreal f = n;
	for (vreal ci = n-1; ci > ln;ci-=1.)
	{
		f *= ci;
	}
	return f;
}

void VCombinations::TestComb(int n , int i)
{
	vreal ci = Comb(n, i);
	VOStrStream str1, str2;
	str1 << "C (" << n << "," << i << ") = ";
	str2 << ci << "integer value: " << (int)ci;
	LogFileWriter::WriteUnknownExceptionLog(str1.str(), str2.str());
}
