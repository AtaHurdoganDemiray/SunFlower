/******************************************************************************
*               File: Selection.cpp											  *
*******************************************************************************
*               Description:                                                  *
*                                                                             *
*******************************************************************************
*               History:                                                      *
*  05.07.2003 18:37:32 Created by:    Ata Hurdogan Demiray                    *
*******************************************************************************
*               (C) 2003 by ModuleWorks GmbH                                  *
******************************************************************************/

#include "stdafx.h"
#include "Selection.h"
#include "SelectionInfo.h"
#include "BasicDefinitions.h"
#include "DoubleList.h"
#include "VPoint.h"

Selection::Selection( SelectionType seltype )
{
	satisfied = false;
	m_selectiontype = seltype;
	selinfo = new SelectionInfo;
	//initialising description txt
	if(m_selectiontype==POINTSELECTION)
		m_description=( IDS_NoktaSecin1);
	else if(m_selectiontype==VECTORSELECTION)
		m_description=( IDS_VekTan);
	else if(m_selectiontype==MODELSELECTION)
		m_description=( IDS_ModelSecin);
	else if(m_selectiontype==VOLUMEBOX)
		m_description=( IDS_KutuSecin);
	m_boxSelectionDisabledFlag = false;
}

//#############################################################################

Selection::~Selection()
{
	delete selinfo;
}

//#############################################################################

void Selection::SetSatisfied( bool myval )
{
	satisfied = myval;
}

//#############################################################################

bool Selection::GetSatisfied() const
{
	return satisfied;
}

//#############################################################################

Selection::SelectionType Selection::GetSelectionType() const
{
	return m_selectiontype;
}

//#############################################################################

SelectionInfo * Selection::GetSelInfo()
{
	return selinfo;
}

//#############################################################################
/*
void Selection::FillPointInfo ( DoubleList &seldata )
{
	position pos = seldata.GetHeadPosition();

	vreal thepoint[3];

	bool chk = seldata.GetPointAtPos ( thepoint , pos );

	if ( !chk )
	{
		return;
	}

	selinfo->selpoint.x(thepoint[0]);
	selinfo->selpoint.y(thepoint[1]);
	selinfo->selpoint.z(thepoint[2]);

	return;
}
*/
//#############################################################################
/*
VPoint Selection::GetSelectedPoint() const
{
	return ( selinfo->GetSelectedPoint() );
}
*/
//#############################################################################

void Selection::SetDescriptionTxt(const VStr &description)
{
	m_description=description;
}

const VStr & Selection::GetDescriptionTxt()const
{
	return m_description;
}

void Selection::SetSelectionType(Selection::SelectionType newtype)
{
	m_selectiontype=newtype;
}

void Selection::SatisfyScalerSelection( const vreal &myscaler )
{
	SetSatisfied ( true );
	selinfo->SetSelectedScaler ( myscaler );
}
//#############################################################################

void Selection::SatisfyPointSelection( const vreal selpoint[3] )
{
	SetSatisfied ( true );
	selinfo->SetSelectedPoint ( selpoint );
}

//#############################################################################

void Selection::SatisfyVectorSelection( const vreal selvect1[3] , const vreal selvect2[3] )
{
	SetSatisfied ( true );
	selinfo->SetSelectedVector ( selvect1 , selvect2 );	
}

//#############################################################################

void Selection::SatisfyModelSelection()
{
	SetSatisfied ( true );
}

//#############################################################################

void Selection::Clear()
{
	SetSatisfied (false);
	selinfo->Clear();	
}

bool Selection::GetBoxSelectionDisabledFlag() const
{
	return m_boxSelectionDisabledFlag;
}

void Selection::SetBoxSelectionDisabledFlag(bool isBoxSelectionDisabled)
{
	m_boxSelectionDisabledFlag = isBoxSelectionDisabled;
}

const SelectableTypes & Selection::GetSelectableTypes()const
{
	return m_selectableTypes;
}

void Selection::SetSelectableTypes (const SelectableTypes &types)
{
	m_selectableTypes = types;
}