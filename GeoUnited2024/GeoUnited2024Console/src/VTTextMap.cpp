#include "stdafx.h"
#include "VTTextMap.h"
#include "VDir.h"
#include "VTrnFile.h"
#include "VTranslatedData.h"
#include "VD.h"

VTTextMap::VTTextMap(const VStr & parentName)
{
	m_parentName = parentName;
	VD d;
	
	//CreatePaths();
	//LoadTranslatedData();
} 

VTTextMap::~VTTextMap(void)
{
}
VStr VTTextMap::Add(const VStr & srcVal)
{
	return VD::Add(srcVal, m_parentName);
	//std::map<VStr,VStr>::iterator it =  m_data.find(srcVal);
	//if(it != m_data.end())
	//{
	//	return it->second;
	//}

	//m_data[srcVal] = srcVal;

	//return srcVal;

}
//VStr VTTextMap::GetTrnText(const VStr & srcVal)
//{
//	std::map<VStr,VStr>::iterator it =  m_data.find(srcVal);
//	if(it != m_data.end())
//	{
//		return it->second;
//	}
//
//	return srcVal;
//}  
//void VTTextMap::CreatePaths()
//{
//	try
//	{
//
//		VStr exePath	= VGlobalFunctions().GetSolidCncExeFolder();
//		VStr mlsPath	= exePath +"MenuLangSettings";
//		VStr langsPath	= exePath +"MenuLangSettings" + "\\Langs"; 
//		VStr info		= exePath +"MenuLangSettings" + "\\Info"; 
//
//		if(!VDir::ExistDirectory(mlsPath.c_str())) 
//			VDir::VCreateFolder(exePath.c_str(),"MenuLangSettings");  
//	   
//		if(VDir::ExistDirectory(mlsPath.c_str()))
//		{
//			if(!VDir::ExistDirectory(langsPath.c_str())) 
//				VDir::VCreateFolder(mlsPath.c_str(),"Langs");
//		 
//			if(!VDir::ExistDirectory(info.c_str())) 
//				VDir::VCreateFolder(mlsPath.c_str(),"Info"); 
//		}
//
//		VStr selectedLang = GetSelectedLanguange(info+"\\SelectedLanguage.txt");
//
//		m_folderPath		   = mlsPath;
//		m_srcFilePath		   = langsPath +"\\SRC.txt";
//		m_selLangFilePath	   = langsPath +"\\"+ selectedLang+".txt";  
//		 
//		if(!VDir::ExistFile((info+"\\SelectedLanguage.txt").c_str()))
//			VDir::VCreateFile(info.c_str(),"SelectedLanguage.txt");
//
//	}
//	catch(std::exception &e)
//	{ 
//		LogFileWriter::WriteExceptionLog (e);
//	}
//	catch(...)
//	{
//		LogFileWriter::WriteUnknownExceptionLog ("Unknown Excep. VTTextMap::CreatePaths");
//	} 
//}
//VStr VTTextMap::GetSelectedLanguange(const VStr& filePath)
//{
//	VStr languange = "ENG";
//
//	std::ifstream ifs (filePath);
//	if(ifs.is_open())
//	{
//		std::vector<std::string> vec;
//	    VTrnFile::GetAllLines(ifs , vec);
//	    ifs.close();
//		if(vec.size() > 0)
//			languange = vec.at(0);
//	}
//	
//	return languange;
//}
//bool VTTextMap::LoadTranslatedData()
//{
//	std::ifstream ifs(m_selLangFilePath);
//	if(!ifs.is_open())
//		return false;
//	 
//	bool ck = VTranslatedData::GetTranslatedData(m_parentName ,ifs,m_data);
//	 
//	ifs.close();
//
//	return ck;
//} 
//void VTTextMap::WriteToFileAllItems(std::ofstream &ofs)
//{
//	std::vector<VStr> lines; 
//	for (std::map<VStr,VStr>::iterator it=m_data.begin(); it!=m_data.end(); ++it)
//	{ 
//		VStr line = it->first +VTranslatedData::VSEPERATOR +
//			        it->first +VTranslatedData::VSEPERATOR +
//			        m_parentName;
//
//		lines.push_back(line);
//	} 
//
//	VTrnFile::WriteAllLines(ofs,lines);
//}