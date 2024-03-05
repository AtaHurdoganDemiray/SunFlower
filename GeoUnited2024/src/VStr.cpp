// VStr.cpp

#include "stdafx.h"

copyVofStream::copyVofStream (const VStr &fname)
:VofStream (fname+( IDS_VpSafeCopy)) , m_fname(fname)
{

}

copyVofStream::~copyVofStream ()
{
	if (this->is_open())
		this->close();
	BOOL ck1 = DeleteFile (m_fname.c_str());
	VStr copyName = m_fname+( IDS_VpSafeCopy);
	BOOL ck2 = CopyFile (copyName.c_str() , m_fname.c_str(),TRUE);
	if (ck1 == FALSE)
	{
		LogFileWriter::WriteUnknownExceptionLog (( IDS_RemOriFilFai),( IDS_CopDes));
	}
	if (ck2 == FALSE)
	{
		LogFileWriter::WriteUnknownExceptionLog (( IDS_CopyFailed),( IDS_CopDes));
	}
	if (ck1 == TRUE && ck2 == TRUE)
	{
		BOOL ck3 = DeleteFile (copyName.c_str());
		if (ck3 == FALSE)
			LogFileWriter::WriteUnknownExceptionLog (( IDS_DelCopFilFai),( IDS_CopDes));
	}
}

copyVIFStream::copyVIFStream (const VStr &fname)
: m_fname (fname)
{
	VStr copyName = m_fname + ( IDS_VpSafeCopy);
	BOOL ck1 = CopyFile ( m_fname.c_str() , copyName.c_str() , FALSE);
	if ( ck1 == FALSE ) 
		LogFileWriter::WriteUnknownExceptionLog ( ( IDS_CopyFailed),( IDS_CopCon));
	else
		this->open ( copyName );
	if ( this->is_open () == false)
	{
		LogFileWriter::WriteUnknownExceptionLog ( ( IDS_OpeCopFilFai) , ( IDS_CopCon));
	}
}

copyVIFStream::~copyVIFStream ()
{
	VStr copyName = m_fname + ( IDS_VpSafeCopy);
	if (this->is_open())
		this->close();
	BOOL ck = DeleteFile (copyName.c_str());
	if (ck == FALSE)
		LogFileWriter::WriteUnknownExceptionLog (( IDS_DelCopFilFai),( IDS_CopDes1));
}
