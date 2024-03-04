// RemoteIfstream.h

#ifndef __RemoteIfstream_h__
#define __RemoteIfstream_h__

#include "Bifstream.h"

class RemoteIfstream
{
public:
	RemoteIfstream ( VStr filename );
	void Read ( vreal & val );
	void Read ( VChr val[] , int length );
	void Read ( int & val );
	void Read ( float & val );
	void Read ( bool & val );
	VStr ReadString ();
protected:
	Bifstream myfile;

};

#endif