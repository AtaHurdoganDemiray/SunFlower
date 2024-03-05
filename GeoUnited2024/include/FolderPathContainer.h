// FolderPathContainer.h

#ifndef __FolderPathContainer_h__
#define __FolderPathContainer_h__
#include "NameList.h"
class Bifstream;
class Bofstream;

class FolderPathContainer:public std::list<FolderPath>
{
public:
	typedef std::list<FolderPath>::const_iterator myConstIterator;
	typedef std::list<FolderPath>::iterator myIterator;

	bool IsExisting(const FolderPath &folderpath)const;
	void Serialize (Bifstream &infile);
	void Serialize (Bofstream &outfile)const;		
};

#endif
