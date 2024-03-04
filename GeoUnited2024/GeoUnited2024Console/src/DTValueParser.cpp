// DTValueParser.cpp

#include "stdafx.h"
#include "DTValueParser.h"
#include "TypeConverters.h"

void DTValueParser::Parse  (const VStr &value 
	, VStr &valuePart , VStr &typeInfoPart , VStr &someValueSetPart , VStr &someValueSetFromTablePart , VStr &scriptFunctionPart)
{
	valuePart = value;
	std::map<VStr,std::vector<int>> strBeginIndexMap[4];
	int bi = 0;
	int ei = valuePart.length()-1;
	strBeginIndexMap[0]["<<"] = std::vector<int>();
	strBeginIndexMap[0][">>"] = std::vector<int>();
	TypeConverters::DetectPlaces (valuePart,bi,ei,strBeginIndexMap[0]);	
	ProcessOnePart (strBeginIndexMap[0]["<<"],strBeginIndexMap[0][">>"],valuePart,typeInfoPart);

	strBeginIndexMap[1]["{{"] = std::vector<int>();
	strBeginIndexMap[1]["}}"] = std::vector<int>();
	ei = valuePart.length() -1;
	TypeConverters::DetectPlaces (valuePart,bi,ei,strBeginIndexMap[1]);
	ProcessOnePart (strBeginIndexMap[1]["{{"],strBeginIndexMap[1]["}}"],valuePart,someValueSetPart);

	strBeginIndexMap[2]["[["] = std::vector<int>();
	strBeginIndexMap[2]["]]"] = std::vector<int>();
	ei = valuePart.length() -1;
	TypeConverters::DetectPlaces (valuePart,bi,ei,strBeginIndexMap[2]);
	ProcessOnePart (strBeginIndexMap[2]["[["],strBeginIndexMap[2]["]]"],valuePart,someValueSetFromTablePart);

	strBeginIndexMap[3]["(("] = std::vector<int>();
	strBeginIndexMap[3]["))"] = std::vector<int>();
	ei = valuePart.length()-1;
	TypeConverters::DetectPlaces (valuePart,bi,ei,strBeginIndexMap[3]);
	ProcessOnePart (strBeginIndexMap[3]["(("],strBeginIndexMap[3]["))"],valuePart,scriptFunctionPart);
}

void DTValueParser::ProcessOnePart (const std::vector<int> &beginStrBeginiVec , const std::vector<int> &endStrBeginiVec
								, VStr &valuePart , VStr &extraPart)
{
	// first instance of {{ , (( ,[[ , << ...  will be read
	size_t mysize = beginStrBeginiVec.size();
	size_t mysize2 = endStrBeginiVec.size();
	for (size_t i=0;i<mysize;++i)
	{
		int curbi = beginStrBeginiVec.at(i);
		if (i < mysize2)
		{
			int curei = endStrBeginiVec.at(i) + 1;
			VStr curExtraPart;
			ExtractRegion (valuePart , curExtraPart,curbi,curei);
			extraPart += curExtraPart;
			//extraPart += '|';
		}
	}
}

void DTValueParser::ExtractRegion (VStr &txt , VStr &extractedRegion , int bi , int ei)
{
	VStr newTxt;
	int len = txt.length();
	for (int i=0;i<len;++i)
	{
		if (i<=ei && i>=bi)
			extractedRegion+=txt.at(i);
		else
			newTxt+=txt.at(i);
	}
	txt=newTxt;
}

void DTValueParser::GetSomeValueSet (const VStr &src , std::vector<VStr> &someValueSet)
{
	VStr subStr = src.substr (2,src.length()-4);
	TypeConverters::SplitString (subStr,'+',someValueSet);
}
