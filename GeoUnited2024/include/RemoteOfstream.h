// RemoteOfstream.h

#ifndef __RemoteOfstream_h__
#define __RemoteOfstream_h__

#include "Bofstream.h"

class RemoteOfstream
{
public:
	RemoteOfstream ( VStr filename );
	void Write ( const vreal & val );
	void Write ( const bool & val );
	void Write ( const VChr val[] , int length );
	void Write ( const VStr val );
	void Write ( const int & val );
	void Write ( const float & val );
	
protected:
	Bofstream myfile;
};

#endif
