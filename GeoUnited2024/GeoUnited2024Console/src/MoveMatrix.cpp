// MoveMatrix.cpp

#include "stdafx.h"
#include "MoveMatrix.h"
#include "VPoint.h"


MoveMatrix::MoveMatrix()
:TransformationMatrix(),movevector (0.)
{

}
MoveMatrix::MoveMatrix ( const VPoint & _movevector )
: TransformationMatrix(),movevector ( _movevector )
{
	SetRow ( 1 , 1.0 , 0.0 , 0.0 , movevector.x() );
	SetRow ( 2 , 0.0 , 1.0 , 0.0 , movevector.y() );
	SetRow ( 3 , 0.0 , 0.0 , 1.0 , movevector.z() );
	SetRow ( 4 , 0.0 , 0.0 , 0.0 , 1.0 );
}
MoveMatrix::MoveMatrix (const MoveMatrix &toCopy)
:TransformationMatrix(toCopy),movevector(toCopy.movevector)
{

}

MoveMatrix::~MoveMatrix()
{
	
}
MoveMatrix & MoveMatrix::operator = ( const MoveMatrix & Foo )
{
	movevector = Foo.movevector;	
	TransformationMatrix::operator = ( Foo );
	return (*this);
}
void MoveMatrix::Serialize(Bofstream &outfile)const
{
	TransformationMatrix::Serialize(outfile);	
	movevector.Serialize(outfile);
}
void MoveMatrix::Serialize (Bifstream &infile)
{
	TransformationMatrix::Serialize(infile);	
	movevector.Serialize(infile);
}
VStr MoveMatrix::GetInformationText()const
{
	VStr info;
	CString tmp1;
	tmp1.Format("X%.4f Y%.4f Z%.4f",movevector.x(),movevector.y(),movevector.z());
	info += sozluk("Move");
	info += "(";
	info += (LPCTSTR)tmp1;
	info += ")";
	return info;
}