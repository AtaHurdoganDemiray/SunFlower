// DTable.h

#ifndef __DTable_h__
#define __DTable_h__

#include "DTableKeys.h"
#include "DTableRow.h"

class DTableContainer;
class DTableKeyAttributes;

class DTable
{
public:
	DTable (DTableContainer *DB);
	const VStr & GetTypeName()const;
	const DTableKeys & GetTableKeys()const;
	const std::vector<DTableRow> & GetRowVec()const;
	std::vector<DTableRow> & GetRowVec();
	bool Read (const VStr &baseFolderName);
	// Write function output 2 files , forexample 1st   CylindiricalToolKeys.txt 2nd  CylindiricalToolData.txt
	bool Write (const VStr &baseFolderName)const;
private:
	VStr m_typeName;							// A table type : eg:  Cylindirical Tool
	DTableKeys m_tableKeys;						// Data Schema : specifically the header of table , eg: Name Diameter Height...
	std::vector<DTableRow>	m_rowVec;			// Data it self : eg: All cylindirical tools values , 1.row  T1 , 5.0 , 50.0...  2.row T2 , 16.0 , 80.0...
	VStr m_originalTypeName;
public:
	void SetTypeName (const VStr &typeName);
	std::vector<VStr> GetDefaultRow ();
	DTableContainer * m_DB;
	bool AddNewRow (const std::vector<VStr> & rowStr);
	DTableKeyAttributes * GetAtt (int colNo);

	// for merged str the syntax is like this  :   color:red;id:135;brand:asas...
	VStr GetOneRowMergedStrAsKeyVal (int rowi)const;
	VStr GetOneRowMergedStrAsKeyVal2 (int rowi)const;
	VStr GetSomeOfRowMergedStrAsKeyVal (int rowi , const VStr &keyValuesToGet)const;
	bool SearchForKey (int rowi , const VStr &key , VStr &val)const;
	std::vector<int> SearchForSameKeyValSet (const VStr &mergedKeyValSet)const;
private:
	VStr m_scriptPath;
public:
	const VStr &GetScriptPath();
	void SetScriptPath(const VStr &name);
	static void MergedRowStrToMap (const VStr &mergedRowStr , std::map<VStr,VStr> &rowMap,char propertySplitter);
	static void MapToMergedRowStr (const std::map<VStr,VStr> &keyVal , VStr &mergedStr,char propertySplitter);
	void DeleteRows (std::map<int,DTableRow> &toDeleteMap);
private:
	std::vector<DTableRow> m_deletedRows;
public:
	void RescueDeletedRows();
	DTableKeys & GetTableKeys();
	DTableRow * SearchForRow(const VStr &key , const VStr &val);
	const VStr& GetOriginalTypeName()const;
	void SetOriginalTypeName(const VStr& originalTypeName);
};

#endif