// VTPointer.cpp

#include "stdafx.h"
#include "VTPointer.h"
#include "TPointerArray.h"
#include "VPoint.h"
#include "ExpandingTPointerArray.h"


int MyF ( int * aptr )
{
	int result = (*aptr)*2;
	return result;
}

void MyFunc()
{

    TPointerArray <int *> myarr ( 15 );
    TPointerArray <int *> myarr2 ( myarr );
	
    int * myint = new int ( 1 );
    VTPointer <int *> aptr ( myint );

    myarr.SetItem ( aptr , 2 );
    MyF ( aptr );

    VPoint * mypt = new VPoint;
	
    ExpandingTPointerArray <int *> smartarray ( 10 );
    int *array = new int [50];

    for ( int i=0; i<50; i++ )
    {
        array[i] = i;
        smartarray.AddItem ( & (array[i]) );
    }


	
}


