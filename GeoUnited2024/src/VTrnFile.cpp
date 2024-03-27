#include "stdafx.h"
#include "VTrnFile.h"


VTrnFile::VTrnFile(void)
{
}


VTrnFile::~VTrnFile(void)
{
}
void VTrnFile::GetAllLines(std::ifstream &ifs ,std::vector<std::string> &lines)
{
	char* arr = new char[2048];
	std::string str ="";
	while(!ifs.eof())
	{
		ifs.getline(arr,2048);
		str = arr;
		if(str != "")
		lines.push_back(str);
	}

	delete [] arr;
}
void VTrnFile::WriteAllLines(std::ofstream &ofs,std::vector<std::string> lines)
{
	try
	{  
		int size = (int)lines.size();
		std::string tmpLine ="";
		for(int i=0;i<size;i++)
		{
			if(lines.at(i)!="")
			{
			  tmpLine = lines.at(i) +"\n"; 
			  ofs.write(tmpLine.c_str(),tmpLine.length());
			}
		}

	}
	catch(std::exception &e)
	{
		LogFileWriter::WriteMessage(e.what());
	}
	catch(...)
	{
		LogFileWriter::WriteUnknownExceptionLog ("Unknown Exception , VTrnFile::WriteItemsTextToFile");
	}
}
std::vector<std::string> VTrnFile::Split(const std::string &sourceStr , char seperator )
{
 
	std::string tmpStr = "";   
	int count   = sourceStr.length();
	   
	std::vector<std::string> retVec;

	for(int i=0;i<count;i++) 
	{
		if(sourceStr[i] == seperator)
		{
			retVec.push_back(tmpStr); 
			tmpStr = "";
		}
		else 
		{
			tmpStr +=sourceStr[i]; 
			if(i == count-1 )
			{
				retVec.push_back(tmpStr); 
				tmpStr = ""; 
			} 
		}  
	}
	  
	return retVec;
 
} 