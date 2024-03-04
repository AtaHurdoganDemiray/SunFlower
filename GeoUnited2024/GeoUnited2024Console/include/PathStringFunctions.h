// PathStringFunctions.h

#ifndef __PathStringFunctions_h__
#define __PathStringFunctions_h__

class PathStringFunctions
{
public:
	class PathExistData
	{
	public:
		VStr m_absolutePath;
		int m_year;
		int m_month;
		int m_day;
		int m_hour;
		int m_min;
		bool m_isExisting;
		static bool WasExistingAtMost1minAgo(const VStr &absolutePath);
		void Construct(VStr absolutePath , int year, int month, int day, int hour, int min, bool isexisting);
		static void Update(const VStr &absolutePath, bool isExist);
	};
	const static VChr dirDel;
	const static VChr dirDelRev;

	VStr TrimExtension ( const VStr & filename );
	VStr GetDirectoryName ( VStr filepath );
	VStr GetFileExtension ( VStr filename );
	VStr GetJustFileName( const VStr & fullfilepath );
	
	void GetDirectoryInfo(const VStr &dir, std::vector<VStr> &singleDirNames , std::vector<VStr> &singleFileNames);
	//void GetDirectoryInfoWithDots(const VStr &dir, std::vector<VStr> &singleDirNames, std::vector<VStr> &singleFileNames);
	void DisplayErrorBox(LPTSTR lpszFunction);
	bool IsDirectoryExist(const VStr &dir);
	bool IsFileExist(const VStr &dir , const VStr &fileName);
	bool IsFileExist(const VStr &completeFileName);
	static VStr ReplaceUnallowedChars(const VStr &originalName,VChr toreplace);
	static std::vector<VChr> m_unallowedChars;
	static bool IsUnallowedChar(VChr c);
	static VStr GetUniqNameForDirectoryIf(const VStr &dirName);// returns dirName back if it does not exist
	static VStr GetUniqNameForFileIf(const VStr &fileName,const VStr &fileExtension);// returns fileName back if it does not exist
	static VStr ReplaceOrAddNumberWithParanthesisAtEnd(const VStr &name , int number);
	static bool IsNumberExistInParanthesisAtEnd(const VStr &name, int number);
	static bool DirectoryExists(const char* absolutePath);
	static std::map<VStr, PathExistData> m_pathExistData;
	static VStr GetLastFolderOfPath(const VStr & fullfilepath);
	static VStr TrimUnallowedChar(const VStr toTrim, VChr unallowedChar);
};

#endif