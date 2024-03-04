// FolderPathContainer.cpp

#include "stdafx.h"
#include "FolderPathContainer.h"
#include <functional>
#include <algorithm>

bool FolderPathContainer::IsExisting(const FolderPath &folderpath)const
{
	myConstIterator myend=end();
	for(myConstIterator i=begin();i!=myend;i++)
	{
		const FolderPath &currfolderpath=*i;
		if (currfolderpath==folderpath)
		{
			return true;
		}
	}
	return false;	
}

void FolderPathContainer::Serialize(Bofstream &outfile)const
{
	int mysize=size();
	outfile.write((VChr*)&mysize,sizeof(int));
	myConstIterator myend=end();
	for(myConstIterator i=begin();i!=myend;i++)
	{
		const FolderPath &currfolderpath=*i;
		currfolderpath.Serialize(outfile);		
	}
}

void FolderPathContainer::Serialize(Bifstream &infile)
{
	int mysize;
	infile.read ((VChr*)&mysize,sizeof(int));
	for(int i=0;i<mysize;i++)
	{
		FolderPath currfolderpath;
		currfolderpath.Serialize(infile);
		push_back(currfolderpath);
	}
}
