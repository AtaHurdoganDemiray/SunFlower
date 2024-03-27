#include "stdafx.h"
#include "VDir.h" 
#include <fstream>

VDir::VDir(void)
{
	
}


VDir::~VDir(void)
{
}
 
bool VDir::ExistDirectory(const CString& directoryPath)
{
   WIN32_FIND_DATA fData;
	 
   
	CString str =  ""; 
	str =  directoryPath; 
	 
	str += "\\*.*";

	HANDLE hSearch = FindFirstFile(str, &fData);

	if(hSearch == INVALID_HANDLE_VALUE)
	{
		FindClose(hSearch);
		return false;
	}
	if ( (fData.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY) == FILE_ATTRIBUTE_DIRECTORY)
	{
		FindClose(hSearch);
		return true;
	}
	FindClose(hSearch);
  return true;
}
bool VDir::ExistFile(const CString &filePath)
{
    WIN32_FIND_DATA fData;
	 
	CString str =""; 

	str =  filePath;  

	HANDLE hSearch = FindFirstFile( str , &fData);
	if(hSearch == INVALID_HANDLE_VALUE)
	{
		FindClose(hSearch);
		return false;
	}

	if ( (fData.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY) == FILE_ATTRIBUTE_DIRECTORY)
	{
		FindClose(hSearch);
		return false;
	}
	FindClose(hSearch);
  return true;
}
void VDir::FillFileListToVector(std::vector<CString> &fileVec, CString rootPath)
{
	WIN32_FIND_DATA fData;
	 
	CString str = ""; 
	str = rootPath; 
	 
	str +="\\*.*";
	

	HANDLE hSearch = FindFirstFile( str , &fData);

	if(hSearch == INVALID_HANDLE_VALUE)
	{
		FindClose(hSearch);
		return;
	}

	do
	{
		if ( (fData.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY) == FILE_ATTRIBUTE_DIRECTORY)		
		{
			// this is a folder 
			CString tempfname = "";
			tempfname =  fData.cFileName ; 

			if (tempfname != "." && tempfname != "..")
			{
				CString yeniKokDizin (rootPath); 

				yeniKokDizin +="\\";
				yeniKokDizin += fData.cFileName ; 
				
				FillFileListToVector(fileVec,yeniKokDizin);
			}
		}
		else
		{
			// this is a File 
			CString toplamyol (rootPath);
			  
			CString tmp =  fData.cFileName;
		 
			toplamyol +="\\" +tmp;  

			fileVec.push_back (toplamyol);
	
		}
		
	}
	while (FindNextFile(hSearch, &fData));
	
	FindClose(hSearch);
}
void VDir::FillFolderListToVector(std::vector<CString> &folderVec, CString rootPath)
{
	WIN32_FIND_DATA fData;
	 
	CString str = ""; 
	str = rootPath; 
	 
	str += "\\*.*";
	

	HANDLE hSearch = FindFirstFile( str , &fData);

	if(hSearch == INVALID_HANDLE_VALUE)
	{
		FindClose(hSearch);
		return;
	}

	do
	{
		if ( (fData.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY) == FILE_ATTRIBUTE_DIRECTORY)		
		{
			// this is a folder 
			CString tempfname = "";
			tempfname = fData.cFileName; 

			if (tempfname != "." && tempfname != "..")
			{
				CString yeniKokDizin (rootPath); 

				yeniKokDizin +="\\" ;
				yeniKokDizin += fData.cFileName;

				folderVec.push_back(yeniKokDizin); 

				FillFolderListToVector(folderVec,yeniKokDizin);
			}
		}
		else
		{
			// this is a File 
			  
		}
		
	}
	while (FindNextFile(hSearch, &fData));
	
	FindClose(hSearch);
}
 std::vector<CString> VDir::GetFileList(const CString &rootPath)
 {
	 std::vector<CString> vec ;
	 FillFileListToVector(vec,rootPath);
	 return vec;
 }
 std::vector<CString> VDir::GetFolderList(const CString &rootPath)
 {
	 std::vector<CString> vec ;
	 FillFolderListToVector(vec,rootPath);
	 return vec;
 }
 void VDir::FillFilesVector(std::vector<CString> &filesVec,CString rootPath)
{
	WIN32_FIND_DATA fData;
	 
	CString str = ""; 
	str = rootPath; 
	 
	str += "\\*.*";
	

	HANDLE hSearch = FindFirstFile( str, &fData);
 
	do
	{ 
		CString tempfname = "";
		tempfname = fData.cFileName; 

		if (tempfname != "." && tempfname != "..")
		{
			CString yeniKokDizin (rootPath); 

			CString dosyaAdi = fData.cFileName;

			yeniKokDizin +="\\"+ dosyaAdi;

			if(ExistDirectory(yeniKokDizin) || ExistFile(yeniKokDizin))
			filesVec.push_back(yeniKokDizin);  
		}
		 
	}
	while (FindNextFile(hSearch, &fData));
	
	FindClose(hSearch);
}
 std::vector<CString> VDir::GetFiles(const CString &rootPath)
{
	std::vector<CString> vec;

	FillFilesVector(vec,rootPath);

	return vec;
}
 CString VDir::VCreateFolder(const CString &root,const CString & folder)
 { 
	 CString path = root +_T("\\")+folder; 
	 if(!ExistDirectory(root))
	 {
		 throw std::exception (_T("Root path not exist : VDir::CreateFolder")); 
	 } 
	 if(!ExistDirectory(path))
	 { 
		 
		 CreateDirectory(path ,NULL);
	 } 

	 if(!ExistDirectory(path))
	 {
		 //// Failed to create directory.
		  throw std::exception (_T("Failed to create directory : VDir::CreateFolder")); 
	 }

	 return path;
 }
 bool VDir::VCreateFile(const CString &rootPath,const CString & fileName)
 {  
	 std::ofstream ofs;
	   
	 bool fileCreated    = false;

	 if(!ExistFile(rootPath +_T("\\")+fileName))
	 { 
		 ofs.open(rootPath +_T("\\")+fileName); 
		 if(ofs.is_open())
		 {
			 fileCreated = true;
		 } 
		 ofs.close();
	 }
	 else
		 fileCreated = true;

	 return fileCreated;
 }
 CString VDir::VGetFileExtension(const CString &fileName)
 {
	 
	  CString extension = "";

	  bool hasExtension =false;

	  for(int i=fileName.GetLength()-1;i>0;i--)
	  {
		  if(fileName[i] =='.')
		  { 
			  CString tmpExt = extension;

			  extension += fileName[i];

			  hasExtension = true;

			  break;
		  }
		  else
		  {
			  CString tmpExt = extension;

			  extension += fileName[i];
		  }
	  }

	  if(!hasExtension)
	  {
		  throw std::exception(fileName+_T(" does not have extension ,Excp:  VDir::VGetFileExtension"));
	  }

	  CString retEx = "";
	  
	  for(int i=extension.GetLength()-1;i>=0;i--)
	  {
		  retEx += extension[i];
	  }


	  return retEx;
 }
 CString VDir::VGetFileNameWithoutExtension(const CString &fileName)
 {
	  CString tmpFN = "";

	  bool hasExtension =false;

	  int findIndex =0;

	  for(int i=fileName.GetLength()-1;i>=0;i--)
	  {
		  if(fileName[i] =='.')
		  {
			  findIndex = i;
		 
			  break;
		  } 
	  }
 
	  
	  if(findIndex == 0)
		    tmpFN += fileName;
	  else
	  {
		  for(int i=0;i<findIndex;i++)
		  {
			  tmpFN += fileName[i];
		  }
	  }

	  
	  return tmpFN;
 }

CString VDir::VGetName(const CString &fullPath)
{
	
	CString tmpFileName = "";
	for(int i = fullPath.GetLength()-1; i>=0 ;i--)
	{
		if(fullPath[i] != '\\')
		{
			tmpFileName += fullPath[i];
		}
		else
		{
			break;
		}
	}

	CString retVal ="";
	for(int i = tmpFileName.GetLength() -1;i>=0;i--)
	{
		retVal += tmpFileName[i];
	}


	return retVal;
} 
void VDir::SplitString(const CString &src, const char seperator, std::vector<CString> &result)
{
	int mysize = src.GetLength();
	CString s;
	for (size_t i = 0; i<mysize; ++i)
	{
		char myc = src[i];
		if (myc == seperator)
		{
			if (s.GetLength())
			{
				result.push_back(s);
				s = "";
			}
		}
		else
		{
			s += myc;
		}
	}
	if (s.GetLength())
		result.push_back(s);
}
CString VDir::VCreateFolder(const CString& folderPath)
{
	CString newFolderPath = "";

	CString parentDir = GetParentDir(folderPath);
	std::vector<CString> vec;
	SplitString(parentDir, '\\', vec);
	int size0 = (int)vec.size();
	CString path = "";
	for (int i = 0; i < size0; i++)
	{
		path += vec.at(i);
		if (!VDir::ExistDirectory(path))
		{
			CreateDirectory(path, NULL);
		}
		path += "\\";
	}
	newFolderPath = folderPath;
	if (!VDir::ExistDirectory(newFolderPath))
	{
		CreateDirectory(newFolderPath, NULL);
	}

	return newFolderPath;
}
CString VDir::GetParentDir(const CString& path)
{
	std::vector<CString> vec;
	SplitString(path, '\\', vec);
	int size = (int)vec.size();
	CString parentDir = "";
	for (int i = 0; i < size - 2; i++)
	{
		parentDir += vec.at(i) + "\\";
	}
	if (size - 2 >= 0)
	{
		parentDir += vec.at(size - 2);
	}

	return parentDir;
}
CString VDir::GetRelativePath(const CString& root, const CString &filePath)
{
	std::string tmpRoot = root;
	std::string tmpFilePath = filePath;

	if (root == "")
		return filePath;
	if (tmpFilePath.find(tmpRoot) != std::string::npos)
	{
		std::string relPath = tmpFilePath.substr(tmpRoot.length(), tmpFilePath.length());
		return relPath.c_str();
	}
	return filePath;
}