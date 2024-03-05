// VFileDirectoryFunctions.cpp

#include "stdafx.h"
#include "VFileDirectoryFunctions.h"
#include "TypeConverters.h"
#include "DosyaPaketleme.h"
#include "PathStringFunctions.h"

bool VFileDirectoryFunctions::CollectAllFilesUnder(const VStr &path, std::list<VStr> &allFiles)
{
	try
	{
		VStr s1(".");
		VStr s2("..");
		VStr pastr(path);
		pastr += "*";
		WIN32_FIND_DATA ffd;
		HANDLE hSearch = FindFirstFile(pastr.c_str() , &ffd);

		if (hSearch == INVALID_HANDLE_VALUE)
			return false;
		std::list<WIN32_FIND_DATA> ffdparents;
		do
		{
			if (ffd.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY)
			{
				//_tprintf(TEXT("  %s   <DIR>\n"), ffd.cFileName);
				//CollectFilesRecursive(path, ffdparents,ffd, allFiles);
				//allFiles.push_back(ffd.cFileName);
				VStr tmp(ffd.cFileName);
				if (tmp != s1 && tmp != s2)
				{
					VStr myp2(path + ffd.cFileName);
					myp2 += "\\";
					CollectAllFilesUnder(myp2, allFiles);
				}
			}
			else
			{
				//LARGE_INTEGER filesize;
				//filesize.LowPart = ffd.nFileSizeLow;
				//filesize.HighPart = ffd.nFileSizeHigh;
				//_tprintf(TEXT("  %s   %ld bytes\n"), ffd.cFileName, filesize.QuadPart);
				//allFiles.push_back(GetCompletePath(path,ffdparents,ffd));
				allFiles.push_back(path + ffd.cFileName);
			}

		} while (FindNextFile(hSearch, &ffd) != FALSE);
		
		DWORD dwError = GetLastError();
		if (dwError != ERROR_NO_MORE_FILES)
		{
			CString mess;
			mess.Format("VFileDirectoryFunctions error message code %d", dwError);
			VpAfxMessageBox(mess);
		}
		FindClose(hSearch);
		return true;
	}
	catch (VException &e)
	{
		LogFileWriter::WriteExceptionLog(e, "VFileDirectoryFunctions::CollectAllFilesUnder");
		return false;
	}
	catch (...)
	{
		LogFileWriter::WriteUnknownExceptionLog("UE " , "VFileDirectoryFunctions::CollectAllFilesUnder");
		return false;
	}
}
bool VFileDirectoryFunctions::CollectAllFilesUnderNonRecursive(const VStr& path, std::list<VStr>& allFiles)
{
	try
	{
		VStr s1(".");
		VStr s2("..");
		VStr pastr(path);
		pastr += "*";
		WIN32_FIND_DATA ffd;
		HANDLE hSearch = FindFirstFile(pastr.c_str(), &ffd);

		if (hSearch == INVALID_HANDLE_VALUE)
			return false;
		std::list<WIN32_FIND_DATA> ffdparents;
		do
		{
			if (ffd.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY)
			{
				////_tprintf(TEXT("  %s   <DIR>\n"), ffd.cFileName);
				////CollectFilesRecursive(path, ffdparents,ffd, allFiles);
				////allFiles.push_back(ffd.cFileName);
				//VStr tmp(ffd.cFileName);
				//if (tmp != s1 && tmp != s2)
				//{
				//	VStr myp2(path + ffd.cFileName);
				//	myp2 += "\\";
				//	CollectAllFilesUnder(myp2, allFiles);
				//}
			}
			else
			{
				//LARGE_INTEGER filesize;
				//filesize.LowPart = ffd.nFileSizeLow;
				//filesize.HighPart = ffd.nFileSizeHigh;
				//_tprintf(TEXT("  %s   %ld bytes\n"), ffd.cFileName, filesize.QuadPart);
				//allFiles.push_back(GetCompletePath(path,ffdparents,ffd));
				allFiles.push_back(path + ffd.cFileName);
			}

		} while (FindNextFile(hSearch, &ffd) != FALSE);

		DWORD dwError = GetLastError();
		if (dwError != ERROR_NO_MORE_FILES)
		{
			CString mess;
			mess.Format("VFileDirectoryFunctions error message code %d", dwError);
			VpAfxMessageBox(mess);
		}
		FindClose(hSearch);
		return true;
	}
	catch (VException& e)
	{
		LogFileWriter::WriteExceptionLog(e, "VFileDirectoryFunctions::CollectAllFilesUnder");
		return false;
	}
	catch (...)
	{
		LogFileWriter::WriteUnknownExceptionLog("UE ", "VFileDirectoryFunctions::CollectAllFilesUnder");
		return false;
	}
}
bool VFileDirectoryFunctions::CollectAllFilesAndSizesUnder(const VStr &path, std::list<VStr> &allFiles, std::list<size_t>&allSizes)
{
	try
	{
		VStr s1(".");
		VStr s2("..");
		VStr pastr(path);
		pastr += "*";
		WIN32_FIND_DATA ffd;
		HANDLE hSearch = FindFirstFile(pastr.c_str(), &ffd);

		if (hSearch == INVALID_HANDLE_VALUE)
			return false;
		std::list<WIN32_FIND_DATA> ffdparents;
		do
		{
			if (ffd.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY)
			{
				//_tprintf(TEXT("  %s   <DIR>\n"), ffd.cFileName);
				//CollectFilesRecursive(path, ffdparents,ffd, allFiles);
				//allFiles.push_back(ffd.cFileName);
				VStr tmp(ffd.cFileName);
				if (tmp != s1 && tmp != s2)
				{
					VStr myp2(path + ffd.cFileName);
					myp2 += "\\";
					CollectAllFilesAndSizesUnder(myp2, allFiles , allSizes);
				}
			}
			else
			{
				//LARGE_INTEGER filesize;
				//filesize.LowPart = ffd.nFileSizeLow;
				//filesize.HighPart = ffd.nFileSizeHigh;
				//_tprintf(TEXT("  %s   %ld bytes\n"), ffd.cFileName, filesize.QuadPart);
				//allFiles.push_back(GetCompletePath(path,ffdparents,ffd));
				allFiles.push_back(path + ffd.cFileName);
				size_t myfilesize = ffd.nFileSizeHigh * MAXDWORD + ffd.nFileSizeLow;
				allSizes.push_back(myfilesize);
			}

		} while (FindNextFile(hSearch, &ffd) != FALSE);

		DWORD dwError = GetLastError();
		if (dwError != ERROR_NO_MORE_FILES)
		{
			CString mess;
			mess.Format("VFileDirectoryFunctions error message code %d", dwError);
			VpAfxMessageBox(mess);
		}
		FindClose(hSearch);
		return true;
	}
	catch (VException &e)
	{
		LogFileWriter::WriteExceptionLog(e, "VFileDirectoryFunctions::CollectAllFilesUnder");
		return false;
	}
	catch (...)
	{
		LogFileWriter::WriteUnknownExceptionLog("UE ", "VFileDirectoryFunctions::CollectAllFilesUnder");
		return false;
	}
}
bool VFileDirectoryFunctions::ImitateAllDirectories(const VStr &imitationBaseLikeViewpoint, const VStr &dirBaseLikeViewpoint, const VStr &imitationBasePath, const VStr &dirBasePath,const VStr &startPath)
{
	try
	{
		VStr s1(".");
		VStr s2("..");
		VStr pastr(startPath);
		pastr += "*";
		WIN32_FIND_DATA ffd;
		HANDLE hSearch = FindFirstFile(pastr.c_str(), &ffd);

		if (hSearch == INVALID_HANDLE_VALUE)
			return false;
	
		
		LPSECURITY_ATTRIBUTES lpSecurityAtr;
		SECURITY_ATTRIBUTES *tmp = new SECURITY_ATTRIBUTES();
		tmp->lpSecurityDescriptor = NULL;
		lpSecurityAtr = tmp;
		
		do
		{
			if (ffd.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY)
			{
				//_tprintf(TEXT("  %s   <DIR>\n"), ffd.cFileName);
				//CollectFilesRecursive(path, ffdparents,ffd, allFiles);
				//allFiles.push_back(ffd.cFileName);
				VStr tmp(ffd.cFileName);
				if (tmp != s1 && tmp != s2)
				{
					VStr myp2(startPath + ffd.cFileName);
					myp2 += "\\";
					VStr dirToCopy = OriginalToDir(imitationBaseLikeViewpoint, dirBaseLikeViewpoint, dirBasePath, myp2);
					dirToCopy += "\\";
	
					BOOL myck = CreateDirectory(dirToCopy.c_str(),lpSecurityAtr);
					if (myck == FALSE)
					{
						DWORD errorcode = GetLastError();
						if (errorcode == ERROR_PATH_NOT_FOUND)
						{
							std::vector<VStr> spl;
							TypeConverters::SplitString(dirToCopy,'\\',spl);
							int splsize = (int)spl.size();
							VStr ppp(spl.at(0));
							ppp += "\\";
							bool startflg = false;
							for (int i = 1; i < spl.size(); ++i)
							{
								VStr curStr = spl.at(i);
								ppp += curStr;
								ppp += "\\";
								if (curStr == dirBaseLikeViewpoint)
									startflg = true;
								if (startflg == true)
								{
									CreateDirectory(ppp.c_str(), lpSecurityAtr);
								}
							}
						}
					}
					ImitateAllDirectories(imitationBaseLikeViewpoint, dirBaseLikeViewpoint, imitationBasePath, dirBasePath, myp2);
				}
			}
		} while (FindNextFile(hSearch, &ffd) != FALSE);
		FindClose(hSearch);
		return true;
	}
	catch (VException &e)
	{
		LogFileWriter::WriteExceptionLog(e,"VFileDirectoryFunctions::ImitateAllDirectories");
		return false;
	}
	catch (...)
	{
		LogFileWriter::WriteUnknownExceptionLog("UE ","VFileDirectoryFunctions::ImitateAllDirectories");
		return false;
	}
}

//bool VFileDirectoryFunctions::CollectFilesRecursive(const VStr &rootpath, std::list<WIN32_FIND_DATA> ffdparents, WIN32_FIND_DATA &ffd , std::list<VStr> &allFiles)
//{
//	VStr mypath = GetCompletePath(rootpath, ffdparents, ffd);
//	WIN32_FIND_DATA ffd2;
//	HANDLE hSearch = FindFirstFile(mypath.c_str(), &ffd2);
//	if (hSearch == INVALID_HANDLE_VALUE)
//		return false;
//	do
//	{
//		if (ffd.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY)
//		{
//			bool ck = CollectFilesRecursive(rootpath, ffdparents, ffd2 , allFiles);
//		}
//		else
//		{
//			allFiles.push_back(GetCompletePath(rootpath,ffdparents,ffd2));
//		}
//
//	} while (FindNextFile(hSearch, &ffd2) != FALSE);
//
//	FindClose(hSearch);
//	return true;
//}
//
//VStr VFileDirectoryFunctions::GetCompletePath(const VStr &rootpath , const std::list<WIN32_FIND_DATA> &ffdparents, const WIN32_FIND_DATA &ffd)
//{
//	VStr mypath(rootpath);
//	std::list<WIN32_FIND_DATA>::const_iterator ii = ffdparents.begin();
//	std::list<WIN32_FIND_DATA>::const_iterator ee = ffdparents.end();
//	for (ii; ii != ee; ++ii)
//	{
//		mypath += ii->cFileName;
//		mypath += "\\";
//	}
//	mypath += ffd.cFileName;
//	return mypath;
//}

bool VFileDirectoryFunctions::CopyFromOriginalToDir(const VStr &originalBaseLikeViewpoint , const VStr &dirBaseLikeViewpoint, const VStr &imitationBaseLikeViewpoint , const VStr &originalBasePath , const VStr &dirBasePath , const VStr &imitationBasePath)
{
	try
	{
		std::list<VStr> allFiles;
		CollectAllFilesUnder(imitationBasePath , allFiles);
		std::list<VStr> originalFiles,destFiles;
		DirToOriginal(originalBaseLikeViewpoint , imitationBaseLikeViewpoint , originalBasePath , allFiles, originalFiles);
		OriginalToDir(originalBaseLikeViewpoint , dirBaseLikeViewpoint,dirBasePath,originalFiles, destFiles);
		std::list<VStr>::iterator i = originalFiles.begin();
		std::list<VStr>::iterator e = originalFiles.end();
		std::list<VStr>::iterator di = destFiles.begin();
		if (originalFiles.size() != destFiles.size())
			throw VException("VFileDirectoryFunctions::CopyFromOriginalToDir exception , originalFiles size not equal to destFiles size",1);
		for (i; i != e; ++i)
		{
			CopyFile((*i).c_str(), (*di).c_str(), FALSE);
			++di;
		}
		return true;
	}
	catch (VException &e)
	{
		LogFileWriter::WriteExceptionLog(e, "VFileDirectoryFunctions::CopyFromOriginalToDir");
		return false;
	}
	catch (...)
	{
		LogFileWriter::WriteUnknownExceptionLog("UE ", "VFileDirectoryFunctions::CopyFromOriginalToDir");
		return false;
	}
}

bool VFileDirectoryFunctions::CopyFromDirToOriginal(const VStr &originalBaseLikeViewpoint , const VStr &dirBaseLikeViewpoint, const VStr &imitationBaseLikeViewpoint ,const VStr &originalBasePath , const VStr &dirBasePath , const VStr &imitationBasePath)
{
	try
	{
		std::list<VStr> allFiles;
		CollectAllFilesUnder(imitationBasePath, allFiles);
		std::list<VStr> dirFiles, destFiles;
		DirToOriginal(dirBaseLikeViewpoint, imitationBaseLikeViewpoint , dirBasePath , allFiles, dirFiles);
		DirToOriginal(originalBaseLikeViewpoint, imitationBaseLikeViewpoint, originalBasePath , allFiles, destFiles);
		std::list<VStr>::iterator i = dirFiles.begin();
		std::list<VStr>::iterator e = dirFiles.end();
		std::list<VStr>::iterator di = destFiles.begin();
		if (dirFiles.size() != destFiles.size())
			throw VException("VFileDirectoryFunctions::CopyFromDirToOriginal dirFiles size not equal to destFiles size",1);
		for (i; i != e; ++i)
		{
			CopyFile((*i).c_str(), (*di).c_str(), FALSE);
			++di;
		}
		return true;
	}
	catch (VException &e)
	{
		LogFileWriter::WriteExceptionLog(e, "VFileDirecotryFunctions::CopyFromDirToOriginal");
		return false;
	}
	catch (...)
	{
		LogFileWriter::WriteUnknownExceptionLog("UE ", "VFileDirecotryFunctions::CopyFromDirToOriginal");
		return false;
	}
}

bool VFileDirectoryFunctions::CopyFromDirToPack(const VStr &fullPackPath , /*const VStr &originalBaseLikeViewpoint, */const VStr &dirBaseLikeViewpoint, const VStr &imitationBaseLikeViewpoint/*, const VStr &originalBasePath*/, const VStr &dirBasePath, const VStr &imitationBasePath)
{
	try
	{
		std::list<VStr> allFiles;
		std::list<size_t> allSizes;
		CollectAllFilesAndSizesUnder(imitationBasePath, allFiles, allSizes);
		Bofstream f(fullPackPath);
		if (f.is_open() == false)
			throw VException("VFileDirectoryFunctions::CopyFromDirToPack exception , package file can not be opened!",1);
		f.WriteString(dirBaseLikeViewpoint);
		f.WriteString(dirBasePath);
		//std::list<VStr> dirFiles/*, destFiles*/;
		//DirToOriginal(dirBaseLikeViewpoint, imitationBaseLikeViewpoint, dirBasePath, allFiles, dirFiles);
		//DirToOriginal(originalBaseLikeViewpoint, imitationBaseLikeViewpoint, originalBasePath, allFiles, destFiles);
		std::list<VStr>::iterator i = allFiles.begin();//dirFiles.begin();
		std::list<VStr>::iterator e = allFiles.end();//dirFiles.end();
		std::list<size_t>::iterator si = allSizes.begin();
		//std::list<VStr>::iterator di = destFiles.begin();
		//if (dirFiles.size() != destFiles.size())
		//	throw VException("VFileDirectoryFunctions::CopyFromDirToOriginal dirFiles size not equal to destFiles size", 1);
		size_t mylistsize = allFiles.size();
		f.WriteSizeT(mylistsize);
		for (i; i != e; ++i)
		{
			//CopyFile((*i).c_str(), (*di).c_str(), FALSE);
			//++di;
			f.WriteString(*i);
			f.WriteSizeT(*si);
			++si;
		}


		i = allFiles.begin();//dirFiles.begin();
		e = allFiles.end();//dirFiles.end();
		si = allSizes.begin();
		
		for (i; i != e; ++i)
		{
			//DosyaPaketleme::PaketeEkle(const std::string &dosyaAd, size_t dosyaBoyu, std::ofstream &f, const std::string &nereyiPaketliyim)
			PathStringFunctions pf;
			VStr onlyFileName = pf.GetJustFileName(*i);
			VStr fileDirectoryWithoutName = pf.GetDirectoryName(*i);
			DosyaPaketleme::PaketeEkle(onlyFileName,*si,f,fileDirectoryWithoutName);
			++si;
		}
		f.close();
		return true;
	}
	catch (VException &e)
	{
		LogFileWriter::WriteExceptionLog(e, "VFileDirecotryFunctions::CopyFromDirToPack");
		return false;
	}
	catch (...)
	{
		LogFileWriter::WriteUnknownExceptionLog("UE ", "VFileDirecotryFunctions::CopyFromDirToPack");
		return false;
	}
}

bool VFileDirectoryFunctions::OpenPackToDir(const VStr &fullPackPath,VStr &dirBaseLikeViewpoint ,VStr &dirBasePath)
{
	try
	{
		Bifstream f(fullPackPath);
		if (f.is_open() == false)
			throw VException("VFileDirectoryFunctions::OpenPackToDir exception , package file can not be opened!",1);
		VStr originalBaseLikeViewpoint, originalBasePath;
		f.ReadString(originalBaseLikeViewpoint);
		f.ReadString(originalBasePath);
		dirBaseLikeViewpoint = originalBaseLikeViewpoint;
		dirBasePath = VGlobalFunctions::GetSolidCncExeFolder() + "VConfiguration\\" + dirBaseLikeViewpoint + "\\";
		size_t mylistsize;
		f.ReadSizeT(mylistsize);
		std::list<VStr> filePaths;
		std::list<size_t> fileSizes;
		for (size_t i = 0; i < mylistsize;++i)
		{
			//CopyFile((*i).c_str(), (*di).c_str(), FALSE);
			//++di;
			VStr curFileName;
			size_t curFileSize;
			f.ReadString(curFileName);
			f.ReadSizeT(curFileSize);
			filePaths.push_back(curFileName);
			fileSizes.push_back(curFileSize);
		}
		std::list<VStr>::iterator pi = filePaths.begin();
		std::list<VStr>::iterator pe = filePaths.end();
		std::list<size_t>::iterator si = fileSizes.begin();

		for (pi; pi != pe; ++pi)
		{
			//static VStr OriginalToDir(const VStr &originalBaseLikeViewpoint, const VStr &dirBaseLikeViewpoint, const VStr &dirBasePath, const VStr &originalName);
			VStr curFileFullPath = OriginalToDir(originalBaseLikeViewpoint,dirBaseLikeViewpoint,dirBasePath,*pi);
			PathStringFunctions pf;
			VStr onlyFileName = pf.GetJustFileName(curFileFullPath);
			VStr fileDir = pf.GetDirectoryName(curFileFullPath);

			//static void PakettenCikar(VIFStream &f, const VStr &dosyaAd, const VStr &anaDizin, size_t dosyaBoyu);
			DosyaPaketleme::PakettenCikar(f, onlyFileName, fileDir, *si);
			++si;
		}
		f.close();
		return true;
	}
	catch (VException &e)
	{
		LogFileWriter::WriteExceptionLog(e, "VFileDirectoryFunctions::OpenPackToDir");
		return false;
	}
	catch (...)
	{
		LogFileWriter::WriteUnknownExceptionLog("UE ", "VFileDirectoryFunctions::OpenPackToDir");
		return false;
	}
}

void VFileDirectoryFunctions::DirToOriginal(const VStr &originalBaseLikeViewpoint , const VStr &dirBaseLikeViewpoint, const VStr &originalBasePath ,const std::list<VStr> &dirs, std::list<VStr> &originals)
{
	std::list<VStr>::const_iterator si = dirs.begin();
	std::list<VStr>::const_iterator se = dirs.end();
	for (si; si != se; ++si)
	{
		VStr curorig = DirToOriginal(originalBaseLikeViewpoint, dirBaseLikeViewpoint, originalBasePath ,*si);
		originals.push_back(curorig);
	}
}

void VFileDirectoryFunctions::OriginalToDir(const VStr &originalBaseLikeViewpoint , const VStr &dirBaseLikeViewpoint, const VStr &dirBasePath , const std::list<VStr> &originals, std::list<VStr> &dirs)
{
	std::list<VStr>::const_iterator si = originals.begin();
	std::list<VStr>::const_iterator se = originals.end();
	for (si; si != se; ++si)
	{
		VStr curdir = OriginalToDir(originalBaseLikeViewpoint, dirBaseLikeViewpoint, dirBasePath ,*si);
		dirs.push_back(curdir);
	}
}

VStr VFileDirectoryFunctions::DirToOriginal(const VStr &originalBaseLikeViewpoint , const VStr &dirBaseLikeViewpoint ,const VStr &originalBaseDir , const VStr &dirName)
{
	std::vector<VStr> spl;
	TypeConverters::SplitString(dirName, '\\', spl);
	VStr result(originalBaseDir);
	bool ck = false;
	VStr backSlashStr("\\");
	CString dirBaseSeperator(dirBaseLikeViewpoint.c_str());
	int counter = 0;
	int splsize = (int)spl.size();
	for (int i = 1; i < splsize;++i)
	{
		if (dirBaseSeperator.CompareNoCase(spl.at(i).c_str()) == 0)
		{
			ck = true;
			counter = i + 1;
			break;
		}
	}
	if (ck == false)
		throw VException("VFileDirectoryFunctions::DirToOriginal exception dirBaseLikeViewPoint not found in dir",1);
	int splsizeminus1(splsize - 1);
	for (int i = counter; i < splsize; ++i)
	{
		result += spl.at(i);
		if (i != splsizeminus1)
		{
			result += "\\";
		}
	}
	return result;
}

VStr VFileDirectoryFunctions::OriginalToDir(const VStr &originalBaseLikeViewpoint, const VStr &dirBaseLikeViewpoint, const VStr &dirBasePath , const VStr &originalName)
{
	std::vector<VStr> spl;
	TypeConverters::SplitString(originalName, '\\', spl);
	VStr result(dirBasePath);
	bool ck = false;
	VStr backSlashStr("\\");
	CString originalBaseSeperator(originalBaseLikeViewpoint.c_str());
	int counter = 0;
	int splsize = (int)spl.size();
	for (int i = 1; i < splsize; ++i)
	{
		if (originalBaseSeperator.CompareNoCase(spl.at(i).c_str()) == 0)
		{
			ck = true;
			counter = i + 1;
			break;
		}
	}
	if (ck == false)
		throw VException("VFileDirectoryFunctions::DirToOriginal exception dirBaseLikeViewPoint not found in dir", 1);
	int splsizeminus1(splsize - 1);
	for (int i = counter; i < splsize; ++i)
	{
		result += spl.at(i);
		if (i != splsizeminus1)
		{
			result += "\\";
		}
	}
	return result;
}

BOOL VFileDirectoryFunctions::CreateDirectoryWithMissingPath(const VStr &dir)
{
	BOOL ck = CreateDirectory(dir.c_str(),0);
	if (ck == ERROR_PATH_NOT_FOUND || ck == FALSE)
	{
	//	// if last path idiom is not existing it will add that other missing items are not added , this error occur
	//	// make the missing ones
	//	bool goontrying = true;
	//	VStr subDir = dir;
	//	while (goontrying)
	//	{
	//		subDir = subDir.substr(0, subDir.rfind('\\', subDir.length() - 2));
	//		BOOL ck2 = CreateDirectory(subDir.c_str(), 0);
	//		
	//		if (ck2 != ERROR_PATH_NOT_FOUND)
	//		{
	//			goontrying = false;
	//		}
	//	}
	//	return 1;
	//}
	//else
	//{
	//	// if directory is created return value is non zero 
	//	// ERROR_ALREADY_EXISTS   ->  already exists
	//	return ck;
		try
		{
			std::vector<VStr> allDirVec;
			TypeConverters::SplitString(dir, '\\', allDirVec);
			size_t mysize = allDirVec.size();
			VStr mypath;
			for (size_t i = 0; i < mysize; ++i)
			{
				mypath += allDirVec.at(i);
				mypath += '\\';
				if (PathStringFunctions::DirectoryExists(mypath.c_str()) == false)
				{
					BOOL ck = CreateDirectory(mypath.c_str() , 0);
					if (ck == FALSE)
					{
						throw VException(VStr("VFileDirectoryFunctions::CreateDirectoryWithMissingPath exception , CreateDirectory failed ") + mypath, 1);
					}
				}
			}
			return TRUE;
		}
		catch (VException &e)
		{
			LogFileWriter::WriteExceptionLog(e,"VFileDirectoryFunctions::CreateDirectoryWithMissingPath");
			return FALSE;
		}
		catch (...)
		{
			LogFileWriter::WriteUnknownExceptionLog("UE ", "VFileDirectoryFunctions::CreateDirectoryWithMissingPath");
			return FALSE;
		}
	}
	return TRUE;
}
