#include "stdafx.h"
#include "ModifyFile.h"
#include <string>

ModifyFile::ModifyFile(const std::string& modifyFilePath,
	                   const std::string& findText
	                  ,const std::string& replaceText
	                  ,const std::string& outputFilePath
						)
	:mModifyFilePath(modifyFilePath),mFindText(findText)
	,mReplaceText(replaceText),mOutpuFilePath(outputFilePath)
	,mReplaceCount(0),mErrorStatus(0)
{
	size_t a = 0, offset = 0;
	std::ifstream fileread;
	std::ofstream filewrite;
	std::string satir;
	std::string OrginalText;
	std::string ModifyText;
	
	fileread.open(mModifyFilePath,std::ios_base::binary);
		
	if (fileread.is_open())
	{
		//while(getline(fileread,satir))
		do
		{
			fileread.read(VGlobalValues::VCHR_LINE_READER, VGlobalValues::VCHR_LINE_READERSIZE);
			satir = VStr(VGlobalValues::VCHR_LINE_READER);
			OrginalText += satir;
			//OrginalText += "\n";
		} while (fileread.eof() == false);
		//cout << OrginalText << endl;
		fileread.close();
	}
	else
	{
		LogFileWriter::WriteUnknownExceptionLog("ModifyFile constructor", " modified file could not be opened");
		mErrorStatus = 2;
	}
	a = OrginalText.find(mFindText, offset);

	if (a != -1)
	{
		//cout << endl << "Replace Text= ";
		//cin >> mReplaceText;
		while (a != -1)
		{
			for (size_t i = offset; i < a; ++i)
			{
				ModifyText += OrginalText[i];
			}
			ModifyText += mReplaceText;
			++mReplaceCount;
			offset = a + mFindText.length();
			a = OrginalText.find(mFindText,offset);
		}
		for (size_t i = offset; i < OrginalText.length(); ++i)
		{
			ModifyText += OrginalText[i];
		}
		//cout << "\nOutPut File Paht=";
		//cin >> mOutpuFilePaht;
		filewrite.open(mOutpuFilePath,std::ios_base::binary);
		if (filewrite.is_open() == true)
		{
			//cout << ModifyText << endl;
			//filewrite << ModifyText << endl;
			for (int i = 0; i < ModifyText.length(); ++i)
			{
				filewrite.put(ModifyText[i]);
			}
			filewrite.close();
		}
	}
	else
	{
		//cout << "! ! Find Text Not Found ! !";
		// original file can not be opened
		LogFileWriter::WriteUnknownExceptionLog("ModifyFile constructor "," original file could not be opened");
		mErrorStatus = 1;
	}
}

int ModifyFile::getErrorStatus()const
{
	//Error status is zero  if file is changed and output is successfully written
	return mErrorStatus;
}
int ModifyFile::getReplaceCount()const
{
	return mReplaceCount;
}