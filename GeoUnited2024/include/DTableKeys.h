// DTableKeys.h

#ifndef __DTableKeys_h__
#define __DTableKeys_h__

class DTableContainer;

class DTableKeys
{
public:
	DTableKeys(DTableContainer *DB);
	const  std::vector<VStr> & GetNames()const;
	const std::vector<VStr> & GetAttributesNames()const;
	bool Write (const VStr &baseFolderName , const VStr &tableName)const;
	bool Read (const VStr &baseFolderName , const VStr &tableName);
private:
	std::vector<VStr> m_names;
	std::vector<VStr> m_attributesNames;
	std::vector<int> m_keyWidths;
public:
	VStr GetDefaultValueForKey (int keyNo);
	VStr GetDefaultValueForKey (const VStr &keyName);
	std::vector<VStr> GetDefaultValuesForEntireKeys ();
	DTableContainer * m_DB;
	void FillSearchItemsVec (const VStr &cellValue , int keyNo , std::vector<VStr> &searchItems)const; 
	std::vector<int> & GetKeyWidths();
	const std::vector<int> & GetKeyWidths()const;
};

#endif