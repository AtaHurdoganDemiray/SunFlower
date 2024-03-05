// CopyList.h

#ifndef __CopyList_h__
#define __CopyList_h__

#include "ModelID.h"

class CopyList
{
public:
	CopyList();
	void Construct(const VStr &path , bool overWrite = true, bool creaUnexistingFolders = true);
	void ConstructAndRead(const VStr &path, bool overWrite = true, bool creaUnexistingFolders = true);
private:
	std::list<std::pair<VStr, VStr>> m_srcDestList;
	bool m_overWrite;
	bool m_createUnexistingFolders;
	VStr m_path;//  full path with name
	ModelID m_id;
	typedef std::list<std::pair<VStr, VStr>>::const_iterator citr;
	typedef std::list<std::pair<VStr, VStr>>::iterator itr;
public:
	void Write(std::ofstream &f)const;
	void Read(std::ifstream &f);
	void AddSinglePath(const VStr &src, const VStr &dest);
	bool ForwardCopy();// from source to destination
	bool BackwardCopy();// from destination to source
	bool CreateFoldersIf(const VStr &singleFolder)const;// returns true if folder created
	bool IsFileExisting(const VStr &singleFile)const;
	bool IsFolder(const VStr &singleLine)const;
	const ModelID & GetModelID()const;
};

#endif