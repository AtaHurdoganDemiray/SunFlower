// VPointerArrayAllocator.h

#include "stdafx.h"
#include "VPointerArrayAllocator.h"

template <class T> T** VPointerArrayAllocator<T>::GetNewTPointerArray
( const long &newcapacity ) const
{
	T** newarray = new T*[newcapacity];
	InitArray ( newarray , newcapacity );
	return newarray;
}

//#############################################################################

template <class T> T** VPointerArrayAllocator<T>::GetNewTPointerArray
( const long &newcapacity , const T** oldarray , long &oldcapacity ) const
{
	T** newarray = new T*[newcapacity];
	InitArray ( newarray , oldarray , newcapacity );
	return newarray;
}
//#############################################################################

template <class T> void VPointerArrayAllocator<T>::InitArray 
( T** newarray , const T** oldarray , const long &newcapacity ) const
{
	for ( long i=0; i<newcapacity; i++ )
	{
		newarray[i] = oldarray[i];
	}
}

//#############################################################################

template <class T> void VPointerArrayAllocator<T>::InitArray 
( T** newarray , const long &newcapacity ) const
{
	for ( long i=0; i<newcapacity; i++ )
	{
		newarray[i] = 0;
	}
}

//#############################################################################



