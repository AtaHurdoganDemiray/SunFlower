#include "stdafx.h"
#include "VTranslatedData.h"
#include "VTrnFile.h"

char VTranslatedData::VSEPERATOR ='@';

VTranslatedData::VTranslatedData(const VStr & srcText ,const VStr &trnText, const VStr &parentName /*,int id*/)
{
	m_srcText    = srcText;
	m_trnText    = trnText;
	m_parentName = parentName;
	//m_id = id;
}


VTranslatedData::~VTranslatedData(void)
{
}
bool VTranslatedData::GetTranslatedData(std::ifstream &ifs ,std::vector<VTranslatedData*> &data)
{
	char sep = VTranslatedData::VSEPERATOR;
	 
	if(!ifs.is_open())
		return false;

	std::vector<VStr> lines;
	VTrnFile::GetAllLines(ifs,lines);
	  
	int size = (int)lines.size();
	for(int i=0;i<size;i++)
	{
		std::vector<VStr> line = VTrnFile::Split(lines.at(i),sep);

		if(line.size() >2)
		{ 
			VTranslatedData *td = new VTranslatedData(line.at(0),line.at(1),line.at(2));
			data.push_back(td);
		}
		 
	}

	return true;
}
bool VTranslatedData::GetTranslatedData(const VStr &parentName ,std::ifstream &ifs ,std::map<VStr,VStr> &data)
{
	char sep = VTranslatedData::VSEPERATOR;
	 
	if(!ifs.is_open())
		return false;

	std::vector<VStr> lines;
	VTrnFile::GetAllLines(ifs,lines);
	  
	int size = (int)lines.size();
	for(int i=0;i<size;i++)
	{
		std::vector<VStr> line = VTrnFile::Split(lines.at(i),sep);

		if(line.size() >2 && line[2] == parentName)
		{ 
			 data[line[0]] = line[1];
		}
		 
	}

	return true;
}
VStr VTranslatedData::GetLine(VTranslatedData  *dt)
{
	if(dt == NULL)
		return "";

	VStr sep ="";
	sep += VTranslatedData::VSEPERATOR;
  
	return dt->m_srcText +sep+ 
		   dt->m_trnText +sep+
		   dt->m_parentName ;
	 
}