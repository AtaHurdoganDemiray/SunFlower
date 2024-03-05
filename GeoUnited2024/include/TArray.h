
#ifndef __TArray_h__
#define __TArray_h__

#include "Bifstream.h"
#include "Bofstream.h"


template <class T> class TArray
{
public:
	inline TArray()
	{
		used=0;
		capacity=1;
		marray=new T[capacity];
	};

	inline TArray ( const long &newcapacity )
	{
		if ( newcapacity > VGlobalValues::MAX_ARRAY_CAPACITY || newcapacity < 0 )
			throw ( VException ( "Array over flow" , 0 ) );

		used = 0;		
		capacity = newcapacity;
		marray = new T [capacity];
	};

	//#############################################################################

	inline TArray ( const TArray<T> &Foo )
	{
		if ( Foo.capacity > VGlobalValues::MAX_ARRAY_CAPACITY || Foo.capacity < 0 )
			throw ( VException ( "Array over flow" , 1  ) );

		capacity = Foo.capacity;
		marray = new T [capacity];
		Copy ( Foo );
	};

	//#############################################################################

	inline TArray ( VIFStream &infile )
	{
		long tmp;
		infile>>tmp;
		used = tmp;
		infile>>tmp;
		capacity = tmp;

		if ( capacity > VGlobalValues::MAX_ARRAY_CAPACITY || capacity < 0 )
			throw (VException ("T array overflow" , 1));

		marray = new T [capacity];

		for ( int i=0; i < used; i++ )
		{
			T tmp ( infile );
			marray[i] = tmp;
		}
	};

	inline void Serialize (Bifstream &binfile)
	{
		long tmp;
		binfile.read ( (VChr*) &tmp , sizeof (long) );
		used = tmp;
		binfile.read ( (VChr*) &tmp , sizeof (long) );
		capacity = tmp;
		
		if ( capacity > VGlobalValues::MAX_ARRAY_CAPACITY || capacity < 0 )
			throw (VException("TArray overflow",1));
		
		delete []marray;
		marray = new T[capacity];

		for ( int i=0; i < used; i++ )
		{
			T tmp ( binfile );
			marray[i] = tmp;
		}
	};

	inline void Copy ( const TArray<T> &source )
	{
		if ( source.used > capacity )
			throw ( VException ( "Source array is larger" , 3 ) );

		for ( long i = 0; i < source.used; i++ )
		{
			marray[i] = source.marray[i];
		}
		used = source.used;
	};

	//#############################################################################

	inline ~TArray()
	{
		delete [] marray;
	};

	//#############################################################################

	inline long Capacity ()const
	{
		return capacity;
	};

	//#############################################################################

	inline T * operator[] ( const long &i ) const		
	{
		if ( i < 0 || i >= used )
			throw ( VException ( "TArray exception , unused part of array requested" , 1 ) );

		return &(marray[i]);
	};

	//#############################################################################

	inline bool SetItem ( const T & newitem )
	{
		if ( used >= capacity )
			return false;

		marray[used] = newitem;
		used++;
		return true;	
	};

	//#############################################################################

	inline void operator >> ( VofStream & outfile ) const
	{
		outfile<<used<<"\n";
		outfile<<capacity<<"\n";

		for ( int i=0; i < used; i++ )
		{
			marray[i] >> outfile;
		}
	};

	inline void Serialize (Bofstream &binfile)const
	{
		binfile.write ( (VChr*) &used , sizeof(long) );
		binfile.write ( (VChr*) &(capacity) , sizeof (long) );

		for ( int i=0; i < used; i++ )
		{
			marray[i].Serialize (binfile);
		}
	};

	inline long GetUsed ()
	{
		return used;
	};

	inline void SetUsed ( int newused )
	{
		used = newused;
	};

private:
	T * marray;
	long capacity;
	long used;
};

#endif //__TArray_h__