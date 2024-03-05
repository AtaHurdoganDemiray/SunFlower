/******************************************************************************
*               File: ProcessLine.cpp											  *
*******************************************************************************
*               Description:                                                  *
*                                                                             *
*******************************************************************************
*               History:                                                      *
*  27.11.2003 22:45:25 Created by:    Ata Hurdogan Demiray                    *
*******************************************************************************
*               (C) 2003 by Pozitif GmbH	                                 *
******************************************************************************/

#include "stdafx.h"
#include "stdlib.h"
#include "ProcessLine.h"

#include <iomanip>

VChr ProcessLine::paramdelimiter=',';
VChr ProcessLine::recorddelimiter=';';

ProcessLine::ProcessLine()
{
	datafilled = false;
	
	for (int j=0;j<PROCESSLINELENGTH-1;j++)
	{
		theline[j] = ' ';//29;
	}
	theline[PROCESSLINELENGTH-1] = '\0';

	//paramdelimiter = ',';
}

//#############################################################################

ProcessLine::~ProcessLine()
{

}

//#############################################################################

void ProcessLine::GetLine ( VIFStream &theiges )
{
	int maxl = 85;
	theiges.getline ( theline , maxl , '\n' );
	datafilled = true;
}

//#############################################################################

int ProcessLine::GetLength ()const
{
	for (int j=0; j<PROCESSLINELENGTH; j++)
	{
		if (theline[j] == 29)
			return (j+1);
	}
	return 0;
}

void ProcessLine::WriteLine ( ofstream &theiges )
{
	//int linelength = GetLength();
	//theiges.write (theline , linelength);
}

void ProcessLine::WriteIntAsEightChar (int mynum , ofstream &outfile,VChr fillc)
{
	ostringstream str;
	str << setfill (fillc);
	str << setw(8);
	str << mynum;
	string s (str.str());
	outfile << s.c_str();
}

VChr ProcessLine::GetSection ()
{
	if ( !datafilled )
		return 0; // exception!

	return theline[72];

}

//#############################################################################

long ProcessLine::GetDEPointer (int poffs)
{
	if ( !datafilled )
		return 0; // exception!

	// valid only for parameter data part.
	VChr temp[9];
	int set = 0;

	for ( int i = 0; i<9; i++ )
	{
		if ( theline[71-i] != ' ' && theline[71-i] != ',' )
		{
			temp[set] = theline[71-i];
			set++;
		}
		else 
			break;
	}
	temp[set] = 0;

	// swap the number
	VChr mynum[10];
	for ( int i=0; i < set; i++ )
		mynum[i] = temp[set - (i+1)];

	mynum[set] = 0;

	VChr * stp;
	long depoint = (strtol ( mynum , &stp , 10 )) + poffs;

	return depoint;

}

//#############################################################################

void ProcessLine::GetAllParamData ( DoubleVector & params )
{
	VChr temp[85];
	VChr *stp;
	int set = 0;

	for ( int i = 0; i < 64; i++ )
	{
	/*	if (theline[i] == recorddelimiter)
		{
			break;
		}*/
		if (theline[i] != paramdelimiter && theline[i] != recorddelimiter)
		{
			if (theline[i] == 'D' || theline[i] == 'd')
				theline[i] = 'E';
			temp[set] = theline[i];
			set++;
		}
		else 
		{
			temp[set] = 0;
			vreal mydata = strtod(temp, &stp);
			params.push_back ( mydata );
			set = 0;
		}
	}
}

//#############################################################################

void ProcessLine::GetAllParamData ( std::vector<VStr> & params )
{
	VChr *temp = VGlobalValues::VCHR_LINE_READER;
	VChr *stp;
	int set = 0;

	for ( int i = 0; i < 64; i++ )
	{
	/*	if (theline[i] == recorddelimiter)
		{
			break;
		}*/
		if (theline[i] != paramdelimiter && theline[i] != recorddelimiter)
		{
			temp[set] = theline[i];
			set++;
		}
		else 
		{
			temp[set] = 0;
			//vreal mydata = strtod ( temp , &stp );
			params.push_back ( temp );
			set = 0;
		}
	}
}

void ProcessLine::LoadInParamData (const std::vector<string> &toLoad)
{
	int mylength = 0;
	std::vector<string>::const_iterator i;
	for (i=toLoad.begin();i!=toLoad.end();i++)
	{
		mylength += (*i).length();
		if (mylength > PROCESSLINELENGTH)
		{
			throw VException (( IDS_ProLinExc),1);
		}
		for (int j=0;j<(*i).length();j++)
		{
			theline[j] = ( (*i).data() )[j];
		}
		theline[mylength] = paramdelimiter;
	}
	theline[mylength]=29;
}

void ProcessLine::GetDeAndPeAndType ( long &myde , long &mype , long &mytype , int poffs)
{
	if (!datafilled )
		return; // exception!

	VChr temp[10];
	temp[8] = 0;
	VChr *stp;
	int offset = 0;

	int i;
	for ( i=0; i < 8; i++)
	{
		temp[i] = theline[i+offset];
	}
	
	mytype = strtol ( temp , &stp , 10 );

	offset = 8;
	for (i=0; i < 8; i++)
	{
		temp[i] = theline[i+offset];
	}
	
	mype = strtol ( temp , &stp , 10 );

	offset = 73;
	for (i=0; i < 8; i++)
	{
		temp[i] = theline[i+offset];
	}
	
	myde = (strtol ( temp , &stp , 10 )) + poffs;
	
	return;
}

//#############################################################################

void ProcessLine::GetDirectoryData ( DoubleVector & dirdata , int datano )
{
	VChr * stp;

	bool cont = true;
	int count = 0;
	int no=0;

	while ( cont )
	{
		if ( theline[count] == ' ' )
		{
			count++;
			continue;
		}
		else 
		{
			bool cont2=true;
			int i = 0;
			VChr temp[10]={' ',' ',' ',' ',' ',' ',' ',' ',' ',' '};
			while (cont2)
			{
				temp[i] = theline[count];
				count++;
				i++;
				if ( theline[count] == ' ' )
					cont2 = false;
			}
			vreal mydata = strtod ( temp , &stp );
			dirdata.push_back ( mydata );
			no++;
		}
		if ( no == datano )
			cont = false;
	}
}

VChr  ProcessLine::GetChar ( int index )
{
	if ( index < 0 || index > 79 )
		throw (VException (( IDS_ProExc) ,0));

	return theline[index];
}

int ProcessLine::GetDirectoryEntry(DoubleVector &dirdata)
{
	int i,j;
	VChr myVChrs[10]={'\0','\0','\0','\0','\0','\0','\0','\0'};
	
	bool stoploop=false;
	for (i=0;i<10;i++)
	{
		bool emptypar=true;
		for (j=0;j<8;j++)
		{
			int myindex=i*8+j;
			VChr currVChr=theline[myindex];
			if (currVChr=='\n')
				stoploop=true;
			if (currVChr!=' ')
				emptypar=false;
			if (currVChr=='D')
				currVChr=' ';
			myVChrs[j]=currVChr;
		}

		VChr * stp;
		long tmp;
		if (emptypar)
			tmp=0;
		else
			tmp = strtol (myVChrs,&stp,10);
		dirdata.push_back((vreal)(tmp));
		
		if(stoploop)
			break;
	}
	return i;
}

const VChr * ProcessLine::GetArray()const
{
	return theline;
}

VChr * ProcessLine::GetArray()
{
	return theline;
}

VChr ProcessLine::GetParamDelimiter()
{
	return paramdelimiter;
}
