// DTableContainer.h

#ifndef __DTableContainer_h__
#define __DTableContainer_h__

#include  "DTable.h"
#include "DTableKeyAttributes.h"

class DTableContainer
{
public:
	DTableContainer();
private:
	std::map<VStr,DTable> m_allTables;
	std::map<VStr,DTableKeyAttributes> m_allTableKeyAttributes;
public:
	std::map<VStr,DTable> & GetAllTables();
	std::map<VStr,DTableKeyAttributes> & GetAllTableKeyAttributes();
	static void Write ( VofStream &s , const VStr &keyName , const VStr &value );
	static void Write ( VofStream &f , const VStr &keyName , bool value );
	static bool Read (VIFStream &f , std::map<VStr,VStr> &keyValueMap);
	static bool Write (VofStream &f , const std::map<VStr,VStr> &keyValueMap);
	static VChr *m_readBuffer;
private:
	std::vector<VStr> m_tableNamesToRead;
	std::vector<VStr> m_tableKeyAttributesNamesToRead;
public:
	void ReadTableNamesToRead ();
	void ReadTableKeyAttributesNamesToRead();
	void ReadAllTables ();
	void WriteAllTables ();
	void ReadAllTableKeyAttributes();
	std::vector<DTableKeyAttributes *> GetAttributes (const VStr &attrStr);
	const DTable * SearchTable(const VStr &tableName)const;
	std::map<VStr, VStr> GetParameterKeyValPairs(const VStr &tableName)const;
};


#endif