// FolderPath.cpp

#include "stdafx.h"
#include "NameList.h"
#include "Bifstream.h"
#include "Bofstream.h"

NameList::NameList ()
{

}

NameList::NameList (const VStr &allnames)
{
	int mylen = allnames.length()-1;
	VStr tmp;
	for (int i=mylen; i>=0;i--)
	{
		VChr currc = allnames[i];
		if (currc!='/')
		{
			tmp.push_back (currc);
		}
		else
		{
			VStr toadd;
			for (VStr::reverse_iterator i=tmp.rbegin();
				i!= tmp.rend(); i++)
			{
				toadd.push_back (*i);
			}
			push_front (toadd);
			tmp.clear();
		}
	}
	if (tmp.length() > 0)
	{
			VStr toadd;
			for (VStr::reverse_iterator i=tmp.rbegin();
				i!= tmp.rend(); i++)
			{
				toadd.push_back (*i);
			}
			push_front (toadd);
	}

}

void NameList::InsertName (VStr aname)
{
	push_back(aname);
}

void NameList::Serialize (Bofstream &outfile)const
{
	int mysize=size();
	outfile.WriteInt(mysize);
	myiterator itr=begin();
	for(int i=0;i<mysize;i++)
	{
		VStr currfoldername=*itr;
		outfile.WriteString(currfoldername);
		itr++;
	}
}

void NameList::Serialize (Bifstream &infile)
{
	int mysize;
	infile.ReadInt(mysize);
	for(int i=0;i<mysize;i++)
	{
		VStr currfoldername;		
		infile.ReadString(currfoldername);
		InsertName(currfoldername);
	}
}

bool NameList::operator == (const NameList &toCompare) const
{
	if (this==&toCompare)
	{
		return true;
	}
	
	if (size()!=toCompare.size())
	{
		return false;
	}

	myiterator myend=end();
	myiterator toCompareItr=toCompare.begin();
	
	for(myiterator i=begin();i!=myend;i++)
	{
		if (*i != *toCompareItr)
		{
			return false;
		}
		toCompareItr++;
	}
	return true;
}

bool NameList::IsInList(const VStr &myname)const
{
	myiterator myend=end();
	for (myiterator i=begin();i!=myend;i++)
	{
		if (*i==myname)
			return true;
	}
	return false;
}

void NameList::AddFrom(NameList &toAdd)
{
	myiterator hisend=toAdd.end();
	for(myiterator i=toAdd.begin();i!=hisend;i++)
	{
		if (IsInList(*i)==false)
			push_back(*i);
	}
}

VStr NameList::GetFolderPath() const
{
	VStr res;
	myiterator i = begin();
	myiterator e = end();
	for (i;i!=e;++i)
	{
		res+=*i;
		res+=("/");
	}
	return res;
}

bool NameList::IsExist (VStr txt) const
{
	std::list<VStr>::const_iterator fi = find (begin(),end(),txt);
	if (fi!=end())
		return true;
	else
		return false;
}

bool NameList::IsDontAddToTreeFolder()const
{
	std::list<VStr>::const_iterator i = begin();
	std::list<VStr>::const_iterator e = end();
	//VStr compStr1 (("DONTADDTOTREE"));
	//VStr compStr2 (("DONTADDTOTREE.igs"));
	for (i;i!=e;++i)
	{
		if ((*i).size() < 5)
			continue;
		if (
			(*i).operator[](0) == 'D' &&
			(*i).operator[](1) == 'O' &&
			(*i).operator[](2) == 'N' &&
			(*i).operator[](3) == 'T' &&
			(*i).operator[](4) == 'A' &&
			(*i).operator[](5) == 'D'
			)
		{
			return true;
		}
	}
	return false;
}
