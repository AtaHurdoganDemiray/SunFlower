/******************************************************************************
*               File: Matrix.h											  *
*******************************************************************************
*               Description:                                                  *
*                                                                             *
*******************************************************************************
*               History:                                                      *
*  17.08.2003 08:00:45 Created by:    Ata Hurdogan Demiray                    *
*******************************************************************************
*               (C) 2003 by Pozitif GmbH	                                 *
******************************************************************************/

#ifndef __Matrix_h__
#define __Matrix_h__
#include "Bofstream.h"
#include "Bifstream.h"
#include "BasicDefinitions.h"

struct Matrix
{
public:
	friend class TransformationMatrix;
	Matrix ( int rowno , int colno );
	Matrix ( int rowno , int colno , const vreal values[] );
	Matrix ( const Matrix & Foo );
	Matrix & operator = ( const Matrix & Foo );
	virtual ~Matrix ();
	const vreal & GetElement ( int row , int col ) const;
	int GetArrayIndex ( int row , int col ) const;
	virtual Matrix operator * ( const Matrix & mat2 ) const;
	friend Matrix operator * (const vreal &f,const Matrix &M);
	friend Matrix operator * (const Matrix &M,const vreal &f);
	void Multiply (const vreal &f);
	int GetRowNo() const;
	int GetColNo() const;
	void SetElement ( const vreal & value , int row , int col );
	vreal * GetMatrixArr ();
	const vreal * GetMatrixArr ()const;
	void Serialize ( Bofstream & outfile )const;
	void Serialize ( Bifstream & infile );
	bool operator == (const Matrix &toCompare)const;
	bool operator != (const Matrix &toCompare)const;
	void MultiplyRowAndAddToRow(const vreal &multiplyBy,int rowToMultiply,int rowToAdd);
	void SwapRows(int i,int j);
	void SwapColomns(int i,int j);
	Matrix GetMinorMatrix (int row,int col)const;
	vreal GetDeterminant()const;
	vreal GetDeterminant2()const;
	vreal GetDeterminant3()const;
	Matrix operator + (const Matrix &M)const;
	Matrix operator - (const Matrix &M)const;

	void SetIdentity();
	void SetZero();
	bool isZero();

	//Transpose a square matrix inplace, returns itself
	Matrix &Transpose();

	//Transpose a non-square matrix onto B, returns false if matrix sizes mismatch
	bool Transpose(Matrix &B);

	//Get sub-matrix into B, returns false on 'out of boundary' or if matrix sizes mismatch
	// i: zero based horizontal index 
	// j: zero based vertical index
	bool GetSubMatrix(int i, int j, Matrix &B);

private:
	int m_rowno , m_colno;
	vreal * matrix;
public:
	bool isIdentity ()const;
	Matrix * MakeAugmentedMatrix (Matrix *A , Matrix *C);
	bool GetPivotRow (int &rowno)const; // returns true if pivot with non zero element can be found , rowno is number of that row
	// pivot definition , for row 1  pivot column is one , for row 5 pivot column is 5 
	bool GetBiggestPivotLower (int &rowno , std::multimap<vreal,int> &absValRowNoMap)const;// returns true if biggest abs val is not 0
	bool GetBiggestPivotUpper (int &rowno , std::multimap<vreal,int> &absValRowNoMap)const;// returns true with same condition as above
	bool PivotSort (Matrix &sorted);
	bool GetPossibleValuePivotRow (int rowno , std::multimap<vreal,int> &possibleValColiMap)const;
	void NegateAllIndex (int i , std::multimap<int,std::pair<int,std::multimap<vreal,int>>> &sizeAndRowNominees);
};

inline Matrix operator * (const vreal &f,const Matrix &M)
{
	Matrix tmp(M);
	tmp.Multiply(f);
	return tmp;
};

inline Matrix operator * (const Matrix &M,const vreal &f)
{
	Matrix tmp(M);
	tmp.Multiply(f);
	return tmp;
};	

#endif //__Matrix_h__
