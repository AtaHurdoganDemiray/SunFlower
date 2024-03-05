/******************************************************************************
*               File: DoubleVec.cpp											  *
*******************************************************************************
*               Description:                                                  *
*                                                                             *
*******************************************************************************
*               History:                                                      *
*  06.01.2004 13:45:34 Created by:    Ata Hurdogan Demiray                    *
*******************************************************************************
*               (C) 2003 by Positive GmbH                                  *
******************************************************************************/

#include "stdafx.h"
#include "DoubleVec.h"
#include "BasicDefinitions.h"

DoubleVec::DoubleVec()
{

}

//#############################################################################

DoubleVec::~DoubleVec()
{

}

//#############################################################################

const vreal & DoubleVec::operator [] ( long index ) const
{
	return ( mydata.at(index) );
}

//#############################################################################

long DoubleVec::Size() const
{
	return ( mydata.size() );
}

//#############################################################################

void DoubleVec::Addvreal ( const vreal &item )
{
	mydata.push_back ( item );
}

//#############################################################################

bool DoubleVec::Addvrealif(const vreal item)
{
	if (mydata.size())
	{
		if ( fabs(mydata.back()-item) > EPSILON )
		{
			mydata.push_back(item);
			return true;
		}
		else
			return false;
	}
	else
	{
		mydata.push_back(item);
		return true;
	}
}

void DoubleVec::RemoveAll ()
{
	mydata.clear();
}

//#############################################################################

bool DoubleVec::IsValueARepeat ( vreal value , long index ) const
{
	for ( int i=0; i < index; i++ )
	{
		vreal currvalue = (*this)[i];
		if (fabs(currvalue-value) < EPSILON )
			return true;
	}
	return false;
}

//#############################################################################

bool DoubleVec::IsValueARepeat ( const vreal &value , const vreal &tolerance ) const
{
	for ( unsigned int i=0; i < mydata.size(); i++ )
	{
		vreal currvalue = mydata[i];
		if (fabs(currvalue-value) < tolerance )
			return true;
	}
	return false;
}

void DoubleVec::SortAndRemoveDuplicate()
{
	Sort();
	int mysize = Size();
	DoubleVec tmp;
	vreal prevVal = operator[](0);
	tmp.Addvreal (prevVal);
	for (int i= 1; i<mysize; i++)
	{
		vreal currVal = operator[] (i);
		if (fabs (currVal - prevVal) > EPSILON)
		{
			tmp.Addvreal (currVal);
		}
		prevVal = currVal;
	}
	(*this) = tmp;
}

void DoubleVec::Reserve(const int mysize)
{
	mydata.reserve(mysize);
}

void DoubleVec::Reverse()
{
	std::vector<double> tmp (mydata.rbegin(),mydata.rend());
	size_t mysize = tmp.size();
	for (size_t i=0;i<mysize;++i)
		tmp[i] = 1. - tmp[i];
	mydata = tmp;
}
