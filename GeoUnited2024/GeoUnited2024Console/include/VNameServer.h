// VNameServer.h

#ifndef __VNameServer_h__
#define __VNameServer_h__

class VNameServer
{
public:
	// eg :  C:\Viewpoint\afile.txt  will return  C:\Viewpoint\afileTMP.txt 
	static VStr MakeNonExistingTmpName(const VStr &fullPathFileName);
};

#endif