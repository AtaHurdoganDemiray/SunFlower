// Test_IgesSerialize.h

#ifndef __Test_IgesSerialize_h__
#define __Test_IgesSerialize_h__
#include "DoubleVector.h"

class IgesEntity;

class Test_IgesSerialize
{
public:
	void Test_WriteEntityOut (IgesEntity &ent 
								, const DoubleVector &paramvector);
private:
	void Test_ConvertDoubleVectorToStringVector (const DoubleVector &DoubleVector
												,std::vector<VStr> &stringvector);

	void ShowError (const std::vector<VStr> &rightvector
							,const std::vector<VStr> &testvector);
	VStr ConvertvrealToString (const vreal &myscaler);
	vreal ConvertStringTovreal (const VStr &mystr);
	bool IsEqual_RightVector_TestVector (const std::vector<VStr> &rightvector
										,const std::vector <VStr> &testvector);
};

#endif
