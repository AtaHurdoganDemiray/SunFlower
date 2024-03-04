// RuledSurfacePointGenerator.h

#ifndef __RuledSurfacePointGenerator_h__
#define __RuledSurfacePointGenerator_h__

#include "SurfacePointGenerator.h"

class RuledSurface;
class CurvePointGenerator;
class SequentialLineSet;

class RuledSurfacePointGenerator:public SurfacePointGenerator
{
public:
	RuledSurfacePointGenerator (const RuledSurface *rs , const IgesEntitySet &allent);
	virtual ~RuledSurfacePointGenerator();
	virtual VPoint GetPoint ( const vreal & u , const vreal & v ) const;
	virtual VPoint GetNormal ( const vreal & u , const vreal & v , const VPoint & surfpoint ) const;
	virtual void GetPointAndNormal (const vreal &u,const vreal &v,VPoint &m,VPoint &n)const;
	virtual void GetUDefIntervals ( DoubleVec & defint ) const;
	virtual void GetVDefIntervals ( DoubleVec & defint ) const;
	virtual VRefTriangleMesh * Digitise (const vreal &tol) const;

private:
	CurvePointGenerator *m_crv1;
	CurvePointGenerator *m_crv2;
	IgesEntity * m_icrv1;
	IgesEntity * m_icrv2;
	
	SequentialLineSet *m_ds1;
	SequentialLineSet *m_ds2;
	/*mutable*/ const RuledSurface* m_surf;
	vreal m_len1;
	vreal m_len2;
	//virtual bool Unproject(const VPoint &mp, vreal &u, vreal &v , vreal tol);
	virtual IgesEntity * GetIgesEntity();
};

#endif