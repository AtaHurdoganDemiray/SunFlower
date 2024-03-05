/******************************************************************************
*               File: SelectionFlags.cpp											  *
*******************************************************************************
*               Description:                                                  *
*                                                                             *
*******************************************************************************
*               History:                                                      *
*  30.07.2003 16:39:47 Created by:    Ata Hurdogan Demiray                    *
*******************************************************************************
*               (C) 2003 by ModuleWorks GmbH                                  *
******************************************************************************/

#include "stdafx.h"
#include "SelectionFlags.h"

SelectionFlags::SelectionFlags ()
{
	dialogstate = SATISFIED;
	doselectiontype = Selection::POINTSELECTION;
	selectionmode = false;
	endmodelselection = false;
	beginmodelselection = false;
	selectwithbox = false;
	selmodelno = 0;
	sketchpoint=false;
	selectanyvertex = false;
	insertselectiontype = Selection::POINTSELECTION;
	selectsurfacenormal = true;
	selectedge = false;
	isdim = false;
	m_drawDirectionsActive = false;
}

//#############################################################################

SelectionFlags::~SelectionFlags()
{

}

//#############################################################################

void SelectionFlags::SetDialogState (DialogState newval)
{
	dialogstate = newval;
}

//#############################################################################

void SelectionFlags::SetDoSelectionType ( Selection::SelectionType newval )
{
	doselectiontype = newval;
}

//#############################################################################

void SelectionFlags::SetSelectionMode ( bool newval )
{
	selectionmode = newval;
}

//#############################################################################

SelectionFlags::DialogState  SelectionFlags::GetDialogState ()
{
	return dialogstate;
}

//#############################################################################

Selection::SelectionType SelectionFlags::GetDoSelectionType ()
{
	return doselectiontype;
}

//#############################################################################

bool SelectionFlags::GetSelectionMode ()
{
	return selectionmode;
}

//#############################################################################

void SelectionFlags::SetEndModelSelection ( bool newval )
{
	endmodelselection = newval;
}

//#############################################################################

bool SelectionFlags::GetEndModelSelection()
{
	return endmodelselection;
}

//#############################################################################

void SelectionFlags::SetSelectWithBox ( bool newval )
{
	selectwithbox = newval;
}

//#############################################################################

bool SelectionFlags::GetSelectWithBox ()
{
	return selectwithbox;
}

//#############################################################################

void SelectionFlags::SetBeginModelSelection ( bool newval )
{
	beginmodelselection = newval;
}

//#############################################################################

bool SelectionFlags::GetBeginModelSelection()
{
	return beginmodelselection;
}

//#############################################################################

int SelectionFlags::GetSelModelNo()
{
	return selmodelno;
}

//#############################################################################

void SelectionFlags::IncSelModelNoByOne ()
{
	//selmodelno++;
}

//#############################################################################

void SelectionFlags::DecSelModelNoByOne ()
{
	//selmodelno--;
}

//#############################################################################

void SelectionFlags::ResetSelModelNo()
{
	//selmodelno = 0;
}

//#############################################################################

void SelectionFlags::SetInsertSelectionType ( Selection::SelectionType newtype )
{
	insertselectiontype = newtype;
}

//#############################################################################

Selection::SelectionType SelectionFlags::GetInsertSelectionType()
{
	return insertselectiontype;
}

//#############################################################################

void SelectionFlags::SetSelectSurfaceNormal ( bool newval )
{
	selectsurfacenormal = newval ;
}

//#############################################################################

bool SelectionFlags::GetSelectSurfaceNormal ()
{
	return selectsurfacenormal;
}

//#############################################################################

void SelectionFlags::SetSelectEdge ( bool newval )
{
	selectedge = newval ;
}

//#############################################################################

bool SelectionFlags::GetSelectEdge ()
{
	return selectedge;
}

//#############################################################################

bool SelectionFlags::GetSketchPoint()const
{
	return sketchpoint;
}

//#############################################################################

void SelectionFlags::SetSketchPoint(bool newval)
{
	sketchpoint=newval;
}

//#############################################################################

bool SelectionFlags::GetSelectAnyVertex()const
{
	return selectanyvertex;
}

//#############################################################################

void SelectionFlags::SetSelectAnyVertex (bool newval)
{
	selectanyvertex = newval;
}

//#############################################################################

void SelectionFlags::IncSelModelNo(int toIncrement)
{
	//selmodelno+=toIncrement;
}

//#############################################################################

bool SelectionFlags::IsDimActive()const
{
	return isdim;
}	

//#############################################################################

void SelectionFlags::SetIsDimActive (bool isdimactive)
{
	isdim = isdimactive;
}

//#############################################################################

bool SelectionFlags::GetDrawDirectionsActive() const
{
	return m_drawDirectionsActive;
}

void SelectionFlags::SetDrawDirectionsActive(bool isdrawdirectionsactive)
{
	m_drawDirectionsActive = isdrawdirectionsactive;
}

int * SelectionFlags::GetPSelModelNo()
{
	return &selmodelno;
}
