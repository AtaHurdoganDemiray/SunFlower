// DTableContainer.cpp

#include "stdafx.h"
#include "DTableContainer.h"
#include "TypeConverters.h"

VChr * DTableContainer::m_readBuffer = 0;

DTableContainer::DTableContainer ()
{
	m_readBuffer = new VChr [MAX_CHR_IN_ONE_LINE];
	ReadTableNamesToRead ();
	ReadTableKeyAttributesNamesToRead();
	ReadAllTableKeyAttributes();
	ReadAllTables();
}

std::map<VStr,DTable> & DTableContainer::GetAllTables ()
{
	return m_allTables;
}

std::map<VStr,DTableKeyAttributes> & DTableContainer::GetAllTableKeyAttributes()
{
	return m_allTableKeyAttributes;
}

void DTableContainer::Write ( VofStream &f , const VStr &keyName , const VStr &value )
{
	f << keyName << std::endl;
	f << value << std::endl;
}

void DTableContainer::Write ( VofStream &f , const VStr &keyName , bool value)
{
	f << keyName << std::endl;
	f << value << std::endl;
}

bool DTableContainer::Read (VIFStream &f , std::map<VStr,VStr> &keyValueMap)
{
	int counter = 0;
	VStr key,value;
	try
	{
		while (f.eof() == false)
		{
			f.getline (m_readBuffer,MAX_CHR_IN_ONE_LINE,'\n');
			VStr tmp (m_readBuffer);
			if (tmp.length() == 0)
				break;
			if (counter % 2 == 0)
				key = tmp;
			else
			{
				value = tmp;
				keyValueMap.insert (std::pair<VStr,VStr> (key,value));
			}
			++counter;
		}
		if (counter % 2 != 0)
			throw VException (( IDS_DTaReaExcOddNum),1);
		return true;
	}
	catch (VException &e)
	{
		LogFileWriter::WriteExceptionLog (e,( IDS_DTaRea));
		return false;
	}
	catch (std::exception &e)
	{
		LogFileWriter::WriteExceptionLog (e,( IDS_DTaRea));
		return false;
	}
	catch (...)
	{
		LogFileWriter::WriteUnknownExceptionLog (( IDS_UnkExc),( IDS_DTaRea));
		return false;
	}
}

bool DTableContainer::Write (VofStream &f , const std::map<VStr,VStr> &keyValueMap)
{
	try
	{
		std::map<VStr,VStr>::const_iterator i=keyValueMap.begin();
		std::map<VStr,VStr>::const_iterator e=keyValueMap.end();
		for (i;i!=e;++i)
		{
			Write (f,i->first,i->second);
		}
		return true;
	}
	catch (VException &e)
	{
		LogFileWriter::WriteExceptionLog (e,( IDS_DTaWriKey));
		return false;
	}
	catch (std::exception &e)
	{
		LogFileWriter::WriteExceptionLog (e,( IDS_DTaWriKey));	
		return false;
	}
	catch (...)
	{
		LogFileWriter::WriteUnknownExceptionLog (( IDS_UnkExc1),( IDS_DTaWriKey));		
		return false;
	}
}

void DTableContainer::ReadTableNamesToRead ()
{
	try
	{
		VStr fpath = VGlobalFunctions::GetSolidCncExeFolder();
		fpath += ( IDS_TabTabTxt);
		VIFStream f (fpath);
		if (f.is_open())
		{
			while (!f.eof())
			{
				f.getline (m_readBuffer,MAX_CHR_IN_ONE_LINE,'\n');
				VStr tempStr (m_readBuffer);
				if (tempStr.length() > 0)
					m_tableNamesToRead.push_back (tempStr);
			}
		}
	}
	catch (VException &e)
	{
		LogFileWriter::WriteExceptionLog (e,( IDS_DTaRea1));
	}
	catch (std::exception &e)
	{
		LogFileWriter::WriteExceptionLog (e,( IDS_DTaRea1));	
	}
	catch (...)
	{
		LogFileWriter::WriteUnknownExceptionLog (( IDS_UnkExc1),( IDS_DTaRea1));
	}
}
	
void DTableContainer::ReadTableKeyAttributesNamesToRead ()
{
	try
	{
		VStr fpath = VGlobalFunctions::GetSolidCncExeFolder();
		fpath += ( IDS_TabTabTxt1);
		VIFStream f (fpath);
		if (f.is_open())
		{
			while (!f.eof())
			{
				f.getline (m_readBuffer,MAX_CHR_IN_ONE_LINE,'\n');
				VStr tempStr (m_readBuffer);
				if (tempStr.length() > 0)
					m_tableKeyAttributesNamesToRead.push_back (tempStr);
			}
		}
	}
	catch (VException &e)
	{
		LogFileWriter::WriteExceptionLog (e,( IDS_DTaRea2));
	}
	catch (std::exception &e)
	{
		LogFileWriter::WriteExceptionLog (e,( IDS_DTaRea2));	
	}
	catch (...)
	{
		LogFileWriter::WriteUnknownExceptionLog (( IDS_UnkExc1),( IDS_DTaRea2));

	}
}

void DTableContainer::ReadAllTables()
{
	try
	{
		VStr fpath = VGlobalFunctions::GetSolidCncExeFolder();
		fpath += ("Tables\\");
		size_t myTableSize = m_tableNamesToRead.size();
		for (size_t i=0;i<myTableSize;++i)
		{
			const VStr &curTableName = m_tableNamesToRead.at(i);
			DTable newTable(this);
			
			newTable.SetTypeName (curTableName);
			bool ck = newTable.Read (fpath);
			if (ck)
			{
				VStr trName = sozluk(curTableName);
				newTable.SetTypeName(trName);
				newTable.SetOriginalTypeName(curTableName);
				m_allTables.insert(std::pair<VStr, DTable>(trName,newTable));//(curTableName,newTable));
			}
		}
	}
	catch (VException &e)
	{
		LogFileWriter::WriteExceptionLog (e,( IDS_DTaRea3));
	}
	catch (std::exception &e)
	{
		LogFileWriter::WriteExceptionLog (e,( IDS_DTaRea3));
	}
	catch (...)
	{
		LogFileWriter::WriteUnknownExceptionLog (( IDS_UnkExc1),( IDS_DTaRea3));
	}
}

void DTableContainer::ReadAllTableKeyAttributes ()
{
	try
	{
		VStr fpath = VGlobalFunctions::GetSolidCncExeFolder();
		fpath+=("Tables\\");
		size_t myAttributeSize = m_tableKeyAttributesNamesToRead.size();
		for (size_t i = 0 ; i<myAttributeSize;++i)
		{
			const VStr &curAttrName = m_tableKeyAttributesNamesToRead.at(i);
			DTableKeyAttributes newAttr;
			newAttr.SetName (curAttrName);
			bool ck = newAttr.Read (fpath);
			m_allTableKeyAttributes.insert (std::pair<VStr,DTableKeyAttributes> (curAttrName , newAttr));
		}
	}
	catch (VException &e)
	{
		LogFileWriter::WriteExceptionLog ( e , ( IDS_DTaRea4));
	}
	catch (std::exception &e)
	{
		LogFileWriter::WriteExceptionLog ( e , ( IDS_DTaRea4));
	}
	catch (...)
	{
		LogFileWriter::WriteUnknownExceptionLog ( ( IDS_UnkExc1) , ( IDS_DTaRea4));
	}
}

std::vector<DTableKeyAttributes *> DTableContainer::GetAttributes (const VStr &attrStr)
{
	std::vector<DTableKeyAttributes *> res;
	std::vector<VStr> allRelatedAttrNames;
	TypeConverters::SplitString (attrStr , PLUS_CHR , allRelatedAttrNames);
	size_t attrSize = allRelatedAttrNames.size();
	for (size_t i=0;i<attrSize;++i)
	{
		const VStr &curAttrName = allRelatedAttrNames.at(i);
		std::map<VStr,DTableKeyAttributes>::iterator fi = m_allTableKeyAttributes.find (curAttrName);
		if (fi != m_allTableKeyAttributes.end())
			res.push_back (&fi->second);
	}
	return res;
}

const DTable * DTableContainer::SearchTable(const VStr &tableName)const
{
	std::map<VStr, DTable>::const_iterator fi = m_allTables.find(tableName);
	if (fi == m_allTables.end())
	{
		return 0;
	}
	else
	{
		return &fi->second;
	}
}

std::map<VStr, VStr> DTableContainer::GetParameterKeyValPairs(const VStr &tableName)const
{
	std::map<VStr, VStr> parKeyVal;
	try
	{
		const DTable *prt = SearchTable(tableName);
		if (prt == 0)
			throw VException("DTableContainer::GetParameterKeyValPairs exception , table name not found",1);
		const std::vector<DTableRow> &rows = prt->GetRowVec();
		int rowsize = (int)rows.size();
		for (int ri = 0; ri < rowsize; ++ri)
		{
			const DTableRow &currow = rows.at(ri);
			const std::vector<VStr> &rv = currow.GetData();
			if (rv.size() < 2)
				throw VException("ParametricScriptMaker::MakeParametersLua exception , parameter table column count must be greater than or equal to 2", 1);
			const VStr &key = rv.at(0);
			const VStr &val = rv.at(1);
			parKeyVal[key] = val;
		}
		return parKeyVal;
	}
	catch (VException &e)
	{
		LogFileWriter::WriteExceptionLog(e, "DTableContainer::GetParameterKeyValPairs");
		return parKeyVal;
	}
	catch (...)
	{
		LogFileWriter::WriteUnknownExceptionLog("UE ", "DTableContainer::GetParameterKeyValPairs");
		return parKeyVal;
	}
}