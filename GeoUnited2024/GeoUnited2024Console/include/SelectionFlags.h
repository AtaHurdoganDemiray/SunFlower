/******************************************************************************
*               File: SelectionFlags.h											  *
*******************************************************************************
*               Description:                                                  *
*                                                                             *
*******************************************************************************
*               History:                                                      *
*  30.07.2003 16:35:59 Created by:    Ata Hurdogan Demiray                    *
*******************************************************************************
*               (C) 2003 by ModuleWorks GmbH                                  *
******************************************************************************/

#ifndef __SelectionFlags_h__
#define __SelectionFlags_h__

#include "Selection.h"

// selection dialog states 
/*
const unsigned int SHOWN = 1;
const unsigned int SATISFIED = 2;
const unsigned int ABORTED = 3;
const unsigned int STARTPACKAGE = 4;
const unsigned int INSERTOTHERSELECTION = 5;
*/

class Model;

class SelectionFlags
{
public:
	typedef enum{SHOWN,SATISFIED,ABORTED,STARTPACKAGE,INSERTOTHERSELECTION} DialogState;
	SelectionFlags();
	virtual ~SelectionFlags();
	void SetDialogState (DialogState newval);
	void SetDoSelectionType (Selection::SelectionType newval);
	void SetSelectionMode ( bool newval );
	DialogState GetDialogState ();
	Selection::SelectionType GetDoSelectionType ();
	bool GetSelectionMode ();
	void SetEndModelSelection ( bool newval );
	void SetBeginModelSelection ( bool newval );	
	bool GetEndModelSelection();
	bool GetBeginModelSelection();
	void SetSelectWithBox(bool newval);
	bool GetSelectWithBox ();
	void IncSelModelNoByOne();
	void IncSelModelNo(int toIncrement);
	void DecSelModelNoByOne();
	void ResetSelModelNo();
	int GetSelModelNo();
	Selection::SelectionType GetInsertSelectionType();
	void SetInsertSelectionType(Selection::SelectionType newtype);
	void SetSelectSurfaceNormal (bool newval);
	bool GetSelectSurfaceNormal ();
	void SetSelectEdge (bool newval);
	bool GetSelectEdge ();
	bool GetSketchPoint()const;
	void SetSketchPoint(bool newval);
	bool GetSelectAnyVertex()const;
	void SetSelectAnyVertex (bool newval);
	bool IsDimActive()const;
	void SetIsDimActive (bool isdimactive);
	bool GetDrawDirectionsActive ()const;
	void SetDrawDirectionsActive (bool isdrawdirectionsactive);
	int * GetPSelModelNo();
private:
	DialogState dialogstate;
	Selection::SelectionType doselectiontype;
	bool selectionmode;
	bool endmodelselection;
	bool beginmodelselection;
	bool selectwithbox;
	int selmodelno;
	Selection::SelectionType insertselectiontype;
	bool selectsurfacenormal;
	bool selectedge;
	bool sketchpoint;
	bool selectanyvertex;
	bool isdim;
	bool m_drawDirectionsActive;
};


#endif //__SelectionFlags_h__
