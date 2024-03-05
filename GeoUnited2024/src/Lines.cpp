// Lines.cpp

#include "stdafx.h"
#include "Lines.h"

Lines::Lines ()
{

}

Lines::Lines (const Lines &toCopy)
{

}

Lines & Lines::operator = (const Lines &toCopy)
{
	return *this;
}

Lines::~Lines()
{

}
unsigned int Lines::GetPointer ()const
{
	Lines * tmp =(Lines *)(this);
	unsigned int result = (unsigned int)(tmp);
	return result;
}
