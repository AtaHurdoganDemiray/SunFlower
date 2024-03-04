// VCombinations.h

#ifndef __VCombinations_h__
#define __VCombinations_h__

class VCombinations
{
public:

	static void CollectCombinations (const std::vector<int> &set
							, std::list<std::vector<int>> &combinations 
							, vreal k
							, std::vector<int> &t
							, vreal q = 0, vreal r = 0);

	static void TestCollectCombinations(int setSize , int k);
	static void PopulateRotationsOneBasedi(int setCount, std::list<std::vector<int>> &allCombinations , const std::vector<bool> &isRotateble); // for a set of items negative index means a rotated item , every index is 1 based to avoid 0
	static void PopulateRotationsOneBasedi(const std::vector<int> &zeroBasedVec, std::list<std::vector<int>> &oneBasedRotations, const std::vector<bool> &isRotateble);
	static void TestPopulateRotations();
	// for C (5,3)  = 5! / (3!x2!)   call Comb (5,3)
	static vreal Comb(vreal n, vreal i);
	// for 5! / 3!  Factorial (5,2)  should be called  , for 5!  Factorial (5,5)  need to be called
	static vreal Factorial(vreal n, vreal i);
	static void TestComb(int n , int i);
};
 

#endif