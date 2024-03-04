// EntityPointer.cpp

#include "stdafx.h"
#include "EntityPointer.h"
#include "IgesEntity.h"

EntityPointer::EntityPointer()
{

}

EntityPointer::~EntityPointer()
{

}

long EntityPointer::GetpDE() 
{
	return p_DE;
}

void EntityPointer::SetpDE ( long newpDE ) 
{
	p_DE = newpDE;
}

long EntityPointer::GetpPE ()
{
	return p_PE;
}

void EntityPointer::SetpPE ( long newpPE )
{
	p_PE = newpPE;
}

IgesEntity * EntityPointer::GetEntityPointer() 
{
	return p_entity;
}

void EntityPointer::SetEntityPointer( IgesEntity * newp )
{
	p_entity = newp;
}

