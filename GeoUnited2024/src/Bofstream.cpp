// Bofstream.cpp

#include "stdafx.h"
#include "Bofstream.h"

//Bofstream::Bofstream()
//{
//
//}

Bofstream::Bofstream ( VStr filename )
: VofStream ( filename.data() , std::ios::binary ),m_filename(filename)
{

}

void Bofstream::WriteString (const VStr &mystring)
{	
	int mylength=(int)mystring.length();
	write ((VChr*)(&mylength),sizeof(int)); 
	for(int i=0;i<mylength;i++)
	{
		VChr currVChr=mystring[i];
		write((VChr*)(&currVChr),sizeof(VChr));
	}	
}

void Bofstream::WriteDouble (const double towrite)
{
	write ((VChr *)&towrite , sizeof (double));
}	

void Bofstream::WriteInt (const int towrite)
{
	write ((VChr *)&towrite , sizeof (int));
}
void Bofstream::WriteUnsInt (const unsigned int towrite)
{
	write ((VChr *)&towrite , sizeof (unsigned int));
}
void Bofstream::WriteFloat (const float towrite)
{
	write ((VChr *)&towrite , sizeof (float));
}

void Bofstream::WriteBool (const bool towrite)
{
	write ((VChr *)&towrite , sizeof (bool));
}

void Bofstream::WriteTime(const VTime &towrite)
{
	write ((VChr *)&towrite , sizeof (VTime));
}


void Bofstream::WriteVoidPtr ( const void * const * towrite)
{
	write ((VChr *)towrite , sizeof (void *));
}

void Bofstream::WriteSizeT (const size_t towrite)
{
	write ((VChr *)&towrite , sizeof (size_t));
}

bool Bofstream::CkVer (int softwareVersion , int fileVersion)
{
	if (softwareVersion >= fileVersion)
		return true;
	else
	{
		LogFileWriter::WriteUnknownExceptionLog (( IDS_SofVerIsOldThaF));
		return false;
	}
}

void Bofstream::WriteChar(const char towrite)
{
	write((VChr*)&towrite, sizeof(char));
}
