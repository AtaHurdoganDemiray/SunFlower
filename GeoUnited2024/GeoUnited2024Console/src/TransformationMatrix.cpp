// TransformationMatrix.cpp

#include "stdafx.h"
#include "TransformationMatrix.h"

#include "igesTransformationMatrix.h"
#include "VPoint.h"
#include "CoordinateSystem.h"
#include "VTriangleE.h"
#include "VLine.h"
#include "OglMatrix.h"
#include "GaussJordanElimination.h"
#include "GaussElemination.h"
#include "VPoint2d.h"

TransformationMatrix::TransformationMatrix ()
: Matrix(4,4)
{
	// Identity Transformation Matrix
	SetRow(1 ,1.0 , 0.0 , 0.0 , 0.0);
	SetRow(2 ,0.0 , 1.0 , 0.0 , 0.0);
	SetRow(3 ,0.0 , 0.0 , 1.0 , 0.0);
	SetRow(4 ,0.0 , 0.0 , 0.0 , 1.0);
}

TransformationMatrix::TransformationMatrix ( const Matrix &newmatrix )
:Matrix(newmatrix)
{
	if ( newmatrix.GetRowNo() != 4 || newmatrix.GetColNo() != 4 )
		throw ( VException ( ( IDS_TraMatExc) , 1 ) );
}

TransformationMatrix::TransformationMatrix ( const TransformationMatrix &Foo )
:Matrix(Foo)
{
}

TransformationMatrix::TransformationMatrix ( const igesTransformationMatrix & im )
:Matrix(4,4)
{
	vreal arr[16] = { im.mR11,im.mR12,im.mR13,im.mT1
					,  im.mR21,im.mR22,im.mR23,im.mT2
					,  im.mR31,im.mR32,im.mR33,im.mT3 
					,  0	,0	,0	,1 };

	int row=4,col=4;
	Matrix m(row,col,arr);
	Matrix::operator=(m);
}

TransformationMatrix::~TransformationMatrix()
{

}

TransformationMatrix TransformationMatrix::operator *(const TransformationMatrix &secondmatrix)const
{
	return Matrix::operator *(secondmatrix);
}

void TransformationMatrix::SetRow ( int row , const vreal &val1 , const vreal &val2 , const vreal &val3 , const vreal &val4 )
{
	SetElement ( val1 , row , 1 );
	SetElement ( val2 , row , 2 );
	SetElement ( val3 , row , 3 );
	SetElement ( val4 , row , 4 );
}

void TransformationMatrix::SetCol ( int col , const vreal &val1 , const vreal &val2 , const vreal &val3 , const vreal &val4 )
{
	SetElement ( val1 , 1 , col );
	SetElement ( val2 , 2 , col );
	SetElement ( val3 , 3 , col );
	SetElement ( val4 , 4 , col );
}

TransformationMatrix &TransformationMatrix::operator = ( const TransformationMatrix &Foo )
{
	for ( int row = 1; row <= 4; row++ )
		for ( int col = 1; col <=4; col++ )
			SetElement ( Foo.GetElement ( row , col ) , row , col );
	
	return (*this);
}
	
VPoint TransformationMatrix::Transform ( const VPoint & source ) const
{
	vreal matarr[4] = { source.x() , source.y() , source.z() , 1.0 };
	Matrix p ( 4 , 1 , matarr );
	Matrix r = (Matrix)(*this) * (Matrix)(p);
	VPoint result ( r.GetElement(1,1) , r.GetElement(2,1) , r.GetElement(3,1) );
	return result;
}

VPoint2d TransformationMatrix::Transform ( const VPoint2d & source ) const
{
	vreal matarr[4] = { source.x() , source.y() , 0.0 , 1.0 };
	Matrix p ( 4 , 1 , matarr );
	Matrix r = (Matrix)(*this) * (Matrix)(p);
	VPoint2d result ( r.GetElement(1,1) , r.GetElement(2,1) );
	return result;
}

VPoint TransformationMatrix::TransformVec (const VPoint &vec)const
{
	VPoint p1(0.,0.,0.);
	VPoint p2(vec.x(),vec.y(),vec.z());
	VLine tmp(p1,p2);
	tmp=Transform(tmp);
	VPoint res=tmp.GetV();
	//res.NormThis();
	return res;
}

VLine TransformationMatrix::Transform (const VLine &source)const
{
	VPoint p1=source.GetP1();
	VPoint p2=source.GetP2();
	p1=Transform(p1);
	p2=Transform(p2);
	VLine transformed(p1,p2);
	return transformed;
}

CoordinateSystem TransformationMatrix::Transform ( const CoordinateSystem & cs ) const
{
	VPoint torg = Transform ( cs.GetOrigin() );
	VPoint tdirx = TransformVec ( cs.GetDirx() );
	VPoint tdiry = TransformVec ( cs.GetDiry() );	
	VPoint tdirz = TransformVec ( cs.GetDirz() );	
	CoordinateSystem transformed( torg , tdirx , tdiry , tdirz );
	return transformed;
}

VTriangleE TransformationMatrix::Transform ( const VTriangleE & tri ) const
{
	VPoint c1 = tri.GetCorner1();
	VPoint c2 = tri.GetCorner2();	
	VPoint c3 = tri.GetCorner3();	

	VPoint nc1 = Transform ( c1 );
	VPoint nc2 = Transform ( c2 );
	VPoint nc3 = Transform ( c3 );

	VTriangleE result ( nc1 , nc2 , nc3 );
	//if ( (tri.GetEdge1()^tri.GetEdge2()) % tri.GetNormal() < 0.0)
	//{
	//
	//}
	return result;
}

void TransformationMatrix::TransposeThis ()
{
	TransformationMatrix cp ( *this );
	SetElement( cp.GetElement (1,1) , 1,1 );
	SetElement( cp.GetElement (1,2) , 2,1 );
	SetElement( cp.GetElement (1,3) , 3,1 );
	SetElement( cp.GetElement (1,4) , 4,1 );

	SetElement( cp.GetElement (2,1) , 1,2 );
	SetElement( cp.GetElement (2,2) , 2,2 );
	SetElement( cp.GetElement (2,3) , 3,2 );
	SetElement( cp.GetElement (2,4) , 4,2 );

	SetElement( cp.GetElement (3,1) , 1,3 );
	SetElement( cp.GetElement (3,2) , 2,3 );
	SetElement( cp.GetElement (3,3) , 3,3 );
	SetElement( cp.GetElement (3,4) , 4,3 );	

	SetElement( cp.GetElement (4,1) , 1,4 );
	SetElement( cp.GetElement (4,2) , 2,4 );
	SetElement( cp.GetElement (4,3) , 3,4 );
	SetElement( cp.GetElement (4,4) , 4,4 );

}

void TransformationMatrix::Serialize(Bofstream & outfile)const
{
	Matrix::Serialize(outfile);
}

void TransformationMatrix::Serialize(Bifstream &infile)
{
	Matrix::Serialize(infile);
}

TransformationMatrix::TransformationMatrix(const OglMatrix &oglmat)
:Matrix ( 4 , 4 )
{
	const vreal * xmat = oglmat.GetArray();
	vreal * p=GetMatrixArr();
	p[0]=xmat[0];
	p[4]=xmat[1];
	p[8]=xmat[2];
	p[12]=xmat[3];

	p[1]=xmat[4];
	p[5]=xmat[5];
	p[9]=xmat[6];
	p[13]=xmat[7];

	p[2]=xmat[8];
	p[6]=xmat[9];
	p[10]=xmat[10];
	p[14]=xmat[11];

	p[3]=xmat[12];
	p[7]=xmat[13];
	p[11]=xmat[14];
	p[15]=xmat[15];
}

bool TransformationMatrix::operator == (const TransformationMatrix &toCompare)const
{
	if(this==&toCompare)
		return true;

	if(Matrix::operator ==(toCompare))
		return true;
	else
		return false;
}

bool TransformationMatrix::operator != (const TransformationMatrix &toCompare)const
{
	return !(this->operator ==(toCompare));
}

TransformationMatrix TransformationMatrix::GetInverse()const
{
	//TransformationMatrix inv;
	//GaussJordanElimination sol;

	//TransformationMatrix a1(*this);
	//vreal v1[4]={1,0,0,0};
	//Matrix c1(4,1,v1);
	//bool ck1=sol.Solve((Matrix&)(a1),c1);	

	//TransformationMatrix a2(*this);
	//vreal v2[4]={0,1,0,0};
	//Matrix c2(4,1,v2);
	//bool ck2=sol.Solve((Matrix&)(a2),c2);	
	//
	//TransformationMatrix a3(*this);
	//vreal v3[4]={0,0,1,0};
	//Matrix c3(4,1,v3);
	//bool ck3=sol.Solve((Matrix&)(a3),c3);	

	//TransformationMatrix a4(*this);
	//vreal v4[4]={0,0,0,1};
	//Matrix c4(4,1,v4);
	//bool ck4=sol.Solve((Matrix&)(a4),c4);	
	//
	//if(ck1==false || ck2==false || ck3==false || ck4==false)
	//	throw VException (("Transformation Matrix inverse exception"),1);

	//for(int i=1;i<=4;i++)
	//{
	//	inv.SetElement(c1.GetElement(i,1),i,1);
	//}
	//for(i=1;i<=4;i++)
	//{
	//	inv.SetElement(c2.GetElement(i,1),i,2);
	//}
	//for(i=1;i<=4;i++)
	//{
	//	inv.SetElement(c3.GetElement(i,1),i,3);
	//}
	//for(i=1;i<=4;i++)
	//{
	//	inv.SetElement(c4.GetElement(i,1),i,4);
	//}
	//return inv;

	GaussElemination gg;
	vreal ba1[4] = {1.0 , 0.0 , 0.0 , 0.0};
	vreal ba2[4] =	{0.0 , 1.0 , 0.0 , 0.0};
	vreal ba3[4] = {0.0 , 0.0 , 1.0 , 0.0};
	vreal ba4[4] = {0.0 , 0.0 , 0.0 , 1.0};

	Matrix b1 (4,1,ba1);
	Matrix b2 (4,1,ba2);
	Matrix b3 (4,1,ba3);
	Matrix b4 (4,1,ba4);

	Matrix s1 = gg.Solve (*this , b1);
	Matrix s2 = gg.Solve (*this , b2);
	Matrix s3 = gg.Solve (*this , b3);
	Matrix s4 = gg.Solve (*this , b4);

	Matrix result(4,4);
	int i;
	for (i=1;i<=4;i++)
	{
		result.SetElement (s1.GetElement(i,1)
							,i   //row
							,1 );//col
	}
	for (i=1;i<=4;i++)
	{
		result.SetElement (s2.GetElement(i,1)
							,i   //row
							,2 );//col
	}
	for (i=1;i<=4;i++)
	{
		result.SetElement (s3.GetElement(i,1)
							,i   //row
							,3 );//col
	}
	for (i=1;i<=4;i++)
	{
		result.SetElement (s4.GetElement(i,1)
							,i   //row
							,4 );//col
	}
	return result;
}

sVector<double,2> TransformationMatrix::Transform (const sVector<double,2> &source)const
{
	VPoint tmp(source.GetElement(0),source.GetElement(1),0.0);
	VPoint transformed = Transform (tmp);
	sVector<double,2> result (transformed.GetArray());
	return result;
}

sVector<double,3> TransformationMatrix::Transform (const sVector<double,3> &source)const
{
	VPoint tmp (source);
	VPoint transformed = Transform (tmp);
	sVector<double,3> result (transformed.GetArray());
	return result;
}

igesTransformationMatrix TransformationMatrix::GetIgesTransformation()const
{
	igesTransformationMatrix result;
	result.mR11 = matrix[0];
	result.mR12 = matrix[1];
	result.mR13 = matrix[2];
	result.mT1 = matrix[3];
	result.mR21 = matrix[4];
	result.mR22 = matrix[5];
	result.mR23 = matrix[6];
	result.mT2 = matrix[7];
	result.mR31 = matrix[8];
	result.mR32 = matrix[9];
	result.mR33 = matrix[10];
	result.mT3 = matrix[11];
	return result;
}

void TransformationMatrix::SetIgesTransformation(igesTransformationMatrix &X)const
{
	X.mR11 = matrix[0];
	X.mR12 = matrix[1];
	X.mR13 = matrix[2];
	X.mT1 = matrix[3];
	X.mR21 = matrix[4];
	X.mR22 = matrix[5];
	X.mR23 = matrix[6];
	X.mT2 = matrix[7];
	X.mR31 = matrix[8];
	X.mR32 = matrix[9];
	X.mR33 = matrix[10];
	X.mT3 = matrix[11];
}

TransformationMatrix & TransformationMatrix::operator *= (const TransformationMatrix &tm)
{
	TransformationMatrix tmp = tm * *this;
	//*this = tmp;
	TransformationMatrix::operator =(tmp);
	return *this;
}

bool TransformationMatrix::Compare (const TransformationMatrix &tm,const vreal tol)const
{
	for (int i=0;i<16;++i)
	{
		if (fabs(matrix[i]-tm.matrix[i]) > tol)
			return false;
	}
	return true;
}

void TransformationMatrix::Transform(std::vector<VPoint> &pvec)const
{
	int psize = (int)pvec.size();
	for (int pi = 0; pi < psize; ++pi)
	{
		pvec.operator[](pi) = Transform(pvec.operator[](pi));
	}
}

VStr TransformationMatrix::GetInformationText()const
{
	VStr info = sozluk("Undefined transformation");
	return info;
}