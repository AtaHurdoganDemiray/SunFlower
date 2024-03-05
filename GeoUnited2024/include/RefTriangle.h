// RefTriangle.h

#ifndef __RefTriangle_h__
#define __RefTriangle_h__
#include "Bifstream.h"
#include "Bofstream.h"

struct RefTriangle
{
public:
	
	RefTriangle();
	RefTriangle ( const long &newref1 , const long &newref2 , const long &newref3 );
	RefTriangle (const int r[]);
	RefTriangle ( const RefTriangle &Foo );
	RefTriangle ( VIFStream & infile );
	RefTriangle & operator = ( const RefTriangle &Foo );
	void operator >> ( VofStream & outfile ) const;
	void operator << ( VIFStream & infile );
	bool operator == (const RefTriangle &toCompare)const;
	bool operator != (const RefTriangle &toCompare)const;	
	void Serialize (Bifstream &infile);
	void Serialize (Bofstream &outfile)const;
	int ref0,ref1,ref2;
};

#endif //__RefTriangle_h__
