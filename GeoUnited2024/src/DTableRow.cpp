// DTableRow.cpp

#include "stdafx.h"
#include "DTableRow.h"
#include "DTableContainer.h"
#include "TypeConverters.h"

DTableRow::DTableRow ()
{

}

DTableRow::DTableRow (const std::vector<VStr> &rowVec)
	:m_data(rowVec)
{

}

const std::vector<VStr> & DTableRow::GetData ()const
{
	return m_data;
}

bool DTableRow::Read (std::map<VStr,VStr> &keyValueMap , int rowNo)
{
	VOStrStream keyStr;
	keyStr << ( IDS_Row) << (rowNo+1);
	std::map<VStr,VStr>::iterator fi0 = keyValueMap.find (keyStr.str());
	if (fi0 == keyValueMap.end())
	{
		LogFileWriter::WriteUnknownExceptionLog (( IDS_DataKeyNotFound),( IDS_DTableRowRead));
		return false;
	}
	TypeConverters::SplitString (fi0->second , VP_ROW_SEPARATOR , m_data);
	return true;
}

void DTableRow::Write (VofStream &f , int rowNo)const
{
	VOStrStream str;
	str << ( IDS_Row) << (rowNo+1);
	VStr mergedData;
	TypeConverters::MergeString (m_data , VP_ROW_SEPARATOR , mergedData);
	DTableContainer::Write (f,str.str(),mergedData);
}

std::vector<VStr> & DTableRow::GetData()
{
	return m_data;
}

bool DTableRow::SearchForKey (const VStr &key , VStr &val,const std::vector<VStr> &keyVec)const
{
	size_t mysize = keyVec.size();
	for (size_t i=0;i<mysize;++i)
	{
		const VStr &curKey = keyVec.at(i);
		if (key == curKey)
		{
			val = m_data.at(i);
			return true;
		}
	}
	return false;
}
