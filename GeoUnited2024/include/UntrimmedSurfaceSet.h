// UntrimmedSurfaceSet.h

#ifndef __UntrimmedSurfaceSet_h__
#define __UntrimmedSurfaceSet_h__

#include <vector>
#include "BSplineSurface.h"
class IgesEntitySet;
class gmTrimmedSurfaceSet;
class TriangleMesh2d;
class TriangleMesh;
class VRefTriangleMesh;
class UntrimmedSurface;

class UntrimmedSurfaceSet
{
public:
	UntrimmedSurfaceSet 
		( const IgesEntitySet &allentities , const gmTrimmedSurfaceSet &alltrimmedsurf );
	long Size() const
	{
		return (allsurfaces.size());
	};

	~UntrimmedSurfaceSet();

	UntrimmedSurface * operator [] ( const long &index ) const
	{
		if ( index < 0 || index >= Size() )
			return NULL;
		
		return allsurfaces[index];
	};
	void CalcMeshForSurf ( const BSplineSurface &surf , TriangleMesh2d &paramtri , TriangleMesh &modeltri ) const;
	void CalcRefMeshForSurf ( BSplineSurface &surf , VRefTriangleMesh &refmesh ) const;

private:
	std::vector<UntrimmedSurface *> allsurfaces;

};

#endif //__UntrimmedSurfaceSet_h__
