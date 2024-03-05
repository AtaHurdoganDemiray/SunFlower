// TabulatedCylinderPointGenerator.h

#ifndef __TabulatedCylinderPointGenerator_h__
#define __TabulatedCylinderPointGenerator_h__

#include "SurfacePointGenerator.h"
#include "VPoint.h"


class CurvePointGenerator;

class ProcessedTabulatedCylinder;

class TabulatedCylinderPointGenerator : public SurfacePointGenerator
{
public:
	TabulatedCylinderPointGenerator (  ProcessedTabulatedCylinder & surf );
	~TabulatedCylinderPointGenerator();
	virtual VPoint GetPoint ( const vreal & u , const vreal & v ) const;	
	virtual VPoint GetNormal ( const vreal & u , const vreal & v , const VPoint & surfpoint ) const;
	virtual void GetPointAndNormal (const vreal &u , const vreal &v , VPoint &m,VPoint &n)const;
	virtual void GetUDefIntervals ( DoubleVec & defint ) const;
	virtual void GetVDefIntervals ( DoubleVec & defint ) const;
	virtual VRefTriangleMesh * Digitise(const vreal &tol)const;
protected:
	/*mutable*/ const TabulatedCylinder& m_surf;
	CurvePointGenerator * pcrv;	
	IgesEntity * m_toExtrude;
	VPoint direct;
public:
	void ResetExtrudeLength(vreal newLen);
	//virtual bool Unproject(const VPoint &mp, vreal &u, vreal &v , vreal tol);
	virtual IgesEntity * GetIgesEntity();
};

#endif