// VPointerArray.h

#ifndef __VPointerArray_h__
#define __VPointerArray_h__

#include "RefTriangle.h"

typedef RefTriangle T;

#include "VPointerArrayAllocator.h"
template<class T> struct VPointerArray
{
public:
	inline VPointerArray ( const long &initialsize )
	{
		VPointerArrayAllocator <T> myallocator;
		mdata = myallocator.GetNewTPointerArray ( initialsize );
		size = 0;
		capacity = initialsize;
	};

	inline virtual ~VPointerArray ()
	{
		delete []mdata;
	};
	
	inline long Size() const
	{
		return size;
	};

	inline long Capacity () const
	{
		return capacity;
	};

	inline const T* operator [] ( const long &index ) const
	{
		if ( index < 0 || index >= Size() )
		return 0;

		return mdata[index];
	};

	inline void AddItem ( T * newitem )
	{
		mdata[size] = newitem;
		size++;
		
		if ( size == capacity )
		{
			 VPointerArrayAllocator <T> myallocator;
			 long newcapacity = capacity*2;
			 T** temp = myallocator.GetNewTPointerArray ( (capacity*2) , mdata , capacity );
			 delete [] mdata;
			 mdata = temp;	
		}

	};

private:
	long size;
	long capacity;
	T** mdata;
};

#endif //__VPointerArray_h__
