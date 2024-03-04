// ExpandingTPointerArray.h

#ifndef __ExpandingTPointerArray_h__
#define __ExpandingTPointerArray_h__

#include "TPointerArray.h"

#include "Bifstream.h"
#include "Bofstream.h"

template < class T > class ExpandingTPointerArray
{
public:
	inline ExpandingTPointerArray ( const long &initialcapacity )
	{
		initcpcty = initialcapacity;
		capacity = initialcapacity;
		size = 0;
		myarray = new TPointerArray<T> ( capacity );
	};
	
	//#############################################################################
	
	inline ExpandingTPointerArray ( const ExpandingTPointerArray & Foo )
	{
		initcpcty = Foo.initcpcty;
		capacity = Foo.capacity;
		size = Foo.size;
		myarray = new TPointerArray<T> ( *(Foo.myarray) );
	};

	//#############################################################################

	inline ExpandingTPointerArray ( VIFStream & infile )
	{
		myarray = new TPointerArray<T> ( infile );	
		
		initcpcty = myarray->Capacity();
		capacity = initcpcty;
		size = capacity;
	}

	//#############################################################################

	inline ExpandingTPointerArray ( Bifstream & binfile )
	{
		myarray = new TPointerArray<T> ( binfile );	
		
		initcpcty = myarray->Capacity();
		capacity = initcpcty;
		size = capacity;
	}

	//#############################################################################

	inline ~ExpandingTPointerArray ()
	{
		delete myarray;
	}

	//#############################################################################

	inline long Size()const
	{
		return size;
	};

	//#############################################################################

	inline long Capacity ()const
	{
		return capacity;
	};

	//#############################################################################
	
	inline VTPointer<T> operator [] ( const long &i ) const
	{
		return ((*myarray)[i]);
	};

	//#############################################################################

	inline void AddItem ( const T & newitem )
	{
		try
		{
			T * nitem = new T ( newitem ); 
			myarray->SetItem ( nitem , size );
			size++;
		
			if ( size == capacity )
				vrealUpCapacity();
		}
		catch ( int exc )
		{
			throw ( VException ( "ExpandingTPointerArray exception",exc) );
		}

	};

	//#############################################################################
	
	inline void operator >> ( VofStream & outfile ) const
	{
		(*myarray)>>outfile;
	}

	//#############################################################################
	
	inline void write ( Bofstream & binfile ) const
	{
		(*myarray).write ( binfile );
	}

private:
	
	TPointerArray < T > *myarray;
	long initcpcty;
	long capacity;
	long size;

	//#############################################################################

	inline void vrealUpCapacity ()
	{
		capacity += initcpcty;
		if ( capacity > MAX_ARRAY_CAPACITY )
			throw ( VException ( "array overflow" , 0 ));

		TPointerArray<T> *temp = new TPointerArray<T> ( capacity );
		temp->Copy ( (*myarray) );
		
		// we don t want content to be deleted
		for (int i=0;i< myarray->Capacity();i++)
		{
			((*myarray)[i]) = (T*)(0);		
		}
		delete myarray;
		myarray = temp;
	};

	//#############################################################################

};



#endif //__ExpandingTPointerArray_h__
