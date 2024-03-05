// NameList.h

#ifndef __NameList_h__
#define __NameList_h__

class Bifstream;
class Bofstream;

class NameList:public std::list<VStr>
{
public:
	NameList ();
	NameList (const VStr &allnames);
	typedef std::list<VStr>::const_iterator myiterator;
	void InsertName(VStr aname);	
	bool operator == (const NameList &toCompare) const;
	void Serialize (Bifstream &infile);
	void Serialize (Bofstream &outfile)const;
	bool IsInList (const VStr &myname)const;
	void AddFrom (NameList &toAdd);
	VStr GetFolderPath ()const;
	bool IsExist(VStr txt)const;
	bool IsDontAddToTreeFolder()const;
};
typedef NameList FolderPath;
#endif