// Boundary.h
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
7+K1  o      PSCPTK1    Pointer   Pointer to the DE of the last associated parameter space curve
                                 entity of the collection for the first model space trimming curve
..           .            .
.   

...  CRVPTN      Pointer   Pointer to the DE of the last model space curve entity in this
                                   Boundary Entity
...           ..
*/

#ifndef __Boundary_h__
#define __Boundary_h__

#include "IgesEntity.h"

class Boundary:public IgesEntity
{
public:
	Boundary();
	~Boundary();
	virtual void GetEntityIn ( FileLines &mylines , int poffs);
	virtual void WriteEntityOut (std::vector <VStr> &strvec)const;		
	virtual void GetChildren (std::vector<int> &childVec)const;
	virtual void AcceptVisitor (IgesEntityVisitor &visitor)const;
	//virtual bool IsPrimitiveEntity()const;
	//virtual double GetSortNo ()const;
	virtual IgesEntity * MakeCopyAndAddToSet (const IgesEntitySet &mainSet , IgesEntitySet &newSet)const;

	int m_TYPE;
	int m_PREF;
	int m_SPTR;
	int m_N;
	std::vector <int> m_CRVPT;
	std::vector<int> m_SENSE;
	std::vector<int> m_K;
    std::vector<std::vector<int> > m_PSCPT;

};


#endif //__Boundary_h__
