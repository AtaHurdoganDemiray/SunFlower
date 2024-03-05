// CompositeCurve.cpp , entity type 102
//------------------------------------------------------------------------------------------
// index	name	type	description
// ------	----	----	----------------------------------------------------------------
// 1	|	N	  |	int		|number of entities
//------------------------------------------------------------------------------------------								
// 2	|	DE1	  |	pointer	|pointer to the DE of the first constiuent entity.
//------------------------------------------------------------------------------------------
// 3	|	...	  |	...		|...		
//------------------------------------------------------------------------------------------
// 1+N	|	DEN	  |pointer  |pointer to DE of the last constiuent entity.
//------------------------------------------------------------------------------------------

#include "stdafx.h"
#include "CompositeCurve.h"
#include "Test_IgesSerialize.h"
#include "IgesEntityVisitor.h"
#include "IgesEntitySet.h"


CompositeCurve::CompositeCurve()
{
	entitytype = 102;
	//DE = NULL;
}

CompositeCurve::~CompositeCurve()
{
	//delete [] DE;
}

void CompositeCurve::GetEntityIn( FileLines & mylines , int poffs)
{
	DoubleVector myvect;
	GetAllParam ( myvect , mylines );
	int vecsize = (int)myvect.size();
	if (vecsize <= 1)
		throw VException (( IDS_ComGetExcNotEno),1);
	N = (int) myvect[1];
	
	if ( N > 0 )
		mDE.resize(N);// = new long[N];

	int offset = 2;
	if (vecsize <= N+1)
		throw VException (( IDS_ComGetExcNotEno),1);

	for ( int i=0; i<N; i++ )
	{
		mDE[i] = (long)(myvect[offset+i]) + poffs;
	}
	ReadModelId(offset+N,myvect);

#ifdef IGES_TEST_MODE
	Test_IgesSerialize mytester;
	mytester.Test_WriteEntityOut(*this,myvect);
#endif
}

void CompositeCurve::WriteEntityOut (std::vector <VStr> &strvec )const
{
	WriteParam (entitytype,strvec);
	WriteParam (N , strvec);

	for ( int i=0; i<N; i++ )
	{
		WriteParam (mDE[i],strvec);
	}
	WriteModelId(strvec);
}

void CompositeCurve::GetChildren (std::vector<int> &childVec)const
{
	for ( int i=0; i<N; i++ )
	{
		int currchild = mDE[i];
		childVec.push_back (currchild);
	}
	GetDirectoryChild (childVec);
}

void CompositeCurve::AcceptVisitor (IgesEntityVisitor &visitor)const
{
	visitor.AcceptVirtual (*this);
}

//bool CompositeCurve::IsPrimitiveEntity()const
//{
//	return false;
//}

//double CompositeCurve::GetSortNo ()const
//{
//	return 9.0;
//}

IgesEntity * CompositeCurve::MakeCopyAndAddToSet(const IgesEntitySet &mainSet, IgesEntitySet &newSet) const
{
	if (m_subsetIndex != 0)
		return newSet.GetEntityPointer(m_subsetIndex);

	CompositeCurve *mycopy = new CompositeCurve (*this);
	for ( int i=0; i<N; i++ )
	{
		int currchild = mDE[i];
		IgesEntity *currcrv = mainSet.GetEntityPointer(currchild);
		if (currcrv)
		{
			IgesEntity *newcrv = currcrv->MakeCopyAndAddToSet (mainSet,newSet);
			mycopy->mDE[i] = newcrv->directorylineno;
		}
	}
	CopyDE (*mycopy,mainSet,newSet);
	newSet.InsertEntity (mycopy);
	m_subsetIndex = mycopy->directorylineno;
	return mycopy;
}
