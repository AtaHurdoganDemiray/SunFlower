// MapSerializer.h

#ifndef __MapSerializerCore_h__
#define __MapSerializerCore_h__

#include "Bofstream.h"
#include "Bifstream.h"

class MapSerializerCore
{
public:
	static void SerializeIntVecStrVecMap (const std::map<int,std::vector<std::vector<VStr>>> &m , VofStream &f);
	static void SerializeIntVecStrVecMap (std::map<int,std::vector<std::vector<VStr>>> &m , VIFStream &f);
	static void AddPointDataToKeyValueMap (const VPoint &p , const VStr &pName , std::map<VStr,VStr> &keyValueMap);
	static void AddVectorDataToKeyValueMap (const VPoint &p , const VStr &pName , std::map<VStr,VStr> &keyValueMap);
	// returns true if key value already had existed in map
	static bool AddKeyValue (const VStr &keyStr , const VStr &valStr , std::map<VStr,VStr> &m);
	static void SerializeStrIntMap (const std::map<VStr,int> &m,Bofstream &f);
	static void SerializeStrIntMap (std::map<VStr,int> &m,Bifstream &f);
	static void SerializeStrStrMap (const std::map<VStr,VStr> &keyValMap,Bofstream &f);
	static void SerializeStrStrMap (std::map<VStr,VStr> &keyValMap,Bifstream &f);
	static void WriteStrStrMap(const std::map<VStr, VStr> &keyValMap, VofStream &f);
	static void ReadStrStrMap(std::map<VStr, VStr> &keyValMap, VIFStream &f);
	static void SerializeIntMapStrStrMap (const std::map<int,std::map<VStr,VStr>> &extraInfoMap,Bofstream &f);
	static void SerializeIntMapStrStrMap (std::map<int,std::map<VStr,VStr>> &extraInfoMap,Bifstream &f);
};

#endif