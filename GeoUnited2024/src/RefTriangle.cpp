// RefTriangle.cpp
#include "stdafx.h"
#include "RefTriangle.h"


RefTriangle::RefTriangle()
{
	ref0 = -1;
	ref1 = -1;
	ref2 = -1;
};
//#############################################################################
RefTriangle::RefTriangle ( const long &newref1 , const long &newref2 , const long &newref3 )
{
	ref0 = newref1;
	ref1 = newref2;
	ref2 = newref3;
};
//#############################################################################
RefTriangle::RefTriangle (const int r[])
:ref0(r[0]),ref1(r[1]),ref2(r[2])
{
}
//#############################################################################
RefTriangle::RefTriangle ( const RefTriangle &Foo )
{
	(*this) = Foo;
};
//#############################################################################	
RefTriangle & RefTriangle::operator = ( const RefTriangle &Foo )
{
	ref0 = Foo.ref0;
	ref1 = Foo.ref1;
	ref2 = Foo.ref2;
	return (*this);
};
//#############################################################################

RefTriangle::RefTriangle ( VIFStream & infile )
{
	(*this)<<infile;
}

void RefTriangle::operator << ( VIFStream & infile )
{
	infile>>ref0;
	infile>>ref1;
	infile>>ref2;
}

void RefTriangle::operator >> ( VofStream & outfile ) const
{
	outfile<<ref0<<( IDS_N);
	outfile<<ref1<<( IDS_N);
	outfile<<ref2<<( IDS_N);
}

bool RefTriangle::operator == (const RefTriangle &toCompare)const
{
	if(this==&toCompare)
		return true;

	if (ref0!=toCompare.ref0)
		return false;
	if (ref1!=toCompare.ref1)
		return false;
	if (ref2!=toCompare.ref2)
		return false;
	
	return true;
}

bool RefTriangle::operator != (const RefTriangle &toCompare)const
{
	return !(this->operator ==(toCompare));
}

void RefTriangle::Serialize (Bifstream &infile)
{
	infile.read ((VChr *)&ref0 , sizeof(int));
	infile.read ((VChr *)&ref1 , sizeof(int));
	infile.read ((VChr *)&ref2 , sizeof(int));
}
void RefTriangle::Serialize (Bofstream &outfile)const
{
	outfile.write ((VChr *)&ref0 , sizeof(int));
	outfile.write ((VChr *)&ref1 , sizeof(int));
	outfile.write ((VChr *)&ref2 , sizeof(int));
}
