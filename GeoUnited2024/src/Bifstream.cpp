// Bifstream.cpp

#include "stdafx.h"
#include "Bifstream.h"

#undef VBIFSTREAMDEBUGMODE 
int VBIFSTREAMSTRINGSIZEWARNINGLIMIT = 1000;
vreal VBIFSTREAMSCALARWARNINGLIMIT = 1.e6;
int VBIFSTREAMINTEGERWARNINGLIMIT = 1000000;
unsigned int VBIFSTREAMUNSIGNEDINTEGERWARNINGLIMIT = 1000000;
float VBIFSTREAMFLOATWARNINGLIMIT = 1.e6f;
size_t VBIFSTREAMSIZETWARNINGLIMIT = 10000;
//Bifstream::Bifstream()
//{
//
//}

Bifstream::Bifstream ( VStr filename )
: VIFStream ( filename.data() , std::ios::binary ),m_filename(filename)
{
}

void Bifstream::ReadString (VStr &mystring)
{ 
	mystring.clear();
	int mylength;    
	read ((VChr*)(&mylength),sizeof(int)); 
#ifdef VBIFSTREAMDEBUGMODE
	if (mylength < 0 || mylength > VBIFSTREAMSTRINGSIZEWARNINGLIMIT)
	{
		CString warningStr;
		warningStr.Format("%s string size is out of warning limits", m_filename.c_str());
		LogFileWriter::WriteUnknownExceptionLog("Bifstream warning! ", (LPCTSTR)warningStr);
	}
#endif
	for(int i=0;i<mylength;i++)
	{
		VChr currVChr;
		read((VChr*)(&currVChr),sizeof(VChr));
		mystring+=currVChr;		
	}	
}

void Bifstream::ReadDouble (double &val)
{
	read ((VChr *)&val , sizeof (double));
#ifdef VBIFSTREAMDEBUGMODE
	if (val < -VBIFSTREAMSCALARWARNINGLIMIT || val > VBIFSTREAMSCALARWARNINGLIMIT)
	{
		CString warningStr;
		warningStr.Format("%s double value is out of warning limits", m_filename.c_str());
		LogFileWriter::WriteUnknownExceptionLog("Bifstream warning! ", (LPCTSTR)warningStr);
	}
#endif
}

void Bifstream::ReadInt (int &val)
{
	read ((VChr *)&val , sizeof (int));
#ifdef VBIFSTREAMDEBUGMODE
	if (val < -VBIFSTREAMINTEGERWARNINGLIMIT || val > VBIFSTREAMINTEGERWARNINGLIMIT)
	{
		CString warningStr;
		warningStr.Format("%s integer value is out of warning limits", m_filename.c_str());
		LogFileWriter::WriteUnknownExceptionLog("Bifstream warning! ", (LPCTSTR)warningStr);
	}
#endif
}
void Bifstream::ReadUnsInt (unsigned int &val)
{
	read ((VChr *)&val , sizeof (unsigned int));
	vreal VBIFSTREAMSCALARWARNINGLIMIT = 1.e6;
#ifdef VBIFSTREAMDEBUGMODE
	if (val > VBIFSTREAMUNSIGNEDINTEGERWARNINGLIMIT)
	{
		CString warningStr;
		warningStr.Format("%s unsigned integer value is out of warning limits", m_filename.c_str());
		LogFileWriter::WriteUnknownExceptionLog("Bifstream warning! ", (LPCTSTR)warningStr);
	}
#endif
}
void Bifstream::ReadFloat (float &val)
{
	read ((VChr *)&val , sizeof(float));
#ifdef VBIFSTREAMDEBUGMODE
	if (val < -VBIFSTREAMFLOATWARNINGLIMIT || val > VBIFSTREAMFLOATWARNINGLIMIT)
	{
		CString warningStr;
		warningStr.Format("%s double value is out of warning limits", m_filename.c_str());
		LogFileWriter::WriteUnknownExceptionLog("Bifstream warning! ", (LPCTSTR)warningStr);
	}
#endif
}

void Bifstream::ReadBool(bool &val)
{
	read ((VChr *)&val , sizeof(bool));
}

void Bifstream::ReadTime (VTime &val)
{
    read ((VChr *)&val , sizeof (VTime));
}

void Bifstream::ReadVoidPtr (void * *val)
{
	read ((VChr *)val,sizeof (void *));
}

void Bifstream::ReadSizeT (size_t &val)
{
	read ((VChr *)&val,sizeof (size_t));
#ifdef VBIFSTREAMDEBUGMODE
	if (val > VBIFSTREAMSIZETWARNINGLIMIT)
	{
		CString warningStr;
		warningStr.Format("%s size_t value is out of warning limits", m_filename.c_str());
		LogFileWriter::WriteUnknownExceptionLog("Bifstream warning! ", (LPCTSTR)warningStr);
	}
#endif
}
void Bifstream::ReadChar(char& val)
{
	read((VChr*)&val, sizeof(char));
#ifdef VBIFSTREAMDEBUGMODE
	if (val < -127 || val > 127)
	{
		CString warningStr;
		warningStr.Format("%s char value is out of warning limits", m_filename.c_str());
		LogFileWriter::WriteUnknownExceptionLog("Bifstream warning! ", (LPCTSTR)warningStr);
	}
#endif
}