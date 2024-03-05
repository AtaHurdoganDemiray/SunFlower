// DTable.cpp

#include "stdafx.h"
#include "DTable.h"
#include "DTableKeys.h"
#include "DTableRow.h"
#include "DTableContainer.h"
#include "TypeConverters.h"

DTable::DTable (DTableContainer *DB)
: m_DB(DB),m_tableKeys(DB)
{
	m_scriptPath = "none";
}

const VStr & DTable::GetTypeName ()const
{
	return m_typeName;
}

const DTableKeys & DTable::GetTableKeys()const
{
	return m_tableKeys;
}

const std::vector<DTableRow> & DTable::GetRowVec ()const
{
	return m_rowVec;
}

bool DTable::Write (const VStr &baseFolderName)const
{
	try
	{
		m_tableKeys.Write(baseFolderName, m_originalTypeName);//m_typeName);
		VStr fname (baseFolderName);
		fname += m_originalTypeName;//m_typeName;
		fname += ( IDS_TableDataTxt);
		copyVofStream f (fname);
		if (f.is_open() == false)
		{
			LogFileWriter::WriteUnknownExceptionLog (( IDS_FileOpenFailed),( IDS_DTableWrite));
			return false;
		}
		DTableContainer::Write(f, (IDS_TypeName), m_originalTypeName);//m_typeName);
		int rowSize = (int)m_rowVec.size();
		VOStrStream rowSizeStr;
		rowSizeStr << rowSize;
		DTableContainer::Write (f , ( IDS_RowSize) , rowSizeStr.str());
		for (int w=0;w<rowSize;++w)
		{
			const DTableRow &curTRow = m_rowVec.at(w);
			curTRow.Write (f , w);
		}
		DTableContainer::Write(f,"ScriptPath",m_scriptPath);
		f.close();
		return true;
	}
	catch (VException &e)
	{
		LogFileWriter::WriteExceptionLog (e,( IDS_DTableWrite1));
		return false;
	}
	catch (std::exception &e)
	{
		LogFileWriter::WriteExceptionLog (e,( IDS_DTableWrite1));
		return false;
	}
	catch (...)
	{
		LogFileWriter::WriteUnknownExceptionLog ( ( IDS_UnkExc),( IDS_DTableWrite1));
		return false;
	}
}

bool DTable::Read (const VStr &baseFolderName)
{
	try
	{
		m_tableKeys.Read(baseFolderName, m_originalTypeName);//m_typeName);
		VStr fname (baseFolderName);
		fname += m_originalTypeName;//m_typeName;
		fname += ( IDS_TableDataTxt);
		copyVIFStream f (fname);
		if (f.is_open() == false)
		{
			LogFileWriter::WriteUnknownExceptionLog (( IDS_UnaToOpeFil),( IDS_DTableRead));
			return false;
		}
		std::map<VStr,VStr> keyValueMap;
		DTableContainer::Read (f , keyValueMap);
		std::map<VStr,VStr>::iterator fi00 = keyValueMap.find (( IDS_TypeName));
		if (fi00 == keyValueMap.end())
		{
			LogFileWriter::WriteUnknownExceptionLog (( IDS_CanNotFinTypKey),( IDS_DTableRead));
			return false;
		}
		m_originalTypeName = fi00->second;
		m_typeName = sozluk(m_originalTypeName);//fi00->second;
		std::map<VStr,VStr>::iterator fi0 = keyValueMap.find (( IDS_RowSize));
		if (fi0 == keyValueMap.end())
		{
			LogFileWriter::WriteUnknownExceptionLog (( IDS_CanNotFinRowKey),( IDS_DTableRead));
			return false;
		}
		VStrStream mystr;
		mystr << fi0->second;
		int rowSize;
		mystr >> rowSize;
		for (int w=0;w<rowSize;++w)
		{
			DTableRow temp;
			bool ck5 = temp.Read (keyValueMap , w);
			if (ck5)
				m_rowVec.push_back (temp);
			else
				throw VException (( IDS_DTaReaExcRowRea),1);
		}
		std::map<VStr,VStr>::iterator fi5 = keyValueMap.find ("ScriptPath");
		if (fi5 != keyValueMap.end())
		{
			m_scriptPath = fi5->second;
		}
		return true;
	}
	catch (VException &e)
	{
		LogFileWriter::WriteExceptionLog (e,( IDS_DTableRead));
		return false;
	}
	catch (std::exception &e)
	{
		LogFileWriter::WriteExceptionLog (e,( IDS_DTableRead));	
		return false;
	}
	catch (...)
	{
		LogFileWriter::WriteUnknownExceptionLog (( IDS_UnkExc),( IDS_DTableRead));
		return false;
	}
}

void DTable::SetTypeName (const VStr &typeName)
{
	m_originalTypeName = typeName;
	m_typeName = sozluk(typeName);
}

std::vector<DTableRow> & DTable::GetRowVec()
{
	return m_rowVec;
}
DTableRow * DTable::SearchForRow(const VStr &key, const VStr &val)
{
	const std::vector<VStr> &tableKeys = GetTableKeys().GetNames();
	int rowCount = (int)m_rowVec.size();
	for (int ri = 0; ri < rowCount; ++ri)
	{
		DTableRow * curRow = &m_rowVec.at(ri);
		VStr myval;
		bool ck = curRow->SearchForKey(key, myval, tableKeys);
		if (ck)
		{
			if (myval == val)
				return curRow;
		}
		else
			break;
	}
	return 0;
}

bool DTable::AddNewRow (const std::vector<VStr> &rowStr)
{
	try
	{
		if (rowStr.size() != m_tableKeys.GetNames().size())
		{
			LogFileWriter::WriteUnknownExceptionLog (( IDS_KeySizNotEquToR),( IDS_DTableAddNewRow));
		}
		m_rowVec.push_back (rowStr);
		//VStr basefolder = VGlobalFunctions::GetSolidCncExeFolder();
		//basefolder += ("Tables\\");
		//Write ( basefolder );
		return true;
	}
	catch (VException &e)
	{
		LogFileWriter::WriteExceptionLog (e,( IDS_DTableAddNewRow));
		return false;
	}
	catch (std::exception &e)
	{
		LogFileWriter::WriteExceptionLog (e,( IDS_DTableAddNewRow));
		return false;
	}
	catch (...)
	{
		LogFileWriter::WriteUnknownExceptionLog (( IDS_UnkExc1),( IDS_DTableAddNewRow));
		return false;
	}
}

std::vector<VStr> DTable::GetDefaultRow ()
{
	return m_tableKeys.GetDefaultValuesForEntireKeys();
}

DTableKeyAttributes * DTable::GetAtt(int colNo)
{
	if (colNo > (int)m_tableKeys.GetAttributesNames().size())
		return 0;
	std::vector<DTableKeyAttributes *> attvec = m_DB->GetAttributes(m_tableKeys.GetAttributesNames().at(colNo));
	if (attvec.size())
		return attvec.at(0);
	else
		return 0;
}

VStr DTable::GetOneRowMergedStrAsKeyVal (int rowi)const
{
	const DTableRow &curRow = m_rowVec.at(rowi);
	const std::vector<VStr> &rowStr = curRow.GetData();
	const std::vector<VStr> &keyNames = m_tableKeys.GetNames();
	VStr totalStr;
	size_t mysize = rowStr.size();
	for (size_t i=0;i<mysize;++i)
	{
		const VStr &curVal = rowStr.at(i);
		const VStr &curKey = keyNames.at(i);
		totalStr += curKey;
		totalStr += ':';
		totalStr += curVal;
		totalStr += ';';
	}
	return totalStr;
}
VStr DTable::GetOneRowMergedStrAsKeyVal2 (int rowi)const
{

	const DTableRow &curRow = m_rowVec.at(rowi);
	const std::vector<VStr> &rowStr = curRow.GetData();
	const std::vector<VStr> &keyNames = m_tableKeys.GetNames();
	if (keyNames.size() != 2)
		return GetOneRowMergedStrAsKeyVal (rowi);
	VStr totalStr;
	const VStr &curVal = rowStr.at(1);
	const VStr &curKey = rowStr.at(0);
	totalStr += curKey;
	totalStr += ':';
	totalStr += curVal;
	totalStr += ';';
	return totalStr;
}	
void DTable::MergedRowStrToMap (const VStr &mergedRowStr , std::map<VStr,VStr> &rowMap,char propertySplitter)
{
	std::vector<VStr> s1;
	TypeConverters::SplitString (mergedRowStr,propertySplitter,s1);
	size_t mysize = s1.size();
	for (size_t i=0;i<mysize;++i)
	{
		const VStr &onePair = s1.at(i);
		std::vector<VStr> s2;
		TypeConverters::SplitString (onePair,':',s2);
		if (s2.size() == 2)
		{
			rowMap[s2.at(0)] = s2.at(1);
		}
	}
}
void DTable::MapToMergedRowStr (const std::map<VStr,VStr> &keyVal , VStr &mergedStr,char propertySplitter)
{
	std::map<VStr,VStr>::const_iterator i = keyVal.begin();
	std::map<VStr,VStr>::const_iterator e = keyVal.end();
	VOStrStream mystr;
	for (i;i!=e;++i)
	{
		mystr << i->first << ':' << i->second << propertySplitter;
	}
	mergedStr = mystr.str();
}
VStr DTable::GetSomeOfRowMergedStrAsKeyVal (int rowi , const VStr &keyValuesToGet)const
{
	std::vector<VStr> filterVec;
	std::map<VStr,VStr> filterMap;
	TypeConverters::SplitString (keyValuesToGet,';',filterVec);
	std::vector<VStr>::const_iterator iii=filterVec.begin();
	std::vector<VStr>::const_iterator eee=filterVec.end();
	for (iii;iii!=eee;++iii)
	{
		filterMap[*iii]=*iii;		
	}

	const DTableRow &curRow = m_rowVec.at(rowi);
	const std::vector<VStr> &rowStr = curRow.GetData();
	const std::vector<VStr> &keyNames = m_tableKeys.GetNames();
	VStr totalStr;
	size_t mysize = rowStr.size();
	for (size_t i=0;i<mysize;++i)
	{
		const VStr &curKey = keyNames.at(i);
		if (filterMap.find(curKey) != filterMap.end())
		{
			const VStr &curVal = rowStr.at(i);
			totalStr += curKey;
			totalStr += ":";
			totalStr += curVal;
			totalStr += ";";
		}
	}
	return totalStr;
}

bool DTable::SearchForKey (int rowi , const VStr &key , VStr &val)const
{
	const DTableRow &curRow = m_rowVec.at(rowi);
	return curRow.SearchForKey (key,val,m_tableKeys.GetNames());
}

std::vector<int> DTable::SearchForSameKeyValSet (const VStr &mergedKeyValSet)const
{
	std::vector<int> result;
	// Extract the key values first
	VStr keyNamesMerged;
	std::vector<VStr> res1;
	TypeConverters::SplitString (mergedKeyValSet,';',res1);
	size_t mysize1 = res1.size();
	for (size_t i=0;i<mysize1;++i)
	{
		const VStr curPair = res1.at(i);
		std::vector<VStr> res2;
		TypeConverters::SplitString (curPair,':',res2);
		keyNamesMerged += res2.at(0);
		keyNamesMerged += ';';
	}
	// for all rows 
	int mysize2 = (int)m_rowVec.size();
	for (int j=0;j<mysize2;++j)
	{
		VStr someRow = this->GetSomeOfRowMergedStrAsKeyVal (j,keyNamesMerged);
		if (someRow == mergedKeyValSet)
			result.push_back(j);
	}
	return result;
}

const VStr & DTable::GetScriptPath()
{
	return m_scriptPath;
}

void DTable::SetScriptPath(const VStr &name)
{
	m_scriptPath = name;
}

void DTable::DeleteRows (std::map<int,DTableRow> &toDeleteMap)
{
	if (toDeleteMap.size()==0)
		return;
	// copy to temp
	std::vector<DTableRow> temp;
	size_t rowsize = m_rowVec.size();
	for (size_t i=0;i<rowsize;++i)
	{
		DTableRow &currow = m_rowVec.at(i);
		std::map<int,DTableRow>::const_iterator fi = toDeleteMap.find((int)i);
		if (fi == toDeleteMap.end())
		{
			temp.push_back (currow);
		}
		else
		{
			m_deletedRows.push_back (currow);
		}
	}
	m_rowVec = temp;
}

void DTable::RescueDeletedRows()
{
	size_t deletedSize = m_deletedRows.size();
	for (size_t i=0;i<deletedSize;++i)
	{
		m_rowVec.push_back (m_deletedRows.at(i));
	}
}

DTableKeys & DTable::GetTableKeys()
{
	return m_tableKeys;
}
const VStr& DTable::GetOriginalTypeName()const
{
	return m_originalTypeName;
}
void DTable::SetOriginalTypeName(const VStr& originalTypeName)
{
	m_originalTypeName = originalTypeName;
}