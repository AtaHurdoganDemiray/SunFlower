// RefLine.h

#ifndef __RefLine_h__
#define __RefLine_h__
#include "Bifstream.h"
#include "Bofstream.h"

class RefLine
{
public:
	
	void Serialize (Bifstream &infile);
	void Serialize (Bofstream &outfile)const;

	int c1,c2;
};

#endif