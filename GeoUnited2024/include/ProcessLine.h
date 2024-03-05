/******************************************************************************
*               File: ProcessLine.h											  *
*******************************************************************************
*               Description:                                                  *
*                                                                             *
*******************************************************************************
*               History:                                                      *
*  27.11.2003 07:10:30 Created by:    Ata Hurdogan Demiray                    *
*******************************************************************************
*               (C) 2003 by Pozitif GmbH	                                  *
******************************************************************************/

#ifndef __ProcessLine_h__
#define __ProcessLine_h__

//#include <fstream>
//#include <vector>
#include "DoubleVector.h"

using namespace std;

const int PROCESSLINELENGTH = 85;

class ProcessLine
{
public:
	ProcessLine();
	~ProcessLine();
	
	VChr GetSection ();
	long GetDEPointer(int poffs);
	int GetLength ()const;
	void GetLine( VIFStream &theiges );
	void WriteLine (ofstream &theiges);
	void WriteIntAsEightChar (int mynum , ofstream &outfile,VChr fillc = ' ');

	void GetAllParamData ( DoubleVector & params );
	void GetAllParamData ( std::vector<VStr> & params );
	void LoadInParamData (const std::vector<string> &toLoad);
	void GetDeAndPeAndType ( long &myde , long &mype , long &mytype , int poffs);
	void GetDirectoryData ( DoubleVector & dirdata , int datano );
	void GetAllGlobalData ( std::vector<VStr> & params );
	VChr GetChar ( int index );
	int GetDirectoryEntry (DoubleVector &dirdata);
	const VChr * GetArray()const;
	VChr * GetArray();
	static VChr GetParamDelimiter ();
private:
	VChr theline[PROCESSLINELENGTH];
	static VChr paramdelimiter;
	static VChr recorddelimiter;
	bool datafilled;
};

typedef vector <ProcessLine *> FileLines;

#endif //__ProcessLine_h__
