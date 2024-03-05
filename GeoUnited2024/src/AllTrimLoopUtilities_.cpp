// AllTrimLoopUtilities.cpp

#include "stdafx.h"
#include "AllTrimLoopUtilities.h"
#include "gmTrimLoopUtility.h"
#include "gmTrimLoop.h"

AllTrimLoopUtilities::~AllTrimLoopUtilities()
{
	for (int i=0; i<Size(); i++ )
	{
		delete (mdata[i]);
	}
}

//#############################################################################

long AllTrimLoopUtilities::Size() const
{
	return (mdata.size() );
}

//#############################################################################

gmTrimLoopUtility* AllTrimLoopUtilities::operator [] ( const long &i ) const
{
	return (mdata[i]);
}

//#############################################################################

gmTrimLoopUtility* AllTrimLoopUtilities::AddItem ( const UntrimmedSurface &parentsurf , bool triminside )
{
	gmTrimLoopUtility *pnew = new gmTrimLoopUtility (parentsurf , triminside);
	mdata.push_back ( pnew );
	return pnew;
}

//#############################################################################


