// DTableKeys.cpp

#include "stdafx.h"
#include "DTableKeys.h"
#include "DTableContainer.h"
#include "TypeConverters.h"

DTableKeys::DTableKeys (DTableContainer *DB)
:m_DB (DB)
{

}

const std::vector<VStr> & DTableKeys::GetNames()const
{
	return m_names;
}

const std::vector<VStr> & DTableKeys::GetAttributesNames ()const
{
	return m_attributesNames;
}

bool DTableKeys::Write (const VStr &baseFolderName , const VStr &tableName)const
{
	try
	{
		VStr fpath (baseFolderName);
		fpath += tableName;
		fpath += ( IDS_TableKeysTxt);
		copyVofStream f (fpath.c_str());
		if (f.is_open() == false)
		{
			LogFileWriter::WriteUnknownExceptionLog (( IDS_UnaToOpeFil),( IDS_DTableKeysWrite));
			return false;
		}
		VStr mergedNames;
		std::vector<VStr> tempNames = m_names;
		TypeConverters::AddCharsAtStartAndEndForEveryStr ( QUOT_CHR , tempNames);
		TypeConverters::MergeString (tempNames , VP_ROW_SEPARATOR , mergedNames);
		DTableContainer::Write (f , ( IDS_TableKeyNames) , mergedNames);
		VStr mergedAttributeNames;
		TypeConverters::MergeString (m_attributesNames , VP_ROW_SEPARATOR , mergedAttributeNames);
		DTableContainer::Write (f, ( IDS_TableKeyAttribu), mergedAttributeNames);
		size_t sizeWidths = m_keyWidths.size();
		std::vector<VStr> keyWidthsStrVec;
		for (size_t i=0;i<sizeWidths;++i)
		{
			VOStrStream curWidthValueStr;
			curWidthValueStr << m_keyWidths.at(i);
			keyWidthsStrVec.push_back (curWidthValueStr.str());
		}
		VStr mergedWidths;
		TypeConverters::MergeString (keyWidthsStrVec,VP_ROW_SEPARATOR,mergedWidths);
		DTableContainer::Write (f,"TableKeyWidths",mergedWidths);
		f.close();
		return true;
	}
	catch (VException &e)
	{
		LogFileWriter::WriteExceptionLog (e,( IDS_DTableKeysWrite));
		return false;
	}
	catch (std::exception &e)
	{
		LogFileWriter::WriteExceptionLog (e,( IDS_DTableKeysWrite));
		return false;
	}
	catch (...)
	{
		LogFileWriter::WriteUnknownExceptionLog (( IDS_UnkExc1),( IDS_DTableKeysWrite));
		return false;
	}
}

bool DTableKeys::Read (const VStr &baseFolderName , const VStr &tableName)
{

	VStr fname (baseFolderName);
	fname += tableName;
	fname += ( IDS_TableKeysTxt);
	copyVIFStream f (fname);
	if (f.is_open() == false)
	{
		LogFileWriter::WriteUnknownExceptionLog (( IDS_UnaToOpeFil),( IDS_DTableKeysRead));
		return false;
	}
	std::map<VStr,VStr> keyValueMap;
	bool ck = DTableContainer::Read (f , keyValueMap);
	if (!ck)
		LogFileWriter::WriteUnknownExceptionLog (( IDS_ErrWhiReaDTaFil),( IDS_DTableKeysRead));
	std::map<VStr,VStr>::iterator fi0 = keyValueMap.find (( IDS_TableKeyNames) );
	if (fi0 != keyValueMap.end())
	{
		TypeConverters::SplitString (fi0->second , VP_ROW_SEPARATOR , m_names);
		TypeConverters::RemoveCharsAtStartAndEndForEveryStr (QUOT_CHR , m_names);
	}
	else
		LogFileWriter::WriteUnknownExceptionLog (( IDS_KeyNamNotExi),( IDS_DTableKeysRead));
	std::map<VStr,VStr>::iterator fi1 = keyValueMap.find (( IDS_TableKeyAttribu));
	if (fi1 != keyValueMap.end())
	{
		TypeConverters::SplitString (fi1->second , VP_ROW_SEPARATOR , m_attributesNames);	
		//TypeConverters::RemoveCharsAtStartAndEndForEveryStr (QUOT_CHR , m_attributesNames);
	}
	else
		LogFileWriter::WriteUnknownExceptionLog (( IDS_NoAttNam),( IDS_DTableKeysRead));
	std::map<VStr,VStr>::iterator fi2 = keyValueMap.find ("TableKeyWidths");
	if (fi2 != keyValueMap.end())
	{
		std::vector<VStr> keyWidths;
		TypeConverters::SplitString (fi2->second,VP_ROW_SEPARATOR , keyWidths);
		size_t mysize5 = keyWidths.size();
		for (size_t i=0;i<mysize5;++i)
		{
			const VStr &curStr = keyWidths.at(i);
			m_keyWidths.push_back ((int)TypeConverters::ConvertStringTovreal(curStr));
		}
	}
	f.close();
	return ck;
}

VStr DTableKeys::GetDefaultValueForKey (int keyNo)
{
	const VStr & attrName = m_attributesNames.at(keyNo);
	std::vector<DTableKeyAttributes *> attrVec = m_DB->GetAttributes ( attrName );
	std::vector<DTableKeyAttributes *>::reverse_iterator i = attrVec.rbegin();
	std::vector<DTableKeyAttributes *>::reverse_iterator e = attrVec.rend();
	for (i;i!=e;++i)
	{
		VStr curDV = (*i)->GetDefaultValue();
		if (curDV != DTableKeyAttributes::KEY_NOT_FOUND_STR)
			return curDV;
	}
	
	return DTableKeyAttributes::KEY_NOT_FOUND_STR; 
}

VStr DTableKeys::GetDefaultValueForKey (const VStr &key)
{
	size_t mysize = m_names.size();
	for (size_t i=0;i<mysize;++i)
	{
		const VStr &curKey = m_names.at(i);
		if (curKey == key)
			return GetDefaultValueForKey (i);
	}
	return DTableKeyAttributes::KEY_NOT_FOUND_STR;
}

std::vector<VStr> DTableKeys::GetDefaultValuesForEntireKeys ()
{
	std::vector<VStr> res;
	int keySize = (int)m_names.size();
	for (int i=0;i<keySize;++i)
	{
		res.push_back (GetDefaultValueForKey (i));	
	}
	return res;
}

void DTableKeys::FillSearchItemsVec (const VStr &cellValue , int keyNo , std::vector<VStr> &searchItems)const
{
	std::vector<VStr> sv1;
	TypeConverters::SplitString (cellValue , SEMICOLON_CHR, sv1);
	if (sv1.size() > 1)
	{
		searchItems = sv1;
		return;
	}
	std::vector<DTableKeyAttributes *> relAtt = m_DB->GetAttributes ( m_attributesNames.at(keyNo) );
	size_t mysize = relAtt.size();
	for (size_t i=0;i<mysize;++i)
	{
		relAtt.at(i)->AddSearchItemsIf (sv1);
	}
	searchItems = sv1;
	return;
}

std::vector<int> & DTableKeys::GetKeyWidths()
{
	return m_keyWidths;
}

const std::vector<int> & DTableKeys::GetKeyWidths()const
{
	return m_keyWidths;
}