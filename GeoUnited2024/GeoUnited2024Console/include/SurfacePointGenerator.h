// SurfacepointGenerator.h

#ifndef __SurfacePointGenerator_h__
#define __SurfacePointGenerator_h__

struct DoubleVec;
struct VPoint;
class VRefTriangleMesh;
class IgesEntity;
#include "IgesEntitySet.h"
class PointSet;
class LineSet;

class SurfacePointGenerator
{
public:
	SurfacePointGenerator ( const vreal & _u0 , const vreal & _u1 , const vreal & _v0 , const vreal & _v1 );
	virtual ~SurfacePointGenerator();
	virtual VPoint GetPoint ( const vreal & u , const vreal & v ) const = 0;
	virtual VPoint GetNormal ( const vreal & u , const vreal & v , const VPoint & surfpoint ) const = 0;
	virtual void GetPointAndNormal (const vreal &u , const vreal &v , VPoint &m , VPoint &n) const;
	virtual void GetUDefIntervals ( DoubleVec & defint ) const = 0;
	virtual void GetVDefIntervals ( DoubleVec & defint ) const = 0;
	virtual VRefTriangleMesh * Digitise (const vreal &tol) const = 0;
	//virtual bool Unproject(const VPoint &mp, vreal &u, vreal &v , vreal tol) = 0;
	VRefTriangleMesh * DigitiseForModelSpaceStep (const vreal &modelSpaceStep)const;
	const vreal & U0() const;
	const vreal & U1() const;
	const vreal & V0() const;
	const vreal & V1() const;
	VPoint GetNormalAppr (const vreal &u , const vreal &v , const VPoint &surfpoint)const;
	void SetU0 (const vreal &newu0)
	{
		u0 = newu0;
		deltau = u1 - u0;
	}
	void SetU1 (const vreal &newu1)
	{
		u1 = newu1;
		deltau = u1 - u0;
	}
	void SetV0 (const vreal &newv0)
	{
		v0 = newv0;
		deltav = v1 - v0;
	}
	void SetV1 (const vreal &newv1)
	{
		v1 = newv1;
		deltav = v1 - v0;
	}
	inline const vreal & GetDeltaU ()const
	{
		return deltau;
	}
	inline const vreal & GetDeltaV ()const
	{
		return deltav;
	}
	void FormRefTriangles (VRefTriangleMesh *rm)const;
	//void GetSnapEnt (const IgesEntity *current , const IgesEntitySet *entset , PointSet &snapp , LineSet &snapl);
	void DivideSpans (const DoubleVec &spans , DoubleVec &divided , int divideNo);
	VRefTriangleMesh * GenericDigitise (const vreal &tol)const;
	VPoint GetDervAprAlongU (const vreal &u,const vreal &v,const VPoint &p)const;
	VPoint GetDervDervAprAlongU (const vreal &u,const vreal &v,const VPoint &p,const VPoint &derv)const;
	vreal GetCurvatureAprAlongU (const vreal &u,const vreal &v,VPoint &derv,VPoint &dervderv)const;
	VPoint GetDervAprAlongV (const vreal &u,const vreal &v,const VPoint &p)const;
	VPoint GetDervDervAprAlongV (const vreal &u,const vreal &v,const VPoint &p,const VPoint &derv)const;
	vreal GetCurvatureAprAlongV (const vreal &u,const vreal &v,VPoint &derv,VPoint &dervderv)const;
	int CalculateDivideCountForIntervalAlongU (const vreal u1
												,const vreal u2
												,const vreal v
												,const vreal tol)const;
	int CalculateDivideCountForIntervalAlongV (const vreal v1
												,const vreal v2
												,const vreal u
												,const vreal tol)const;
	void InsertSegments (const int segmentno
						,const vreal v1
						,const vreal v2
						,std::vector<vreal> &pvec2)const;
	void ShiftParIf (vreal u,vreal v,vreal &newu,vreal &newv)const;
	//void SetUParShift (vreal uParShift);
	//vreal GetUParShift ()const;
	//void SetVParShift (vreal vParShift);
	//vreal GetVParShift ()const;
protected:
	IgesEntitySet m_eSet;
	//vreal m_uParShift;
	//vreal m_vParShift;
private:
	vreal u0,u1,v0,v1;
	vreal deltau;
	vreal deltav;

	void DigitiseIntervalForModelSpaceStep (const vreal &u1,const vreal &u2,const vreal &v1,const vreal &v2,const vreal &modelSpaceStep,VRefTriangleMesh *newmesh)const;
	int NewCalcDivideCountForIntervalAlongU (vreal u1,vreal u2,vreal v, vreal tol)const;
	int NewCalcDivideCountForIntervalAlongV (vreal v1 , vreal v2 , vreal u , vreal tol)const;
	double GetApproximationErrorAlongU (vreal u1 , vreal u2 , vreal v)const;
	double GetApproximationErrorAlongV (vreal u1 , vreal u2 , vreal v)const;
	int m_initialDigitiseSegmentNo;
public:
	int GetInitialDigitiseSegmentNo ()const;
	void SetInitialDigitiseSegmentNo(int initialDigitiseSegmentNo);
private:
	bool m_isPeriodicInU;
	bool m_isPeriodicInV;
	vreal m_uPeriod;
	vreal m_vPeriod;
public:
	bool GetIsPeriodicInU()const;
	void SetIsPeriodicInU (bool isPeriodicInU);
	bool GetIsPeriodicInV()const;
	void SetIsPeriodicInV (bool isPeriodicInV);
	vreal GetUPeriod()const;
	vreal GetVPeriod()const;
	void SetUPeriod(vreal uPeriod);
	void SetVPeriod(vreal vPeriod);
	vreal GetEquavalentInMainRange (vreal par,vreal period,vreal t0,vreal t1)const;
public:
	static int MAX_DIVIDE_COUNT;
	static int DIVIDE_COUNT_FOR_ERROR;
	virtual IgesEntity * GetIgesEntity() = 0;
	static int m_divideCountCalcMaxSegmentLimit;
	static int m_divideCountCalcMaxSegmentNo;
};

#endif