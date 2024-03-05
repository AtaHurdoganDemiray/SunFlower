// stdafx.cpp : source file that includes just the standard includes
//	Core.pch will be the pre-compiled header
//	stdafx.obj will contain the pre-compiled type information

#include "stdafx.h"

void VReadUnitParams()
{
	try
	{
		VStr exeFolder = VGlobalFunctions::GetSolidCncExeFolderOld();
		Bifstream f(exeFolder + "VUnitParams.txt");
		if (f.is_open())
		{
			f.ReadDouble(VGlobalValues::ONEMICRON_TO_VUNIT);
			f.ReadDouble(VGlobalValues::TENMICRONS_TO_VUNIT);
			f.ReadDouble(VGlobalValues::HUNDREDMICRONS_TO_VUNIT);
			f.ReadDouble(VGlobalValues::ONEMM_TO_VUNIT);
			f.ReadDouble(VGlobalValues::ONECM_TO_VUNIT);
			f.ReadDouble(VGlobalValues::ONEDM_TO_VUNIT);
			f.ReadDouble(VGlobalValues::ONEMETER_TO_VUNIT);
			f.ReadDouble(VGlobalValues::VUNIT_TO_MICRON);
			f.ReadDouble(VGlobalValues::VUNIT_TO_TENMICRONS);
			f.ReadDouble(VGlobalValues::VUNIT_TO_HUNDREDMICRONS);
			f.ReadDouble(VGlobalValues::VUNIT_TO_MM);
			f.ReadDouble(VGlobalValues::VUNIT_TO_CM);
			f.ReadDouble(VGlobalValues::VUNIT_TO_DM);
			f.ReadDouble(VGlobalValues::VUNIT_TO_METRE);
			f.close();
		}
	}
	catch (...)
	{
		 VGlobalValues::VUNIT_TO_MICRON = 1000.0;
		 VGlobalValues::VUNIT_TO_TENMICRONS = 100.0;
		 VGlobalValues::VUNIT_TO_HUNDREDMICRONS = 10.0;
		 VGlobalValues::VUNIT_TO_MM = 1.0;
		 VGlobalValues::VUNIT_TO_CM = 0.1;
		 VGlobalValues::VUNIT_TO_DM = 0.01;
		 VGlobalValues::VUNIT_TO_METRE = 0.001;
		 VGlobalValues::ONEMICRON_TO_VUNIT = 0.001;
		 VGlobalValues::TENMICRONS_TO_VUNIT = 0.01;
		 VGlobalValues::HUNDREDMICRONS_TO_VUNIT = 0.1;
		 VGlobalValues::ONEMM_TO_VUNIT = 1.0;
		 VGlobalValues::ONECM_TO_VUNIT = 10.0;
		 VGlobalValues::ONEDM_TO_VUNIT = 100.0;
		 VGlobalValues::ONEMETER_TO_VUNIT = 1000.0;
		 VGlobalValues::GCODE_PRECISION = 5;
	}
}

void VWriteUnitParams()
{
	try
	{
		VStr exeFolder = VGlobalFunctions::GetSolidCncExeFolder();
		Bofstream f(exeFolder + "VUnitParams.txt");
		if (f.is_open())
		{
			f.WriteDouble(VGlobalValues::ONEMICRON_TO_VUNIT);
			f.WriteDouble(VGlobalValues::TENMICRONS_TO_VUNIT);
			f.WriteDouble(VGlobalValues::HUNDREDMICRONS_TO_VUNIT);
			f.WriteDouble(VGlobalValues::ONEMM_TO_VUNIT);
			f.WriteDouble(VGlobalValues::ONECM_TO_VUNIT);
			f.WriteDouble(VGlobalValues::ONEDM_TO_VUNIT);
			f.WriteDouble(VGlobalValues::ONEMETER_TO_VUNIT);
			f.WriteDouble(VGlobalValues::VUNIT_TO_MICRON);
			f.WriteDouble(VGlobalValues::VUNIT_TO_TENMICRONS);
			f.WriteDouble(VGlobalValues::VUNIT_TO_HUNDREDMICRONS);
			f.WriteDouble(VGlobalValues::VUNIT_TO_MM);
			f.WriteDouble(VGlobalValues::VUNIT_TO_CM);
			f.WriteDouble(VGlobalValues::VUNIT_TO_DM);
			f.WriteDouble(VGlobalValues::VUNIT_TO_METRE);
			f.close();
		}
	}
	catch (...)
	{
		VGlobalValues::VUNIT_TO_MICRON = 1000.0;
		VGlobalValues::VUNIT_TO_TENMICRONS = 100.0;
		VGlobalValues::VUNIT_TO_HUNDREDMICRONS = 10.0;
		VGlobalValues::VUNIT_TO_MM = 1.0;
		VGlobalValues::VUNIT_TO_CM = 0.1;
		VGlobalValues::VUNIT_TO_DM = 0.01;
		VGlobalValues::VUNIT_TO_METRE = 0.001;
		VGlobalValues::ONEMICRON_TO_VUNIT = 0.001;
		VGlobalValues::TENMICRONS_TO_VUNIT = 0.01;
		VGlobalValues::HUNDREDMICRONS_TO_VUNIT = 0.1;
		VGlobalValues::ONEMM_TO_VUNIT = 1.0;
		VGlobalValues::ONECM_TO_VUNIT = 10.0;
		VGlobalValues::ONEDM_TO_VUNIT = 100.0;
		VGlobalValues::ONEMETER_TO_VUNIT = 1000.0;
		VGlobalValues::GCODE_PRECISION = 5;
	}
}

const long  VGlobalValues::MAX_ARRAY_CAPACITY =	1000000;
const int   VGlobalValues::DEFAULT_INITIAL_SIZE = 910;
const vreal VGlobalValues::MYDERV_DIFF = 5.0e-4;
const vreal VGlobalValues::DERV_DIFF = 1.0e-3;
const int   VGlobalValues::DOUBLE_SIGNIFICANT_DIGITS = 15;
const int   VGlobalValues::FLOAT_SIGNIFICANT_DIGITS = 7;
const int   VGlobalValues::DOUBLE_MAX_POWER_OF_TEN = 200;
const int   VGlobalValues::FLOAT_MAX_POWER_OF_TEN = 45;
vreal VGlobalValues::TF_PRECISION = 0.1;
vreal VGlobalValues::VUNIT_TO_MICRON = 1000.0;
vreal VGlobalValues::VUNIT_TO_TENMICRONS = 100.0;
vreal VGlobalValues::VUNIT_TO_HUNDREDMICRONS = 10.0;
vreal VGlobalValues::VUNIT_TO_MM = 1.0;
vreal VGlobalValues::VUNIT_TO_CM = 0.1;
vreal VGlobalValues::VUNIT_TO_DM = 0.01;
vreal VGlobalValues::VUNIT_TO_METRE = 0.001;
vreal VGlobalValues::ONEMICRON_TO_VUNIT = 0.001;
vreal VGlobalValues::TENMICRONS_TO_VUNIT = 0.01;
vreal VGlobalValues::HUNDREDMICRONS_TO_VUNIT = 0.1;
vreal VGlobalValues::ONEMM_TO_VUNIT = 1.0;
vreal VGlobalValues::ONECM_TO_VUNIT = 10.0;
vreal VGlobalValues::ONEDM_TO_VUNIT = 100.0;
vreal VGlobalValues::ONEMETER_TO_VUNIT = 1000.0;
int  VGlobalValues::GCODE_PRECISION = 5;
ViewPointDefaults VGlobalValues::viewpointdefaults = ViewPointDefaults();
vreal VGlobalValues::m_unitMultiplier = 1.0;
VStringTableTranslater * VGlobalFunctions::m_vstTranslater = 0;
VTTextMap * VGlobalFunctions::m_vttMapTree = 0;
VTTextMap * VGlobalFunctions::m_vttMapToolbar = 0;
bool VGlobalValues::m_silentMode = false;
GUICustomize VGlobalFunctions::m_guiCustomize = GUICustomize();
VStr VGlobalFunctions::m_solidCncExeFolder = VGlobalFunctions::m_guiCustomize.GetSettingsBaseDir();
unsigned int VGlobalValues::m_runTimeSeperator = 0;
VChr VGlobalValues::m_numberSeperator = ';';

std::vector<VStr> VGlobalFunctions::ParseSolidCncCommandLine()
{
	std::vector<VStr> parsedsolidcnccmdline;
	VStr solidcnccmdline = GetCommandLine ();
	//TypeConverters::SplitString (solidcnccmdline , ' ' , parsedsolidcnccmdline);
	std::map<char,std::vector<int>> cmp;
	cmp.insert (std::pair<char,std::vector<int>> ('"' , std::vector<int>()));
	TypeConverters::DetectPlaces (solidcnccmdline , 0 , (int)solidcnccmdline.length()-1 , cmp);
	if (cmp.empty())
		throw VException (( IDS_VGlParExcVecEmp),1);
	std::vector<int> &ivec = cmp.begin()->second;
	int mysize = (int) ivec.size();
	if (mysize % 2 != 0)
		throw VException (( IDS_VGlParExcQuoSiz),1);
	for (int i=0;i<mysize;i+=2)
	{
		parsedsolidcnccmdline.push_back (TypeConverters::GetSubSet (solidcnccmdline,ivec.at(i),ivec.at(i+1)));
	}
	return parsedsolidcnccmdline;
}

VStr VGlobalFunctions::GetSolidCncExeFolderOld ()
{
	std::vector<VStr> parsedsolidcnccmdline = ParseSolidCncCommandLine();
	if (parsedsolidcnccmdline.size())
	{
		VStr fname = parsedsolidcnccmdline.front();
		fname = fname.substr (1,fname.rfind('\\'));
		return fname;
	}
	else
		throw VException ( ( IDS_VGlGetExcParSol),1);

}

VStr VGlobalFunctions::GetSolidCncExeFolder()
{
	return m_solidCncExeFolder;
}

HINSTANCE VGlobalFunctions::m_solidCncResourceDllHi = NULL;

VChr * VGlobalValues::VCHR_LINE_READER = new VChr [MAX_CHR_IN_ONE_LINE];
int VGlobalValues::VCHR_LINE_READERSIZE = MAX_CHR_IN_ONE_LINE;

void VGlobalFunctions::SetVSolidCncResourceDllHi ( HINSTANCE solidCncResourceDllHi)
{
	m_solidCncResourceDllHi = solidCncResourceDllHi;
}

VStr VGlobalFunctions::VLoadString (UINT nID)
{
    int ck = LoadString (m_solidCncResourceDllHi , nID , VGlobalValues::VCHR_LINE_READER , MAX_CHR_IN_ONE_LINE);
	if (ck == 0)
	{
		VGlobalValues::VCHR_LINE_READER [0] = 'N';
		VGlobalValues::VCHR_LINE_READER [1] = 'U';
		VGlobalValues::VCHR_LINE_READER [2] = 'L';
		VGlobalValues::VCHR_LINE_READER [3] = 'L';
		VGlobalValues::VCHR_LINE_READER [4] = '\0';
		VOStrStream strstr;
		strstr << nID;
		VStr em ( ( IDS_VGlVLoCouNotLoa) );
		em += strstr.str();
		LogFileWriter::WriteUnknownExceptionLog (em);
	}
	CString str = CString (VGlobalValues::VCHR_LINE_READER); 

	VStr retStr = GetStringTableTranslater()->Add(nID, (VStr)str);
	//VStr retStr = m_vstTranslater->GetTrnText((VStr)str).c_str();

	if(retStr == "-1" || retStr == "")
		return (LPCTSTR)str;

	return retStr.c_str();
	 
	 
	//return CString (VGlobalValues::VCHR_LINE_READER);
	//return VStr (VGlobalValues::VCHR_LINE_READER);
}

CString VGlobalFunctions::VLoadString2 (UINT nID)
{
    int ck = LoadString (m_solidCncResourceDllHi , nID , VGlobalValues::VCHR_LINE_READER , MAX_CHR_IN_ONE_LINE);
	if (ck == 0)
	{
		VGlobalValues::VCHR_LINE_READER [0] = 'N';
		VGlobalValues::VCHR_LINE_READER [1] = 'U';
		VGlobalValues::VCHR_LINE_READER [2] = 'L';
		VGlobalValues::VCHR_LINE_READER [3] = 'L';
		VGlobalValues::VCHR_LINE_READER [4] = '\0';
		VOStrStream strstr;
		strstr << nID;
		VStr em ( ( IDS_VGlVLoCouNotLoa) );
		em += strstr.str();
		LogFileWriter::WriteUnknownExceptionLog (em);
	}

	CString str = CString (VGlobalValues::VCHR_LINE_READER); 

	VStr retStr = GetStringTableTranslater()->Add(nID, (VStr)str);
	//VStr retStr = m_vstTranslater->GetTrnText((VStr)str).c_str();

	if(retStr == "-1" || retStr == "")
		return str;

	return retStr.c_str();
	 
	 
	return CString (VGlobalValues::VCHR_LINE_READER);
}
bool VGlobalFunctions::GetProductAndVersion(CString & strProductName, CString & strProductVersion)
{
	try
	{ 
		TCHAR szFilename[MAX_PATH + 1] = {0};
		if (GetModuleFileName(NULL, szFilename, MAX_PATH) == 0)
		{ 
			return false;
		} 

		DWORD dummy;
		DWORD dwSize = GetFileVersionInfoSize(szFilename, &dummy);
		if (dwSize == 0)
		{ 
			return false;
		}
		std::vector<BYTE> data(dwSize);

    
		if (!GetFileVersionInfo(szFilename, NULL, dwSize, &data[0]))
		{ 
			return false;
		}

		LPVOID pvProductName = NULL;
		unsigned int iProductNameLen = 0;
		LPVOID pvProductVersion = NULL;
		unsigned int iProductVersionLen = 0;

    
		if (!VerQueryValue(&data[0], ("\\StringFileInfo\\080004b0\\ProductName"), &pvProductName, &iProductNameLen) ||
			!VerQueryValue(&data[0], ("\\StringFileInfo\\080004b0\\ProductVersion"), &pvProductVersion, &iProductVersionLen))
		{ 
			return false;
		}

		strProductName.SetString((LPCTSTR)pvProductName, iProductNameLen);
		strProductVersion.SetString((LPCTSTR)pvProductVersion, iProductVersionLen);
	}
	catch(std::exception &ex)
	{
		throw std::exception(ex);
	}
	catch(...)
	{
		throw std::exception(("Unknown Exception : VGlobalFunctions::GetProductAndVersion"));
	}

    return true;
}
VStringTableTranslater * VGlobalFunctions::GetStringTableTranslater()
{ 
	if (m_vstTranslater == NULL)
	{
		m_vstTranslater = new VStringTableTranslater();
	}

	return m_vstTranslater;
}
 VTTextMap * VGlobalFunctions::GetVTTextMapTree()
 { 
	 if (m_vttMapTree == NULL)
	 { 
		 m_vttMapTree =  new VTTextMap("VTree");
	 }
	 return m_vttMapTree;
 } 
 VTTextMap * VGlobalFunctions::GetVTTextMapToolbar()
 {
	 if (m_vttMapToolbar == NULL)
	 { 
		 m_vttMapToolbar = new VTTextMap("VToolbar");
	 }
	 return m_vttMapToolbar;
 }

 int VpAfxMessageBox(LPCTSTR message , UINT nType , UINT nIDHelp)
 {
	 if (VGlobalValues::m_silentMode == false)
	 {
		 return AfxMessageBox(message, nType, nIDHelp);
	 }
	 else
	 {
		 LogFileWriter::WriteUnknownExceptionLog("VpAfxMessageBox :" , message);
		 return IDYES;
	 }
 }

 VStr VGlobalFunctions::EatOneLine(VStr &str)
 {
	 VStr myline;
	 int counter = 0;
	 while (counter < str.length())
	 {
		 myline += str.at(counter);
		 ++counter;
		 if (*myline.rend() == '\n')
			 break;
	 }
	 VStr remaining;
	 while (counter < str.length())
	 {
		 remaining += str.at(counter);
		 ++counter;
	 }
	 str = remaining;
	 return myline;
 }