// Bifstream.h

#ifndef __Bifstream_h__
#define __Bifstream_h__

class Bifstream : public VIFStream
{
public:
	//Bifstream (); 
	Bifstream ( VStr filename );
	void ReadString (VStr &mystring);
	void ReadDouble (double &val);
	void ReadInt (int &val);
	void ReadUnsInt (unsigned int &val);
	void ReadFloat (float &val);
	void ReadBool (bool &val);

    void ReadTime (VTime &val);

	void ReadVoidPtr (void * *val);
	void ReadSizeT (size_t &val);
	void ReadChar(char& val);

	VStr m_filename;
};

#endif
