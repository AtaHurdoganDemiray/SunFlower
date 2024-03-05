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

#include "stdafx.h"
#include "DoubleList.h"


const vreal NULL_NUMBER = -1 / EPSILON;

DoubleList::DoubleList()
{
	iterating = false;
}

//#############################################################################

DoubleList::~DoubleList()
{

}

//#############################################################################

position  DoubleList::GetHeadPosition() 
{
	return mydata.begin();
}

//#############################################################################

position  DoubleList::GetTailPosition() 
{
	return mydata.end();
}

//#############################################################################

int DoubleList::GetCount() 
{
	return mydata.size();
}

//#############################################################################

vreal DoubleList::GetHead() 
{
	if ( !mydata.size() )
		return NULL_NUMBER;

	return *(mydata.begin());
}

//#############################################################################

vreal DoubleList::GetTail() 
{
	if ( !mydata.size() )
		return NULL_NUMBER;	

	return *(mydata.end());
}

//#############################################################################

vreal DoubleList::GetNext ( position &pos ) 
{
	if ( !mydata.size() )
		return NULL_NUMBER;

	vreal returnval = *(pos);

//	if ( pos != mydata.end() )
//		pos++;
//	else
//		pos = (position)(NULL);

	//VC8 change
	pos++;

	return returnval;
}

//#############################################################################

vreal DoubleList::GetPrev ( position &pos ) 
{
	if ( !mydata.size() )
		return NULL_NUMBER;

	vreal returnval = *(pos);
	
	if ( pos != mydata.begin() )
		pos--;
	else
	{
		//pos = NULL;
		//VC8 change
		pos = mydata.end();
	}
	
	return returnval;
}

//#############################################################################

void DoubleList::AddTail ( const vreal & newitem )
{
	try
	{
		mydata.push_back ( newitem );
	}

	catch (CMemoryException* e)
	{
		throw VException (( IDS_MemExcInDouAddS),1);
	}
	catch (CFileException* e)
	{
		throw VException (( IDS_FilExcInDouAdd),1);
	}
	catch (CException* e)
	{
		throw VException (( IDS_UnkExcInDouAddS),1);	
    }

	catch (...)
	{
		throw VException (( IDS_UnkExcInDouAddS),1);
	}
}

//#############################################################################

bool DoubleList::GetPointAtPos( vreal point[] , position &pos ) 
{
   	if ( pos != mydata.end() )
	{
		point[0] = *(pos);
		//if ( pos != mydata.end() )
		//	pos++;
		//else
		//	pos = NULL;
		
		//VC8 change
		pos++;
	}
	else
		return false;

	if ( pos != mydata.end() )
	{
		point[1] = *(pos);
		//if ( pos != mydata.end() )
		//	pos++;
		//else
		//	pos = NULL;

		//VC8 change
		pos++;
	}
	else
		return false;

	if ( pos != mydata.end() )
	{
		point[2] = *(pos);
		//if ( pos != mydata.end() )
		//	pos++;
		//else
		//	pos = NULL;

		//VC8 change
		pos++;
	}
	else
		return false;

	return true;
}

//#############################################################################

bool DoubleList::GetPoint2dAtPos( vreal point[] , position &pos ) 
{
   	if ( pos != mydata.end() )
	{
		point[0] = *(pos);
		//if ( pos != mydata.end() )
		//	pos++;
		//else
		//	pos = NULL;

		//VC8 change
		pos++;
	}
	else
		return false;

	if ( pos != mydata.end() )
	{
		point[1] = *(pos);
		//if ( pos != mydata.end() )
		//	pos++;
		//else
		//	pos = NULL;

		//VC8 change
		pos++;
	}
	else
		return false;

	return true;
}

//#############################################################################

vreal DoubleList::Iterate()
{

	if ( !mydata.size() )
	{
		return NULL_NUMBER;
	}
	else
	{
		if (iterating)
		{
			if ( posit != mydata.end() )
			{
				return *(posit++);
			}
			else
			{
				iterating = false;
				posit = mydata.end();
				return NULL_NUMBER;
			}
		}
		else
		{
			posit = mydata.begin();
			iterating = true;
			return *posit;
		}
	}
}

//#############################################################################

vreal DoubleList::IterateBackwards()
{
	if ( !mydata.size() )
	{
		return NULL_NUMBER;
	}
	else
	{
		if (iterating)
		{
			if ( posit != mydata.begin() )
			{
				return *(posit--);
			}
			else
			{
				iterating = false;
				posit = mydata.end();
				return NULL_NUMBER;
			}
		}
		else
		{
			posit = mydata.end();
			iterating = true;
			return *posit;
		}
	}
}

//#############################################################################

void DoubleList::AddPoint3D ( const vreal point[] )
{
	AddTail ( point[0] );
	AddTail ( point[1] );
	AddTail ( point[2] );
}

//#############################################################################

void DoubleList::AddPoint2D ( const vreal point[] )
{
	AddTail ( point[0] );
	AddTail ( point[1] );
}

//#############################################################################

void DoubleList::RemoveAll ()
{
	// Clears the list
	mydata.clear();
}

//#############################################################################

position DoubleList::End ()
{
	return mydata.end();
}
