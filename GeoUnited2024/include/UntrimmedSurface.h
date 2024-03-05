// UntrimmedSurface.h

#ifndef __UntrimmedSurface_h__
#define __UntrimmedSurface_h__
#include "TransformationMatrix.h"
#include "VPoint2d.h"

class IgesEntity;
class IgesEntitySet;
struct VPoint;
class SurfacePointGenerator;
struct DoubleVec;
class PointSet;
class VRefTriangleMesh;
class LineSet;
class CopiousData;

class SurfaceSpace2;

class UntrimmedSurface
{
public:
	friend class dSurface;
	friend class AnalyzeSurface;
	enum IsoParametricType {u0vchanging , u1vchanging , v0uchanging , v1uchanging}; 
	UntrimmedSurface ( const IgesEntity * pigs , IgesEntitySet & allent );
	//UntrimmedSurface  ( const IgesEntity *sectionCurve , const IgesEntitySet &allent 
	//					, double r0 , double c0 , double z0 , double z1,double pitch);
	UntrimmedSurface (SurfacePointGenerator *surfPointGen);
	virtual ~UntrimmedSurface();
	const vreal & U0() const;
	const vreal & U1() const;
	const vreal & V0() const;
	const vreal & V1() const;
	void GetPointAndNormal 
	( const vreal &f_u , const vreal &f_v , VPoint &mypoint , VPoint &mynormal ) const;
	VPoint GetPoint ( const vreal &f_u , const vreal & f_v ) const;
	VPoint GetNormal ( const vreal & u , const vreal & v 
							, const VPoint & surfpoint ) const;
	void GetUDefIntervals ( DoubleVec & defint ) const;
	void GetVDefIntervals ( DoubleVec & defint ) const;
	VRefTriangleMesh * Digitise (const vreal &tol)const;
	VRefTriangleMesh * DigitiseForModelSpaceStep ( const vreal &modelSpaceStep)const;
	//void GetSnapEnt (const IgesEntity * current , const IgesEntitySet *entset , PointSet &snapp , LineSet &snapl);
	CopiousData * MakeIsoParametricCurve (IsoParametricType ityp,IgesEntitySet &entset,const vreal tol , int &refi);
	void IncrementTransform (const TransformationMatrix &tm);
	void ResetTransform (const TransformationMatrix &tm);
	const TransformationMatrix & GetTransformation()const;
	void ClearTransformation();
	bool IsTransformed()const;
	int GetSurfDe ()const;
	SurfacePointGenerator & GetDigitiser();
	const SurfacePointGenerator & GetDigitiser()const;
	//void SetTransformationMatrixIf ();
	//void SetUParShift (vreal uParShift);
	//vreal GetUParShift ()const;
	//void SetVParShift (vreal vParShift);
	//vreal GetVParShift ()const;
	VPoint2d RePar (const VPoint &p)const;
private:
	const IgesEntity *m_surf;
	UntrimmedSurface ();
	SurfacePointGenerator * digitiser;
	void SetTransformationMatrixBase ( const IgesEntitySet & allent 
								, const IgesEntity * current );
	
	void InsertSegments (int segNo , const vreal &p1 , const vreal &p2,DoubleVec &param2)const;
	int CalculateDivideCountForInterval  (const vreal u1 , const vreal v1
		, const vreal u2 , const vreal v2
		, const vreal tol)const;
	bool istm_given;
	TransformationMatrix m_tm;
	static int UNTRIMMED_SURF_BOUNDARY_STEP;
	static vreal UNTRIMMED_SURF_PAR_STEP;
	void SetUVMarginParams ();
	VPoint2d m_P0;
	double m_dUinv , m_dVinv;
	bool m_isTmBaseGiven;
	TransformationMatrix m_tmBase;
public:
	void SetTmBase (const TransformationMatrix &tm);
	bool GetIsTmBaseGiven();
	const TransformationMatrix & GetTmBase ()const;
	int GetSurfType()const;
	bool GenericUnproject(const VPoint &mp, vreal &u, vreal &v, vreal tol);
private:
	SurfaceSpace2 *m_surfSpace;
};

#endif //__UntrimmedSurface_h__
