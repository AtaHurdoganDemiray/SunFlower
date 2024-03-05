// MapSerializer.cpp

#include "stdafx.h"
#include "MapSerializerCore.h"
#include "TypeConverters.h"

void MapSerializerCore::SerializeIntVecStrVecMap (const std::map<int,std::vector<std::vector<VStr>>> &m , VofStream &f)
{
		int mysize = (int)m.size();
		f <<  IDS_TotalTypeNumber <<  IDS_N;
		f << mysize <<  IDS_N;
		std::map<int,std::vector<std::vector<VStr>>>::const_iterator i = m.begin();
		std::map<int,std::vector<std::vector<VStr>>>::const_iterator e = m.end();
		for (i;i!=e;++i)
		{
			f <<  IDS_TypeIndex << i->first <<  IDS_N;
			int mysize2 = (int)i->second.size();
			f <<   IDS_TotRowCouForTyp <<  IDS_N;
			f << mysize2 <<  IDS_N;
			
			for (int ii=0;ii<mysize2;++ii)
			{
				const std::vector<VStr> &curRow = i->second.at(ii);
				int mysize3 = (int)curRow.size();
				VStr oneRow;
				TypeConverters::MergeString (curRow , '|' , oneRow);
				f << oneRow.c_str() <<  IDS_N;
			}
	}
}


void MapSerializerCore::SerializeIntVecStrVecMap (std::map<int,std::vector<std::vector<VStr>>> &m , VIFStream &f)
{
	char temp[1000];
	int mysize = 0;
	f.getline (temp,1000,'\n'); //"Total Type Number" << "\n";
	f.getline (temp,1000,'\n');
	VStr tempstr (temp);
	double tempd = TypeConverters::ConvertStringTovreal (tempstr);
	mysize = (int)tempd;
	for (int i=0;i<mysize;++i)
	{
		f.getline (temp,1000,'\n');  //"Type index: " << i->first << "\n";
		int typei = 0;
		f.getline (temp,1000,'\n');
		VStr tempstr2 (temp);
		double tempd2 = TypeConverters::ConvertStringTovreal (tempstr2);
		typei = (int)tempd2;
		
		int mysize2 = 0;
		f.getline(temp,1000,'\n');//  "Total row count for type:"  << "\n";
		f.getline (temp,1000,'\n');
		VStr tempstr3 (temp);
		double tempd3 = TypeConverters::ConvertStringTovreal (tempstr3);
		mysize2 = (int)tempd3;
		std::vector<std::vector<VStr>> vecvec;
		for (int ii = 0; ii<mysize2; ++ii)
		{
			f.getline(temp,1000,'\n');
			VStr oneRow (temp);
			std::vector<VStr> sRowVec;
			TypeConverters::SplitString (oneRow , '|' , sRowVec);
			vecvec.push_back(sRowVec);
		}
		m.insert (std::pair<int,std::vector<std::vector<VStr>>> (typei , vecvec));
	}
}
 
void MapSerializerCore::AddPointDataToKeyValueMap (const VPoint &p , const VStr &pName , std::map<VStr,VStr> &keyValueMap)
{
	VOStrStream mystr1,mystr2,mystr3;
	mystr1 << p.x();
	mystr2 << p.y();
	mystr3 << p.z();
	VStr k1 (pName),k2(pName),k3(pName);
	k1 += ( IDS_X);
	k2 += ( IDS_Y);
	k3 += ( IDS_Z);
	
	keyValueMap.insert (std::pair<VStr,VStr> (k1,mystr1.str()));
	keyValueMap.insert (std::pair<VStr,VStr> (k2,mystr2.str()));
	keyValueMap.insert (std::pair<VStr,VStr> (k3,mystr3.str()));
}

void MapSerializerCore::AddVectorDataToKeyValueMap (const VPoint &p , const VStr &pName , std::map<VStr,VStr> &keyValueMap)
{
	VOStrStream mystr1,mystr2,mystr3;
	mystr1 << p.x();
	mystr2 << p.y();
	mystr3 << p.z();
	VStr k1 (pName),k2(pName),k3(pName);
	k1 += ( IDS_I);
	k2 += ( IDS_J);
	k3 += ( IDS_K);
	
	keyValueMap.insert (std::pair<VStr,VStr> (k1,mystr1.str()));
	keyValueMap.insert (std::pair<VStr,VStr> (k2,mystr2.str()));
	keyValueMap.insert (std::pair<VStr,VStr> (k3,mystr3.str()));
}


bool MapSerializerCore::AddKeyValue (const VStr &keyStr , const VStr &valueStr,std::map<VStr,VStr> &keyValueMap)
{
	std::map<VStr,VStr>::iterator fi = keyValueMap.find(keyStr);
	if (fi == keyValueMap.end())
	{
		keyValueMap.insert (std::pair<VStr,VStr> (keyStr,valueStr));
		return false;
	}
	fi->second = valueStr;
	return true;
}
void MapSerializerCore::SerializeStrIntMap (const std::map<VStr,int> &m , Bofstream &f)
{
	int mysize = (int) m.size();
	f.WriteInt (mysize);
	std::map<VStr,int>::const_iterator i,e;
	i = m.begin();
	e = m.end();
	for (i;i!=e;++i)
	{
		f.WriteString (i->first);
		f.WriteInt (i->second);
	}
}

void MapSerializerCore::SerializeStrIntMap (std::map<VStr,int> &m , Bifstream &f)
{
	int mysize;
	f.ReadInt(mysize);
	for (int i=0;i<mysize;++i)
	{
		VStr s;
		f.ReadString (s);
		int myval;
		f.ReadInt (myval);
		m[s]=myval;
	}
}
void MapSerializerCore::SerializeStrStrMap (const std::map<VStr,VStr> &keyValMap,Bofstream &f)
{
	int mysize = (int)keyValMap.size();
	f.WriteInt (mysize);
	std::map<VStr,VStr>::const_iterator i = keyValMap.begin();
	std::map<VStr,VStr>::const_iterator e = keyValMap.end();
	for (i;i!=e;++i)
	{
		f.WriteString (i->first);
		f.WriteString (i->second);
	}
}
void MapSerializerCore::SerializeStrStrMap (std::map<VStr,VStr> &keyValMap,Bifstream &f)
{
	int mysize;
	f.ReadInt (mysize);
	for (int i=0;i<mysize;++i)
	{
		VStr tempKey,tempVal;
		f.ReadString (tempKey);
		f.ReadString (tempVal);
		keyValMap.operator[](tempKey) = tempVal;
	}
}

void MapSerializerCore::WriteStrStrMap(const std::map<VStr, VStr> &keyValMap, VofStream &f)
{
	int mysize = (int)keyValMap.size();
	VStr sizeStr = TypeConverters::ConvertIntToString(mysize);
	f.write(sizeStr.c_str(), sizeStr.length());
	std::map<VStr, VStr>::const_iterator i = keyValMap.begin();
	std::map<VStr, VStr>::const_iterator e = keyValMap.end();
	for (i; i != e; ++i)
	{
		f.write(i->first.c_str(),i->first.length());
		f.write(i->second.c_str(),i->second.length());
	}
}

void MapSerializerCore::ReadStrStrMap(std::map<VStr, VStr> &keyValMap, VIFStream &f)
{
	int mysize;
	VStr sizeStr = TypeConverters::ConvertIntToString(mysize);
	f.getline(VGlobalValues::VCHR_LINE_READER, VGlobalValues::VCHR_LINE_READERSIZE, '\n');
	sizeStr = VStr(VGlobalValues::VCHR_LINE_READER);
	mysize = TypeConverters::ConvertStringToInt(sizeStr);
	
	for (int i = 0; i < mysize;++i)
	{
		f.getline(VGlobalValues::VCHR_LINE_READER, VGlobalValues::VCHR_LINE_READERSIZE , '\n');
		VStr key(VGlobalValues::VCHR_LINE_READER);
		f.getline(VGlobalValues::VCHR_LINE_READER, VGlobalValues::VCHR_LINE_READERSIZE, '\n');
		VStr val(VGlobalValues::VCHR_LINE_READER);
		keyValMap[key] = val;
	}
}

void MapSerializerCore::SerializeIntMapStrStrMap (const std::map<int,std::map<VStr,VStr>> &extraInfoMap,Bofstream &f)
{
	int mysize = (int)extraInfoMap.size();
	f.WriteInt(mysize);
	std::map<int,std::map<VStr,VStr>>::const_iterator i = extraInfoMap.begin();
	std::map<int,std::map<VStr,VStr>>::const_iterator e = extraInfoMap.end();
	for (i;i!=e;++i)
	{
		f.WriteInt (i->first);
		SerializeStrStrMap (i->second , f);
	}
}
void MapSerializerCore::SerializeIntMapStrStrMap (std::map<int,std::map<VStr,VStr>> &extraInfoMap,Bifstream &f)
{
	int mysize;
	f.ReadInt (mysize);
	for (int i=0;i<mysize;++i)
	{
		int tempMoveNo;
		f.ReadInt (tempMoveNo);
		std::map<VStr,VStr> tempKeyValMap;
		SerializeStrStrMap (tempKeyValMap,f);
		extraInfoMap.operator[](tempMoveNo) = tempKeyValMap;
	}
}