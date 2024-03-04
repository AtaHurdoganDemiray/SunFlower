// SelectableTypesVector.h

#ifndef __SelectableTypesVector_h__
#define __SelectableTypesVector_h__

//#include "ExpandingTPointerArray.h"

class SelectableTypes;

class SelectableTypesVector
{
public:
	SelectableTypesVector ();
	//SelectableTypesVector ( int initialcapacity );
	~SelectableTypesVector();
	long Size() const;
	SelectableTypes * operator[] ( int index );
	void AddItem ( const SelectableTypes & newitem );
protected:
	//ExpandingTPointerArray<SelectableTypes> vec;
	std::vector<SelectableTypes *>vec;
};

#endif