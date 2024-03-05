// ProcessedTabulatedCylinder.cpp

#include "stdafx.h"
#include "ProcessedTabulatedCylinder.h"
#include "IgesEntitySet.h"
#include "TabulatedCylinder.h"

ProcessedTabulatedCylinder::ProcessedTabulatedCylinder ( const TabulatedCylinder & _surf , IgesEntitySet & _allent )
:surf ( _surf ),allent(_allent)
{
	crv = allent.GetEntityPointer ( surf.m_DE );
}
