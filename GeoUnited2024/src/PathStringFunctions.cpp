// PathStringFunctions.cpp

#include "stdafx.h"
#include "PathStringFunctions.h"
#include <tchar.h> 
#include <stdio.h>
#include <strsafe.h>
#include <io.h>     // For access().
#include <sys/types.h>  // For stat().
#include <sys/stat.h>   // For stat().
#include "VDir.h"

#define VP_USEWINDOWSFUNCTIONSFORPATHS 1

const VChr PathStringFunctions::dirDel = '\\';
const VChr PathStringFunctions::dirDelRev = '/';
         
std::vector<VChr> PathStringFunctions::m_unallowedChars = { '*', '.', '"', '/', '\\' , '[' , ']', ':', ';', '|',',','<','>' };
std::map<VStr, PathStringFunctions::PathExistData> PathStringFunctions::m_pathExistData;

VStr PathStringFunctions::TrimExtension ( const VStr & filename )
{
	if (filename == "")
		throw VException("PathStringFunctions::TrimExtension exception , filename is empty",1);
	if (filename.size() >= VGlobalValues::VCHR_LINE_READERSIZE)
		throw VException("PathStringFunctions::TrimExtension exception , filedirectoryname length too long" , 1);
	const VChr * mydata = filename.data();
	VChr * result = VGlobalValues::VCHR_LINE_READER;
	for ( unsigned int i=0; i < filename.length(); i++ )
	{
		if ( mydata[i] == '.' )
		{
			result[i] = '\0';
			break;
		}
		result[i] = mydata[i];
		result[i + 1] = '\0';
	}

	VStr resultstring (result );
	return resultstring;
}

VStr PathStringFunctions::GetDirectoryName ( VStr filepath )
{
	if (filepath == "")
		throw VException("PathStringFunctions::GetDirectoryName exception , filepath is empty",1);
	VStr s1 = filepath.substr( 0, filepath.rfind(dirDel) );//file.length() - file.rfind(dirDel) + 2 );
	VStr s2 = filepath.substr( 0, filepath.rfind(dirDelRev));//file.length() - file.rfind(dirDel) + 2 );
	if ( s1.length() < s2.length() )
		return s1;
	else
		return s2;
}

VStr PathStringFunctions::GetFileExtension ( VStr filename )
{
	if (filename == "")
		throw VException("PathStringFunctions::GetFileExtension exception , filename is empty", 1);
	//VChr myext[20];
	//myext[0] = 0;
	VStr myext;

	int i=filename.length() -1;
	for (i; i >= 0; i-- )
	{
		VChr temp = filename[i];
		if ( temp == '.' )
			break;
	}

	if ( i < filename.length() )
	{
		unsigned int offset = i+1;
		for ( unsigned int j = 0; j < (filename.length()-offset); j++ )
		{
			//myext[j] = filename[j+offset];
			myext += filename.at(j + offset);
		}
		// VC8 change
		//myext[/*j*/(filename.length()-offset)] = 0;
	}

	//VStr retval ( myext );

	return myext;// retval;
}

VStr PathStringFunctions::GetJustFileName(const VStr & fullfilepath )
{
	if (fullfilepath == "")
		throw VException("PathStringFunctions::GetJustFileName exception , fullfilepath is empty",1);
	VStr::const_iterator myit = fullfilepath.end();
	
	while(--myit != fullfilepath.begin() ) 
	{
		if ( *myit == dirDelRev || *myit == dirDel )
			break;
	};
	
	VStr myresult;

	if ( *myit == dirDel ||  *myit == dirDelRev )
	{
		while (++myit != fullfilepath.end())
		{
			myresult+=(*myit);
		};
	}
	else
	{
		// no seperators
		myresult = fullfilepath;
	}
	
	return myresult;
}
VStr PathStringFunctions::GetLastFolderOfPath(const VStr & fullfilepath)
{
	if (fullfilepath == "")
		throw VException("PathStringFunctions::GetJustFileName exception , fullfilepath is empty", 1);
	VStr::const_iterator myit = fullfilepath.end();

	while (--myit != fullfilepath.begin())
	{
		if (*myit == dirDel || *myit == dirDelRev)
			break;
	};
	while (--myit != fullfilepath.begin())
	{
		if (*myit == dirDel || *myit == dirDelRev)
			break;
	};
	VStr myresult;

	if (*myit == dirDel || *myit == dirDelRev)
	{
		while (*(++myit) != dirDel && *myit!=dirDelRev && myit != fullfilepath.end())
		{
			myresult += (*myit);
		};
	}
	else
	{
		// no seperators
		myresult = fullfilepath;
	}

	return myresult;
}
void PathStringFunctions::GetDirectoryInfo(const VStr &dir, std::vector<VStr> &singleDirNames, std::vector<VStr> &singleFileNames)
{
	WIN32_FIND_DATA ffd;
	//LARGE_INTEGER filesize;
	TCHAR szDir[MAX_PATH];
	size_t length_of_arg;
	HANDLE hFind = INVALID_HANDLE_VALUE;
	DWORD dwError = 0;

	// Check that the input path plus 3 is not longer than MAX_PATH.
	// Three characters are for the "\*" plus NULL appended below.

	StringCchLength(dir.c_str(), MAX_PATH, &length_of_arg);

	if (length_of_arg > (MAX_PATH - 3))
	{
		throw VException( "PathStringFunctions::GetDirectoryInfo,  Directory path is too long.",1);
	}

	//_tprintf(TEXT("\nTarget directory is %s\n\n"), argv[1]);

	// Prepare string for use with FindFile functions.  First, copy the
	// string to a buffer, then append '\*' to the directory name.

	StringCchCopy(szDir, MAX_PATH, dir.c_str());
	StringCchCat(szDir, MAX_PATH, TEXT("\\*"));

	// Find the first file in the directory.

	hFind = FindFirstFile(szDir, &ffd);

	if (INVALID_HANDLE_VALUE == hFind)
	{
		//FindClose(hFind);
		DisplayErrorBox(TEXT("FindFirstFile"));
		throw VException("PathStringFunctions::GetDirectoryInfo exception find first file failed",1);
	}

	// List all the files in the directory with some info about them.

	do
	{
		if (ffd.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY)
		{
			
			//_tprintf(TEXT("  %s   <DIR>\n"), ffd.cFileName);
			VStr tmp((LPTSTR)ffd.cFileName);
			if (tmp.size() > 0 && tmp != "." && tmp != "..")
				singleDirNames.push_back(tmp);
			//VStr tmp2 = GetDirectoryName(tmp);
			//singleDirNames.push_back(tmp2);
		}
		else
		{
			//filesize.LowPart = ffd.nFileSizeLow;
			//filesize.HighPart = ffd.nFileSizeHigh;
			//_tprintf(TEXT("  %s   %ld bytes\n"), ffd.cFileName, filesize.QuadPart);
			VStr tmpFileName((LPTSTR)ffd.cFileName);
			if (tmpFileName.size() > 0)
				singleFileNames.push_back(tmpFileName);
			//singleFileNames.push_back(this->GetJustFileName(tmpFileName));
		}
	} while (FindNextFile(hFind, &ffd) != 0);

	dwError = GetLastError();
	if (dwError != ERROR_NO_MORE_FILES)
	{
		DisplayErrorBox(TEXT("FindFirstFile"));
	}

	FindClose(hFind);
	//return dwError;
}

void PathStringFunctions::DisplayErrorBox(LPTSTR lpszFunction)
{
	// Retrieve the system error message for the last-error code

	LPVOID lpMsgBuf;
	LPVOID lpDisplayBuf;
	DWORD dw = GetLastError();

	FormatMessage(
		FORMAT_MESSAGE_ALLOCATE_BUFFER |
		FORMAT_MESSAGE_FROM_SYSTEM |
		FORMAT_MESSAGE_IGNORE_INSERTS,
		NULL,
		dw,
		MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT),
		(LPTSTR)&lpMsgBuf,
		0, NULL);

	// Display the error message and clean up

	lpDisplayBuf = (LPVOID)LocalAlloc(LMEM_ZEROINIT,
		(lstrlen((LPCTSTR)lpMsgBuf) + lstrlen((LPCTSTR)lpszFunction) + 40)*sizeof(TCHAR));
	StringCchPrintf((LPTSTR)lpDisplayBuf,
		LocalSize(lpDisplayBuf) / sizeof(TCHAR),
		TEXT("%s failed with error %d: %s"),
		lpszFunction, dw, lpMsgBuf);
	//MessageBox(NULL, (LPCTSTR)lpDisplayBuf, TEXT("Error"), MB_OK);
	LogFileWriter::WriteUnknownExceptionLog((LPTSTR)lpDisplayBuf);
	LocalFree(lpMsgBuf);
	LocalFree(lpDisplayBuf);
}

bool PathStringFunctions::DirectoryExists(const char* absolutePath)
{
#ifdef VP_USEWINDOWSFUNCTIONSFORPATHS
	CString tmp(absolutePath);
	return VDir::ExistDirectory(tmp);
#else
	//return true;
	VStr absPath(absolutePath);
	//bool ck = PathExistData::WasExistingAtMost1minAgo(absPath);
	//if (ck)
	//{
	//	Sleep(1000);
	//	return true;
	//}
	size_t mysize = absPath.size();
	if (mysize > 0)
	{
		//if (tmp.at(mysize - 1) == '\\' || tmp.at(mysize - 1) == '/')
		//{
		//	tmp = tmp.substr(0, mysize - 1);
		//}
		if (_access(absolutePath, 0) == 0)
		{
			struct stat status;
			stat(absolutePath , &status);
			bool myresult = (status.st_mode & S_IFDIR) != 0;
			PathExistData::Update(absPath, myresult);
			Sleep(1000);
			return myresult;
		}
	}
	PathExistData::Update(absPath, false);
	//Sleep(1000);
	return false;
#endif
}

bool PathStringFunctions::IsDirectoryExist(const VStr &dir)
{
	return DirectoryExists(dir.c_str());

	WIN32_FIND_DATA ffd;
	//LARGE_INTEGER filesize;
	TCHAR szDir[MAX_PATH];
	size_t length_of_arg;
	HANDLE hFind = INVALID_HANDLE_VALUE;
	DWORD dwError = 0;

	// Check that the input path plus 3 is not longer than MAX_PATH.
	// Three characters are for the "\*" plus NULL appended below.

	StringCchLength(dir.c_str(), MAX_PATH, &length_of_arg);

	if (length_of_arg > (MAX_PATH - 3))
	{
		throw VException("PathStringFunctions::GetDirectoryInfo,  Directory path is too long.", 1);
	}

	//_tprintf(TEXT("\nTarget directory is %s\n\n"), argv[1]);

	// Prepare string for use with FindFile functions.  First, copy the
	// string to a buffer, then append '\*' to the directory name.

	StringCchCopy(szDir, MAX_PATH, dir.c_str());
	StringCchCat(szDir, MAX_PATH, TEXT("\\*"));

	// Find the first file in the directory.

	hFind = FindFirstFile(szDir, &ffd);

	if (INVALID_HANDLE_VALUE == hFind)
	{
		//FindClose(hFind);
		DisplayErrorBox(TEXT("FindFirstFile"));
		throw VException("PathStringFunctions::GetDirectoryInfo exception find first file failed", 1);
	}

	// List all the files in the directory with some info about them.

	do
	{
		if (ffd.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY)
		{

			//_tprintf(TEXT("  %s   <DIR>\n"), ffd.cFileName);
			VStr tmp((LPTSTR)ffd.cFileName);
			if (tmp == dir)
				return true;
			//VStr tmp2 = GetDirectoryName(tmp);
			//singleDirNames.push_back(tmp2);
		}
	} while (FindNextFile(hFind, &ffd) != 0);

	dwError = GetLastError();
	if (dwError != ERROR_NO_MORE_FILES)
	{
		DisplayErrorBox(TEXT("FindFirstFile"));
	}

	FindClose(hFind);
	return false;
}

bool PathStringFunctions::IsFileExist(const VStr&dir , const VStr &fileName)
{
	//VStr fullpath(dir);
	//size_t dirSize = dir.size();
	//if (dirSize > 0)
	//{
	//	if (dir.at(dirSize - 1) != '\\')
	//		fullpath += '\\';
	//}
	//else
	//	throw VException("PathStringFunctions::IsFileExist exception dir is empty",1);
	//fullpath += fileName;
	//return DirectoryExists(fullpath.c_str());

	WIN32_FIND_DATA ffd;
	//LARGE_INTEGER filesize;
	TCHAR szDir[MAX_PATH];
	size_t length_of_arg;
	HANDLE hFind = INVALID_HANDLE_VALUE;
	DWORD dwError = 0;

	// Check that the input path plus 3 is not longer than MAX_PATH.
	// Three characters are for the "\*" plus NULL appended below.

	StringCchLength(dir.c_str(), MAX_PATH, &length_of_arg);

	if (length_of_arg > (MAX_PATH - 3))
	{
		throw VException("PathStringFunctions::GetDirectoryInfo,  Directory path is too long.", 1);
	}

	//_tprintf(TEXT("\nTarget directory is %s\n\n"), argv[1]);

	// Prepare string for use with FindFile functions.  First, copy the
	// string to a buffer, then append '\*' to the directory name.

	StringCchCopy(szDir, MAX_PATH, dir.c_str());
	StringCchCat(szDir, MAX_PATH, TEXT("\\*"));

	// Find the first file in the directory.

	hFind = FindFirstFile(szDir, &ffd);

	if (INVALID_HANDLE_VALUE == hFind)
	{
		DisplayErrorBox(TEXT("FindFirstFile"));
		throw VException("PathStringFunctions::GetDirectoryInfo exception find first file failed", 1);
	}

	// List all the files in the directory with some info about them.

	do
	{
		if (ffd.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY)
		{
			// this is directory
		}
		else
		{
			//filesize.LowPart = ffd.nFileSizeLow;
			//filesize.HighPart = ffd.nFileSizeHigh;
			//_tprintf(TEXT("  %s   %ld bytes\n"), ffd.cFileName, filesize.QuadPart);
			VStr tmpFileName((LPTSTR)ffd.cFileName);
			if (tmpFileName == fileName)
				return true;
		}
	} while (FindNextFile(hFind, &ffd) != 0);

	dwError = GetLastError();
	if (dwError != ERROR_NO_MORE_FILES)
	{
		DisplayErrorBox(TEXT("FindFirstFile"));
	}

	FindClose(hFind);
	return false;
}
bool PathStringFunctions::IsFileExist(const VStr &completeFilePath)
{
	if (_access(completeFilePath.c_str(), 0) == 0)
		return true;   // file exists
	else
		return false;
}
VStr PathStringFunctions::ReplaceUnallowedChars(const VStr &originalName,VChr toreplace)
{
	VStr name;
	int slen = (int)originalName.length();
	for (int s = 0; s < slen; ++s)
	{
		VChr curc = originalName.at(s);
		if (IsUnallowedChar(curc))
		{
			name += toreplace;
		}
		else
		{
			name += curc;
		}
	}
	return name;
}

bool PathStringFunctions::IsUnallowedChar(VChr c)
{
	int mysize = (int)m_unallowedChars.size();
	for (int i = 0; i < mysize; ++i)
	{
		if (m_unallowedChars.at(i) == c)
			return true;
	}
	return false;
}

VStr PathStringFunctions::GetUniqNameForDirectoryIf(const VStr &dirName)// returns dirName back if it does not exist
{
	PathStringFunctions tmp;
	bool ck = tmp.IsDirectoryExist(dirName);
	if (ck == false)
		return dirName;
	else
	{
		int number = 1;
		bool ck2 = IsNumberExistInParanthesisAtEnd(dirName, number);
		while (ck2)
		{
			number++;
			ck2 = IsNumberExistInParanthesisAtEnd(dirName, number);
		}
		VStr newDirName = ReplaceOrAddNumberWithParanthesisAtEnd(dirName, number);
		return newDirName;
	}
}

VStr PathStringFunctions::GetUniqNameForFileIf(const VStr &fileName,const VStr &fileExtension)// returns fileName back if it does not exist
{
	PathStringFunctions tmp;
	VStr fullName = fileName;
	fullName += ".";
	fullName += fileExtension;
	bool ck = tmp.IsFileExist(fullName);
	if (ck == false)
	{
		VStr newFileName(fileName);
		newFileName += ".";
		return (newFileName + fileExtension);
	}
	else
	{
		int number = 1;
		bool ck2 = IsNumberExistInParanthesisAtEnd(fileName, number);
		while (ck2)
		{
			number++;
			ck2 = IsNumberExistInParanthesisAtEnd(fileName, number);
		}
		VStr newFileName = ReplaceOrAddNumberWithParanthesisAtEnd(fileName, number);
		newFileName += ".";
		return (newFileName + fileExtension);
	}
}

VStr PathStringFunctions::ReplaceOrAddNumberWithParanthesisAtEnd(const VStr &name, int number)
{
	if (name.size() >= VGlobalValues::VCHR_LINE_READERSIZE)
		throw VException("PathStringFunctions::ReplaceOrAddNumberWithParanthesisAtEnd exception , name length too long", 1);
	const VChr * mydata = name.data();
	VChr * result = VGlobalValues::VCHR_LINE_READER;
	int len = (int)name.length();
	if (len == 0)
		throw VException("PathStringFunctions::ReplaceOrAddNumberWithParanthesisAtEnd exception name length is zero",1);
	bool noParanthesisAtEnd = mydata[len - 1] == ')' ? false : true;
	if (noParanthesisAtEnd == true)
	{
		CString temp;
		temp.Format("(%d)",number);
		VStr tmp((LPCTSTR)temp);
		return name + tmp;
	}
	int itostop = -1;
	for (int i = len-1; i >= 0; i--)
	{
		if (mydata[i] == '(')
		{
			itostop = i;
			break;
		}
	}
	CString temp;
	temp.Format("(%d)", number);
	VStr tmp((LPCTSTR)temp);
	if (itostop == -1)
	{
		return name + tmp;
	}
	else
	{
		VStr name2;
		for (int j = 0; j < itostop; ++j)
		{
			name2 += mydata[j];
		}
		return name2 + tmp;
	}
}

bool PathStringFunctions::IsNumberExistInParanthesisAtEnd(const VStr &name , int number)
{
	if (name.size() >= VGlobalValues::VCHR_LINE_READERSIZE)
		throw VException("PathStringFunctions::ReplaceOrAddNumberWithParanthesisAtEnd exception , name length too long", 1);
	const VChr * mydata = name.data();
	VChr * result = VGlobalValues::VCHR_LINE_READER;
	int len = (int)name.length();
	if (len == 0)
		throw VException("PathStringFunctions::ReplaceOrAddNumberWithParanthesisAtEnd exception name length is zero", 1);
	bool noParanthesisAtEnd = mydata[len - 1] == ')' ? false : true;
	if (noParanthesisAtEnd == true)
	{
		return false;
	}
	int itostop = -1;
	for (int i = len - 1; i >= 0; i--)
	{
		if (mydata[i] == '(')
		{
			itostop = i;
			break;
		}
	}
	if (itostop == -1)
	{
		return false;
	}
	VStr numstr;
	for (int j = itostop + 1; j < len - 2; ++j)
	{
		numstr += mydata[j];
	}
	if (number == TypeConverters::ConvertStringToInt(numstr))
		return true;
	else
		return false;
}

bool PathStringFunctions::PathExistData::WasExistingAtMost1minAgo(const VStr &absolutePath)
{
	std::map<VStr, PathExistData>::iterator fi = m_pathExistData.find(absolutePath);
	if (fi == m_pathExistData.end())
		return false;
	PathExistData &ped = fi->second;
	if (ped.m_isExisting == false)
		return false;
	VTime mytimer;
	int year = mytimer.GetYear();
	int month = mytimer.GetMonth();
	int day = mytimer.GetDay();
	int hour = mytimer.GetHour();
	int min = mytimer.GetMinute();
	if (year != ped.m_year)
		return false;
	if (month != ped.m_month)
		return false;
	if (day != ped.m_day)
		return false;
	if (hour != ped.m_hour)
		return false;
	if (min != ped.m_min)
		return false;
	return true;
}

void PathStringFunctions::PathExistData::Construct(VStr absolutePath , int year, int month, int day, int hour, int min, bool isexisting)
{
	m_absolutePath = absolutePath;
	m_year = year;
	m_month = month;
	m_day = day;
	m_hour = hour;
	m_min = min;
	m_isExisting = isexisting;
}

void PathStringFunctions::PathExistData::Update(const VStr &absolutePath, bool isExist)
{
	VTime mytimer;
	int year = mytimer.GetYear();
	int month = mytimer.GetMonth();
	int day = mytimer.GetDay();
	int hour = mytimer.GetHour();
	int min = mytimer.GetMinute();
	PathExistData uped;
	uped.Construct(absolutePath, year, month, day, hour, min, isExist);
	m_pathExistData[absolutePath] = uped;
}

VStr PathStringFunctions::TrimUnallowedChar(const VStr toTrim, VChr unallowedChar)
{
	VStr trimmed;
	size_t len = toTrim.length();
	for (size_t i = 0; i < len;++i)
	{
		char curchr = toTrim[i];
		if (curchr != unallowedChar)
			trimmed += curchr;
	}
	return trimmed;
}
