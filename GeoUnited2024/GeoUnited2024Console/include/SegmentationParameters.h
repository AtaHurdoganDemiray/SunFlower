// SegmentationParameters.h

#ifndef __SegmentationParameters_h__
#define __SegmentationParameters_h__

class SegmentationParameters
{
public:
	SegmentationParameters();
	int m_optExtractRegion;
	int m_thresholdWhiteMin;
	int m_thresholdWhiteMax;
	int m_thresholdRMin;
	int m_thresholdRMax;
	int m_thresholdGMin;
	int m_thresholdGMax;
	int m_thresholdBMin;
	int m_thresholdBMax;
	int m_deltaThreshold;
	CString m_LuaScriptPath;
	void Serialize (Bofstream &f)const;
	void Serialize (Bifstream &f);
	void LoadDefaults();
	int m_medianFilterPagen;
	int m_deltaCheckThreshold;
};

#endif