// DTableKeyAttributes.cpp

#include "stdafx.h"
#include "DTableKeyAttributes.h"
#include "VectorSerializer.h"
#include "TypeConverters.h"
#include "DTableContainer.h"
#include "MapSerializerCore.h"
#include "DTableContainer.h"

//DTableKeyAttributes::DTableKeyAttributes ()
//	:m_name((""))
//	,m_isReadOnly("0")
//	,m_defaultValue((""))
//	,m_commaSeparatedSearchItemsVec((""))
//{
//
//}
//
//const VStr & DTableKeyAttributes::GetName()const
//{
//	return m_name;
//}
//
VStr DTableKeyAttributes::GetIsReadOnly ()const
{
	return SearchValue (( IDS_IsReadOnly));
}

VStr DTableKeyAttributes::GetDefaultValue()const
{
	return SearchValue(( IDS_DefaultValue));
}

VStr DTableKeyAttributes::GetSearchItemsVec()const
{
	return SearchValue(( IDS_SearchItemsVec));
}

VStr DTableKeyAttributes::GetShortDescription()const
{
	return SearchValue(( IDS_ShortDescriptio));
}

VStr DTableKeyAttributes::GetInfoPicturePath ()const
{
	return SearchValue(( IDS_InfoPicturePath));
}

VStr DTableKeyAttributes::GetIsAddEverySingleInstanceToSearch()const
{
	return SearchValue(( IDS_IsAddEverySingl));
}
//
//void DTableKeyAttributes::Write (const VStr &baseFolderName)const
//{
//	VStr fname (baseFolderName);
//	fname += m_name;
//	fname += (".txt");
//	VofStream f (fname);
//	if (f.is_open () == false)
//		LogFileWriter::WriteUnknownExceptionLog (("file not opened") , ("DTableKeyAttributes::Write"));
//
//	DTableContainer::Write (f, ("Name"),m_name);
//	DTableContainer::Write (f, ("IsReadOnly") , m_isReadOnly);
//	DTableContainer::Write (f, ("DefaultValue") , m_defaultValue);
//	DTableContainer::Write (f, ("DataType") , m_dataType);
//	DTableContainer::Write (f, ("ShortDescription"),m_shortDescription);
//	DTableContainer::Write (f, ("InfoPicturePath"),m_infoPicturePath);
//	DTableContainer::Write (f, ("CommaSeparatedSearchItemsVec"),m_commaSeparatedSearchItemsVec);
//	DTableContainer::Write (f, ("IsAddEverySingleInstanceToSearch"), m_isAddEverySingleInstanceToSearch);
//	DTableContainer::Write (f, ("IsVisible"),m_isVisible);
//	DTableContainer::Write (f, ("IsList") ,  m_isList); 
//	DTableContainer::Write (f,("MinValue") , m_minValue);
//	DTableContainer::Write (f, ("MaxValue"),m_maxValue);
//}
//
//void DTableKeyAttributes::Read (const VStr &baseFolderName)
//{
//	VStr fname (baseFolderName);
//	fname += m_name;
//	fname += (".txt");
//	VIFStream f (fname);
//	if (f.is_open () == false)
//		LogFileWriter::WriteUnknownExceptionLog (("file not opened") , ("DTableKeyAttributes::Read"));
//
//	std::map<VStr,VStr> keyValueMap;
//	bool ck = DTableContainer::Read (f , keyValueMap);
//	if (ck == false)
//		LogFileWriter::WriteUnknownExceptionLog (("File reading failed"),("DTableKeyAttributes::Read"));
//
//	std::map<VStr,VStr>::iterator fi0 = keyValueMap.find (("Name"));
//	if (fi0 != keyValueMap.end())
//		m_name = fi0->second;
//
//	std::map<VStr,VStr>::iterator fi1 =  keyValueMap.find(("IsReadOnly"));
//	if (fi1 != keyValueMap.end())
//		m_isReadOnly = fi1->second;
//
//	std::map<VStr,VStr>::iterator fi2 = keyValueMap.find (("DefaultValue"));
//	if (fi2 != keyValueMap.end())
//		m_defaultValue = fi2->second;
//
//	std::map<VStr,VStr>::iterator fi3 = keyValueMap.find (("DataType"));
//	if (fi3 != keyValueMap.end())
//		m_dataType = fi3->second;
//	
//	std::map<VStr,VStr>::iterator fi4 = keyValueMap.find (("ShortDescription"));
//	if (fi4 != keyValueMap.end())
//		m_shortDescription = fi4->second;
//	std::map<VStr,VStr>::iterator fi5 = keyValueMap.find (("InfoPicturePath"));
//	if (fi5 != keyValueMap.end())
//		m_infoPicturePath = fi5->second;
//	std::map<VStr,VStr>::iterator fi6 = keyValueMap.find (("CommaSeparatedSearchItemsVec"));
//	if (fi6 != keyValueMap.end())
//		m_commaSeparatedSearchItemsVec = fi6->second;
//	std::map<VStr,VStr>::iterator fi7 = keyValueMap.find ( ("IsAddEverySingleInstanceToSearch"));
//	if (fi7 != keyValueMap.end())
//		m_isAddEverySingleInstanceToSearch = fi7->second;
//	std::map<VStr,VStr>::iterator fi8 = keyValueMap.find ( ("IsVisible"));
//	if (fi8 != keyValueMap.end())
//		m_isVisible = fi8->second;
//	
//	std::map<VStr,VStr>::iterator fi9 = keyValueMap.find (("IsList"));
//	if (fi9 != keyValueMap.end())
//		m_isList = fi9->second; 
//	std::map<VStr,VStr>::iterator fi10 = keyValueMap.find(("MinValue"));
//	if (fi10 != keyValueMap.end())	
//		m_minValue = fi10->second;
//	std::map<VStr,VStr>::iterator fi11 = keyValueMap.find (("MaxValue"));
//	if (fi11 != keyValueMap.end())
//		m_maxValue = fi11->second;
//}
//
VStr DTableKeyAttributes::GetIsVisible ()const
{
	return SearchValue(( IDS_IsVisible));
}

VStr DTableKeyAttributes::GetIsList()const
{
	return SearchValue(( IDS_IsList));
}
	
VStr DTableKeyAttributes::GetMinValue()const
{
	return SearchValue(( IDS_MinValue));
}
	
VStr DTableKeyAttributes::GetMaxValue()const
{
	return SearchValue(( IDS_MaxValue));
}

DTableKeyAttributes::DTableKeyAttributes()
{

}

const VStr & DTableKeyAttributes::GetName()const
{
	return m_name;
}

void DTableKeyAttributes::SetName (const VStr &name)
{
	m_name = name;
}

bool DTableKeyAttributes::Read (const VStr &baseFolder)
{
	try
	{
		VStr fname (baseFolder);
		fname+=m_name;
		fname+=( IDS_TabTxt);
		VIFStream f (fname);
		bool ck = DTableContainer::Read (f,m_attrNameValueMap);
		return ck;
	}
	catch (VException &e)
	{
		LogFileWriter::WriteExceptionLog (e,( IDS_DTaRea5));
		return false;
	}
	catch (std::exception &e)
	{
		LogFileWriter::WriteExceptionLog (e,( IDS_DTaRea5));
		return false;
	}
	catch (...)
	{
		LogFileWriter::WriteUnknownExceptionLog (( IDS_UnkExc1),( IDS_DTaRea5));
		return false;
	}
}

bool DTableKeyAttributes::Write (const VStr &baseFolder)const
{
	try
	{
		VStr fname (baseFolder);
		fname+=m_name;
		fname+=( IDS_TabTxt);
		VofStream f (fname);
		bool ck = DTableContainer::Write (f,m_attrNameValueMap);
		return ck;
	}
	catch (VException &e)
	{
		LogFileWriter::WriteExceptionLog (e,( IDS_DTaWri));
		return false;
	}
	catch (std::exception &e)
	{
		LogFileWriter::WriteExceptionLog (e,( IDS_DTaWri));
		return false;
	}
	catch (...)
	{
		LogFileWriter::WriteUnknownExceptionLog (( IDS_UnkExc1),( IDS_DTaWri));
		return false;
	}
}
std::map<VStr,VStr> & DTableKeyAttributes::GetAttrNameValueMap()
{
	return m_attrNameValueMap;
}
const VStr DTableKeyAttributes::KEY_NOT_FOUND_STR = ( IDS_Null);
VStr DTableKeyAttributes::GetValueFor (const VStr &key)const
{
	VStr res ( KEY_NOT_FOUND_STR );
	std::map<VStr,VStr>::const_iterator fi = m_attrNameValueMap.find ( key );
	if (fi != m_attrNameValueMap.end())
		res = fi->second;
	return res;
}
void DTableKeyAttributes::AddAttr (const VStr &attrName , const VStr &value)
{
	m_attrNameValueMap.insert (std::pair<VStr,VStr> (attrName,value));
}

std::map<VStr,VStr> DTableKeyAttributes::MergeAttributes (const std::vector<DTableKeyAttributes *> &attrVec)
{
	std::map<VStr,VStr> mergeMap;
	size_t attrSize = attrVec.size();
	for (size_t i=0;i<attrSize;++i)
	{
		DTableKeyAttributes *curAttr = attrVec.at(i);
		std::map<VStr,VStr>::const_iterator ii = curAttr->GetAttrNameValueMap().begin();
		std::map<VStr,VStr>::const_iterator ee = curAttr->GetAttrNameValueMap().end();
		for (ii;ii!=ee;++ii)
		{
			MapSerializerCore::AddKeyValue (ii->first,ii->second,mergeMap);
		}
	}
	return mergeMap;
}

VStr DTableKeyAttributes::SearchValue (const VStr &key)const
{
	std::map<VStr,VStr>::const_iterator fi = m_attrNameValueMap.find (key);
	if (fi != m_attrNameValueMap.end())
		return fi->second;
	return KEY_NOT_FOUND_STR;
}

void DTableKeyAttributes::AddSearchItemsIf (std::vector<VStr> &searchItems)
{
	const VStr &si = GetSearchItemsVec ();
	if (si!= KEY_NOT_FOUND_STR)
		TypeConverters::SplitString (si , VP_ROW_SEPARATOR , searchItems);
}
