#ifndef MODIFYFILE_H
#define MODIFYFILE_H

#include <iostream>
#include <fstream>
#include <string.h>
#include <locale.h>
#include <clocale>

using namespace std;

class ModifyFile
{

public:

	ModifyFile(const std::string& modifyFilePath
		, const std::string& findText
		, const std::string& replaceText
		, const std::string& outputFilePath
	);
	int getErrorStatus()const;//Error status is zero  if file is changed and output is successfully written 
	int getReplaceCount()const;
private:
	const std::string mModifyFilePath;
	const std::string mFindText;
	const std::string mReplaceText;
	const std::string mOutpuFilePath;
	int mErrorStatus;
	int mReplaceCount;
};
#endif
