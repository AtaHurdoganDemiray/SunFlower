// LocalGaussWeightedAvarageFilter.cpp

#include "stdafx.h"
#include "LocalGaussWeightedAvarageFilter.h"

LocalGaussWeightedAvarageFilter::LocalGaussWeightedAvarageFilter (vreal sigma , int nhood)
	:m_sigma(sigma),m_nhood(nhood)
{
	vreal twoSigma2 = 2. * m_sigma * m_sigma;
	m_c0 = 1. / (sqrt(TWO_PI) * m_sigma);
	m_c1 = -1. / (twoSigma2);
	if (nhood == 0)
		throw VException ("LocalGaussWeightedAvarageFilter exception , kernel size is 0",1);
}

void LocalGaussWeightedAvarageFilter::SmoothValues (const std::map<vreal,vreal> &src , std::map<vreal,vreal> &modified)
{
	std::vector<std::pair<vreal,vreal>> nhoodVec;
	PrepareNHoodVecWithBoundaryOffset (src,m_nhood,nhoodVec);
	int mysize = (int)src.size();
	int mysize2 = m_nhood + mysize;
	vreal invKernelSize = 1. / ((vreal)(m_nhood) * 2. + 1.);


	for (int i=m_nhood;i<mysize2;++i)
	{
		std::pair<vreal,vreal> &kernelCenter = nhoodVec.at(i);
		int mysize4 = i + m_nhood + 1;
		vreal weightedSum = 0.;
		for (int j=i-m_nhood;j<mysize4;++j)
		{
			std::pair<vreal,vreal> &curval = nhoodVec.at(j);
			vreal mydelta = curval.first - kernelCenter.first;
			weightedSum += (exp( m_c1 * mydelta*mydelta) * (curval.second-kernelCenter.second)); 
		}
		vreal localGaussWeightedAvarage = kernelCenter.second + weightedSum * invKernelSize;//* m_c0; //
		modified.insert (std::pair<vreal,vreal>(kernelCenter.first,localGaussWeightedAvarage));
	}
}

void LocalGaussWeightedAvarageFilter::PrepareNHoodVecWithBoundaryOffset (const std::map<vreal,vreal> &src ,int nhood, std::vector<std::pair<vreal,vreal>> &nhoodVec)
{// at the borders (front and end)  symetric mirroring is used for adding new extra elements (both x and y are mirrored)
	if (src.size() == 0)
	{
		throw VException ("LocalGaussWeightedAvarageFilter::PrepareNHoodVec exception , src size is 0",1);
	}
	std::list <std::pair<vreal,vreal>> tmpList;
	std::map<vreal,vreal>::const_iterator i = src.begin();
	std::pair<vreal,vreal> myfirstPair (i->first,i->second);
	std::map<vreal,vreal>::const_iterator e = src.end();
	for (i;i!=e;++i)
	{
		tmpList.push_back(std::pair<vreal,vreal>(i->first,i->second));
	}
	i = src.begin();
	++i;	
	std::map<vreal,vreal>::const_iterator pi = i;
	for (int j=0;j<nhood;++j)
	{
		if (i!=e)
		{
			tmpList.push_front (std::pair<vreal,vreal>(myfirstPair.first + myfirstPair.first - i->first ,  i->second));
			pi = i;
			++i;
		}
		else
		{
			tmpList.push_front (std::pair<vreal,vreal>(myfirstPair.first + myfirstPair.first - pi->first , pi->second));
		}
	}
	std::map<vreal,vreal>::const_reverse_iterator ri = src.rbegin();
	std::map<vreal,vreal>::const_reverse_iterator re = src.rend();
	std::pair<vreal,vreal> mylastPair (ri->first,ri->second);
	++ri;
	std::map<vreal,vreal>::const_reverse_iterator rpi = ri;
	for (int j=0;j<nhood;++j)
	{
		if (ri!=re)
		{
			tmpList.push_back(std::pair<vreal,vreal>(mylastPair.first+mylastPair.first-ri->first , ri->second));
			rpi = ri;
			++ri;
		}
		else
		{
			tmpList.push_back(std::pair<vreal,vreal>(mylastPair.first + mylastPair.first - rpi->first , rpi->second));
		}
	}
	nhoodVec.clear();
	nhoodVec.insert(nhoodVec.begin(),tmpList.begin(),tmpList.end());
}
void LocalGaussWeightedAvarageFilter::PrepareNHoodVecNoBoundaryOffset (const std::map<vreal,vreal> &src ,int nhood, std::vector<std::pair<vreal,vreal>> &nhoodVec)
{// at the borders (front and end)  symetric mirroring is used for adding new extra elements (both x and y are mirrored)
	if (src.size() == 0)
	{
		throw VException ("LocalGaussWeightedAvarageFilter::PrepareNHoodVec exception , src size is 0",1);
	}
	std::map<vreal,vreal>::const_iterator i = src.begin();
	std::map<vreal,vreal>::const_iterator e = src.end();
	nhoodVec.clear();
	
	for (i;i!=e;++i)
	{
		nhoodVec.push_back(std::pair<vreal,vreal>(i->first,i->second));
	}
}
vreal LocalGaussWeightedAvarageFilter::CalculateLocalMean (int kernelCenteri,int nhood,const std::vector<std::pair<vreal,vreal>> &nhoodVec)
{
	vreal sum = 0.;
	int counter = 0;
	
	int localStarti = kernelCenteri - nhood;
	if (localStarti < 0)
		localStarti = 0;
		
	int localEndi = kernelCenteri + nhood + 1;
	if (localEndi > (((int)nhoodVec.size()) - 1))
		localEndi = ((int)nhoodVec.size())-1;

	for (int j=localStarti;j<localEndi;++j)
	{
		const std::pair<vreal,vreal> &curval = nhoodVec.at(j);
		sum += curval.second;
		++counter;
	}
	vreal meanValue = sum / (vreal)counter;
	return meanValue;
}
	
vreal LocalGaussWeightedAvarageFilter::CalculateLocalStandardDeviation (int kernelCenteri , int nhood , const std::vector<std::pair<vreal,vreal>> &nhoodVec)
{
	vreal meanValue = CalculateLocalMean(kernelCenteri,nhood,nhoodVec);

	vreal sumDeviation = 0.;
	const std::pair<vreal,vreal> &kernelCenter = nhoodVec.at(kernelCenteri);
	int counter = 0;

	int localStarti = kernelCenteri - nhood;
	if (localStarti < 0)
		localStarti = 0;
		
	int localEndi = kernelCenteri + nhood + 1;
	if (localEndi > (((int)nhoodVec.size()) - 1))
		localEndi = ((int)nhoodVec.size())-1;


	for (int j=localStarti;j<localEndi;++j)
	{
		const std::pair<vreal,vreal> &curval = nhoodVec.at(j);
		vreal mydelta = curval.second - meanValue;
		sumDeviation += (mydelta*mydelta);
		++counter;
	}
	vreal standardDeviationValue = sqrt (sumDeviation) / (vreal)counter;
	return standardDeviationValue;
}

vreal LocalGaussWeightedAvarageFilter::CalculateOverallMean (const std::map<vreal,vreal> &src)
{
	std::map<vreal,vreal>::const_iterator i = src.begin();
	std::map<vreal,vreal>::const_iterator e = src.end();
	vreal sum = 0.;
	int counter = 0;
	for (i;i!=e;++i)
	{
		sum += i->second;	
		++counter;
	}
	vreal meanValue = sum / (vreal)counter;
	return meanValue;
}

vreal LocalGaussWeightedAvarageFilter::CalculateOverallStandardDeviation (const std::map<vreal,vreal> &src,vreal &meanValue)
{
	meanValue = CalculateOverallMean (src);
	std::map<vreal,vreal>::const_iterator i = src.begin();
	std::map<vreal,vreal>::const_iterator e = src.end();
	vreal sum = 0.;
	int counter = 0;
	for (i;i!=e;++i)
	{
		vreal mydelta = i->second - meanValue;
		sum += (mydelta*mydelta);
		++counter;
	}
	vreal standardDeviationValue = sqrt(sum) / (vreal)counter;
	return standardDeviationValue;
}

void LocalGaussWeightedAvarageFilter::RemoveByStandardDeviationThreshold (const std::map<vreal,vreal> &src , std::map<vreal,vreal> &modified , vreal thresholdK)
{
	vreal meanValue;
	vreal stdDev = CalculateOverallStandardDeviation (src,meanValue);
	vreal stdDevThr = stdDev * thresholdK;
	std::map<vreal,vreal>::const_iterator i = src.begin();
	std::map<vreal,vreal>::const_iterator e = src.end();
	for (i;i!=e;++i)
	{
		vreal curDev = fabs(i->second - meanValue);
		if (curDev < stdDevThr)
		{
			modified.insert (std::pair<vreal,vreal>(i->first , i->second));
		}
	}
}
