//GaussElemination.cpp

#include "stdafx.h"
#include "GaussElemination.h"
#include "Matrix.h"

GaussElemination::GaussElemination()
{

}

Matrix GaussElemination::Solve(Matrix a,Matrix b)
{
	for (int i=1;i<=a.GetRowNo()-1;i++)
	{
		Pivot(a,b,i);
		MakeHeadOne(a,b,i);
		//ShowMatrix (a);
		//ShowMatrix (b);
		SubtractSuperRow(a,b,i);
		//ShowMatrix (a);
		//ShowMatrix (b);
	}
	MakeHeadOne (a,b,a.GetRowNo());

	Matrix x(a.GetRowNo(),b.GetColNo());
	BackSubs (a,b,x);
	
	//double cont1=x.GetElement (1,1);
	//double cont2=x.GetElement (2,1);
	//double cont3=x.GetElement (3,1);

	//VOStrStream mystream;
	//mystream << cont1 << (" ") << cont2 << (" ") << cont3;
	//VpAfxMessageBox (mystream.str().data());

	return x; 
}

void GaussElemination::Pivot(Matrix &a,Matrix &b,int diagonalno)
{
	double MaxEl=fabs(a.GetElement(diagonalno,diagonalno));
	int MaxRow=diagonalno;
	for (int i=diagonalno+1;i<=a.GetRowNo();i++)
	{
		double CurrentEl=fabs(a.GetElement(i,diagonalno));
		int CurrentRow=i;

		if (CurrentEl>MaxEl)
		{
			MaxEl=CurrentEl;
			MaxRow=CurrentRow;
		}
	}

	a.SwapRows(diagonalno,MaxRow);

	b.SwapRows(diagonalno,MaxRow);
}

void GaussElemination::MakeHeadOne(Matrix &a,Matrix &b,int diagonalno)
{	
	double rowhead=a.GetElement(diagonalno,diagonalno);
	int ColNo=a.GetColNo();
	for (int i=diagonalno;i<=ColNo;i++)
	{
		double CurrentElement=a.GetElement(diagonalno,i);
		a.SetElement(CurrentElement/rowhead,diagonalno,i);
	}	
	
	int bColNo=b.GetColNo();
	for (int j=1;j<=bColNo;j++)
	{
		double CurrentElement2=b.GetElement(diagonalno,j);
		b.SetElement(CurrentElement2/rowhead,diagonalno,j);
	}
}

void GaussElemination::SubtractSuperRow(Matrix &a,Matrix &b,int diagonalno)
{
	for (int n=diagonalno+1;n<=a.GetRowNo();n++)
	{
		double FirstNonzeroElement = a.GetElement(n,diagonalno);
		int ColNo=a.GetColNo();
		
			for (int i=diagonalno;i<=ColNo;i++)
			{
				double myval=a.GetElement(n,i);
				double newval=myval-a.GetElement(diagonalno,i)*FirstNonzeroElement;
				a.SetElement(newval,n,i);		
			}
			
			for (int j=1;j<=b.GetColNo();j++)
			{
				double myval2=b.GetElement(n,j);
				double newval2=myval2-b.GetElement(diagonalno,j)*FirstNonzeroElement;
				b.SetElement(newval2,n,j);
			}		
	}
}

void GaussElemination::BackSubs (Matrix &a,Matrix &b,Matrix &x)
{
	for (int i=a.GetRowNo() ;i>=1 ;i--)
	{
		double xcurr = b.GetElement (i,1);
		for (int j=x.GetRowNo();j>i;j--)
		{
			double f = a.GetElement (i , j);
			xcurr = xcurr - x.GetElement (j,1) * f;
		}
		x.SetElement (xcurr , i , 1);
		//ShowMatrix (x);
	}
}

//void GaussElemination::ShowMatrix (Matrix &a)
//{
//	VOStrStream mystream;
//	for (int i=1;i<=a.GetRowNo();i++)
//	{
//		for (int j=1;j<=a.GetColNo();j++)
//		{
//			double currVal = a.GetElement (i,j);	
//			mystream << currVal << ("  ");
//		}
//		VpAfxMessageBox (mystream.str().data());
//	}
//}
