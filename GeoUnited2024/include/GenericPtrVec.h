// GenericPtrVec.h

#ifndef __GenericPtrVec_h__
#define __GenericPtrVec_h__

//#include <vector>


template <typename T> class GenericPtrVec
{
public:
	inline GenericPtrVec()
	{

	};

	inline GenericPtrVec ( GenericPtrVec <T> & Foo )
	{
		for ( int i=0; i < Foo.Size(); i++ )
		{
			T * newitem = Foo[i];
			InsertItem ( newitem );
		}
	};

	~GenericPtrVec ()
	{
	};
	
	inline GenericPtrVec & operator = (GenericPtrVec <T> & toCopy)
	{
		for ( int i=0; i < toCopy.Size(); i++ )
		{
			T * newitem = toCopy[i];
			InsertItem ( newitem );
		}
		return *this;
	};

	inline void InsertItem ( T * newitem )
	{ 
		data.push_back ( newitem );	
	};

	T * operator [] ( int index )
	{
		if ( index < 0 || index >= Size() )
			throw ( VException ( "GenericVec exception" , 0 ) );
	
		return ( data[index] );
	};

	const T * operator [] ( int index )const
	{
		if ( index < 0 || index >= Size() )
			throw ( VException ( "GenericVec exception" , 0 ) );
	
		return ( data[index] );
	};

	int Size ()const
	{
		return (int)data.size();
	};

protected:
	std::vector <T *> data;

};

#endif