//Output.cpp

#include "stdafx.h"
#include "Output.h"
#include "Matrix.h"

Output::Output()
{

}

void Output::ShowScaler(vreal myscaler)
{
	VOStrStream mystream;
	mystream << myscaler;
	//VpAfxMessageBox (mystream.str().data());
	VofStream outfile (( IDS_CNumTxt),std::ios::app);
	outfile << mystream.str()<<( IDS_N);
}

void Output::ShowMatrix(Matrix &mymatrix)
{
	VofStream outfile (( IDS_CNumTxt),std::ios::app);
	for (int i=1;i<=mymatrix.GetRowNo();i++)
	{
		VOStrStream mystream;
		for (int j=1;j<=mymatrix.GetColNo();j++)
		{
			vreal currVal = mymatrix.GetElement (i,j);	
			mystream << currVal << ("  ");
		}
		outfile << mystream.str()<<( IDS_N);
	}
}
void Output::ShowInteger(int myinteger)
{
	VOStrStream mystream;
	mystream << myinteger;
	//VpAfxMessageBox (mystream.str().data());
	VofStream outfile (( IDS_CNumTxt),std::ios::app);
	outfile << mystream.str()<<( IDS_N);
}
void Output::ShowText (VStr mystr)
{
	VofStream outfile (( IDS_CNumTxt),std::ios::app);
	outfile << mystr << ( IDS_N);	
}
