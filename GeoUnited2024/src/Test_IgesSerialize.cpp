// Test_IgesSerialize.cpp

#include "stdafx.h"
#include "Test_IgesSerialize.h"
#include "IgesEntity.h"
#include "Output.h"
#include <iomanip>
#include "BasicDefinitions.h"

void Test_IgesSerialize::Test_WriteEntityOut (IgesEntity &ent
											,const DoubleVector &paramvector)
{
	std::vector <VStr> rightvector;
	Test_ConvertDoubleVectorToStringVector (paramvector , rightvector);
	
	std::vector<VStr> testvector;
	ent.WriteEntityOut (testvector);
	Output myoutput;
	myoutput.ShowText (( IDS_IgeNum));
	myoutput.ShowScaler (ent.entitytype);
	myoutput.ShowText (( IDS_ComRigParVStVec));
	if ( IsEqual_RightVector_TestVector(rightvector , testvector) )
	{
		myoutput.ShowText (( IDS_Correct));
	}
	else
	{
		ShowError (rightvector , testvector);
	}
	myoutput.ShowText (("-----------------------------------------------------------------"));
}

void Test_IgesSerialize::Test_ConvertDoubleVectorToStringVector (const DoubleVector &DoubleVector 
																,std::vector<VStr> &stringvector) 
{
	for (size_t i=0;i<DoubleVector.size();i++)
	{
		vreal currVal=DoubleVector[i];
		VStr mystrin = ConvertvrealToString(currVal);
		stringvector.push_back (mystrin);
	}	
}

void Test_IgesSerialize::ShowError (const std::vector<VStr> &rightvector
									  ,const std::vector<VStr> &testvector)
{
	Output myoutput;
	myoutput.ShowText (( IDS_Wrong));
	myoutput.ShowText (( IDS_Rightvector));
	for (size_t i=0;i<rightvector.size();i++)
	{
		myoutput.ShowText (rightvector[i]);	
	}
	myoutput.ShowText (( IDS_Testvector));
	for (size_t j=0;j<testvector.size();j++)
	{
		myoutput.ShowText (testvector[j]);
	}
}

VStr Test_IgesSerialize::ConvertvrealToString (const vreal &myscaler)
{
	VOStrStream mystream;
	mystream /*<< std::setprecision(NUMBER_OF_DIGITS_AFTER_DECIMAL_POINT)*/ << myscaler;
	VStr result = mystream.str();
	return result;
}

vreal Test_IgesSerialize::ConvertStringTovreal (const VStr &mystr)
{
	VChr * errorAdress;
	vreal result = strtod (mystr.data() , &errorAdress);
	return result;
}

bool Test_IgesSerialize::IsEqual_RightVector_TestVector (const std::vector<VStr> &rightvector
														,const std::vector <VStr> &testvector)
{
	/*if (rightvector.size()!=testvector.size())
	{
		return false;
	}*/

	for (size_t i=0;i<testvector.size();i++)
	{
		VStr rs = rightvector[i];
		VStr ts = testvector[i];
		vreal rn = ConvertStringTovreal (rs);
		vreal tn = ConvertStringTovreal (ts);
		if (fabs (rn-tn)>1.e-2)
		{
			return false;
		}
	}
	return true;
}
