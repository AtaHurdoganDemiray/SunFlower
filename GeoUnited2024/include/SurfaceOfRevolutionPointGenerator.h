// SurfaceOfRevolutionPointGenerator.h

#ifndef __SurfaceOfRevolutionPointGenerator_h__
#define __SurfaceOfRevolutionPointGenerator_h__

#include "SurfacePointGenerator.h"
#include "VLine.h"
#include "SurfaceOfRevolution.h"

class IgesEntity;
struct DoubleVec;
struct CoordinateSystem;
class IgesEntitySet;
class CurvePointGenerator;

class SurfaceOfRevolutionPointGenerator:public SurfacePointGenerator
{
public:
	SurfaceOfRevolutionPointGenerator ( const SurfaceOfRevolution &surf , const const VLine & _l ,  const IgesEntity * generatrixcurve , const IgesEntitySet & allent , const vreal & _sa , const vreal & _ta );
	~SurfaceOfRevolutionPointGenerator();	
	virtual VPoint GetPoint ( const vreal & u , const vreal & v ) const;
	virtual VPoint GetNormal ( const vreal & u , const vreal & v , const VPoint & surfpoint ) const;
	virtual void GetPointAndNormal ( const vreal &u , const vreal &v , VPoint &surfpoint , VVector &normal) const;
	virtual void GetUDefIntervals ( DoubleVec & defint ) const;
	virtual void GetVDefIntervals ( DoubleVec & defint ) const;
	virtual VRefTriangleMesh * Digitise(const vreal &tol)const;
	vreal GetAngle ( const vreal & v ) const;
	VPoint GetNonAxisPoint ();
protected:
	/*mutable*/ const SurfaceOfRevolution& m_surf;
	VLine l;
	CurvePointGenerator *generatrix;
	IgesEntity *m_generatrixCurve;
	vreal sa , ta;
	vreal m_deltaa;
	CoordinateSystem * localcoord;
	VPoint m_nonAxisPoint;
public:
	//virtual bool Unproject(const VPoint &mp, vreal &u, vreal &v , vreal tol);
	virtual IgesEntity * GetIgesEntity();
};

#endif