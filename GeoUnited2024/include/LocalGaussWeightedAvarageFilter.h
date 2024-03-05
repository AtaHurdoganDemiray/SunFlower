// LocalGaussWeightedAvarageFilter.h

#ifndef __LocalGaussWeightedAvarageFilter_h__
#define __LocalGaussWeightedAvarageFilter_h__

class LocalGaussWeightedAvarageFilter
{
public:
	LocalGaussWeightedAvarageFilter (vreal sigma,int nhood);
	void SmoothValues (const std::map<vreal,vreal> &src , std::map<vreal,vreal> &modified);

private:
	vreal m_sigma;
	int m_nhood;
	vreal m_c0,m_c1;
public:
	static void PrepareNHoodVecWithBoundaryOffset (const std::map<vreal,vreal> &src , int nhood , std::vector<std::pair<vreal,vreal>> &nhoodVec);
	static void PrepareNHoodVecNoBoundaryOffset (const std::map<vreal,vreal> &src , int nhood , std::vector<std::pair<vreal,vreal>> &nhoodVec);
	static vreal CalculateLocalMean (int kernelCenteri,int nhood,const std::vector<std::pair<vreal,vreal>> &nhoodVec);
	static vreal CalculateLocalStandardDeviation (int kernelCenteri,int nhood,const std::vector<std::pair<vreal,vreal>> &nhoodVec);
	static vreal CalculateOverallMean (const std::map<vreal,vreal> &src);
	static vreal CalculateOverallStandardDeviation (const std::map<vreal,vreal> &src,vreal &meanValue);
	static void RemoveByStandardDeviationThreshold (const std::map<vreal,vreal> &src , std::map<vreal,vreal> &modified , vreal thresholdK);
};

#endif