// RemoteIfstream.cpp

#include "stdafx.h"
#include "RemoteIfstream.h"

RemoteIfstream::RemoteIfstream ( VStr filename )
:myfile (filename)
{

}

void RemoteIfstream::Read ( bool & val )
{
	myfile.read ( (VChr *) (&val) , sizeof (bool) );
}

void RemoteIfstream::Read ( int & val )
{
	myfile.read ( (VChr *) (&val) , sizeof (int) );
}

void RemoteIfstream::Read ( float & val )
{
	myfile.read ( (VChr *) (&val) , sizeof (float) );
}

void RemoteIfstream::Read ( vreal & val )
{
	myfile.read ( (VChr *) (&val) , sizeof (vreal) );
}

void RemoteIfstream::Read ( VChr val[] , int length )
{
	for ( int i=0; i<length; i++ )
	{
		myfile.read ( &(val[i]) , sizeof (VChr) );
	}
}

VStr RemoteIfstream::ReadString ()
{
	int length;
	Read ( length );
	VStr result;

	for ( int i=0; i<length; i++ )
	{
		VChr tmp;
		myfile.read (&tmp,sizeof(VChr));
		result += tmp;
	}
	return result;
}
