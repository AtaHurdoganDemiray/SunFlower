// RemoteOfstream.cpp

#include "stdafx.h"
#include "RemoteOfstream.h"

RemoteOfstream::RemoteOfstream (VStr filename )
:myfile (filename)
{

}

void RemoteOfstream::Write ( const bool & val )
{
	myfile.write ( (VChr *) (&val) , sizeof (bool) );
}

void RemoteOfstream::Write ( const int & val )
{
	myfile.write ( (VChr *) (&val) , sizeof (int) );
}

void RemoteOfstream::Write ( const float & val )
{
	myfile.write ( (VChr *) (&val) , sizeof (float) );
}

void RemoteOfstream::Write ( const vreal & val )
{
	myfile.write ( (VChr *) (&val) , sizeof (vreal) );
}

void RemoteOfstream::Write ( const VChr val[] , int length )
{
	for ( int i=0; i < length; i++ )
	{
		myfile.write ( &(val[i]) , sizeof (VChr) );
	}
}

void RemoteOfstream::Write ( const VStr val )
{
	int length = val.length();
	Write ( length );
	Write ( val.data() , length );
}
