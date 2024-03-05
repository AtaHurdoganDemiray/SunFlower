// Serializer.cpp: implementation of the Serializer class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "Serializer.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

coreSerializer::coreSerializer()
{

}

coreSerializer::~coreSerializer()
{

}

bool coreSerializer::IsLower ( VChr letter ) const
{
	if ( letter > 96 )
		return true;
	else
		return false;
}

bool coreSerializer::IsUpper ( VChr letter ) const
{
	if ( letter < 97 )
		return true;
	else 
		return false;
}

VStr coreSerializer::MakeUpper ( const VStr &source ) const
{
	VChr temp;
	VStr result;

	for ( int i = 0; i < source.length(); i++ )
	{
		if ( IsLower ( source[i] ) )
			temp = source[i] - 32;
		else
			temp = source[i];

		VStr tmp ( &temp );
		result += temp;
	}

	return result;
}

VStr coreSerializer::MakeLower ( const VStr &source ) const
{
	VChr temp;
	VStr result;

	for ( int i = 0; i < source.length(); i++ )
	{
		if ( IsUpper ( source[i] ) )
			temp = temp + 32;
		else
			temp = source[i];

		VStr tmp ( &temp );
		result += tmp;
	}

	return result;
}

