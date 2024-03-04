// TPointerArray.h

#ifndef __TPointerArray_h__
#define __TPointerArray_h__

#include "VTPointer.h"
#include "Bifstream.h"
#include "Bofstream.h"

template <class T> class TPointerArray
{
public:

	inline TPointerArray ( const long &newcapacity )
	{
		if ( newcapacity > VGlobalValues::MAX_ARRAY_CAPACITY || newcapacity < 0 )
			throw ( VException ( "Array over flow" , 0 ) );

		capacity = newcapacity;
		marray = new VTPointer<T> [ capacity ];
		
		for ( long i=0; i < capacity; i++ )
			marray[i] = 0;

	};
	
	//#############################################################################

	inline TPointerArray ( const TPointerArray<T> &Foo )
	{
		if ( Foo.capacity > VGlobalValues::MAX_ARRAY_CAPACITY || Foo.capacity < 0 )
			throw ( VException ( "Array over flow" , 1  ) );

		capacity = Foo.capacity;
		marray = new VTPointer<T> [ capacity ];
		Copy ( Foo );
	};

	//#############################################################################
	
	inline TPointerArray ( VIFStream & infile )
	{
		int cpc;
		infile>>cpc;
		
		if ( cpc > VGlobalValues::MAX_ARRAY_CAPACITY || cpc < 0 )
			throw;
		
		capacity = cpc;
		marray = new VTPointer<T> [capacity];
		
		for ( int i=0; i < capacity; i++ )
		{
			T * newitem = new T ( infile );
			marray[i] = newitem;
		}
    };

	inline TPointerArray ( Bifstream & binfile )
	{
		int cpc;
		binfile.read ( (VChr * ) &cpc , sizeof (int) );
		
		if ( cpc > VGlobalValues::MAX_ARRAY_CAPACITY || cpc <= 0 )
			throw;
		
		capacity = cpc;
		marray = new VTPointer<T> [capacity];
		
		for ( int i=0; i < capacity; i++ )
		{
			T * newitem = new T ( binfile );
			marray[i] = newitem;
		}
	};

	inline void Copy ( const TPointerArray<T> &source )
	{
		if ( source.capacity > capacity )
			throw ( VException ( "Source array is larger" , 3 ) );
			
		for ( long i = 0; i < source.capacity; i++ )
		{
			marray[i] = source.marray[i];
		}
        for ( long i=source.capacity; i<capacity; i++)
		{
			marray[i]=0;
		}
	};

	//#############################################################################

	inline ~TPointerArray()
	{
		for ( int i = 0; i<capacity; i++ )
		{
			T* curritem = marray[i];
			delete curritem;
		}
		
		delete [] marray;
	};

	//#############################################################################

	inline long Capacity ()const
	{
		return capacity;
	};

	//#############################################################################

	inline VTPointer<T> operator[] ( const long &i ) const		
	{
		if ( i < 0 || i >= capacity )
			throw;// ( VException ( "exp" , 1 ) );

		return marray[i];
	};

	//#############################################################################
	
	inline void SetItem ( VTPointer<T> newitem , const long &i)
	{
		if ( i < 0 || i > capacity )
			throw; // ( VException ( "Invalid index" , 2 ) );
	
		marray[i] = newitem;

	}

	//#############################################################################
	
	inline void operator >> ( VofStream & outfile ) const
	{
		int currsize = GetFilledCapacity();
		//int currsize = capacity;

		outfile<<currsize<<"\n";
		
		for ( int i=0; i < currsize; i++ )
		{
			(*(marray[i])) >> outfile;
		}
	};

	inline void write ( Bofstream & binfile ) const
	{
		int currsize = GetFilledCapacity();
		//int currsize = capacity;

		binfile.write ( (VChr *) & (currsize) , sizeof (int) );
		
		for ( int i=0; i < currsize; i++ )
		{
			(*(marray[i])).write ( binfile );
		}
	};

	inline int GetFilledCapacity () const
	{
		int currsize = 0;

		for ( int i=0; i<capacity; i++ )
		{
			if ( (marray[i]) != 0 )
				currsize++;
			else
				break;
		}
		
		return currsize;

	};

private:
	VTPointer<T> * marray;
	long capacity;
};

#endif //__TPointerArray_h__
