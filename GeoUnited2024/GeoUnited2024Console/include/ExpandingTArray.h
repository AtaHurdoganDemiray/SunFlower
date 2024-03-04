// ExpandingTArray.h

#ifndef __ExpandingTArray_h__
#define __ExpandingTArray_h__

#include "TArray.h"

#include "Bifstream.h"
#include "Bofstream.h"

template < class T > class ExpandingTArray
{
public:
	inline ExpandingTArray ( const long &initialcapacity )
	{
		initcpcty = initialcapacity;
		myarray = new TArray<T> ( initcpcty );
	};

	//#############################################################################

	inline ExpandingTArray ( const ExpandingTArray & Foo )
	{
		initcpcty = Foo.initcpcty;
		myarray = new TArray<T> ( *(Foo.myarray) );
	};

	//#############################################################################
	
	inline ExpandingTArray & operator = (const ExpandingTArray &toCopy) 
	{
		initcpcty = toCopy.initcpcty;
		delete myarray;
		myarray = new TArray<T>(*toCopy.myarray);
		return *this;
	};
	
	//#############################################################################

	inline ExpandingTArray ( VIFStream & infile )
	{
		infile>>initcpcty;
		myarray = new TArray<T> ( infile );	
	};

	//#############################################################################

	inline void Serialize(Bifstream &binfile)
	{
		binfile.read((VChr*)&initcpcty , sizeof(long));
		myarray = new TArray<T>;
		myarray->Serialize(binfile);	
	};

	//#############################################################################

	inline ~ExpandingTArray ()
	{
		delete myarray;
	};

	//#############################################################################

	inline long Size()const
	{
		return myarray->GetUsed();
	};

	//#############################################################################

	inline long Capacity ()const
	{
		return myarray->Capacity();
	};

	//#############################################################################

	inline T * operator [] ( const long &i ) const
	{
		return (*myarray)[i];
	};

	//#############################################################################

	inline void AddItem ( const T & newitem )
	{
		try
		{
			bool succes = myarray->SetItem ( newitem );
			
			if ( !succes )
			{
				vrealUpCapacity();
				myarray->SetItem(newitem);
			}
		}
		catch ( int exc )
		{
			throw ( VException ( "ExpandingTPointerArray exception",exc) );
		}

	};

	//#############################################################################

	inline void Clear()
	{
		myarray->SetUsed(0);
	};

	//#############################################################################

	inline void operator >> ( VofStream & outfile ) const
	{
		outfile<<initcpcty;
		(*myarray)>>outfile;
	};

	//#############################################################################

	inline void Serialize (Bofstream &binfile)const
	{
		binfile.write((VChr*)&initcpcty,sizeof(long));
		(*myarray).Serialize(binfile);
	};

private:

	TArray < T > *myarray;
	long initcpcty;

	//#############################################################################

	inline void vrealUpCapacity ()
	{
		int capacity = myarray->Capacity() + initcpcty;
		if ( capacity > VGlobalValues::MAX_ARRAY_CAPACITY )
			throw ( VException ( "array overflow" , 0 ));

		TArray<T> *temp = new TArray<T> ( capacity );
		temp->Copy ( (*myarray) );
		delete myarray;
		myarray = temp;
	};

	//#############################################################################

};



#endif //__ExpandingTArray_h__
