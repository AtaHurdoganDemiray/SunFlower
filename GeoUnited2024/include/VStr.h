// VStr.h

#ifndef __VStr_h__
#define __VStr_h__

#define __V_UNICODE__  
	#ifndef  __V_UNICODE__ 
		typedef wchar_t					VChr;
		typedef std::wstring			VStr;
		typedef std::wifstream		    VIFStream;
		typedef std::wofstream			VofStream;
		typedef std::wiostream			VIoStream;
		typedef std::wostringstream		VOStrStream;
		typedef std::wstringstream		VStrStream;
		typedef unsigned char			VUChr;
	#else
		typedef char					VChr;
		typedef std::string				VStr;
        typedef std::ifstream           VIFStream;
		typedef std::ofstream			VofStream;
		typedef std::iostream			VIoStream;
		typedef std::ostringstream		VOStrStream;
		typedef std::stringstream		VStrStream;
		typedef unsigned char			VUChr;
	#endif

class copyVofStream : public VofStream
{
public:
	copyVofStream (const VStr &fname);
	virtual ~copyVofStream();
private:
	VStr m_fname;
};

class copyVIFStream : public VIFStream 
{
public:
	copyVIFStream (const VStr &fname);
	virtual ~copyVIFStream ();
private:
	VStr m_fname;
};

#endif
