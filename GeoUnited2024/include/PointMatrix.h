// PointMatrix.h

#ifndef __PointMatrix_h__
#define __PointMatrix_h__

#include "Matrix.h"
class TransformationMatrix;


struct VPoint;

class PointMatrix
{
public:
	PointMatrix ();
	PointMatrix ( const PointMatrix &Foo );
	PointMatrix ( const Matrix & newpm );
	PointMatrix ( const VPoint &newpoint );
	const PointMatrix & operator = ( const PointMatrix & Foo );
	const vreal & GetElement ( int row , int col ) const;
	PointMatrix operator * ( const TransformationMatrix & secondmatrix ) const;
	void Serialize (Bofstream &outfile)const;
	void Serialize (Bifstream &infile);
protected:
	Matrix pm;
};

#endif //__PointMatrix_h__
