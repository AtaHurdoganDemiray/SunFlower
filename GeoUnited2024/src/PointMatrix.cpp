// PointMatrix.cpp

#include "stdafx.h"
#include "PointMatrix.h"
#include "TransformationMatrix.h"
#include "VPoint.h"


PointMatrix::PointMatrix ()
: pm ( 1 , 4 )
{

}

//#############################################################################

PointMatrix::PointMatrix ( const PointMatrix & Foo )
: pm ( Foo.pm )
{

}

//#############################################################################

PointMatrix::PointMatrix ( const Matrix & newpm )
: pm ( newpm )
{
	if ( newpm.GetRowNo() != 1 || newpm.GetColNo() != 4 )
		throw ( VException ( ( IDS_PoiMatExc) , 1 ) );
}

//#############################################################################

PointMatrix::PointMatrix ( const VPoint &newpoint )
: pm ( 1 , 4 )
{
	pm.SetElement ( newpoint.x() , 1 , 1 );
	pm.SetElement ( newpoint.y() , 1 , 2 );
	pm.SetElement ( newpoint.z() , 1 , 3 );
	pm.SetElement ( 1.0 , 1 , 4 );
}

//#############################################################################

const vreal & PointMatrix::GetElement ( int row , int col )const
{
	return ( pm.GetElement ( row , col ) );
}

//#############################################################################

PointMatrix PointMatrix::operator * ( const TransformationMatrix & secondmatrix ) const
{
	return ( PointMatrix ( pm * (Matrix &)secondmatrix ) );
}

//#############################################################################

const PointMatrix & PointMatrix::operator = ( const PointMatrix & Foo )
{
	pm.SetElement ( Foo.GetElement ( 1,1 ) , 1 , 1 );
	pm.SetElement ( Foo.GetElement ( 1,2 ) , 1 , 2 );	
	pm.SetElement ( Foo.GetElement ( 1,3 ) , 1 , 3 );
	pm.SetElement ( Foo.GetElement ( 1,4 ) , 1 , 4 );
	return (*this);
}

//#############################################################################

void PointMatrix::Serialize(Bofstream &outfile)const
{
	pm.Serialize(outfile);	
}

//#############################################################################

void PointMatrix::Serialize(Bifstream &infile)
{
	pm.Serialize(infile);
}

//#############################################################################
