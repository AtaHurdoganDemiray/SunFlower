// Bofstream.h

#ifndef __Bofstream_h__
#define __Bofstream_h__



class Bofstream : public VofStream
{
public:
	//Bofstream ();
	Bofstream ( VStr filename );
	void WriteString (const VStr &mystring);
	void WriteDouble (const double towrite);
	void WriteInt (const int towrite);
	void WriteUnsInt (const unsigned int towrite);
	void WriteFloat (const float towrite);
	void WriteBool (const bool towrite);
    void WriteTime (const VTime &towrite);

	void WriteVoidPtr ( const void * const * towrite);
	void WriteSizeT (const size_t towrite);
	VStr m_filename;
	static bool CkVer (int softwareVersion,int fileVersion);
	void WriteChar(const char c);
};

#endif
