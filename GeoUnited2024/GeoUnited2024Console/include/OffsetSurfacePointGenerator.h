// OffsetSurfacePointGenerator.h

#ifndef __OffsetSurfacePointGenerator_h__
#define __OffsetSurfacePointGenerator_h__

#include "SurfacePointGenerator.h"
#include "UntrimmedSurface.h"
#include "VPoint.h"
#include "OffsetSurface.h"

class OffsetSurfacePointGenerator : public SurfacePointGenerator
{
public:
	OffsetSurfacePointGenerator ( const OffsetSurface &surf , const IgesEntity * basesurf 
									, IgesEntitySet & allent 
									, const VPoint & offsetindicator 
									, const vreal & _offsetvalue );
	virtual VPoint GetPoint ( const vreal & u , const vreal & v ) const;	
	virtual VPoint GetNormal ( const vreal & u , const vreal & v , const VPoint & surfpoint ) const;
	virtual void GetPointAndNormal (const vreal &u , const vreal &v , VPoint &m , VPoint &n)const;
	virtual void GetUDefIntervals ( DoubleVec & defint ) const;
	virtual void GetVDefIntervals ( DoubleVec & defint ) const;
	virtual VRefTriangleMesh * Digitise(const vreal &tol)const;
protected:
	/*mutable*/ const OffsetSurface& m_surf;
	UntrimmedSurface *m_basesurf;
	vreal offsetvalue;
	IgesEntity *m_ibasesurf;
	IgesEntity *m_mySelf;
public:
	//virtual bool Unproject(const VPoint &mp, vreal &u, vreal &v , vreal tol);
	virtual IgesEntity * GetIgesEntity();
};

#endif