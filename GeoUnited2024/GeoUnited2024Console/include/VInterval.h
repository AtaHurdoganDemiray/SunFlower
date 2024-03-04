// VInterval.h

#ifndef __VInterval_h__
#define __VInterval_h__

class VInterval
{
public:
	// t0 is always smaller than t1 , this means VInterval does not hold any direction information 
	VInterval ();
	VInterval (const vreal &t0 , const vreal &t1);
	VInterval (const VInterval &toCopy);
	VInterval & operator = (const VInterval &toCopy);
	bool operator == (const VInterval &toCompare)const;
	bool operator != (const VInterval &toCompare)const;
	std::vector<VInterval> Intersect (const VInterval &second)const;
	std::vector<VInterval> Merge(const VInterval &second)const;
	std::vector<VInterval> Subtract (const VInterval &second)const;
	static std::vector<VInterval> Subtract (const std::vector<VInterval> &v , const VInterval &i);
	static std::vector<VInterval> Subtract (const std::vector<VInterval> &v1 , const std::vector<VInterval> &v2);
	static std::vector<VInterval> Merge (const std::vector<VInterval> &v1 , const VInterval &i1);
	static std::vector<VInterval> Merge (const std::vector<VInterval> &v1 , const std::vector<VInterval> &v2);
	static std::vector<VInterval> Intersect (const std::vector<VInterval> &v1 , const VInterval &i);
	std::vector<VInterval> ClipedTo_0_1 ();	
	const vreal & GetT0()const;
	const vreal & GetT1()const;
	void SetT0(const vreal &newt0);
	void SetT1(const vreal &newt1);
	void Swap (vreal &t0 , vreal &t1);
	void SwapIf (vreal &t0 , vreal &t1);
	bool IsSeperate (const VInterval &toCompare)const;
	bool IsContaining (const VInterval &toCompare)const;
	vreal GetLength()const;
private:
	vreal m_t0;
	vreal m_t1;
	void AddToVector (const VInterval &newi , std::vector<VInterval> &vec)const;

	static bool MergeFirstNonSeperate (std::vector<VInterval> &v1 , VInterval &i1);
public:	
	static std::vector<VInterval> ShiftIntervals (const std::vector<VInterval> &toOfset , vreal ofsetValue);
	VInterval GetShifted (vreal ofsetValue)const;
	static vreal GetLengthSum (const std::vector<VInterval> &ivec);
	static bool IsStockRowContainingPartRow (const std::vector<VInterval> *stockRow , const std::vector<VInterval> *partRow , vreal partOfset,vreal tol);
	static bool SearchStartCouplesGetSmallestIncluded (const std::vector<VInterval> *stockRow , const std::vector<VInterval> *partRow , vreal &partOfset,vreal tol);//calculates absolute part ofset , return true if there is solution
	static bool SearchStartCouplesGetSmallestIncludedReversed (const std::vector<VInterval> *stockRow , const std::vector<VInterval> *partRow , vreal &partOfset,vreal tol);
	static void CollectAllStartToEnds (const std::vector<VInterval> *startPart , vreal startOfset , const std::vector<VInterval> *endPart , vreal endOfset , std::map<vreal,vreal> &pushValues); 
	static void CollectAllEndToStarts (const std::vector<VInterval> *endPart , vreal endOfset , const std::vector<VInterval> *startPart , vreal startOfset , std::map<vreal,vreal> &pushValues);
	bool IsContaining (vreal val)const;
	static void IndexVecToIntervals (const std::vector<int> &ivec , std::vector<VInterval> &intVec);
	void Serialize(Bofstream &f)const;
	void Serialize(Bifstream &f);
	bool IsEmptySet()const;
	static int IsContaining(const std::vector<VInterval> &ivec, vreal val);// returns index of containing interval , if none returns -1
	static void GetMinMax(std::vector<VInterval> &ivec , vreal &xmin , vreal &xmax);
};

std::list<VInterval> GetSortedList (const std::vector<VInterval> &toSort);
std::vector<VInterval> GetSortedVec (const std::vector<VInterval> &toSort);

class VSignedInterval
{
public:
	VSignedInterval ();
	VSignedInterval (const VInterval &i , int signF);
	const VInterval & GetInterval()const;
	void SetInterval (VInterval &i);
	int GetSignF()const;
	void SetSignF(int signF);
private:
	VInterval m_interval;
	int m_signF;
};

#endif