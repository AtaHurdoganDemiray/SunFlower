// GaussJordanElimination.cpp

#include "stdafx.h"
#include "GaussJordanElimination.h"
#include "Matrix.h"

const int PIVOT_RECURSIVE_LIMIT=30;

bool GaussJordanElimination::Solve(Matrix &a,Matrix &c)
{
	int n=a.GetColNo();
	if(n!=a.GetRowNo()||c.GetRowNo()!=n)
		return false;//can not solve
	int recursiveDepth=0;
	bool ck=PartialPivot(a,c,recursiveDepth);
	if(!ck)
		return false;
	vreal Pik(0.0);
	for(int k=1;k<=n;k++)//for every row except last,parent row 
	{
		for(int i=k+1;i<=n;i++)//for every row after parent
		{
			vreal div=a.GetElement(k,k);
			if(fabs(div)<EPSILON)
				return false;//singular
			Pik=-(a.GetElement(i,k)/div);		
			a.MultiplyRowAndAddToRow(Pik,k,i);
			vreal newc=c.GetElement(i,1/*!*/)+Pik*c.GetElement(k,1/*!*/);
			c.SetElement(newc, i, 1);
		}//for every row after parent
	}//for every row except last,parent row 

	for (int i=n;i>0;i--)
	{
		for(int j=i+1;j<=n;j++)
		{
			vreal newc=c.GetElement(i,1)-c.GetElement(j,1)*a.GetElement(i,j);
			c.SetElement(newc,i,1);
		}
		vreal nc=c.GetElement(i,1)/a.GetElement(i,i);
		c.SetElement(nc,i,1);
	}
	return true;
}

bool GaussJordanElimination::PartialPivot(Matrix &a,Matrix &c,int &recursiveDepth)
{
	if(recursiveDepth>PIVOT_RECURSIVE_LIMIT)
		return false;
	int n=a.GetRowNo();
	for(int i=1;i<=n;i++)
	{
		vreal max_el=fabs(a.GetElement(i,i));
		int max_row=i;
		for (int j=i+1;j<=n;j++)
		{
			vreal curr_el=fabs(a.GetElement(j,i));
			if(curr_el>max_el)
			{
				max_el=curr_el;
				max_row=j;
			}
		}
		if(max_row!=i)
		{
			a.SwapRows(i,max_row);
			c.SwapRows(i,max_row);
		}
	}
	//check if there is a zero on diagonal
	for(int i=1;i<=n;i++)
	{	
		vreal diagonalElement=a.GetElement(i,i);
		if(fabs(diagonalElement)<EPSILON)
		{
			//swap and retry
			a.SwapRows(1,i);
			c.SwapRows(1,i);
			++recursiveDepth;
			bool ck=PartialPivot(a,c,recursiveDepth);
			if(!ck)
				return false;
		}
	}
	return true;
}

bool GaussJordanElimination::SolveIf (Matrix &AU)
{
	// first every row needs a non zero pivot element (for row n , col n must be non zero)



	return true;
}