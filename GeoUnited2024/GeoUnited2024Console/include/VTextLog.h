// VTextLog.h

#ifndef __VTextLog_h__
#define __VTextLog_h__

class VTextLog
{
public:
	VTextLog ( VofStream & _outfile );
	void AddTextToFile ( VStr newtext );
	void operator << ( VStr  newtext );

private:
	VofStream & outfile;
};

#endif