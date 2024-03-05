// VInterval.cpp

#include "stdafx.h"
#include "VInterval.h"


VInterval::VInterval ()
{
	m_t0 = 0.;
	m_t1 = 1.;
}
VInterval::VInterval (const vreal &t0 , const vreal &t1)
:m_t0 (t0) , m_t1 (t1)
{
	SwapIf (m_t0,m_t1);
}
VInterval::VInterval (const VInterval &toCopy)
{
	m_t0 = toCopy.m_t0;
	m_t1 = toCopy.m_t1;
}
VInterval & VInterval::operator = (const VInterval &toCopy)
{
	m_t0 = toCopy.m_t0;
	m_t1 = toCopy.m_t1;
	return *this;
}
bool VInterval::operator == (const VInterval &toCompare)const
{
	if (&toCompare == this)
		return true;
	if (m_t0 == toCompare.m_t0 && m_t1 == toCompare.m_t1)
		return true;
	else
		return false;
}
bool VInterval::operator != (const VInterval &toCompare)const
{
	return operator == (toCompare);
}
std::vector<VInterval> VInterval::Intersect (const VInterval &second)const
{
	std::vector<VInterval> result;
	if (IsSeperate (second))
	{
		//intersection is empty set
	}
	else
	{
		// there is intersection
		VInterval common (m_t0 > second.m_t0 ? m_t0 : second.m_t0 , 
							m_t1 < second.m_t1 ? m_t1 : second.m_t1 );
		AddToVector (common , result);
	}

	return result;
}
std::vector<VInterval> VInterval::Merge(const VInterval &second)const
{
	std::vector<VInterval> result;
	if (IsSeperate (second))
	{
		AddToVector ((*this) , result);
		AddToVector (second , result);
	}
	else
	{
		// there is intersection
		VInterval merged (m_t0 < second.m_t0 ? m_t0 : second.m_t0 , 
							m_t1 > second.m_t1 ? m_t1 : second.m_t1 );
		AddToVector (merged , result);
	}
	
	return result;
}
std::vector<VInterval> VInterval::Subtract (const VInterval &second)const
{
	std::vector<VInterval> result;
	if (IsSeperate(second))
	{
		//result is this
		result.push_back (*this);
	}
	else if (second == *this)
	{
		//result is empty
	}
	else
	{
		//there is regular intersection
		if (IsContaining (second))
		{
			VInterval r1 (m_t0,second.m_t0);
			VInterval r2 (second.m_t1,m_t1);
			AddToVector (r1,result);
			AddToVector (r2 , result);
		}
		else if (second.IsContaining (*this))
		{
			//result is empty set
		}
		else if(m_t0 < second.m_t0 )//&& m_t1 < second.m_t1)
		{
			VInterval ai (m_t0 ,  second.m_t0);
			AddToVector (ai , result);
		}
		else if (m_t0 > second.m_t0 )//&& m_t1 > second.m_t1)
		{
			VInterval ai (second.m_t1 , m_t1);
			AddToVector (ai , result);
		}
	}

	return result;
}
const vreal & VInterval::GetT0()const
{
	return m_t0;
}
const vreal & VInterval::GetT1()const
{
	return m_t1;
}
void VInterval::SetT0(const vreal &newt0)
{
	m_t0 = newt0;
	//SwapIf (m_t0,m_t1);
}
void VInterval::SetT1(const vreal &newt1)
{
	m_t1 = newt1;
	//SwapIf (m_t0,m_t1);
}
std::vector<VInterval> VInterval::ClipedTo_0_1 ()
{
	vreal t0 = m_t0;
	vreal t1 = m_t1;
	if (t0 < 0.0)
		t0 = 0.0;
	if (t1 < 0.0)
		t1 = 0.0;
	if (t1 > 1.0)
		t1 = 1.0;
	if (t0 > 1.0)
		t0 = 1.0;

	std::vector<VInterval> result;
	VInterval toAdd (t0,t1);
	AddToVector (toAdd , result);
	return result;
}
void VInterval::SwapIf (vreal &t0,vreal &t1)
{
	if (t0 > t1)
		Swap (t0 , t1);
}
void VInterval::Swap (vreal &t0 , vreal &t1)
{
	vreal tmp = t0;
	t0 = t1;
	t1 = tmp;
}
bool VInterval::IsSeperate (const VInterval &toCompare)const
{
	if (toCompare.m_t1 < m_t0
		|| m_t1 < toCompare.m_t0)
		return true;
	else
		return false;
}
bool VInterval::IsContaining (const VInterval &toCompare)const
{
	if (m_t0 <= toCompare.m_t0 && m_t1 >= toCompare.m_t1)
		return true;
	else
		return false;
}
void VInterval::AddToVector (const VInterval &newi , std::vector<VInterval> &vec)const
{
	vreal length = newi.GetLength();
	if (length < EPSILON)
		return;

	vec.push_back (newi);
}
vreal VInterval::GetLength()const
{
	return m_t1 - m_t0;
}

std::vector<VInterval> VInterval::Subtract (const std::vector<VInterval> &v , const VInterval &i)
{
	std::vector<VInterval> res;
	size_t mysize = v.size();
	for (size_t j=0;j<mysize;++j)
	{
		const VInterval &i0 = v[j];
		std::vector<VInterval> sr = i0.Subtract(i);
		std::vector<VInterval>::const_iterator vi = sr.begin();
		std::vector<VInterval>::const_iterator ve = sr.end();
		for (vi;vi!=ve;++vi)
			res.push_back(*vi);
	}
	return res;
}

std::vector<VInterval> VInterval::Subtract (const std::vector<VInterval> &v1 , const std::vector<VInterval> &v2)
{
	std::vector<VInterval> rem = v1;
	size_t mysize = v2.size();
	for (size_t i=0;i<mysize;++i)
	{
		const VInterval &i2 = v2[i];
		rem = Subtract(rem , i2);
	}
	return rem;
}

bool VInterval::MergeFirstNonSeperate (std::vector<VInterval> &v1 , VInterval &i1)
{
	bool merged = false;
	size_t mysize = v1.size();
	std::vector<VInterval> sep;
	for (size_t i=0;i<mysize;++i)
	{
		const VInterval &i0 = v1[i];
		if (i0.IsSeperate(i1))
		{
			sep.push_back(i0);
		}
		else
		{
			std::vector<VInterval> tmp = i0.Merge(i1);
			if (tmp.size()!=1)
				throw VException (( IDS_VInMerExcMerSho),1);
			i1 = tmp.front();
	
			for (size_t j=i+1;j<mysize;++j)
			{
				const VInterval &i3 = v1[j];
				sep.push_back(i3);
			}
			v1 = sep;
			return true;
		}
	}
	v1 = sep;
	return false;
}

std::vector<VInterval> VInterval::Merge(const std::vector<VInterval> &v1, const VInterval &i1)
{
	if (v1.size()==0)
	{
		std::vector<VInterval> res;
		res.push_back(i1);
		return res;
	}
	std::vector<VInterval> v1c = v1;
	VInterval i1c = i1;
	bool merged = true;
	while (merged)
	{
		merged = MergeFirstNonSeperate(v1c,i1c);
	}
	v1c.push_back(i1c);
	return v1c;
}

std::vector<VInterval> VInterval::Merge(const std::vector<VInterval> &v1, const std::vector<VInterval> &v2)
{
	if (v1.size() == 0)
		return v2;
	if (v2.size() == 0)
		return v1;
	size_t mysize = v1.size();
	std::vector<VInterval> v2c = v2;
	for (size_t i=0;i<mysize;++i)
	{
		const VInterval &i1 = v1[i];
		v2c = Merge(v2c,i1);
	}
	return v2c;
}

bool IsLess (const VInterval &i1 , const VInterval &i2)
{
	if (i1.GetT0() < i2.GetT0())
		return true;
	else
		return false;
}

std::list<VInterval> GetSortedList (const std::vector<VInterval> &toSort)
{
	std::list<VInterval> tmp (toSort.begin(),toSort.end());
	tmp.sort(IsLess);
	return tmp;
}

std::vector<VInterval> GetSortedVec (const std::vector<VInterval> &toSort)
{
	std::list<VInterval> tmp (toSort.begin(),toSort.end());
	tmp.sort(IsLess);
	std::vector<VInterval> sorted (tmp.begin(),tmp.end());
	return sorted;
}

std::vector<VInterval> VInterval::Intersect(const std::vector<VInterval> &v1, const VInterval &i)
{
	std::vector<VInterval> resi;
	size_t mysize = v1.size();
	for (size_t j=0;j<mysize;++j)
	{
		const VInterval &curi = v1.at(j);
		std::vector<VInterval> inti = curi.Intersect (i);
		size_t mysize2 = inti.size();
		for (size_t w=0;w<mysize2;++w)
		{
			const VInterval &tmpi = inti.at(w);
			resi.push_back (tmpi);
		}
	}
	return resi;
}

std::vector<VInterval> VInterval::ShiftIntervals (const std::vector<VInterval> &toOfset , vreal ofsetValue)
{
	size_t mysize = toOfset.size();
	std::vector<VInterval> siVec (mysize);
	for (size_t i=0;i<mysize;++i)
	{
		const VInterval &curi = toOfset.at(i);
		siVec[i] = curi.GetShifted (ofsetValue);
	}
	return siVec;
}

VInterval VInterval::GetShifted (vreal ofsetValue)const
{
	VInterval si (m_t0 + ofsetValue , m_t1 +ofsetValue);
	return si;
}

vreal VInterval::GetLengthSum (const std::vector<VInterval> &ivec)
{
	size_t mysize = ivec.size();
	vreal lSum = 0.0;
	for (size_t i=0;i<mysize;++i)
	{
		const VInterval &curI = ivec.at(i);
		lSum += curI.GetLength();
	}
	return lSum;
}

bool VInterval::IsStockRowContainingPartRow (const std::vector<VInterval> *stockRow , const std::vector<VInterval> *partRow , vreal partOfset,vreal tol)
{
	if (partRow->size())
	{
		size_t ss = stockRow->size();
		if(ss == 0)
			return false;
		size_t pp = partRow->size();
		for (size_t xxx=0;xxx<pp;++xxx)
		{
			const VInterval &partL = partRow->at(xxx);
			VInterval shiftedPartL (partL.GetT0()+partOfset+tol,partL.GetT1()+partOfset-tol);
			bool oneIntervalContains = false;
			for (size_t xxxx = 0;xxxx < ss; ++xxxx)
			{
				const VInterval &stockL = stockRow->at(xxxx);
				if (stockL.IsContaining (shiftedPartL) == true)
				{
					oneIntervalContains = true;
					break;
				}
			}
			if (oneIntervalContains == false)
				return false;
		}
	}
	return true;
}

bool VInterval::SearchStartCouplesGetSmallestIncluded (const std::vector<VInterval> *stockRow , const std::vector<VInterval> *partRow , vreal &partOfset , vreal tol)//calculates absolute part ofset , returns true if successfull
{
	size_t partSize = partRow->size();
	size_t stockSize = stockRow->size();
	partOfset = MAX_NUMBER_LIMIT;
	for (size_t k=0;k<partSize;++k)
	{
		const VInterval &curParti = partRow->at(k);
		for (size_t i=0;i<stockSize;++i)
		{
			const VInterval &curStocki = stockRow->at(i);
			vreal curOfset = curStocki.GetT0() - curParti.GetT0();
			if (IsStockRowContainingPartRow (stockRow,partRow,curOfset,tol))
			{
				if (partOfset > curOfset)
					partOfset = curOfset;
			}
		}
	}
	if (partOfset == MAX_NUMBER_LIMIT)
		return false;
	else
		return true;
}

bool VInterval::SearchStartCouplesGetSmallestIncludedReversed (const std::vector<VInterval> *stockRow , const std::vector<VInterval> *partRow,vreal &partOfset , vreal tol)
{
	int stockSize = (int)stockRow->size()-1;
	int partSize = (int)partRow->size()-1;
	partOfset = MIN_NUMBER_LIMIT;
	for (int pi = partSize; pi>= 0;--pi)
	{
		const VInterval &parti = partRow->at(pi);
		for (int si=stockSize;si>=0;--si)
		{
			const VInterval &stocki = stockRow->at(si);
			vreal curOfset = stocki.GetT1()-parti.GetT1();
			if (IsStockRowContainingPartRow (stockRow,partRow,curOfset,tol))
			{
				if (curOfset > partOfset)
				{
					partOfset = curOfset;
				}
			}
		}
	}
	if (partOfset == MIN_NUMBER_LIMIT)
		return false;
	else
		return true;
}

void VInterval::CollectAllStartToEnds (const std::vector<VInterval> *startPart , vreal startOfset , const std::vector<VInterval> *endPart , vreal endOfset , std::map<vreal,vreal> &pushValues)
{
	int myStartSize = (int)startPart->size();
	int myEndSize = (int)endPart->size();
	for (int si = myStartSize-1; si >= 0; --si)
	{
		const VInterval &cursi = startPart->at(si);
		for (int ei=0;ei<myEndSize;++ei)
		{
			const VInterval &curei = endPart->at(ei);
			vreal curDelta = (curei.GetT1() + endOfset) - (cursi.GetT0() + startOfset);
			pushValues[curDelta] = curDelta;
		}
	}
}

void VInterval::CollectAllEndToStarts (const std::vector<VInterval> *endPart , vreal endOfset , const std::vector<VInterval> *startPart , vreal startOfset , std::map<vreal,vreal> &pushValues)
{
	int myEndSize = (int)endPart->size();
	int myStartSize = (int)startPart->size();
	for (int ei = 0; ei<myEndSize; ++ei)
	{
		const VInterval &curei = endPart->at(ei);
		for (int si=myStartSize-1;si >= 0;--si)
		{
			const VInterval &cursi = startPart->at(si);
			vreal curDelta = (cursi.GetT0() + startOfset) - (curei.GetT1() + endOfset);
			pushValues[curDelta] = curDelta;
		}
	}
}

VSignedInterval::VSignedInterval ()
{
	m_signF = 1;
}
VSignedInterval::VSignedInterval (const VInterval &i , int signF)
: m_interval(i),m_signF(signF)
{

}
const VInterval & VSignedInterval::GetInterval()const
{
	return m_interval;
}
void VSignedInterval::SetInterval (VInterval &i)
{
	m_interval = i;
}
int VSignedInterval::GetSignF()const
{
	return m_signF;
}
void VSignedInterval::SetSignF(int signF)
{
	m_signF = signF;
}

bool VInterval::IsContaining (vreal val)const
{
	if (val + EPSILON > this->m_t0 && val < this->m_t1 + EPSILON)
	{
		return true;
	}
	else
	{
		return false;
	}
}

void VInterval::IndexVecToIntervals (const std::vector<int> &ivec , std::vector<VInterval> &intVec)
{// suppose you have a sequence 0,1,2,3,6,7,8,14  you should get a result  intVec[0] = [0,3] intVec[1] = [6,8] intVec[2] = [14,14]
	int isize = (int)ivec.size();
	if (isize == 0)
		return;
	int pi = ivec.at(0);
	intVec.push_back(VInterval  (pi,pi));
	for (int ii=1;ii<isize;++ii)
	{
		int curi = ivec.at(ii);
		if (curi-pi != 1)
		{
			//intVec.back().SetT1(intVec.back().GetT1()+1);// expand the end one (line index to point index)
			intVec.push_back(VInterval(curi,curi));
		}
		else
		{
			intVec.back().SetT1(curi);
		}
		pi = curi;
	}
}

void VInterval::Serialize(Bofstream &f)const
{
	int version = 1;
	f.WriteInt(version);
	f.WriteDouble(m_t0);
	f.WriteDouble(m_t1);
}
void VInterval::Serialize(Bifstream &f)
{
	int version;
	f.ReadInt(version);
	f.ReadDouble(m_t0);
	f.ReadDouble(m_t1);
}

bool VInterval::IsEmptySet()const
{
	if (fabs(m_t1 - m_t0) < EPSILON)
		return true;
	return false;
}

int VInterval::IsContaining(const std::vector<VInterval> &ivec, vreal val)// returns index of containing interval , if none returns -1
{
	int isize = (int)ivec.size();
	for (int i = 0; i < isize; ++i)
	{
		const VInterval &curi = ivec.at(i);
		if (curi.IsContaining(val) == true)
			return i;
	}
	return -1;
}

void VInterval::GetMinMax(std::vector<VInterval> &ivec, vreal &xmin, vreal &xmax)
{
	xmin = MAX_NUMBER_LIMIT;
	xmax = MIN_NUMBER_LIMIT;
	int isize = (int)ivec.size();
	for (int i = 0; i < isize; ++i)
	{
		const VInterval &curi = ivec.at(i);
		if (xmin > curi.GetT0())
			xmin = curi.GetT0();
		if (xmax < curi.GetT1())
			xmax = curi.GetT1();
	}
}
