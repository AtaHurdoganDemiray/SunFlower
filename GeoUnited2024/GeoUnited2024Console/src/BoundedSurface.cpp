// BoundedSurface.cpp
// Entity Type 143
/*
Parameter Data:
Index__      Name____     Type___   Description___
1            TYPE        Integer   The type of bounded surface representation:
                                    0 = The boundary entities may only reference model space
                                    curves.   The  associated  surface  representation  (located  by
                                    SPTR) may be parametric.
                                    1 = The boundary entities must reference both model space
                                    curves and the associated parameter space curve collections.
                                    The associated surface (located by SPTR) must be a parametric
                                    representation.
2            SPTR        Pointer   Pointer  to  the  DE  of  the  untrimmed  surface  entity  to  be
                                   bounded. If parameter space trimming curves are being trans-
                                   ferred (TYPE = 1) the surface representations must be paramet-
                                   ric.
3            N            Integer   The number of boundary entities
4            BDPT1       Pointer   Pointer to the DE of the first Boundary Entity (Type 141)
..           .            .
.            ..           ..
3+N         BDPTN       Pointer   Pointer to the DE of the last Boundary Entity (Type 141)

*/

#include "stdafx.h"
#include "BoundedSurface.h"
#include "Test_IgesSerialize.h"
#include "IgesEntityVisitor.h"
#include "IgesEntitySet.h"


BoundedSurface::BoundedSurface()
{
	entitytype = 143;
	//m_BDPT = NULL;
}

//#############################################################################

BoundedSurface::~BoundedSurface()
{
	//delete [] m_BDPT;
}

//#############################################################################

void BoundedSurface::GetEntityIn ( FileLines &mylines , int poffs)
{
	DoubleVector myvect;
	GetAllParam ( myvect , mylines );
	
	if (myvect.size() <= 3)
		throw VException (( IDS_BouGetExcNotEno1),1);
	m_TYPE = (int)myvect[1];
	m_SPTR = (int)myvect[2] + poffs;
	m_N = (int)myvect[3];
	if (myvect.size() <= m_N + 3)
		throw VException (( IDS_BouGetExcNotEno1),1);
	m_BDPT.resize (m_N); //= new long [m_N];

	for ( long i = 0; i < m_N; i++ )
	{
		m_BDPT[i] = (int)myvect [4+i] + poffs;
	}
#ifdef IGES_TEST_MODE
	Test_IgesSerialize mytester;
	mytester.Test_WriteEntityOut (*this,myvect);
#endif

}

//#############################################################################

void BoundedSurface::WriteEntityOut (std::vector<VStr> &strvec)const
{
	WriteParam (entitytype , strvec);
	WriteParam (m_TYPE , strvec);
	WriteParam (m_SPTR , strvec);
	WriteParam (m_N , strvec);
	for (int i=0; i<m_N; i++)
	{
		WriteParam (m_BDPT[i] , strvec);
	}
}

void BoundedSurface::GetChildren (std::vector<int> &childVec)const
{
	childVec.push_back (m_SPTR);
	for (int i=0; i<m_N; i++)
	{
		int currchild = m_BDPT[i];
		childVec.push_back (currchild);
	}
	GetDirectoryChild (childVec);
}

void BoundedSurface::AcceptVisitor (IgesEntityVisitor &visitor)const
{
	visitor.AcceptVirtual (*this);
}

//bool BoundedSurface::IsPrimitiveEntity()const
//{
//	return false;
//}

//double BoundedSurface::GetSortNo ()const
//{
//	return 18.0;
//}

IgesEntity * BoundedSurface::MakeCopyAndAddToSet(const IgesEntitySet &mainSet, IgesEntitySet &newSet)const
{
	if (m_subsetIndex != 0)
		return newSet.GetEntityPointer (m_subsetIndex);

	BoundedSurface *mycopy = new BoundedSurface(*this);
	for (int i=0; i<m_N; i++)
	{
		int currBouDe = m_BDPT[i];
		IgesEntity *currBou = mainSet.GetEntityPointer (currBouDe);
		if (currBou)
		{
			IgesEntity *newBou = currBou->MakeCopyAndAddToSet (mainSet,newSet);
			mycopy->m_BDPT[i] = newBou->directorylineno;	
		}
	}
	IgesEntity *surf = mainSet.GetEntityPointer(m_SPTR);
	IgesEntity *newsurf = surf->MakeCopyAndAddToSet(mainSet,newSet);
	mycopy->m_SPTR = newsurf->directorylineno;
	CopyDE (*mycopy,mainSet,newSet);
	newSet.InsertEntity (mycopy);
	m_subsetIndex = mycopy->directorylineno;
	return mycopy;
}
