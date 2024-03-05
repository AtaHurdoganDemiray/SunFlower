// Serialized.cpp
#include "stdafx.h"
#include "Serialized.h"
	
Serialized::Serialized ()
{
}
Serialized::Serialized (const Serialized &toCopy)
{
}
Serialized & Serialized::operator = (const Serialized &toCopy)
{
	return *this;
}
void Serialized::WriteVersion (Bofstream &outfile,const int &version)const
{
	outfile.write((VChr*)version,sizeof(int));
}
void Serialized::ReadVersion(Bifstream &infile,int &version)
{
	infile.read ((VChr*)&version,sizeof(int));
}
