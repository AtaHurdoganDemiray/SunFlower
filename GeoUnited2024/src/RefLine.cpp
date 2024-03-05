// RefLine.cpp

#include "stdafx.h"
#include "RefLine.h"

void RefLine::Serialize (Bifstream &infile)
{
	infile.read ((VChr*)&c1,sizeof(int));
	infile.read ((VChr*)&c2,sizeof(int));
}

void RefLine::Serialize(Bofstream &outfile) const
{
	outfile.write ((VChr*)&c1,sizeof(int));
	outfile.write ((VChr*)&c2,sizeof(int));
}

