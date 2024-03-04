// ProcessedTabulatedCylinder.h

#ifndef __ProcessedTabulatedCylinder_h__
#define __ProcessedTabulatedCylinder_h__

class TabulatedCylinder;
class IgesEntity;
class IgesEntitySet;

class ProcessedTabulatedCylinder
{
public:
	ProcessedTabulatedCylinder ( const TabulatedCylinder & _surf , IgesEntitySet & _allent );

	/*mutable*/ const TabulatedCylinder& surf;
	IgesEntity * crv;
	IgesEntitySet & allent;
};

#endif