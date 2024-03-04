#include "stdafx.h"
#include "VStringTableTranslater.h"
#include "VTrnFile.h"
#include "VDir.h"
#include "VTranslatedData.h"
#include "VD.h"

VStringTableTranslater::VStringTableTranslater(void)
{
	VD d;
	/*CreatePaths();
	LoadTranslatedData();*/
} 
VStringTableTranslater::~VStringTableTranslater(void)
{
}
VStr VStringTableTranslater::Add(int id ,const VStr & srcVal)
{
	try
	{
		 return VD::Add(srcVal, "StringTable");
		 //VTranslatedData *dt = new VTranslatedData(srcVal,srcVal,"StringTable"); 
		// m_srcData.push_back(VTranslatedData::GetLine(dt)); 
		// Translate(dt);

	}
	catch(VException &e)
	{
		LogFileWriter::WriteExceptionLog (e);
	}
	catch(std::exception &e)
	{ 
		LogFileWriter::WriteExceptionLog (e);
	}
	catch(...)
	{
		LogFileWriter::WriteUnknownExceptionLog ("Unknown Excep. VStringTableTranslater::Add");
	} 
}
//void VStringTableTranslater::Translate(VTranslatedData *dt)
//{ 
//	int size = (int)m_srcData.size();
//	for(int i=0;i<size;i++)
//	{
//		if(dt->m_srcText == m_trnData.at(i)->m_srcText)
//		{
//			CString str = m_trnData.at(i)->m_trnText.c_str();
//			break;
//		}
//	} 
//} 
//VStr VStringTableTranslater::GetSelectedLanguange(const VStr& filePath)
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
//void VStringTableTranslater::CreatePaths()
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
//		LogFileWriter::WriteUnknownExceptionLog ("Unknown Excep. VStringTableTranslater::CreatePaths");
//	} 
//
//}
//bool VStringTableTranslater::LoadTranslatedData()
//{
//	std::ifstream ifs(m_selLangFilePath);
//	if(!ifs.is_open())
//		return false;
//
//	bool ck = VTranslatedData::GetTranslatedData(ifs,m_trnData);
//	 ifs.close();
//
//	return ck;
//
//} 
//
//void VStringTableTranslater::WriteToFileAllItems(std::ofstream &ofs)
//{   
//	VTrnFile::WriteAllLines(ofs, m_srcData);
//} 
//
//VStr VStringTableTranslater::GetTrnText(const VStr & srcText )
//{
//	int size = (int)m_trnData.size();
//	for(int i=0;i<size;i++)
//		if(m_trnData.at(i)->m_srcText == srcText)
//           return m_trnData.at(i)->m_trnText;
//
//	return "-1";
//}