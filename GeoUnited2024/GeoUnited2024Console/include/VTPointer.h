// VTPointer.h

#ifndef __VTPointer_h__
#define __VTPointer_h__

template < class T > class VTPointer
{
public:
	inline VTPointer()
	{	
		pdata = 0;
	};

	//#############################################################################

	inline VTPointer ( T * newdata )
	{
		pdata = newdata;
	};
	
	//#############################################################################
	
	inline operator T *()
	{
		return pdata;
	};

	//#############################################################################
	
	inline T * operator = ( T * Foo )
	{
		pdata = Foo;
		return pdata;
	};

	inline T * operator -> () 
	{
		return pdata;
	};

private:
	T * pdata;
};

#endif //__VTPointer_h__
