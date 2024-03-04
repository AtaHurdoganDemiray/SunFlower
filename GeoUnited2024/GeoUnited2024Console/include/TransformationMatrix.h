// TransformationMatrix.h

#ifndef __TranformationMatrix_h__
#define __TranformationMatrix_h__

#include "Matrix.h"
#include "sVector.h"
class igesTransformationMatrix;


struct VPoint;
struct VPoint2d;
struct CoordinateSystem;
struct VTriangleE;
struct VLine;
struct OglMatrix;

class TransformationMatrix:public Matrix
{
public:
	friend class PointMatrix;
	//friend TransformationMatrix operator *(const TransformationMatrix &firstmatrix , const TransformationMatrix &secondmatrix);
	TransformationMatrix();
	TransformationMatrix ( const Matrix & newmatrix );
	TransformationMatrix ( const TransformationMatrix & Foo );
	TransformationMatrix ( const igesTransformationMatrix & igesm );
	TransformationMatrix ( const OglMatrix &oglmat);
	TransformationMatrix &operator = (const TransformationMatrix &Foo);
	virtual ~TransformationMatrix();
	virtual TransformationMatrix operator * (const TransformationMatrix & secondmatrix)const;
	void SetRow ( int row , const vreal &val1 , const vreal &val2 , const vreal &val3 , const vreal &val4 );
	void SetCol ( int col , const vreal &val1 , const vreal &val2 , const vreal &val3 , const vreal &val4 );
	VPoint Transform ( const VPoint & source ) const;
	VPoint TransformVec (const VPoint &vec)const;
	VPoint2d Transform (const VPoint2d &source)const;
	CoordinateSystem Transform ( const CoordinateSystem & cs ) const;	
	VTriangleE Transform ( const VTriangleE & tri )const;
	VLine Transform (const VLine &source)const;
	sVector<double,2> Transform (const sVector<double,2> &source)const;
	sVector<double,3> Transform (const sVector<double,3> &source)const;
	void TransposeThis ();
	void Serialize (Bofstream &outfile)const;
	void Serialize (Bifstream &infile);
	bool operator == (const TransformationMatrix &toCompare)const;
	bool operator != (const TransformationMatrix &toCompare)const;
	TransformationMatrix GetInverse()const;
	igesTransformationMatrix GetIgesTransformation()const;
	void SetIgesTransformation (igesTransformationMatrix &X)const;
	TransformationMatrix & operator *= (const TransformationMatrix &tm);
	bool Compare (const TransformationMatrix &tm,const vreal tol)const;//returns true if given tm is equal to this within tolerance
	void Transform(std::vector<VPoint> &pvec)const;
	virtual VStr GetInformationText()const;
protected:
	//Matrix tm;
};

//inline TransformationMatrix operator *(const TransformationMatrix &firstmatrix , const TransformationMatrix &secondmatrix)
//{
//	Matrix mat = firstmatrix * secondmatrix;
//	TransformationMatrix result ( mat );
//	return result;
//}

#endif //__TranformationMatrix_h__
