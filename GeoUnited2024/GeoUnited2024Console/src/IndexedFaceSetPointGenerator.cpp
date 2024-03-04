// IndexedFaceSetPointGenerator.cpp

#include "stdafx.h"
#include "IndexedFaceSetPointGenerator.h"
#include "CoordinateSystem.h"

IndexedFaceSetPointGenerator::IndexedFaceSetPointGenerator (double _u0 , double _u1 , double _v0 , double _v1
															, const std::vector<double> * points
															, const std::vector<double> * normals
															, const std::vector<double> * parameters
															, const std::vector<int> * iPoints
															, const std::vector<int> * iNormals
															, const std::vector<int> * iParameters
															)
:SurfacePointGenerator (_u0,_u1,_v0,_v1) , m_points (points) , m_normals (normals) , m_parameters (parameters)
, m_iPoints (iPoints) , m_iNormals (iNormals) , m_iParameters (iParameters)
{

}
IndexedFaceSetPointGenerator::~IndexedFaceSetPointGenerator()
{

}
VPoint IndexedFaceSetPointGenerator::GetPoint ( const vreal & u , const vreal & v ) const
{
	//double um = (u-U0())*GetDeltaU();
	//double vm = (v-V0())*GetDeltaV();
	double um = U0() + GetDeltaU() * u;
	double vm = V0() + GetDeltaV() * v;
	int indexNo;
	// find related triangle2d that parameter point is inside
	bool ck = GetParameterTriangleFor (um , vm , indexNo , m_parTri);
	if (ck)
	{
		int c1i = m_iPoints->at(indexNo);
		int c2i = m_iPoints->at(indexNo+1);
		int c3i = m_iPoints->at(indexNo+2);
		VPoint p1 (m_points->at(c1i*3),m_points->at(c1i*3+1),m_points->at(c1i*3+2));
		VPoint p2 (m_points->at(c2i*3),m_points->at(c2i*3+1),m_points->at(c2i*3+2));
		VPoint p3 (m_points->at(c3i*3),m_points->at(c3i*3+1),m_points->at(c3i*3+2));
		m_corTri = VTriangleE (p1,p2,p3);
		VPoint convertedPoint = SpaceConversionFromTriangleToTriangle (um,vm,m_parTri,m_corTri);
		return convertedPoint;
	}
	else
	{
		//throw VException (("IndexedFaceSetPointGenerator::GetPoint exception , including parameter triangle not found!"),1);	
		return VPoint (-12.2,-12.2,-12.2);
	}

	return 1.;
}
VPoint IndexedFaceSetPointGenerator::GetNormal ( const vreal & u , const vreal & v , const VPoint & surfpoint ) const
{
	VPoint nor = SurfacePointGenerator::GetNormalAppr (u,v,surfpoint);
	return nor;
}

void IndexedFaceSetPointGenerator::GetPointAndNormal (const vreal &u , const vreal &v , VPoint &m , VPoint &n) const
{
	m = GetPoint (u,v);
	n = SurfacePointGenerator::GetNormalAppr (u,v,m);
}
void IndexedFaceSetPointGenerator::GetUDefIntervals ( DoubleVec & defint ) const
{

}
void IndexedFaceSetPointGenerator::GetVDefIntervals ( DoubleVec & defint ) const
{

}
VRefTriangleMesh * IndexedFaceSetPointGenerator::Digitise (const vreal &tol) const
{
	return GenericDigitise (tol);
}

bool IndexedFaceSetPointGenerator::GetParameterTriangleFor(double u, double v, int &indexNo ,VTriangle2dE &parTri)const
{
	VPoint2d pp (u,v);
	// Iterate for all triangles and return the first triangle2d that contain u v
	size_t mysize = m_iParameters->size();
	int p1i,p2i,p3i;
	int tmpi;
	for (size_t i=0;i<mysize;)
	{
		tmpi = (int)i;
		p1i = m_iParameters->at(i);
		++i;
		p2i = m_iParameters->at(i);
		++i;
		p3i = m_iParameters->at(i);
		++i;
		int ii = m_iParameters->at(i);
		if (ii!= -1)
			throw VException (( IDS_IndGetExcTheAre),1);
		++i;
		VPoint2d p1 (m_parameters->at(p1i*2),m_parameters->at(p1i*2 + 1));
		VPoint2d p2 (m_parameters->at (p2i*2),m_parameters->at(p2i*2 + 1));
		VPoint2d p3 (m_parameters->at(p3i*2),m_parameters->at(p3i*2 + 1));
		VTriangle2dE tri(p1,p2,p3);
		if (tri.IsInTriangle (pp))
		{
			indexNo = tmpi;
			parTri = tri;
			return true;
		}
	}
	return false;
}

VPoint IndexedFaceSetPointGenerator::SpaceConversionFromTriangleToTriangle (double u , double v , const VTriangle2dE &parTri , const VTriangleE &tri)const
{
	VTriangleE temp (VPoint (parTri.GetCorner1().x() , parTri.GetCorner1().y(), 0.0) 
					,VPoint (parTri.GetCorner2().x(),parTri.GetCorner2().y(),0.0)
					,VPoint (parTri.GetCorner3().x(),parTri.GetCorner3().y(),0.0));
	CoordinateSystem csp = temp.GetCs1(); 	
	CoordinateSystem csd = tri.GetCs1();
	VPoint lpU = csp.AbsoluteCoordToRelativeCoord (temp.GetCorner2());
	VPoint lpV = csp.AbsoluteCoordToRelativeCoord (temp.GetCorner3());
	VPoint luv = csp.AbsoluteCoordToRelativeCoord ( VPoint (u,v,0.0) );
	double lu = luv.x() / lpU.x();
	double lv = luv.y() / lpV.y();
	VPoint ldX = csd.AbsoluteCoordToRelativeCoord (tri.GetCorner2());
	VPoint ldY = csd.AbsoluteCoordToRelativeCoord (tri.GetCorner3());
	double lx = ldX.x() * lu;
	double ly = ldY.y() * lv;
	VPoint convertedPoint = csd.RelativeCoordToAbsoluteCoord (VPoint(lx,ly,0.0));
	return convertedPoint;
}

//bool IndexedFaceSetPointGenerator::Unproject(const VPoint &mp, vreal &u, vreal &v, vreal tol)
//{
//	return GenericUnproject(mp, u, v, tol);
//}

IgesEntity * IndexedFaceSetPointGenerator::GetIgesEntity()
{
	return 0;
}