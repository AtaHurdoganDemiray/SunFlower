// SelectableTypesVector.cpp

#include "stdafx.h"
#include "SelectableTypesVector.h"
#include "SelectableTypes.h"

SelectableTypesVector::SelectableTypesVector ()
{
	
}

//SelectableTypesVector::SelectableTypesVector ( int initialcapacity )
//:vec (initialcapacity)
//{
//	
//}

SelectableTypesVector::~SelectableTypesVector ()
{
	size_t mysize = vec.size();
	for (size_t i=0;i<mysize;++i)
	{
		SelectableTypes *curseltypes = vec.at(i);
		delete curseltypes;
	}
}

long SelectableTypesVector::Size () const
{
	return (long)(vec.size());
}

SelectableTypes * SelectableTypesVector::operator []  ( int index )
{
	return (vec.at(index));
}

void SelectableTypesVector::AddItem ( const SelectableTypes & newitem )
{
	vec.push_back ( new SelectableTypes (newitem) );
}



