/******************************************************************************
*               File: IgesFileRead.h											  *
*******************************************************************************
*               Description:                                                  *
*                                                                             *
*******************************************************************************
*               History:                                                      *
*  27.11.2003 07:41:49 Created by:    Ata Hurdogan Demiray                    *
*******************************************************************************
*               (C) 2003 by Pozitif GmbH	                                  *
******************************************************************************/

#ifndef __IgesFileRead_h__
#define __IgesFileRead_h__

#include "IgesEntitySet.h"
#include "ProcessLine.h"

using namespace std;

//typedef vector<ProcessLine *> FileLines;
class GlobalSection;

class IgesFileRead
{
public:
	IgesFileRead ( VStr newfilename , IgesEntitySet &entset);
	~IgesFileRead();
	void CreateEntity ( FileLines &mylines , int poffs);
	void EmptyFileLines( FileLines &mylines );
	ProcessLine * GetRestOfTheParameters ( VIFStream &theiges , FileLines &mylines , int poffs);
	void FileRead (GlobalSection & myglobalsection);
	IgesEntitySet &allentities;

private:
	string filename;
	

};



#endif //__IgesFileRead_h__
