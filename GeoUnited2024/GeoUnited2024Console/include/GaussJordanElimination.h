// GaussJordanElimination.h

#ifndef __GaussJordanElimination_h__
#define __GaussJordanElimination_h__
struct Matrix;
class GaussJordanElimination
{
public:
	bool Solve(Matrix &a,Matrix &c);
private:
	bool PartialPivot(Matrix &a,Matrix &c,int &recursiveDepth);
public:
	bool SolveIf (Matrix &AU);//Get augmented matrix for A and C , returns true only if all unknowns have real solution   
};

#endif