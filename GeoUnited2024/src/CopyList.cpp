// CopyList.cpp

#include "stdafx.h"
#include "CopyList.h"

CopyList::CopyList()
{

}

void CopyList::Construct(const VStr &path, bool overWrite , bool creaUnexistingFolders)
{
	m_path = path;
	m_overWrite = overWrite;
	m_createUnexistingFolders = creaUnexistingFolders;

}

void CopyList::ConstructAndRead(const VStr &path, bool overWrite, bool creaUnexistingFolders)
{
	m_path = path;
	m_overWrite = overWrite;
	m_createUnexistingFolders = creaUnexistingFolders;
}


void CopyList::Write(std::ofstream &f)const
{
	VStr overWriteStr = m_overWrite ? "1" : "0";
	VStr createUnexistingFoldersStr = m_createUnexistingFolders ? "1" : "0";
	f << m_id.GetIdString() << "\n";
	f << overWriteStr << "\n";
	f << createUnexistingFoldersStr << "\n";
	citr i = m_srcDestList.begin();
	citr e = m_srcDestList.end();
	for (i; i != e; ++i)
	{
		f << i->first << "\n";
		f << i->second << "\n";
	}
}
	
void CopyList::Read(std::ifstream &f)
{
	VStr overWriteStr, createUnexistingFolderStr, modelIdStr;
	f >> modelIdStr;
	f >> overWriteStr;
	f >> createUnexistingFolderStr;
	m_id.SetIdString(modelIdStr);
	m_overWrite = overWriteStr == "1" ? true : false;
	m_createUnexistingFolders = createUnexistingFolderStr == "1" ? true : false;
	while (!f.eof())
	{
		VStr src, dest;
		f >> src;
		f >> dest;
		AddSinglePath(src, dest);
	}
}

void CopyList::AddSinglePath(const VStr &src, const VStr &dest)
{
	m_srcDestList.push_back(std::pair<VStr, VStr>(src,dest));
}

bool CopyList::ForwardCopy()// from source to destination
{

	return true;
}
	
bool CopyList::BackwardCopy()// from destination to source
{

	return true;
}
	
bool CopyList::CreateFoldersIf(const VStr &singleFolder)const// returns true if folder created
{

	return false;
}
	
bool CopyList::IsFileExisting(const VStr &singleFile)const
{

	return false;
}
	
bool CopyList::IsFolder(const VStr &singleLine)const
{

	return false;
}
	
const ModelID & CopyList::GetModelID()const
{
	
	return m_id;
}
