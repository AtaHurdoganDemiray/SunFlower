#ifndef VDir_h
#define VDir_h

#include <iostream>
#include <Windows.h>
#include <vector>


class VDir
{
public:
	VDir(void);
	~VDir(void);
	//static std::string ConvertWcharToString(const WCHAR *wc);
	//static std::string ConverWstringToString(const std::wstring &wstr);
	//static std::wstring ConvertStringToWString(const std::string & str);
	static bool ExistDirectory(const CString& directoryPath);
	static bool ExistFile(const CString &filePath);  
	static std::vector<CString> GetFiles(const CString &rootPath);

	static CString VCreateFolder(const CString &root,const CString & folder);
	static bool VCreateFile(const CString &rootPath,const CString & fileName);
	static CString VGetFileExtension(const CString &fileName);
	static CString VGetFileNameWithoutExtension(const CString &fileName);
	static CString VGetName(const CString &fullPath);
private:
	static void FillFileListToVector(std::vector<CString> &fileVec,CString rootPath);
	static void FillFolderListToVector(std::vector<CString> &folderVec, CString rootPath);
	static void FillFilesVector(std::vector<CString> &filesVec, CString rootPath);
public:
	static std::vector<CString> GetFileList(const CString &rootPath);
	static std::vector<CString> GetFolderList(const CString &rootPath);
	static void SplitString(const CString &src, const char seperator, std::vector<CString> &result);
	static CString VCreateFolder(const CString& folderPath);
	static CString GetParentDir(const CString& path);
	static CString GetRelativePath(const CString& rootPath, const CString &fullPath);

};

#endif
