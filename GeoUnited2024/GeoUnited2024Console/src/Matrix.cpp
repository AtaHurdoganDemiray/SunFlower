/******************************************************************************
*               File: Matrix.cpp											  *
*******************************************************************************
*               Description:                                                  *
*                                                                             *
*******************************************************************************
*               History:                                                      *
*  17.08.2003 08:08:56 Created by:    Ata Hurdogan Demiray                    *
*******************************************************************************
*               (C) 2003 by ModuleWorks GmbH                                  *
******************************************************************************/

#include "stdafx.h"
#include "Matrix.h"


Matrix::Matrix ( int rowno , int colno )
{
	int elementno = rowno * colno;

	matrix = new vreal [elementno];

	for ( int i = 0; i < elementno; i++ )
		matrix[i] = 0.0;

	m_colno = colno;
	m_rowno = rowno;
	
}

//#############################################################################

Matrix::Matrix ( int rowno , int colno , const vreal values[] )
{
	int elementno = rowno * colno;

	matrix = new vreal [elementno];

	for ( int i = 0; i < elementno; i++ )
	{
		matrix[i] = values[i];
	}
	
	m_colno = colno;
	m_rowno = rowno;
}

//#############################################################################

Matrix::Matrix ( const Matrix &Foo )
{
	int elementno = Foo.m_rowno * Foo.m_colno;

	matrix = new vreal [elementno];

	for ( int i = 0; i < elementno; i++ )
	{
		matrix[i] = Foo.matrix[i];
	}
	
	m_colno = Foo.m_colno;
	m_rowno = Foo.m_rowno;
}

//#############################################################################

Matrix & Matrix::operator = ( const Matrix & Foo )
{
	m_colno = Foo.m_colno;
	m_rowno = Foo.m_rowno;
	int elementno = m_rowno * m_colno;

	if (matrix)
		delete [] matrix;
	
	matrix = new vreal [elementno];

	for ( int i = 0; i < elementno; i++ )
	{
		matrix[i] = Foo.matrix[i];
	}
	return (*this);
}

Matrix::~Matrix()
{
	delete [] matrix;
}

//#############################################################################

int Matrix::GetArrayIndex ( int row , int col ) const
{
	if ( row > m_rowno )
		throw( VException ( ( IDS_MatrixException) , 2 ) );

	if ( col > m_colno )
		throw( VException ( ( IDS_MatrixException) , 3 ) );

	return ( ( col - 1 ) + ( row - 1 ) * m_colno );
}

//#############################################################################

const vreal & Matrix::GetElement ( int row , int col ) const
{
	if (row == 0 || col == 0 || row > m_rowno || col > m_colno)
	{
		LogFileWriter::WriteUnknownExceptionLog ("Matrix::GetElement expects 1 based index. There is 0 for i or j , exception");
		throw VException ("Matrix::GetElement , 0 based index!",1);
	}
	int index = GetArrayIndex( row , col );
	return (matrix[index]);
}

//#############################################################################

int Matrix::GetRowNo () const
{
	return m_rowno;
}

//#############################################################################

int Matrix::GetColNo () const
{
	return m_colno;
}

//#############################################################################

Matrix Matrix::operator * ( const Matrix & mat2 ) const
{
	int mat1_rowno = this->GetRowNo();
	int mat1_colno = this->GetColNo();

	int mat2_rowno = mat2.GetRowNo();
	int mat2_colno = mat2.GetColNo();
	
	if ( mat1_colno != mat2_rowno )
		throw ( VException ( ( IDS_MatrixException) , 1 ));
	
	int res_rowno , res_colno;
	
	res_rowno = mat1_rowno;
	res_colno = mat2_colno;
	vreal val;
	vreal * matval;
	matval = new vreal [res_rowno * res_colno];
	int index;

	for ( int row = 1; row <= res_rowno; row++ )
		for ( int col = 1; col <= res_colno; col++ )
		{
			val = 0.0;
			index =  ( col - 1 ) + ( row - 1 ) * res_colno; 
			for ( int cur = 1; cur <= mat1_colno; cur++ )
			{
				val += (this->GetElement ( row , cur ) * mat2.GetElement ( cur , col ));
			}

			matval[index] = val;
		}

	Matrix result ( res_rowno , res_colno , matval );

	delete matval;

	return result;

}
	
//#############################################################################

void Matrix::SetElement ( const vreal &value , int row , int col )
{
	int arrayindex = GetArrayIndex ( row , col );
	matrix [arrayindex] = value;
}

//#############################################################################

vreal * Matrix::GetMatrixArr()
{
	return matrix;
}

//#############################################################################

//Matrix::Matrix ( VIFStream & infile )
//{
//	infile >> m_colno;
//	infile >> m_rowno;
//	
//	int elementno = m_rowno * m_colno;
//
//	matrix = new vreal [elementno];
//
//	for ( int i = 0; i < elementno; i++ )
//		infile>>(matrix[i]);
//}

//#############################################################################

//void Matrix::operator >> ( VofStream & outfile ) const
//{
//	outfile << m_colno;
//	outfile << m_rowno;
//
//	int elementno = m_rowno * m_colno;
//
//	for ( int i = 0; i < elementno; i++ )
//		outfile<<(matrix[i]);
//}

//#############################################################################

void Matrix::Serialize (Bofstream & outfile )const
{
	outfile.write ( (VChr*)&m_colno , sizeof(int));
	outfile.write ( (VChr*)&m_rowno , sizeof(int));

	int elementno = m_rowno * m_colno;

	for ( int i = 0; i < elementno; i++ )
		outfile.write((VChr*)&(matrix[i]) , sizeof(vreal));

}

//#############################################################################

void Matrix::Serialize (Bifstream & infile)
{
	infile.read((VChr*)&m_colno,sizeof(int));
	infile.read((VChr*)&m_rowno,sizeof(int));	

	int elementno = m_rowno * m_colno;

	for ( int i = 0; i < elementno; i++ )
		infile.read((VChr*)&(matrix[i]) , sizeof(vreal));
}

//#############################################################################

bool Matrix::operator == (const Matrix &toCompare)const
{
	if(this==&toCompare)
		return true;
	
	int elementno=m_colno*m_rowno;
	if (elementno!=toCompare.m_colno*toCompare.m_rowno)
		return false;
	for(int i=0;i<elementno;i++)
	{
		if(fabs(matrix[i]-toCompare.matrix[i])>EPSILON)
		{
			return false;
		}
	}
	
	return true;
}

//#############################################################################

bool Matrix::operator != (const Matrix &toCompare)const
{
	return !(this->operator ==(toCompare));
}

//#############################################################################

void Matrix::MultiplyRowAndAddToRow(const vreal &multiplyBy,int rowToMultiply,int rowToAdd)
{
	for(int i=1;i<m_colno+1;i++)
	{
		vreal newval=GetElement(rowToMultiply,i)*multiplyBy + GetElement(rowToAdd,i);
		SetElement(newval,rowToAdd,i);
	}
}

//#############################################################################

void Matrix::SwapRows(int i,int j)
{
	int n=GetColNo();
	for(int k=1;k<=n;k++)
	{
		vreal el1=GetElement(i,k);
		vreal el2=GetElement(j,k);
		SetElement(el2,i,k);
		SetElement(el1,j,k);
	}
}

//#############################################################################

void Matrix::SwapColomns(int i,int j)
{
	int n=GetRowNo();
	for(int k=1;k<=n;k++)
	{
		vreal el1=GetElement(k,i);
		vreal el2=GetElement(k,j);
		SetElement(el2,k,i);
		SetElement(el1,k,j);
	}
}

//#############################################################################

Matrix Matrix::GetMinorMatrix (int row,int col)const
{
	int ni=GetRowNo();
	int nj=GetColNo();
	
	if(ni-1<1 || nj-1<1)
	{
		throw VException (( IDS_MatMinMatCanNot),1);
	}

	Matrix m(ni-1,nj-1);
	int counter=0;
	vreal *myarray=m.GetMatrixArr();
	for(int i=1;i<=ni;i++)
	{
		if(i==row)
			continue;
		for(int j=1;j<=nj;j++)
		{
			if(j==col)
				continue;
			myarray[counter]=GetElement(i,j);
			++counter;
		}
	}
	return m;
}

//#############################################################################

void Matrix::Multiply(const vreal &f)
{
	for(int i=1;i<=m_rowno;i++)
	{
		for(int j=1;j<=m_colno;j++)
		{
			vreal currEl=GetElement(i,j);
			SetElement(currEl*f,i,j);
		}
	}
}

Matrix Matrix::operator + (const Matrix &M)const
{
	if(m_rowno!=M.GetRowNo() || m_colno!=M.GetColNo())
		throw VException (( IDS_MatAddWitDifSiz),1);
	
	Matrix result(m_rowno,m_colno);
	for(int i=1;i<=m_rowno;i++)
	{
		for(int j=1;j<=m_colno;j++)
		{
			vreal e1=GetElement(i,j);
			vreal e2=M.GetElement(i,j);
			result.SetElement(e1+e2,i,j);
		}
	}

	return result;	
}
Matrix Matrix::operator - (const Matrix &M)const
{
	if(m_rowno!=M.GetRowNo() || m_colno!=M.GetColNo())
		throw VException (( IDS_MatAddWitDifSiz),1);

	Matrix result(m_rowno,m_colno);
	for(int i=1;i<=m_rowno;i++)
	{
		for(int j=1;j<=m_colno;j++)
		{
			vreal e1=GetElement(i,j);
			vreal e2=M.GetElement(i,j);
			result.SetElement(e1-e2,i,j);
		}
	}

	return result;
}

const vreal * Matrix::GetMatrixArr ()const
{
	return matrix;
}

vreal Matrix::GetDeterminant()const
{
	int ni=GetRowNo();
	int nj=GetColNo();
	if(ni!=nj)
		throw VException (( IDS_MatIsNotSquNoDe),1);
	if(ni==2)
	{
		return GetDeterminant2();
	}
	else if (ni==3)
	{
		return GetDeterminant3();
	}
	else
	{
		vreal determinant=0.;
		for(int j=1;j<=nj;j++)
		{
			Matrix m=GetMinorMatrix(1,j);
			vreal myval = GetElement (1,j);
			vreal subDet=m.GetDeterminant();
			vreal f=pow(-1.,1.+j);
			determinant+=(f*subDet*myval);
		}
		return determinant;
	}
}
vreal Matrix::GetDeterminant2()const
{
	vreal res = GetElement(1,1)*GetElement(2,2) - GetElement(2,1)*GetElement(1,2);
	return res;
}
vreal Matrix::GetDeterminant3()const
{
	vreal res = GetElement (1,1)*GetElement(2,2)*GetElement(3,3)
				+ GetElement(1,2)*GetElement(2,3)*GetElement(3,1)
				+ GetElement(1,3)*GetElement(2,1)*GetElement(3,2)
				-(GetElement(3,1)*GetElement(2,2)*GetElement(1,3)
					+GetElement(3,2)*GetElement(2,3)*GetElement(1,1)
					+GetElement(3,3)*GetElement(2,1)*GetElement(1,2) );
	return res;
}

void Matrix::SetIdentity()
{
	SetZero();

    int nn = min(m_rowno, m_colno);
	for(int i = 0; i < nn; i++)
	{
		matrix[i * m_colno + i] = 1;
	}
}

void Matrix::SetZero()
{
	memset(matrix, 0, m_rowno * m_colno * sizeof(vreal));
}

bool Matrix::isZero()
{
	for(int j = 0; j < m_rowno; j++)
	{
		for(int i = 0; i < m_colno; i++)
		{
			if(abs(matrix[j * m_colno + i]) >= EPSILON)
				return false;
		}
	}

	return true;
}

// Transpose a square matrix inplace, returns itself
Matrix &Matrix::Transpose()
{
	if(m_rowno != m_colno)
		return *this;

	for(int j = 1; j < m_rowno; j++)
	{
		for(int i = 0; i < j; i++)
		{
			vreal tmp = matrix[j * m_colno + i];
			matrix[j * m_colno + i] = matrix[i * m_colno + j];
			matrix[i * m_colno + j] = tmp;
		}
	}

	return *this;
}

//Transpose a non-square matrix onto B, returns false if matrix sizes mismatch
bool Matrix::Transpose(Matrix &B)
{
	if(B.m_rowno != m_colno || B.m_colno != m_rowno)
		return false;

	for(int j = 0; j < m_rowno; j++)
	{
		for(int i = 0; i < m_colno; i++)
		{
			B.matrix[i * B.m_colno + j] = matrix[j * m_colno + i];
		}
	}

	return true;
}

//Get sub-matrix into B, returns false on 'out of boundary' or if matrix sizes mismatch
// i: zero based horizontal index 
// j: zero based vertical index
bool Matrix::GetSubMatrix(int i, int j, Matrix &B)
{
	if(i < 0 || j < 0)
		return false;

	if((i + B.m_colno) > m_colno || (j + B.m_rowno) > m_rowno)
		return false;

	for(int jj = 0; jj < B.m_rowno; jj++)
	{
		for(int ii = 0; ii < B.m_colno; ii++)
		{
			B.matrix[jj * B.m_colno + ii] = matrix[(jj + j) * m_colno + ii + i];
		}
	}

	return true;
}

bool Matrix::isIdentity()const
{
	if (m_rowno != m_colno)
		return false;
	for (int i=0;i<m_rowno;++i)
	{
		for (int j=0;j<m_rowno;++j)
		{
			if (i != j)
			{
				if(fabs(this->GetElement (i+1,j+1)) > EPSILON)
					return false;
			}
			else
			{
				if (fabs (this->GetElement(i+1,j+1)-1.0) > EPSILON)
					return false;
			}
		}
	}
	return true;
}

Matrix * Matrix::MakeAugmentedMatrix (Matrix *A , Matrix *C)
{
	int Arowno = A->GetRowNo();
	if (C->GetRowNo() != Arowno)
		throw VException ("Matrix::MakeAugmentedMatrix exc., A rowno not equal to C rowno",1);
	int Acolno = A->GetColNo();	
	int totalcolno = Acolno + C->GetColNo();
	Matrix *AU = new Matrix (A->GetRowNo() , totalcolno);
	for (int row=1;row<=Arowno;++row)
	{
		for (int col=1;col<=Acolno;++col)
		{
			AU->SetElement (A->GetElement(row,col),row,col);
		}
		for (int col=Acolno+1;col<=totalcolno;++col)
		{
			AU->SetElement (C->GetElement(row,col-Acolno),row,col);
		}
	}
	return AU;
}

bool Matrix::GetBiggestPivotLower (int &rowno , std::multimap<vreal,int> &absValRowNoMap)const// returns true if biggest abs val is not 0
{
	// rowno is the goal row for finding pivot element , if rowno = 3 pivot will be 3,3 th element
	for (int row = rowno; row <= m_rowno; ++row)
	{
		vreal curpivot = fabs(GetElement (row,rowno));
		absValRowNoMap.insert (std::pair<vreal,int>(curpivot,row));
	}
	rowno = absValRowNoMap.rbegin()->first;
	if (absValRowNoMap.rbegin()->second > EPSILON)
		return true;
	else
		return false;
}
bool Matrix::GetBiggestPivotUpper (int &rowno , std::multimap<vreal,int> &absValRowNoMap)const// returns true with same condition as above
{
	for (int row =0; row <= rowno; ++row)
	{
		vreal curpivot = fabs (GetElement(row,rowno));
		absValRowNoMap.insert (std::pair<vreal,int>(curpivot,row));
	}
	rowno = absValRowNoMap.rbegin()->first;
	if (absValRowNoMap.rbegin()->second > EPSILON)
		return true;
	else
		return false;
}

bool Matrix::PivotSort (Matrix &sorted)
{
	// build possibleValuePivotMap
	std::vector<std::multimap<vreal,int>> allPossibleValColiMap;
	std::list<std::multimap<vreal,int>> rowPivotNominees;
	for (int row=0;row<=m_rowno;++row)
	{
		std::multimap<vreal,int>curPossibleValColiMap;
		bool ck = GetPossibleValuePivotRow (row,curPossibleValColiMap);
		if (ck == false)
			return false;
		allPossibleValColiMap.push_back (curPossibleValColiMap);
		std::multimap<vreal,int>::const_reverse_iterator pvi = curPossibleValColiMap.rbegin();
		std::multimap<vreal,int>::const_reverse_iterator pve = curPossibleValColiMap.rend();
		rowPivotNominees.push_back (std::multimap<vreal,int>());
		for (pvi;pvi!=pve;++pvi)
		{
			rowPivotNominees.back().insert(std::pair<vreal,int>(pvi->first,row));
		}
	}
	std::multimap<int , std::pair<int,std::multimap<vreal,int>>> sizeAndRowNominees;
	std::list<std::multimap<vreal,int>>::iterator lni = rowPivotNominees.begin();
	std::list<std::multimap<vreal,int>>::iterator lne = rowPivotNominees.end();
	int counter = 0;
	for (lni;lni!=lne;++lni)
	{
		sizeAndRowNominees.insert(std::pair<int,std::pair<int,std::multimap<vreal,int>>>((int)lni->size()
			,std::pair<int,std::multimap<vreal,int>>(counter,*lni)));
		++counter;
	}
	std::vector<int> rowBestMatch (this->GetRowNo() , -1);
	std::multimap<int,std::pair<int,std::multimap<vreal,int>>>::iterator srni = sizeAndRowNominees.begin();
	std::multimap<int,std::pair<int,std::multimap<vreal,int>>>::iterator srne = sizeAndRowNominees.end();
	for (srni;srni!=srne;++srni)
	{
		int curRow = srni->second.first;
		if (srni->second.second.size() == 0)
			return false;
		std::multimap<vreal,int>::reverse_iterator mmi = srni->second.second.rbegin();
		std::multimap<vreal,int>::reverse_iterator mme = srni->second.second.rend();
		for (mmi;mmi!=mme;++mmi)
		{
			if (mmi->second > 0)
			{
				rowBestMatch.at(curRow) = mmi->second;
				NegateAllIndex (mmi->second,sizeAndRowNominees);
			}
		}
	}


	return true;
}

bool Matrix::GetPossibleValuePivotRow (int rowno , std::multimap<vreal,int> &possibleValColiMap)const
{
	bool ck = false;
	for (int col=1;col<m_colno;++col)// last col is ignored , this must be augmented matrix for Gauss
	{
		vreal curval = fabs(GetElement (rowno,col));
		if (curval>EPSILON)
		{
			possibleValColiMap.insert (std::pair<vreal,int>(curval,col));
			ck = true;
		}
	}
	return ck;
}

void Matrix::NegateAllIndex (int i,std::multimap<int,std::pair<int,std::multimap<vreal,int>>> &sizeAndRowNominees)
{
	std::multimap<int,std::pair<int,std::multimap<vreal,int>>>::iterator srni = sizeAndRowNominees.begin();
	std::multimap<int,std::pair<int,std::multimap<vreal,int>>>::iterator srne = sizeAndRowNominees.end();
	for (srni;srne!=srni;++srne)
	{
		std::multimap<vreal,int>::reverse_iterator mmi = srni->second.second.rbegin();
		std::multimap<vreal,int>::reverse_iterator mme = srni->second.second.rend();
		for (mmi;mmi!=mme;++mmi)
		{
			if (mmi->second == i)
			{
				mmi->second *= -1;
				break;
			}
		}
	}
}