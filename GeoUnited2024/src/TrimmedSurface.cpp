// TrimmedSurface.cpp

#include "stdafx.h"
#include "TrimmedSurface.h"
#include "Test_IgesSerialize.h"
#include "IgesEntityVisitor.h"
#include "IgesEntitySet.h"


TrimmedSurface::TrimmedSurface()
{
	N1 = 0;
	N2 = 0;
	PTO = 0;
	PTS = 0;
	entitytype = 144;
	//PTI = NULL;
}

TrimmedSurface::~TrimmedSurface()
{
	//delete [] PTI;
}

void TrimmedSurface::GetEntityIn ( FileLines & mylines , int poffs)
{
	DoubleVector myvect;
	GetAllParam ( myvect , mylines );
	if (myvect.size() <= 4)
		throw VException (( IDS_TriGetExcNotEno),1);
	PTS = (int) myvect[1] + poffs;
	N1 = (int) myvect[2];
	
	if ( N1 == 0 )
		PTO = 0;
	else
		PTO = (long)(myvect[4]) + poffs;
	

	N2 = (int) myvect[3];

	if ( N2 > 0 )
	{
		//PTI = new long[N2];
		PTI.resize (N2);
	}

	
	int offset = 5;
	if (myvect.size() <= 4 + N2)
		throw VException (( IDS_TriGetExcNotEno),1);
	for ( int i = 0; i < N2; i++ )
	{
		PTI[i] = (long)(myvect[offset + i]) + poffs;
	}
#ifdef IGES_TEST_MODE
	Test_IgesSerialize mytester;
	mytester.Test_WriteEntityOut(*this,myvect);
#endif
}

void TrimmedSurface::WriteEntityOut (std::vector <VStr> &strvec)const
{
	WriteParam (entitytype , strvec);
	WriteParam (PTS , strvec);
	WriteParam (N1 , strvec);
	WriteParam (N2 , strvec);
	WriteParam (PTO , strvec);

	for (int i=0;i<N2;i++)
	{
		WriteParam (PTI[i] , strvec);
	}
}

void TrimmedSurface::GetChildren (std::vector<int> &childVec)const
{
	childVec.push_back (PTS);
	childVec.push_back (PTO);
	int mysize = (int)PTI.size();
	for (int i=0;i<mysize;i++)
	{
		int currchild = PTI[i];
		childVec.push_back (currchild);
	}
	GetDirectoryChild (childVec);
}

void TrimmedSurface::AcceptVisitor (IgesEntityVisitor &visitor)const
{
	visitor.AcceptVirtual (*this);
}

//bool TrimmedSurface::IsPrimitiveEntity()const
//{
//	return false;
//}

//double TrimmedSurface::GetSortNo ()const
//{
//	return 17.0;
//}

IgesEntity * TrimmedSurface::MakeCopyAndAddToSet(const IgesEntitySet &mainSet, IgesEntitySet &newSet)const
{
	if (m_subsetIndex !=0)
		return newSet.GetEntityPointer(m_subsetIndex);
	TrimmedSurface *mycopy = new TrimmedSurface (*this);
	IgesEntity *surf = mainSet.GetEntityPointer (PTS);
	if (surf)
	{
		IgesEntity *newsurf = surf->MakeCopyAndAddToSet (mainSet,newSet);
		mycopy->PTS = newsurf->directorylineno;
	}
	else
		mycopy->PTS =0;

	if (N1)
	{
		IgesEntity *outcrv = mainSet.GetEntityPointer (PTO);
		if (outcrv)
		{
			IgesEntity *newoutcrv = outcrv->MakeCopyAndAddToSet (mainSet,newSet);
			mycopy->PTO = newoutcrv->directorylineno;
		}
		else
			mycopy->PTO=0;
	}
	for (int i=0;i<N2;i++)
	{
		IgesEntity *incrv = mainSet.GetEntityPointer (PTI[i]);
		if (incrv)
		{
			IgesEntity *newincrv = incrv->MakeCopyAndAddToSet(mainSet,newSet);
			mycopy->PTI[i] = newincrv->directorylineno;
		}
		else
			mycopy->PTI[i]=0;

	}

	CopyDE (*mycopy,mainSet,newSet);
	newSet.InsertEntity (mycopy);
	m_subsetIndex = mycopy->directorylineno;
	return mycopy;
}
