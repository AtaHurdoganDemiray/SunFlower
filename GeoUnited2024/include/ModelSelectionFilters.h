/******************************************************************************
*               File: ModelSelectionFilters.h											  *
*******************************************************************************
*               Description:                                                  *
*                                                                             *
*******************************************************************************
*               History:                                                      *
*  14.09.2003 12:17:04 Created by:    Ata Hurdogan Demiray                    *
*******************************************************************************
*               (C) 2003 by Pozitif GmbH                                      *
******************************************************************************/

#ifndef __ModelSelectionFilters_h__
#define __ModelSelectionFilters_h__

#include "SelectableTypesVector.h"
struct VColor;
class SelectableTypes;

class ModelSelectionFilters
{
public:
	ModelSelectionFilters();
	~ModelSelectionFilters();
	void AddSelectableType ( const SelectableTypes & st );
	//bool IsTypeSelectable ( VStr mytype ) const;
	void ClearSelectableTypesVector ();

	long Size() const;
	SelectableTypes * operator[] ( int index );
	SelectableTypesVector * GetSelectableTypesVector ();
private:
	SelectableTypesVector sel_model_type;

};


#endif //__ModelSelectionFilters_h__
