#include "stdafx.h"
#include "VD.h"
#include "VDir.h"
//#include "GUICustomize.h"
#include "VTrnFile.h"
#include "TypeConverters.h"
#include "PathStringFunctions.h"

//GUICustomize * VD::m_guiCustomize = 0;
char VD::VD_SEPERATOR = '@';
VD::VTRN* VD::m_trn = 0;

VD::VD()
{
	try
	{
		if (m_trn == NULL)
		{
			m_trn = new VD::VTRN();
		}
	}
	catch (...)
	{
		VpAfxMessageBox("..");
	}
}
VD::~VD()
{
 
}
VStr VD::Add(const VStr &key, VStr category)
{
	return m_trn->Add(key, category);
}
void VD::WriteSRC(const VStr &srcFilePath)
{
	m_trn->WriteSRC(srcFilePath);
}

VStr VD::GetSelTranslateFilePath()
{
	return m_trn->m_selLangFilePath;
}
VStr VD::GetSelVisibleFilePath()
{
	return m_trn->m_visibleFilePath;
}
void  VD::SetSelectedLanguange(const VStr &langName)
{
	m_trn->SetSlectedLanguange(langName);
}


VD::VTRN::VTRN()
{
	CreatePaths();
	ReadDic();
}


VD::VTRN::~VTRN()
{

}

void VD::VTRN::ReadDic()
{
	std::ifstream ifs;
	ifs.open(m_selLangFilePath);
	VStr categoryFile = (VStr)VDir::GetParentDir(m_selLangFilePath.c_str());
	categoryFile += "\\Categories.txt";
	if (!ifs.is_open())
	{
		LogFileWriter::WriteUnknownExceptionLog("Unknown Excep. Translated file is not open!, VD::ReadDic()"); 
		return;
	}
	std::ifstream ifs2;
	ifs2.open(categoryFile);
	if (!ifs2.is_open())
	{
		LogFileWriter::WriteUnknownExceptionLog("Unknown Excep. Category file is not open!, VD::ReadDic()");
		return;
	}

	std::vector<VStr> lines;
	VTrnFile::GetAllLines(ifs, lines);
	ifs.close();
	std::vector<VStr> linesCate;
	VTrnFile::GetAllLines(ifs2, linesCate);
	ifs2.close();

	int size = (int)lines.size();
	for (int i = 0; i < size; ++i)
	{
		if (lines.at(i) != "" && lines.at(i) != ""+VD_SEPERATOR +VD_SEPERATOR)
		{
			std::vector<CString> vec;
			VDir::SplitString(lines.at(i).c_str(), VD_SEPERATOR, vec);  
			if (vec.size() >= 2)
			{
				VStr tmpKey = (VStr)vec.at(0);
				ToLower(tmpKey);
				VStr key = (VStr)vec.at(0);
				VStr val = (VStr)vec.at(1);
				if (m_loweDic.find(tmpKey) == m_loweDic.end())
				{ 
					m_loweDic[tmpKey] = val;
					m_dic[key] = val;

				}
				else
				{
					m_dic[key] = val;
				}
			}
		}
	}

	int size2 = (int)linesCate.size();
	for (int i = 0; i < size2; i++)
	{
		if (linesCate.at(i) != "" && linesCate.at(i) != "" + VD_SEPERATOR + VD_SEPERATOR)
		{
			std::vector<CString> vec;
			VDir::SplitString(linesCate.at(i).c_str(), VD_SEPERATOR, vec);
			
			if (vec.size() >= 2)
			{
				VStr cate = vec.at(0);
				VStr key = vec.at(1);

				VStr tmpCate = cate;
				VStr tmpKey = key;
				ToLower(tmpCate);
				ToLower(tmpKey);

				if (m_loweCategory.find(tmpCate) == m_loweCategory.end())
				{
					std::vector<VStr> tmpKeyVec;
					tmpKeyVec.push_back(tmpKey);
					m_loweCategory[tmpCate] = tmpKeyVec;
					
					std::vector<VStr> keyVec;
					keyVec.push_back(key);
					m_category[cate] = keyVec;

				}
				else
				{
					int size3 = (int)m_loweCategory[tmpCate].size();
					bool isExist = false;
					for (int k = 0; k < size3; k++)
					{
						if (m_loweCategory[tmpCate].at(k) == tmpKey)
						{
							isExist = true;
							break;
						}
					}
					if (!isExist)
					{
						m_loweCategory[tmpCate].push_back(tmpKey);
						m_category[cate].push_back(key);
					}
				}
			}
		}
	}


}
void VD::VTRN::WriteSRC(const VStr &rcFilePath)
{
	std::vector<VStr> dicLines;
	std::vector<VStr> cateLines;

	std::map<VStr, VStr>::iterator i1 = m_dic.begin();
	std::map<VStr, VStr>::iterator e1 = m_dic.end();
	for (i1; i1 != e1; ++i1)
	{
		dicLines.push_back(i1->first + VD_SEPERATOR + i1->second);
	}
	std::map<VStr, std::vector<VStr>>::iterator i2 = m_category.begin();
	std::map<VStr, std::vector<VStr>>::iterator e2 = m_category.end();
	for (i2; i2 != e2; ++i2)
	{
		int size = (int)i2->second.size();
		for (int i = 0; i < size; ++i)
		{
			cateLines.push_back(i2->first + VD_SEPERATOR + i2->second.at(i));
		} 
	}

	VStr categoryFilePath = (VStr)VDir::GetParentDir(rcFilePath.c_str());

	std::ofstream ofs;
	ofs.open(rcFilePath);
	if (ofs.is_open())
	{
		VTrnFile::WriteAllLines(ofs, dicLines);
	}
	ofs.close();

	std::ofstream ofs2;
	ofs2.open(categoryFilePath);
	if (ofs2.is_open())
	{
		VTrnFile::WriteAllLines(ofs2, cateLines);
	}
	ofs2.close();

}
//VStr VD::VTRN::Add(const VStr &key, const VStr &category)
//{
//	VStr tmpKey = key;
//	VStr tmpCate = category;
//	ToLower(tmpKey);
//	ToLower(tmpCate);
//
//	VStr retVal = key;
//	std::map<VStr, VStr>::iterator fi = m_loweDic.find(tmpKey);
//	//if (fi == m_loweDic.end())
//	//{
//	//	m_loweDic[tmpKey] = tmpKey;
//	//	m_dic[key] = key;
//		
//	//}
//	//else
//	//{
//		std::map<VStr, VStr>::iterator fii = m_dic.find(key);
//		if (fii != m_dic.end())
//			retVal = fii->second;//m_dic[key];
//		//else
//		//{
//		//	LogFileWriter::WriteUnknownExceptionLog("VD::VTRN  Key is in low dictionary , but not exist in real dictionary",key);
//		//	retVal = key;
//		//}
//	//}
//
//	if (m_loweCategory.find(tmpCate) == m_loweCategory.end())
//	{
//		std::vector<VStr> keyVec;
//		keyVec.push_back(tmpKey);
//		m_loweCategory[tmpCate] = keyVec;
//	
//	}
//	else
//	{
//		int size = (int)m_loweCategory[tmpCate].size();
//		bool isExist = false;
//		for (int i = 0; i < size; i++)
//		{
//			if (m_loweCategory[tmpCate].at(i) == tmpKey)
//			{
//				isExist = true;
//				break;
//			}
//		}
//		if (!isExist)
//		{
//			m_loweCategory[tmpCate].push_back(tmpKey);
//			m_category[category].push_back(key);
//		}
//	}
//
//
//	return retVal;
//
//
//}
VStr VD::VTRN::Add(const VStr &key, const VStr &category)
{
	std::map<VStr, VStr>::iterator fii = m_dic.find(key);
	if (fii != m_dic.end())
		return fii->second;
	else
		m_dic[key] = key;
	return key;
}
VStr VD::VTRN::GetSelectedLanguange(const VStr& filePath)
{
	VStr languange = "ENG";

	std::ifstream ifs(filePath);
	if (ifs.is_open())
	{
		std::vector<std::string> vec;
		VTrnFile::GetAllLines(ifs, vec);
		ifs.close();
		if (vec.size() > 0)
			languange = vec.at(0);
	}

	return languange;
}
void VD::VTRN::CreatePaths()
{
	try
	{
		VStr menuLang = "MenuLangSettings";// m_guiCustomize->GetMenuLangSettings();

		VStr exePath = VGlobalFunctions().GetSolidCncExeFolder();
		VStr mlsPath = exePath + menuLang;
		VStr langsPath = exePath + menuLang + "\\Langs";
		VStr info = exePath + menuLang + "\\Info";
		VStr visible = exePath + menuLang + "\\Visible";

		if (!PathStringFunctions::DirectoryExists(mlsPath.c_str()))//(!VDir::ExistDirectory(mlsPath.c_str()))
			VDir::VCreateFolder(exePath.c_str(),/*"MenuLangSettings"*/menuLang.c_str());

		if (PathStringFunctions::DirectoryExists(mlsPath.c_str()))//(VDir::ExistDirectory(mlsPath.c_str()))
		{
			if (!PathStringFunctions::DirectoryExists(langsPath.c_str()))//(!VDir::ExistDirectory(langsPath.c_str()))
				VDir::VCreateFolder(mlsPath.c_str(), "Langs");

			if (!PathStringFunctions::DirectoryExists(info.c_str()))//(!VDir::ExistDirectory(info.c_str()))
				VDir::VCreateFolder(mlsPath.c_str(), "Info");
			if (!PathStringFunctions::DirectoryExists(visible.c_str()))//(!VDir::ExistDirectory(visible.c_str()))
				VDir::VCreateFolder(mlsPath.c_str(), "Visible");
		}

		VStr selectedLang = GetSelectedLanguange(info + "\\SelectedLanguage.txt");

		//m_folderPath = mlsPath;
		//m_srcFilePath = langsPath + "\\SRC.txt";
		m_selLangFilePath = langsPath + "\\" + selectedLang + ".txt";
		//m_ckRCWriteFilePath = info + "\\RCWrite.txt";
		m_visibleFilePath = visible + "\\Visible.txt";

		if (!VDir::ExistFile((info + "\\SelectedLanguage.txt").c_str()))
			VDir::VCreateFile(info.c_str(), "SelectedLanguage.txt");

	}
	catch (std::exception &e)
	{
		LogFileWriter::WriteExceptionLog(e);
	}
	catch (...)
	{
		LogFileWriter::WriteUnknownExceptionLog("Unknown Excep. VDialogTranslater::CreatePaths");
	}

}

void VD::VTRN::ToLower(VStr &str)
{
	std::transform(str.begin(), str.end(), str.begin(), ::tolower);
}
void VD::VTRN::SetSlectedLanguange(const VStr & langName)
{
	VStr exePath = VGlobalFunctions().GetSolidCncExeFolder(); 
	VStr menuLang = "MenuLangSettings"; //m_guiCustomize->GetMenuLangSettings();
	VStr fPath	 = exePath + menuLang + "\\Info\\SelectedLanguage.txt"; 
	  
	std::ofstream ofs(fPath);  
	if(ofs.is_open())
	{ 
		std::vector<std::string> lines;
		lines.push_back(langName);
		VTrnFile::WriteAllLines(ofs ,lines);
	}
	ofs.close();
}
 