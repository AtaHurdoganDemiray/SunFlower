// VTextLog.cpp

#include "stdafx.h"
#include "VTextLog.h"


#include <tchar.h>


VTextLog::VTextLog ( VofStream & _outfile )
:outfile (_outfile)
{

}

void VTextLog::AddTextToFile ( VStr  newtext )
{
	outfile << newtext << ( IDS_N);
	outfile<<("---------------------------------------------")<<( IDS_N);
}

void VTextLog::operator << ( VStr  newtext )
{
	AddTextToFile ( newtext );
}

