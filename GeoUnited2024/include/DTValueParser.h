// DTValueParser.h

#ifndef __DTValueParser_h__
#define __DTValueParser_h__

class DTValueParser
{
public:
	static void Parse (const VStr &value , VStr &valuePart , VStr &typeInfoPart , VStr &someValueSetPart , VStr &someValueSetFromTablePart , VStr &scriptFunctionPart);
	static void ProcessOnePart (const std::vector<int> &beginStrBeginiVec , const std::vector<int> &endStrBeginiVec
								, VStr &valuePart , VStr &extraPart);
	static void ExtractRegion (VStr &txt , VStr &extractedRegion , int bi , int ei);
	static void GetSomeValueSet (const VStr &src , std::vector<VStr> &someValueSet);
};

#endif