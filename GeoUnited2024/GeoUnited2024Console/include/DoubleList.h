/******************************************************************************
*               File: DoubleList.h											  *
*******************************************************************************
*               Description:                                                  *
*                                                                             *
*******************************************************************************
*               History:                                                      *
*  02.07.2003 16:16:52 Created by:    Ata Hurdogan Demiray                    *
*******************************************************************************
*               (C) 2003 by Pozitif GmbH                                      *
******************************************************************************/

#ifndef __DoubleList_h__
#define __DoubleList_h__

#include "BasicDefinitions.h"

typedef std::list <vreal>::iterator position;

struct DoubleList
{
public:
	DoubleList();
	~DoubleList();
	void AddTail ( const vreal & newelement );
	vreal GetHead();
	vreal GetTail();
	position GetHeadPosition();
	position GetTailPosition(); 
	vreal GetNext ( position & pos );
	vreal GetPrev ( position & pos );
	int GetCount();
	bool GetPointAtPos ( vreal point[] , position & pos );
	bool GetPoint2dAtPos ( vreal point[] , position & pos );
	void AddPoint3D ( const vreal point[] );
	void AddPoint2D ( const vreal point[] );
	vreal Iterate();
	vreal IterateBackwards();
	void RemoveAll ();
	position End();
private:
	std::list <vreal> mydata;
	std::list <vreal>::iterator posit;
	bool iterating;
};

#endif //__DoubleList_h__

