// GenericVec.h

#ifndef __GenericVec_h__
#define __GenericVec_h__

//#include <vector>


template <typename T> class GenericVec
{
public:
	inline GenericVec()
	{

	};

	inline GenericVec ( const GenericVec <T> & Foo )
	{
		for ( int i=0; i < Foo.Size(); i++ )
		{
			T * newitem = Foo[i];
			InsertItem ( *newitem );
		}
	};

	~GenericVec ()
	{
		for ( int i=0; i < Size(); i++ )
		{
			T * curritem = data[i];
			delete curritem;
		}
	};

	inline void InsertItem ( const T & newitem )
	{
		T * mynewitem = new T ( newitem ); 
		data.push_back ( mynewitem );	
	};

	T * operator [] ( int index )
	{
		if ( index < 0 || index >= Size() )
			throw ( VException ( "GenericVec exception" , 0 ) );
	
		return ( data[index] );
	};

	int Size ()const
	{
		return data.size();
	};

protected:
	std::vector <T *> data;
};

#endif