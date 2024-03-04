// Triangles.cpp

#include "stdafx.h"
#include "Triangles.h"
#include "TransformationMatrix.h"

Triangles::Triangles ()
{

}

//Triangles::Triangles (const Triangles &toCopy)
//{

//}

Triangles::~Triangles()
{

}

//Triangles & Triangles::operator = (const Triangles & toCopy)
//{
//	return *this;
//}

unsigned int Triangles::GetPointer ()const
{
	Triangles * tmp = (Triangles *)(this);
	unsigned int result = (unsigned int)(tmp);
	return result;
}