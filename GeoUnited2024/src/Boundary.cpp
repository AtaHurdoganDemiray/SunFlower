// Boundary.cpp
// Boundary , Entitytype = 141
/*Parameter Data
Index__      Name____     Type___   Description___
1            TYPE        Integer   The type of bounded surface representation:
                                    0 = The boundary entities may only reference model space trim-
                                    ming curves. The associated surface representation (located by
                                    SPTR) may be parametric.
                                    1 = The boundary entities must reference model space curves
                                    and associated parameter space curve collections.  The associ-
                                    ated surface (located by SPTR) must be a parametric represen-
                                    tation.
2            PREF        Integer   Indicates the preferred representation of the trimming curves in
                                   the sending system:
                                    0 = Unspecified
                                    1 = Model space
                                    2 = Parameter space
                                    3 = Representations are of equal preference
3            SPTR        Pointer   Pointer  to  the  DE  of  the  untrimmed  surface  entity  to  be
                                   bounded. If associated parameter space curves are being trans-
                                   ferred (TYPE = 1) the surface representations must be paramet-
                                   ric.
4            N            Integer   Number of curves included in this boundary entity
5            CRVPT1      Pointer   Pointer to the DE of the first model space curve entity of this
                                   Boundary Entity
6            SENSE1      Integer   An orientation flag indicating whether the direction of the first
                                   model space curve should be reversed before use in the bound-
                                   ary. The possible values for the sense flag are:
                                    1 = The direction of the model space curve does not require
                                    reversal
                                    2 = The direction of the model space curve needs to be reversed
7            K1           Integer   A count of the associated parameter space curves in the collec-
                                    tion for the first model space trimming curve. In the case of a
                                    TYPE = 0 transfer, this count must be zero.
8            PSCPT1      Pointer   Pointer to the DE of the first associated parameter space entity
                                   curve of the collection for the first model space trimming curve
..           .            .
.            ..           ..
7+K1        PSCPTK1    Pointer   Pointer to the DE of the last associated parameter space curve
                                 entity of the collection for the first model space trimming curve
..           .            .
.   

...  CRVPTN      Pointer   Pointer to the DE of the last model space curve entity in this
                                   Boundary Entity
...           ..
*/

#include "stdafx.h"
#include "Boundary.h"
#include "Test_IgesSerialize.h"
#include "IgesEntityVisitor.h"
#include "IgesEntitySet.h"


Boundary::Boundary()
{
	entitytype = 141;
	//m_CRVPT = NULL;
	//m_PSCPT = NULL;
}

//#############################################################################

Boundary::~Boundary()
{
	//delete [] m_CRVPT;
	//delete [] m_K;

	//for ( int i = 0; i < m_N; i++ )
	//	delete [] m_PSCPT[i];
}

//#############################################################################

void Boundary::GetEntityIn( FileLines &mylines , int poffs)
{
	DoubleVector myvect;
	GetAllParam ( myvect , mylines );

	m_TYPE = (int)myvect[1];
	m_PREF = (int)myvect[2];
	m_SPTR = ((int)myvect[3] + poffs);
	m_N = (int)myvect[4];

	// newing model space curves
	m_CRVPT.resize(m_N); //= new long[m_N];
	// newing SENSE
	m_SENSE.resize (m_N); //= new int[m_N];
	// newing K
	m_K.resize(m_N); //= new int[m_N];
	// newing parameter space curves
	m_PSCPT.resize(m_N); //= new long*[m_N];

	int offset = 0;
	int vecsize = (int)myvect.size();
	// loop for all curves
	for ( int i = 0; i < m_N; i++ )
	{
		if (vecsize<=(7+offset))
			throw VException (( IDS_BouGetExcNotEno),1);

		m_CRVPT[i] = (int)(myvect[5+offset] + poffs);
		m_SENSE[i] = (int)myvect[6+offset];
		m_K[i] = (int)myvect[7+offset];
		// newing parameter space curves 
		if ( m_K[i] > 0 )
		{
			m_PSCPT[i].resize(m_K[i]);// = new long[m_K[i]];
		}
		// filling parameter space curve pointer array
		for ( int j = 0; j < m_K[i]; j++ )
		{
			if (vecsize <= (8+j+offset))
				throw VException (( IDS_BouGetExcNotEno),1);
			m_PSCPT[i][j] = (int) (myvect[8+j+offset] + poffs);
		}

		offset += (3 + m_K[i]);
		
	}
#ifdef IGES_TEST_MODE
	Test_IgesSerialize mytester;
	mytester.Test_WriteEntityOut (*this,myvect);
#endif
}

//#############################################################################

void Boundary::WriteEntityOut (std::vector<VStr> &strvec)const
{
	WriteParam (entitytype , strvec);
	WriteParam (m_TYPE , strvec);
	WriteParam (m_PREF , strvec);
	WriteParam (m_SPTR , strvec);
	WriteParam (m_N , strvec);
	
	for (int i=0; i<m_N; i++)
	{
		WriteParam (m_CRVPT[i] , strvec);
		WriteParam (m_SENSE[i] , strvec);
		WriteParam (m_K[i] , strvec);
		for (int j=0;j<m_K[i];j++)
		{
			WriteParam (m_PSCPT[i][j] , strvec);
		}
	}
}

void Boundary::GetChildren (std::vector<int> &childVec)const
{
	childVec.push_back (m_SPTR);
	for (int i = 0; i<m_N;i++)
	{
		int currCrvPt = m_CRVPT[i];
		childVec.push_back (currCrvPt);
		int currK = m_K[i];
		for (int j=0;j<currK;j++)
		{
			int currPsc = m_PSCPT[i][j];
			childVec.push_back (currPsc);
		}
	}
	GetDirectoryChild (childVec);
}

void Boundary::AcceptVisitor (IgesEntityVisitor &visitor)const
{
	visitor.AcceptVirtual (*this);
}

//bool Boundary::IsPrimitiveEntity()const
//{
//	return false;
//}

//double Boundary::GetSortNo ()const
//{
//	return 11.0;
//}

IgesEntity * Boundary::MakeCopyAndAddToSet(const IgesEntitySet &mainSet, IgesEntitySet &newSet)const 
{
	if (m_subsetIndex != 0)
		return newSet.GetEntityPointer (m_subsetIndex);

	Boundary *mycopy = new Boundary(*this);
	for (int i = 0; i<m_N;i++)
	{
		int currCrvPt = m_CRVPT[i];
		IgesEntity * currCrv = mainSet.GetEntityPointer(currCrvPt);
		if (currCrv)
		{
			IgesEntity * newCrv = currCrv->MakeCopyAndAddToSet(mainSet,newSet);
			mycopy->m_CRVPT[i] = newCrv->directorylineno;
		}
		
		int currK = m_K[i];
		for (int j=0;j<currK;j++)
		{
			int currPsc = m_PSCPT[i][j];
			IgesEntity * currParCrv = mainSet.GetEntityPointer (currPsc);
			if (currParCrv)
			{
				IgesEntity * newParCrv = currParCrv->MakeCopyAndAddToSet(mainSet,newSet);
				mycopy->m_PSCPT[i][j] = newParCrv->directorylineno;
			}
		}
	}
	
	IgesEntity * surf = mainSet.GetEntityPointer (m_SPTR);
	IgesEntity * newsurf = surf->MakeCopyAndAddToSet (mainSet,newSet);
	mycopy->m_SPTR = newsurf->directorylineno;
	CopyDE (*mycopy,mainSet,newSet);
	newSet.InsertEntity (mycopy);
	m_subsetIndex = mycopy->directorylineno;
	return mycopy;
}
