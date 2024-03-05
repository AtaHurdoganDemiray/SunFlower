// Points.cpp

#include "stdafx.h"
#include "Points.h"

Points::Points ()
{
	
}

Points::Points (const Points &toCopy)
{
	
}

Points & Points::operator = (const Points &toCopy)
{
	return *this;
}

Points::~Points()
{

}
unsigned int Points::GetPointer ()const
{
	Points * tmp = (Points *)(this);
	unsigned int result = (unsigned int)(tmp);
	return result;
}
