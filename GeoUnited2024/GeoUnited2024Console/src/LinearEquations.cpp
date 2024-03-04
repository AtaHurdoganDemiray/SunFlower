// LinearEquations.cpp

#include "stdafx.h"
#include "LinearEquations.h"
#include "Matrix.h"
#include "GaussJordanElimination.h"
//#include "SVD.h"

bool LinearEquations::SolveLinear2Unknown ( const vreal & a1 , const vreal & b1 , const vreal & c1
											, const vreal & a2 , const vreal & b2 , const vreal & c2
											, vreal &x , vreal &y ) 
{
	// Function solves linear eq. system
	// a1 x + b1 y + c1 = 0
	// a2 x + b2 y + c2 = 0
	// if there is a solution returns true and fills x , y solutions,
	// otherwise it returns false

	vreal det = a1 * b2 - a2 * b1;


	if ( fabs(det) < EPSILON )
		return false;

	x = ( c2 * b1 - c1 * b2 ) / det;

	y = ( c1 * a2 - c2 * a1 ) / det;

	return true;
}

 bool LinearEquations::SolveLinear2UnknownByGauss ( const vreal & a1 , const vreal & b1 , const vreal & c1
												 , const vreal & a2 , const vreal & b2 , const vreal & c2
												 , vreal &x , vreal &y ) 
 {
	vreal aval[4]={a1,b1,a2,b2};
	vreal cval[2]={-c1,-c2};
	Matrix a(2,2,aval);
	Matrix c(2,1,cval);
	GaussJordanElimination solver;
	bool ck=solver.Solve(a,c);
	x=c.GetElement(1,1);
	y=c.GetElement(2,1);
	return ck;
 }

//Inhomogeneous Linear system Ax = b
//Modifies A
//bool LinearEquations::SolveSVD(Matrix &A, const Matrix &b, Matrix &x, bool symA)
//{
//	int m = A.GetRowNo();
//	int n = A.GetColNo();
//
//	if(m < n)	// underdetermined system
//		return false;
//
//	if(symA && m != n)
//		return false;
//
//	// b must be m x 1
//	if(b.GetRowNo() != m || b.GetColNo() != 1)
//		return false;
//
//	// x must be n x 1
//	if(x.GetRowNo() != n || x.GetColNo() != 1)
//		return false;
//
//	SVD svd(m, n);
//
//	Matrix S(n, 1);
//	Matrix V(n, n);
//
//	//U us needed only if A is not symmetric
//	Matrix *Ut = 0;
//	if(!symA)
//	{
//		Ut = new Matrix(n, m);	//only first n vectors if U is needed
//	}
//
//	int err = svd.Calc(&A, &S, Ut, &V, SVD_OPT_MODIFY_A | SVD_OPT_UT);
//
//	if(err != SVD_OK)
//	{
//		if(Ut) delete Ut;
//		return false;
//	}
//
//	if(svd.GetRank() < n) // A is singular / not-invertable
//	{
//		if(Ut) delete Ut;
//		return false;
//	}
//
//	// perform backsubstitution
//	// x = V * inv(S) * Ut * b
//	
//	Matrix k(n, 1);	// tmp matrix
//
//	vreal *kData = k.GetMatrixArr();
//	const vreal *bData = b.GetMatrixArr();
//	const vreal *vData = V.GetMatrixArr();
//	const vreal *sData = S.GetMatrixArr();
//	vreal *xData = x.GetMatrixArr();
//
//	// k = Ut * b
//	if(symA)
//	{
//		for(int j = 0; j < n; j++)
//		{
//			kData[j] = 0;
//
//			for(int i = 0; i < n; i++)
//			{
//				kData[j] += (vData[i*n+j] * bData[i]);
//			}
//		}
//	}
//	else
//	{
//		const vreal *utData = Ut->GetMatrixArr();
//
//		for(int j = 0; j < n; j++)
//		{
//			kData[j] = 0;
//
//			for(int i = 0; i < m; i++)
//			{
//				kData[j] += (utData[j*m+i] * bData[i]);
//			}
//		}
//	}
//
//	// k = inv(S) * k
//	for(int i = 0; i < n; i++)
//	{
//		kData[i] /= sData[i];	// Elements of S are not zero since rank(A) is n
//	}
//
//	// x =  V * k
//	for(int j = 0; j < n; j++)
//	{
//		xData[j] = 0;
//
//		for(int i = 0; i < n; i++)
//		{
//			xData[j] += (vData[j*n+i] * kData[i]);
//		}
//	}
//
//	if(Ut) delete Ut;
//
//	return true;
//}

//Homogeneous Linear system Ax = 0
//Modifies A
//bool LinearEquations::SolveSVD(Matrix &A, Matrix &x)
//{
//	int m = A.GetRowNo();
//	int n = A.GetColNo();
//
//	if(m < n)	// underdetermined system
//		return false;
//
//	// x must be n x 1
//	if(x.GetRowNo() != n || x.GetColNo() != 1)
//		return false;
//
//	SVD svd(m, n);
//
//	Matrix S(n, 1);
//	Matrix Vt(n, n);
//
//	int err = svd.Calc(&A, &S, 0, &Vt, SVD_OPT_MODIFY_A | SVD_OPT_VT);
//
//	if(err != SVD_OK)
//		return false;
//
//	if(svd.GetRank() < (n-1))
//		return false;
//
//	if(svd.GetRank() == n)
//	{
//		x.SetZero();	// trivial solution
//
//		return true;
//	}
//
//	// If rank is n - 1, then
//	// solution is the eigen vector of AtA with the smallest eigen value
//	// or equivalently the last row of Vt
//	memcpy(x.GetMatrixArr(), Vt.GetMatrixArr() + (n-1) * n, n * sizeof(vreal));
//
//	return true;
//}
