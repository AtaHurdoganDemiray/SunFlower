// Serialized.h

#ifndef __Serialized_h__
#define __Serialized_h__

#include "Bifstream.h"
#include "Bofstream.h"
#include "VectorSerializer.h"


class Serialized
{
public:
	Serialized ();
	Serialized (const Serialized &toCopy);
	Serialized & operator = (const Serialized &toCopy);
	virtual void Serialize (Bifstream &infile)=0;
	virtual void Serialize (Bofstream &outfile)const=0;
	void WriteVersion (Bofstream &outfile,const int &version)const;
	void ReadVersion(Bifstream &infile,int &version);
	
protected:
	int m_version;
};

#endif