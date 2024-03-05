// GUICustomize.cpp

#include "stdafx.h"
#include "GUICustomize.h"

GUICustomize::GUICustomize()
{
	LoadDefaults();
	Read();
	std::map<VStr, VStr>::iterator fi = m_keyValMap.find("SettingsBaseFolder");
	VStr rootDir;
	if (fi != m_keyValMap.end())
	{
		std::vector<VStr> strVec;
		TypeConverters::SplitString(fi->second, ';', strVec);
		rootDir = strVec.at(strVec.size() - 1);
	}
	else
	{
		rootDir = "C:\\SolidCNCFactorySettings\\";
		m_keyValMap["SettingsBaseFolder"] = rootDir;
	}
	VGlobalFunctions::m_solidCncExeFolder = rootDir;
}

void GUICustomize::ChangeApplicationName(const VStr &newName)
{
	m_keyValMap["ApplicationName"] = newName;
}
void GUICustomize::ChangeApplicationIcon(int iconResourceId)
{
	VOStrStream str;
	str << iconResourceId;
	m_keyValMap["ApplicationIconId"] = str.str();
}
void GUICustomize::ChangeSplashWindowBmpId(int splashWindowBmpId)
{
	VOStrStream str;
	str << splashWindowBmpId;
	m_keyValMap["SplashWindowBmpId"] = str.str();
}
void GUICustomize::Write()
{
	try
	{
		VStr filePath = VGlobalFunctions::GetSolidCncExeFolderOld();
		filePath += "GUI.txt";
		VofStream f(filePath);
		std::map<VStr, VStr>::const_iterator i = m_keyValMap.begin();
		std::map<VStr, VStr>::const_iterator e = m_keyValMap.end();
		for (i; i != e; ++i)
		{
			f << i->first << "\n";
			f << i->second << "\n";
		}
		f.close();
	}
	catch (VException &e)
	{
		LogFileWriter::WriteExceptionLog(e, "GUICustomizer::Write");
	}
	catch (std::exception &e)
	{
		LogFileWriter::WriteExceptionLog(e, "GUICustomizer::Write");
	}
	catch (...)
	{
		LogFileWriter::WriteUnknownExceptionLog("UE ", "GUICustomizer::Write");
	}
}
void GUICustomize::Read()
{
	try
	{
		VStr filePath = VGlobalFunctions::GetSolidCncExeFolderOld();
		filePath += "GUI.txt";
		VIFStream f(filePath);
		
		if (f.is_open() == false)
			throw VException("GUICustomize::Read , GUI.txt not found",1);
		VChr temp[1024];

		while (f.eof() == false)
		{
			f.getline(temp, 1024);
			VStr key(temp);
			f.getline(temp,1024);
			VStr val(temp);
			m_keyValMap[key] = val;
		}
	}
	catch (VException &e)
	{
		LogFileWriter::WriteExceptionLog(e, "GUICustomizer::Read");
		LoadDefaults();
		Write();
	}
	catch (std::exception &e)
	{
		LogFileWriter::WriteExceptionLog(e, "GUICustomizer::Read");
		LoadDefaults();
		Write();
	}
	catch (...)
	{
		LogFileWriter::WriteUnknownExceptionLog("UE", "GUICustomizer::Read");
		LoadDefaults();
		Write();
	}
}

VStr GUICustomize::GetApplicationName()const
{
	try
	{
		VStrStream s;
		s << m_keyValMap.at("ApplicationName");
		VStr aname;
		s >> aname;
		return aname;
	}
	catch (...)
	{
		return "SolidCNC";
	}
}
int GUICustomize::GetApplicationIconId()const
{
	try
	{
		VStrStream s;
		s << m_keyValMap.at("ApplicationIconId");
		int appIconId;
		s >> appIconId;
		return appIconId;
	}
	catch (VException &e)
	{
		LogFileWriter::WriteExceptionLog(e, "GUICustomize::GetApplicationIconId");
		return IDR_VIEWPOTYPE;
	}
	catch (std::exception &e)
	{
		LogFileWriter::WriteExceptionLog(e, "GUICustomize::GetApplicationIconId");
		return IDR_VIEWPOTYPE;
	}
	catch (...)
	{
		LogFileWriter::WriteUnknownExceptionLog("UE ", "GUICustomize::GetApplicationIconId");
		return IDR_VIEWPOTYPE;
	}
}
int GUICustomize::GetSplashWindowBmpId()const
{
	try
	{
		VStrStream s;
		s << m_keyValMap.at("SplashWindowBmpId");
		int splashBmpid;
		s >> splashBmpid;
		return splashBmpid;
	}
	catch (...)
	{
		return IDB_BITMAP_SOLIDCNC;
	}
}

void GUICustomize::LoadDefaults()
{
	m_keyValMap["ApplicationName"] = "SolidCNC";
	m_keyValMap["ApplicationIconId"] = TypeConverters::ConvertIntToString(IDR_VIEWPOTYPE);
	m_keyValMap["SplashWindowBmpId"] = TypeConverters::ConvertIntToString(IDB_BITMAP_SOLIDCNC);
	m_keyValMap["ShowTreePane"] = TypeConverters::ConvertIntToString(TRUE);
	m_keyValMap["ShowNetPane"] = TypeConverters::ConvertIntToString(FALSE);
	m_keyValMap["ShowSpinFormPane"] = TypeConverters::ConvertIntToString(FALSE);
	m_keyValMap["TreeHolderPaneWidth"] = TypeConverters::ConvertIntToString(300);
	m_keyValMap["TreeHolderPaneHeight"] = TypeConverters::ConvertIntToString(400);
	m_keyValMap["NetHolderPaneWidth"] = TypeConverters::ConvertIntToString(500);
	m_keyValMap["NetHolderPaneHeight"] = TypeConverters::ConvertIntToString(400);
	m_keyValMap["SpinFormPaneWidth"] = TypeConverters::ConvertIntToString(300);
	m_keyValMap["SpinFormPaneHeight"] = TypeConverters::ConvertIntToString(400);
	m_keyValMap["MenuLangSettings"] = "MenuLangSettings";
	m_keyValMap["ApplicationIconStr"] = "Viewpoint.ico";
	m_keyValMap["DocumentIconId"] = TypeConverters::ConvertIntToString(129);
	m_keyValMap["SettingsBaseFolder"] = "C:\\SolidCNCFactorySettings\\";
}

int GUICustomize::GetShowNetPane()const
{
	try
	{
		VStr showStr = m_keyValMap.at("ShowNetPane");
		int res = (int)TypeConverters::ConvertStringTovreal(showStr);
		return res;
	}
	catch (...)
	{
		return FALSE;
	}
}

int GUICustomize::GetShowTreePane()const
{
	try
	{
		VStr showStr = m_keyValMap.at("ShowTreePane");
		int res = (int)TypeConverters::ConvertStringTovreal(showStr);
		return res;
	}
	catch (...)
	{
		return FALSE;
	}
}

int GUICustomize::GetShowSpinFormPane()const
{
	try
	{
		VStr showStr = m_keyValMap.at("ShowSpinFormPane");
		int res = (int)TypeConverters::ConvertStringTovreal(showStr);
		return res;
	}
	catch (...)
	{
		return FALSE;
	}
}

int GUICustomize::GetTreeHolderPaneWidth()const
{
	try
	{
		VStr valStr = m_keyValMap.at("TreeHolderPaneWidth");
		int res = (int)TypeConverters::ConvertStringTovreal(valStr);
		return res;
	}
	catch (...)
	{
		return 400;
	}
}
int GUICustomize::GetTreeHolderPaneHeight()const
{
	try
	{
		VStr valStr = m_keyValMap.at("TreeHolderPaneHeight");
		int res = (int)TypeConverters::ConvertStringTovreal(valStr);
		return res;
	}
	catch (...)
	{
		return 400;
	}
}
int GUICustomize::GetNetHolderPaneWidth()const
{
	try
	{
		VStr valStr = m_keyValMap.at("NetHolderPaneWidth");
		int res = (int)TypeConverters::ConvertStringTovreal(valStr);
		return res;
	}
	catch (...)
	{
		return 400;
	}
}
int GUICustomize::GetNetHolderPaneHeight()const
{
	try
	{
		VStr valStr = m_keyValMap.at("NetHolderPaneHeight");
		int res = (int)TypeConverters::ConvertStringTovreal(valStr);
		return res;
	}
	catch (...)
	{
		return 400;
	}
}
int GUICustomize::GetSpinFormPaneWidth()const
{
	try
	{
		VStr valStr = m_keyValMap.at("SpinFormPaneWidth");
		int res = (int)TypeConverters::ConvertStringTovreal(valStr);
		return res;
	}
	catch (...)
	{
		return 400;
	}
}
int GUICustomize::GetSpinFormPaneHeight()const
{
	try
	{
		VStr valStr = m_keyValMap.at("SpinFormPaneHeight");
		int res = (int)TypeConverters::ConvertStringTovreal(valStr);
		return res;
	}
	catch (...)
	{
		return 400;
	}
}

VStr GUICustomize::GetMenuLangSettings()const
{
	try
	{
		VStr valStr = m_keyValMap.at("MenuLangSettings");
		return valStr;
	}
	catch (...)
	{
		return "MenuLangSettings";
	}
}

VStr GUICustomize::GetApplicationIconStr()const
{
	try
	{
		VStr valStr = m_keyValMap.at("ApplicationIconStr");
		return valStr;
	}
	catch (...)
	{
		return "Viewpoint.ico";
	}
}

int GUICustomize::GetDocumentIconId()const
{
	try
	{
		VStr valStr = m_keyValMap.at("DocumentIconId");
		return (int)TypeConverters::ConvertStringTovreal(valStr);
	}
	catch (...)
	{
		return 129;
	}
}

VStr GUICustomize::GetSettingsBaseDir()
{
	try
	{
		VStr valStr = m_keyValMap.at("SettingsBaseFolder");
		return valStr;
	}
	catch (...)
	{
		return "C:\\SolidCNCFactorySettings\\";
	}
}

void GUICustomize::SetSettingsBaseDir(const VStr &baseDirStr)
{
	m_keyValMap.at("SettingsBaseFolder") = baseDirStr;
}

int GUICustomize::GetMacroTablesDialogPaneHeight()const
{
	try
	{
		VStr valStr = m_keyValMap.at("MacroTablesDialogPaneHeight");
		int res = (int)TypeConverters::ConvertStringTovreal(valStr);
		return res;
	}
	catch (...)
	{
		return 250;
	}
}
int GUICustomize::GetMacroTablesDialogPaneWidth()const
{
	try
	{
		VStr valStr = m_keyValMap.at("MacroTablesDialogPaneWidth");
		int res = (int)TypeConverters::ConvertStringTovreal(valStr);
		return res;
	}
	catch (...)
	{
		return 1800;
	}
}
int GUICustomize::GetShowMacroTablesDialogPane()const
{
	try
	{
		VStr valStr = m_keyValMap.at("ShowMacroTablesDialogPane");
		int res = (int)TypeConverters::ConvertStringTovreal(valStr);
		return res;
	}
	catch (...)
	{
		return FALSE;
	}
}