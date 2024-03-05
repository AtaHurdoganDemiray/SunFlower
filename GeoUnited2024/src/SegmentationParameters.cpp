// SegmentationParameters.cpp

#include "stdafx.h"
#include "SegmentationParameters.h"

SegmentationParameters::SegmentationParameters()
{
	LoadDefaults();
}

void SegmentationParameters::Serialize (Bofstream &f)const
{
	int version = 3;
	f.WriteInt (version);
	f.WriteInt (m_optExtractRegion);
	f.WriteInt (m_thresholdWhiteMin);
	f.WriteInt (m_thresholdWhiteMax);
	f.WriteInt (m_thresholdRMin);
	f.WriteInt (m_thresholdRMax);
	f.WriteInt (m_thresholdGMin);
	f.WriteInt (m_thresholdGMax);
	f.WriteInt (m_thresholdBMin);
	f.WriteInt (m_thresholdBMax);
	VStr temp ((LPCTSTR)m_LuaScriptPath);
	f.WriteString (temp);
	f.WriteInt (m_deltaThreshold);
	f.WriteInt (m_medianFilterPagen);
	f.WriteInt (m_deltaCheckThreshold);
}
void SegmentationParameters::Serialize (Bifstream &f)
{
	int version = 1;
	f.ReadInt (version);
	if (version >= 1)
	{
		f.ReadInt (m_optExtractRegion);
		f.ReadInt (m_thresholdWhiteMin);
		f.ReadInt (m_thresholdWhiteMax);
		f.ReadInt (m_thresholdRMin);
		f.ReadInt (m_thresholdRMax);
		f.ReadInt (m_thresholdGMin);
		f.ReadInt (m_thresholdGMax);
		f.ReadInt (m_thresholdBMin);
		f.ReadInt (m_thresholdBMax);
		VStr temp;
		f.ReadString (temp);
		m_LuaScriptPath = temp.c_str();
	}
	if (version >= 2)
	{
		f.ReadInt (m_deltaThreshold);
	}
	if (version >= 3)
	{
		f.ReadInt(m_medianFilterPagen);
		f.ReadInt(m_deltaCheckThreshold);
	}
}
void SegmentationParameters::LoadDefaults()
{
	m_optExtractRegion = 0;
	m_thresholdWhiteMin = 128;
	m_thresholdWhiteMax = 255;
	m_thresholdRMin = 128;
	m_thresholdRMax = 255;
	m_thresholdGMin = 128;
	m_thresholdGMax = 255;
	m_thresholdBMin = 128;
	m_thresholdBMax = 255;
	m_LuaScriptPath = "C:\\DummyScript.lua";
	m_deltaThreshold = 20;
	m_medianFilterPagen = 4;
	m_deltaCheckThreshold = 300;
}