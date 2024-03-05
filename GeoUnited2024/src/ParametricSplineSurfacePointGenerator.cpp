// ParametricSplineSurfacePointGenerator.cpp

#include "stdafx.h"

#include "VPoint.h"
#include "DoubleVec.h"
#include "SurfacePointGenerator.h"
#include "ParametricSplineSurface.h"
#include "ParametricSplineSurfacePointGenerator.h"

ParametricSplineSurfacePointGenerator::ParametricSplineSurfacePointGenerator(const ParametricSplineSurface & surf)
: SurfacePointGenerator(surf.m_Tu[0], surf.m_Tu[surf.m_Tu.size()-1], surf.m_Tv[0], surf.m_Tv[surf.m_Tv.size()-1]), m_surf(surf)
{
}

VPoint ParametricSplineSurfacePointGenerator::GetPoint( const vreal & u , const vreal & v ) const
{
	vreal su,sv;
	ShiftParIf(u,v,su,sv);
	// un-normalize u and v
	vreal uu = m_surf.m_Tu[0] + su*(m_surf.m_Tu[m_surf.m_Tu.size()-1] - m_surf.m_Tu[0]);
	vreal vv = m_surf.m_Tv[0] + sv*(m_surf.m_Tv[m_surf.m_Tv.size()-1] - m_surf.m_Tv[0]);

	// find i and j (patch coordinate)
	unsigned int i = m_surf.m_Tu.size() - 2;
	unsigned int j = m_surf.m_Tv.size() - 2;

	// find i
	for(size_t ii = 1; ii < m_surf.m_Tu.size(); ii++)
	{
		if(uu < m_surf.m_Tu[ii])
		{
			i = ii - 1;
			break;
		}
	}

	// find j
	for(size_t jj = 1; jj < m_surf.m_Tv.size(); jj++)
	{
		if(vv < m_surf.m_Tv[jj])
		{
			j = jj - 1;
			break;
		}
	}

	// calc s and t
	vreal s = uu - m_surf.m_Tu[i];
	vreal t = vv - m_surf.m_Tv[j];

	// calculate p
	vreal px = m_surf.m_Ax[i][j] + s * m_surf.m_Bx[i][j] + s*s * m_surf.m_Cx[i][j] + s*s*s * m_surf.m_Dx[i][j]
			 + t * m_surf.m_Ex[i][j] + s*t * m_surf.m_Fx[i][j] + s*s*t * m_surf.m_Gx[i][j] + s*s*s*t * m_surf.m_Hx[i][j]
			 + t*t * m_surf.m_Kx[i][j] + s*t*t * m_surf.m_Lx[i][j] + s*s*t*t * m_surf.m_Mx[i][j] + s*s*s*t*t * m_surf.m_Nx[i][j]
			 + t*t*t * m_surf.m_Px[i][j] + s*t*t*t * m_surf.m_Qx[i][j] + s*s*t*t*t * m_surf.m_Rx[i][j] + s*s*s*t*t*t * m_surf.m_Sx[i][j];

    vreal py = m_surf.m_Ay[i][j] + s * m_surf.m_By[i][j] + s*s * m_surf.m_Cy[i][j] + s*s*s * m_surf.m_Dy[i][j]
			 + t * m_surf.m_Ey[i][j] + s*t * m_surf.m_Fy[i][j] + s*s*t * m_surf.m_Gy[i][j] + s*s*s*t * m_surf.m_Hy[i][j]
			 + t*t * m_surf.m_Ky[i][j] + s*t*t * m_surf.m_Ly[i][j] + s*s*t*t * m_surf.m_My[i][j] + s*s*s*t*t * m_surf.m_Ny[i][j]
			 + t*t*t * m_surf.m_Py[i][j] + s*t*t*t * m_surf.m_Qy[i][j] + s*s*t*t*t * m_surf.m_Ry[i][j] + s*s*s*t*t*t * m_surf.m_Sy[i][j];

	vreal pz = m_surf.m_Az[i][j] + s * m_surf.m_Bz[i][j] + s*s * m_surf.m_Cz[i][j] + s*s*s * m_surf.m_Dz[i][j]
			 + t * m_surf.m_Ez[i][j] + s*t * m_surf.m_Fz[i][j] + s*s*t * m_surf.m_Gz[i][j] + s*s*s*t * m_surf.m_Hz[i][j]
			 + t*t * m_surf.m_Kz[i][j] + s*t*t * m_surf.m_Lz[i][j] + s*s*t*t * m_surf.m_Mz[i][j] + s*s*s*t*t * m_surf.m_Nz[i][j]
			 + t*t*t * m_surf.m_Pz[i][j] + s*t*t*t * m_surf.m_Qz[i][j] + s*s*t*t*t * m_surf.m_Rz[i][j] + s*s*s*t*t*t * m_surf.m_Sz[i][j];

	return VPoint(px, py, pz);
}

VPoint ParametricSplineSurfacePointGenerator::GetNormal( const vreal & u , const vreal & v , const VPoint & surfpoint ) const
{
	return GetNormalAppr(u, v, surfpoint);
}

void ParametricSplineSurfacePointGenerator::GetUDefIntervals ( DoubleVec & defint ) const
{
	vreal invULen = 1 / (m_surf.m_Tu[m_surf.m_Tu.size()-1] - m_surf.m_Tu[0]);

	for(size_t i = 0; i < m_surf.m_Tu.size(); i++)
	{
		vreal uu = (m_surf.m_Tu[i] - m_surf.m_Tu[0]) * invULen;

		defint.Addvreal(uu);
	}
}

void ParametricSplineSurfacePointGenerator::GetVDefIntervals ( DoubleVec & defint ) const
{
	vreal invVLen = 1 / (m_surf.m_Tv[m_surf.m_Tv.size()-1] - m_surf.m_Tv[0]);

	for(size_t i = 0; i < m_surf.m_Tv.size(); i++)
	{
		vreal vv = (m_surf.m_Tv[i] - m_surf.m_Tv[0]) * invVLen;

		defint.Addvreal(vv);
	}
}

VRefTriangleMesh *ParametricSplineSurfacePointGenerator::Digitise(const vreal &tol) const
{
	throw VException(( IDS_ParDigNotImp), 1);
	return 0;
}

//bool ParametricSplineSurfacePointGenerator::Unproject(const VPoint &mp, vreal &u, vreal &v, vreal tol)
//{
//	return GenericUnproject(mp, u, v, tol);
//}

IgesEntity * ParametricSplineSurfacePointGenerator::GetIgesEntity()
{
	return (IgesEntity *)&m_surf;
}