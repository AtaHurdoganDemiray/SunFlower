// VPointerArrayAllocator.h

#ifndef __VPointerArrayAllocator_h__
#define __VPointerArrayAllocator_h__

template <class T> class VPointerArrayAllocator
{
public:
	T** GetNewTPointerArray ( const long &newcapacity )const;
	T** GetNewTPointerArray ( const long &newcapacity 
		, const T** oldarray , long &oldcapacity )const;
	void InitArray ( T** newarray , const long &newcapacity )const;
	void InitArray ( T** newarray , const T**oldarray , const long &newcapacity )const;
	
};

#endif //__VPointerArrayAllocator_h__
