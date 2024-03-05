/******************************************************************************
*               File: Selection.h											  *
*******************************************************************************
*               Description:                                                  *
*                                                                             *
*******************************************************************************
*               History:                                                      *
*  05.07.2003 18:35:03 Created by:    Ata Hurdogan Demiray                    *
*******************************************************************************
*               (C) 2003 by ModuleWorks GmbH                                  *
******************************************************************************/

#ifndef __Selection_h__
#define __Selection_h__

class SelectionInfo;
struct DoubleList;
struct VPoint;
#include "SelectableTypes.h"

// selection types
/*
// insert selection types
const unsigned int	TWOPOINT = 1;
const unsigned int POINTSELECTION = 1;
const unsigned int LINESELECTION = 2;
const unsigned int PLANESELECTION = 3;
const unsigned int MODELSELECTION = 4;
const unsigned int VECTORSELECTION = 5;
*/
class Selection
{
public:
	typedef enum{NOSELECTION,SCALERSELECTION,POINTSELECTION,LINESELECTION,PLANESELECTION,MODELSELECTION,VECTORSELECTION,TWOPOINT,VOLUMEBOX,DRAGSELECTION,COORDINATESELECTION,SURFPOINTNAVI,MOVEONWORKPLANE,FREESELECTION} SelectionType;
	Selection( SelectionType seltype );
	virtual ~Selection();
	// gets screen coordinates by selinfo
	// sets view volume
	// every type of selection calls open gl draw functions in selection mode
	// selinfo is filled accordingly
	//virtual bool DoSelection ( SelectionInfo & selinfo ) const=0;
	// gets screen coordinates of box by selinfo
	// sets view volume
	// every type of selection calls open gl draw functions in selection mode
	// selinfo is filled accordingly
	//virtual bool DoBoxSelection ( SelectionInfo & selinfo ) const=0;

	void SetSatisfied(bool myval);
	bool GetSatisfied () const;
	SelectionType GetSelectionType() const;
	SelectionInfo * GetSelInfo ();
	//void FillPointInfo( DoubleList & seldata );
	//VPoint GetSelectedPoint() const;	
	void SetDescriptionTxt(const VStr &description);
	const VStr & GetDescriptionTxt()const;
	void SetSelectionType (SelectionType newtype);
	void SatisfyScalerSelection( const vreal &myscaler );
	void SatisfyPointSelection( const vreal selpoint[3] );
	void SatisfyVectorSelection( const vreal selvect1[3] , const vreal selvect2[3] );
	void SatisfyModelSelection();
	void Clear();
	bool GetBoxSelectionDisabledFlag ()const;
	void SetBoxSelectionDisabledFlag (bool isBoxSelectionDisabled);
private:
	bool satisfied;
	//unsigned int selectiontype;
	SelectionType m_selectiontype;
	SelectionInfo * selinfo;
	VStr m_description;
	bool m_boxSelectionDisabledFlag;
	SelectableTypes m_selectableTypes;
public:
	const SelectableTypes & GetSelectableTypes()const;
	void SetSelectableTypes (const SelectableTypes &types);
};

#endif //__Selection_h__
		