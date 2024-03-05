// TypeConverters.h

#ifndef __TypeConverters_h__
#define __TypeConverters_h__

struct VPoint;
struct VLine;
class VArc;
class TransformationMatrix;

class TypeConverters
{
public:
	static VStr ConvertvrealToString (const vreal &myscaler);
	static VStr ConvertFloatToString (const float &myscaler);
	static VStr ConvertIntToString (const int &myinteger);
	static vreal ConvertStringTovreal (const VStr &mystr);
	static void SplitString(const VStr &src, const VChr seperator, std::vector<VStr> &result);
	static void MergeString (const std::vector<VStr> &src , const VChr seperator , VStr &result);
	static bool DetectPlaces (const VStr &str , int bi , int ei , std::map<char,std::vector<int>> &charPlaceIndexMap);
	static VStr GetSubSet (const VStr &str , int bi , int ei);// bi and ei are included
	static bool RemoveCharsAtStartAndEnd (char cToRemove , VStr &s);
	static void AddCharsAtStartAndEnd (char cToAdd , VStr &s);
	static bool RemoveCharsAtStartAndEndForEveryStr (char cToRemove , std::vector<VStr> &sVec);
	static void AddCharsAtStartAndEndForEveryStr (char cToAdd , std::vector<VStr> &sVec);
	typedef std::map<VStr,VStr>::const_iterator itrs;
	static int GetInt (const std::map<VStr,VStr> &argMap , const VStr &nameStr);
	static vreal GetScalar (const std::map<VStr,VStr> &argMap , const VStr &nameStr);
	static VPoint GetPoint (const std::map<VStr,VStr> &argMap , const VStr &nameStr);
	static VLine GetLine (const std::map<VStr,VStr> &argMap , const VStr &lineName);
	static VArc GetArc (const std::map<VStr,VStr> &argMap , const VStr &arcName);
	static VStr exm1;
	static void StrVecToMap (const std::vector<VStr> &strVec , std::map<VStr,VStr> &keyValueMap);
	static void MapToStrVec (const std::map<VStr,VStr> &keyValueMap , std::vector<VStr> &strVec);
	static VStr GetString (const std::map<VStr,VStr> &argMap , const VStr &keyStr);
	static bool DetectPlaces (const VStr &str , int bi , int ei , std::map<VStr,std::vector<int>> &strBeginIndexMap);
	static std::vector<vreal> GetRealArray (const VStr &line , VChr sep);
	static std::vector<int> GetIntArray (const VStr &line , VChr sep);
	static VStr ConvertvrealToString2(const vreal &val);
	static int ConvertStringToInt(const VStr val);
	static VPoint GetPoint(const std::map<VStr, VStr> &argMap, const VStr &nameStr, const char & seperator);
	static VStr PointToString(const VPoint & p, const char &seperator);
	static TransformationMatrix GetMatrix(const std::map<VStr, VStr> &argMap, const VStr &nameStr);
	static VPoint ConvertStringToPoint(VStr val , char seperator);
};

#endif