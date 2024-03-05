// IndexedFaceSetPointGenerator.h

#ifndef __IndexedFaceSetPointGenerator_h__
#define __IndexedFaceSetPointGenerator_h__

#include "SurfacePointGenerator.h"
#include "VTriangle2dE.h"
#include "VTriangleE.h"

class IndexedFaceSetPointGenerator : public SurfacePointGenerator
{
public:
	IndexedFaceSetPointGenerator (double _u0 , double _u1 , double _v0 , double _v1
		, const std::vector<double> *points
		, const std::vector<double> *normals
		, const std::vector<double> *parameters
		, const std::vector<int> *iPoints
		, const std::vector<int> *iNormals
		, const std::vector<int> *iParameters);
	virtual ~IndexedFaceSetPointGenerator();
	virtual VPoint GetPoint ( const vreal & u , const vreal & v ) const;
	virtual VPoint GetNormal ( const vreal & u , const vreal & v , const VPoint & surfpoint ) const;
	virtual void GetPointAndNormal (const vreal &u , const vreal &v , VPoint &m , VPoint &n) const;
	virtual void GetUDefIntervals ( DoubleVec & defint ) const;
	virtual void GetVDefIntervals ( DoubleVec & defint ) const;
	virtual VRefTriangleMesh * Digitise (const vreal &tol) const;
protected:
	const std::vector<double> * m_points;
	const std::vector<double> * m_normals;
	const std::vector<double> * m_parameters;
	const std::vector<int> * m_iPoints;
	const std::vector<int> * m_iNormals;
	const std::vector<int> * m_iParameters;
public:	
	bool GetParameterTriangleFor (double u , double v , int &indexNo , VTriangle2dE &parTri)const;
	// converts from 2d triangle to 3d triangle position
	VPoint SpaceConversionFromTriangleToTriangle (double u , double v , const VTriangle2dE &parTri , const VTriangleE &tri)const;
	mutable VTriangle2dE m_parTri;
	mutable VTriangleE m_corTri;
	//virtual bool Unproject(const VPoint &mp, vreal &u, vreal &v , vreal tol);
	virtual IgesEntity * GetIgesEntity();
};

#endif