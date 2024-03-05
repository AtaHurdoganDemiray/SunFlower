// CurveOnParametricSurface.cpp
// CURVE ON A PARAMETRIC SURFACE ENTITY (TYPE 142)
//------------------------------------------------------------------------------------------
// index	name	type	description
// ------	----	----	----------------------------------------------------------------
// 1	|	CRTN  |	int		|indicates the way the curve on surface has been created
//		|		  |			|0=unspecified 1=projection of a given curve on surface
//		|		  |			|2=intersection of 2 surfaces 3=isoparametric u or v param curve	
//------------------------------------------------------------------------------------------								
// 2	|	SPTR  |	pointer	|pointer to the DE of surface on which the curve lies.
//------------------------------------------------------------------------------------------
// 3	|	BPTR  |	pointer	|pointer to the DE of the entity that contains the definition of
//		|		  |			|the curve B in parametric space (u,v) of the surface.			
//------------------------------------------------------------------------------------------
// 4	|	CPTR  |pointer  |pointer to DE ofcurve C
//------------------------------------------------------------------------------------------
// 5	|   PREF  |	int		|indicates the preffered representation in the sending system.
//		|		  |			|0=unspecified 1=SOB is preffered 2=C is preffered 3=C and SOB
//		|		  |			|equally preffered.
//------------------------------------------------------------------------------------------

#include "stdafx.h"
#include "CurveOnParametricSurface.h"
#include "Test_IgesSerialize.h"
#include "IgesEntityVisitor.h"
#include "IgesEntitySet.h"
#include "CastIf.h"


CurveOnParametricSurface::CurveOnParametricSurface()
{
	entitytype = 142;
	CRTN = 0;
	CPTR = 0;
	SPTR = 0;
	BPTR = 0;
	PREF = 0;
}

CurveOnParametricSurface::~CurveOnParametricSurface()
{

}

void CurveOnParametricSurface::GetEntityIn( FileLines & mylines , int poffs)
{
	DoubleVector myvect;
	GetAllParam ( myvect , mylines );
	if (myvect.size() <= 5)
		throw VException (( IDS_CurGetExcNotEno),1);
	CRTN = (int) myvect[1];
	SPTR = (long) myvect[2] + poffs;
	BPTR = (long) myvect[3] + poffs;
	if (myvect[3] == 0)
		BPTR = 0;
	CPTR = (long) myvect[4] + poffs;
	if (myvect[4] == 0)
		CPTR = 0;
	PREF = (int) myvect[5];
#ifdef IGES_TEST_MODE
	Test_IgesSerialize mytester;
	mytester.Test_WriteEntityOut(*this,myvect);
#endif
}

void CurveOnParametricSurface::WriteEntityOut (std::vector <VStr> &strvec)const
{
	WriteParam (entitytype , strvec);
	WriteParam (CRTN , strvec);
	WriteParam (SPTR , strvec);
	WriteParam (BPTR , strvec);
	WriteParam (CPTR , strvec);
	WriteParam (PREF , strvec);
}

void CurveOnParametricSurface::GetChildren (std::vector<int> &childVec)const
{
	childVec.push_back (SPTR);
	childVec.push_back (BPTR);
	childVec.push_back (CPTR);
	GetDirectoryChild (childVec);
}

void CurveOnParametricSurface::AcceptVisitor (IgesEntityVisitor &visitor)const
{
	visitor.AcceptVirtual (*this);
}

//bool CurveOnParametricSurface::IsPrimitiveEntity()const
//{
//	return false;
//}

//double CurveOnParametricSurface::GetSortNo ()const
//{
//	return 10.0;
//}

IgesEntity * CurveOnParametricSurface::MakeCopyAndAddToSet(const IgesEntitySet &mainSet, IgesEntitySet &newSet)const
{
	if (m_subsetIndex != 0)
		return newSet.GetEntityPointer (m_subsetIndex);

	CurveOnParametricSurface *mycopy = new CurveOnParametricSurface (*this);
	// Surface
	IgesEntity *surf = mainSet.GetEntityPointer(SPTR);
	if (surf)
	{
		IgesEntity *newsurf = surf->MakeCopyAndAddToSet (mainSet,newSet);
		mycopy->SPTR = newsurf->directorylineno;
	}
	else
		mycopy->SPTR =0;

	// BPTR
	if (BPTR)
	{
		IgesEntity *crvb = mainSet.GetEntityPointer(BPTR);
		IgesEntity *newcrvb = crvb->MakeCopyAndAddToSet (mainSet,newSet);
		mycopy->BPTR = newcrvb->directorylineno;
	}
	else
		mycopy->BPTR = 0;
	// CPTR
	if (CPTR)
	{
		IgesEntity *crvc = mainSet.GetEntityPointer (CPTR);
		IgesEntity *newcrvc = crvc->MakeCopyAndAddToSet (mainSet,newSet);
		mycopy->CPTR = newcrvc->directorylineno;
	}
	else 
		mycopy->CPTR = 0;
	
	CopyDE (*mycopy,mainSet,newSet);
	newSet.InsertEntity (mycopy);
	m_subsetIndex = mycopy->directorylineno;
	return mycopy;
}
