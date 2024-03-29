// AllTrimLoopUtilities.h

#ifndef __AllTrimLoopUtilities_h__
#define __AllTrimLoopUtilities_h__

#include <vector>
class gmTrimLoopUtility;
class UntrimmedSurface;

class AllTrimLoopUtilities
{
public:
	~AllTrimLoopUtilities();
	long Size() const;
	gmTrimLoopUtility * operator [] ( const long &i ) const;
	gmTrimLoopUtility* AddItem ( const UntrimmedSurface &parentsurf , bool triminside );

private:
	std::vector <gmTrimLoopUtility *> mdata;

};

#endif //__AllTrimLoopUtilities_h__
