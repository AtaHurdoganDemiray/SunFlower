// HelicalRevolvedSection.h

#ifndef __HelicalRevolvedSection_h__
#define __HelicalRevolvedSection_h__

#include "SurfacePointGenerator.h"
#include "CoordinateSystem.h"
class CurvePointGenerator;
class SurfaceOfRevolution;

class HelicalRevolvedSection : public SurfacePointGenerator
{
public:
public:
	HelicalRevolvedSection (const SurfaceOfRevolution *surfaceOfRev , const IgesEntitySet &allent);
	virtual ~HelicalRevolvedSection();
	virtual VPoint GetPoint ( const vreal & u , const vreal & v ) const;
	virtual VPoint GetNormal ( const vreal & u , const vreal & v , const VPoint & surfpoint ) const;
	virtual void GetPointAndNormal (const vreal &u,const vreal &v,VPoint &m,VPoint &n)const;
	virtual void GetUDefIntervals ( DoubleVec & defint ) const;
	virtual void GetVDefIntervals ( DoubleVec & defint ) const;
	virtual VRefTriangleMesh * Digitise (const vreal &tol) const;
	//virtual bool Unproject(const VPoint &mp, vreal &u, vreal &v, vreal tol);
private:
	CurvePointGenerator *m_section;
	const SurfaceOfRevolution *m_surfaceOfRev;
	IgesEntity *m_sectionCurve;
	double m_pitch;
	double m_deltac;	
	vreal m_totalDeltaPitch;
	vreal m_totalDeltaDiameter;
	vreal m_inv360;
public:
	virtual IgesEntity * GetIgesEntity();
};

#endif