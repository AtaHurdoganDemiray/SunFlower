// UntrimmedSurface2QuadSet.h

#ifndef __UntrimmedSurface2QuadSet_h__
#define __UntrimmedSurface2QuadSet_h__

class UntrimmedSurface;
class QuadSetHolder;

class UntrimmedSurface2QuadSet
{
public:
	void CalculateQuadSet 
		( const UntrimmedSurface &untrimmedsurf , QuadSetHolder &quadsetholder ) const;

};

#endif //__UntrimmedSurface2QuadSet_h__
