//GaussElemination.h

#ifndef __GaussElemination_h__
#define __GaussElemination_h__
struct Matrix;

class GaussElemination
{

public:
	GaussElemination();
	Matrix Solve(Matrix a,Matrix b);


private:
	void Pivot(Matrix &a,Matrix &b,int diagonalno);
	void MakeHeadOne(Matrix &a,Matrix &b,int diagonalno);
	void SubtractSuperRow(Matrix &a,Matrix &b,int diagonalno);
	void BackSubs (Matrix &a,Matrix &b,Matrix &x);
	//void ShowMatrix (Matrix &a);
};
#endif

