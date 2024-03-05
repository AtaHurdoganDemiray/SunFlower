/******************************************************************************
*               File: ModelSelectionFilters.cpp											  *
*******************************************************************************
*               Description:                                                  *
*                                                                             *
*******************************************************************************
*               History:                                                      *
*  14.09.2003 19:33:46 Created by:    Ata Hurdogan Demiray                    *
*******************************************************************************
*               (C) 2003 by Pozitif GmbH                                      *
******************************************************************************/

#include "stdafx.h"
#include "ModelSelectionFilters.h"

#include "VColor.h"
#include "SelectableTypes.h"

ModelSelectionFilters::ModelSelectionFilters ()
{
}

//#############################################################################

ModelSelectionFilters::~ModelSelectionFilters()
{

}

//#############################################################################

void ModelSelectionFilters::AddSelectableType ( const SelectableTypes & st )
{
	sel_model_type.AddItem(st);
}

//#############################################################################

//#############################################################################


long ModelSelectionFilters::Size () const
{
	return sel_model_type.Size();
}

//#############################################################################

SelectableTypes * ModelSelectionFilters::operator [] ( int index )
{
	if ( index < 0 || index > Size() )
		throw ( VException ( ( IDS_ModSelFilExc) , 0 ) );

	return sel_model_type[index];
}

//#############################################################################

SelectableTypesVector * ModelSelectionFilters::GetSelectableTypesVector ()
{
	return &(sel_model_type);
}
