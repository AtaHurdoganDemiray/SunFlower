// ParametricSplineSurfacePointGenerator.h

#ifndef __ParametricSplineSurfacePointGenerator_h__
#define __ParametricSplineSurfacePointGenerator_h__

class ParametricSplineSurface;

class ParametricSplineSurfacePointGenerator : public SurfacePointGenerator
{
public:
	ParametricSplineSurfacePointGenerator ( const ParametricSplineSurface & surf );
	virtual VPoint GetPoint ( const vreal & u , const vreal & v ) const;	
	virtual VPoint GetNormal ( const vreal & u , const vreal & v , const VPoint & surfpoint ) const;
	virtual void GetUDefIntervals ( DoubleVec & defint ) const;
	virtual void GetVDefIntervals ( DoubleVec & defint ) const;
	virtual VRefTriangleMesh * Digitise(const vreal &tol)const;
protected:
	/*mutable*/ const ParametricSplineSurface& m_surf;
public:
	//virtual bool Unproject(const VPoint &mp, vreal &u, vreal &v , vreal tol);
	virtual IgesEntity * GetIgesEntity();
};

#endif
