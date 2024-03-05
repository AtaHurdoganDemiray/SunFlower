// ParametricSplineSurface.cpp

#include "stdafx.h"
#include "ParametricSplineSurface.h"

#include "IgesEntityVisitor.h"
#include "IgesEntitySet.h"
#include "Matrix.h"
#include "GaussJordanElimination.h"

ParametricSplineSurface::ParametricSplineSurface ()
{
	entitytype = 114;
}

ParametricSplineSurface::ParametricSplineSurface (int patchun , int patchvn)
	:m_Ax(patchun,std::vector<vreal>(patchvn))
	,m_Bx(patchun,std::vector<vreal>(patchvn))
	,m_Cx(patchun,std::vector<vreal>(patchvn))
	,m_Dx(patchun,std::vector<vreal>(patchvn))
	,m_Ex(patchun,std::vector<vreal>(patchvn))
	,m_Fx(patchun,std::vector<vreal>(patchvn))
	,m_Gx(patchun,std::vector<vreal>(patchvn))
	,m_Hx(patchun,std::vector<vreal>(patchvn))
	,m_Kx(patchun,std::vector<vreal>(patchvn))
	,m_Lx(patchun,std::vector<vreal>(patchvn))
	,m_Mx(patchun,std::vector<vreal>(patchvn))
	,m_Nx(patchun,std::vector<vreal>(patchvn))
	,m_Px(patchun,std::vector<vreal>(patchvn))
	,m_Qx(patchun,std::vector<vreal>(patchvn))
	,m_Rx(patchun,std::vector<vreal>(patchvn))
	,m_Sx(patchun,std::vector<vreal>(patchvn))
	,m_Ay(patchun,std::vector<vreal>(patchvn))
	,m_By(patchun,std::vector<vreal>(patchvn))
	,m_Cy(patchun,std::vector<vreal>(patchvn))
	,m_Dy(patchun,std::vector<vreal>(patchvn))
	,m_Ey(patchun,std::vector<vreal>(patchvn))
	,m_Fy(patchun,std::vector<vreal>(patchvn))
	,m_Gy(patchun,std::vector<vreal>(patchvn))
	,m_Hy(patchun,std::vector<vreal>(patchvn))
	,m_Ky(patchun,std::vector<vreal>(patchvn))
	,m_Ly(patchun,std::vector<vreal>(patchvn))
	,m_My(patchun,std::vector<vreal>(patchvn))
	,m_Ny(patchun,std::vector<vreal>(patchvn))
	,m_Py(patchun,std::vector<vreal>(patchvn))
	,m_Qy(patchun,std::vector<vreal>(patchvn))
	,m_Ry(patchun,std::vector<vreal>(patchvn))
	,m_Sy(patchun,std::vector<vreal>(patchvn))
	,m_Az(patchun,std::vector<vreal>(patchvn))
	,m_Bz(patchun,std::vector<vreal>(patchvn))
	,m_Cz(patchun,std::vector<vreal>(patchvn))
	,m_Dz(patchun,std::vector<vreal>(patchvn))
	,m_Ez(patchun,std::vector<vreal>(patchvn))
	,m_Fz(patchun,std::vector<vreal>(patchvn))
	,m_Gz(patchun,std::vector<vreal>(patchvn))
	,m_Hz(patchun,std::vector<vreal>(patchvn))
	,m_Kz(patchun,std::vector<vreal>(patchvn))
	,m_Lz(patchun,std::vector<vreal>(patchvn))
	,m_Mz(patchun,std::vector<vreal>(patchvn))
	,m_Nz(patchun,std::vector<vreal>(patchvn))
	,m_Pz(patchun,std::vector<vreal>(patchvn))
	,m_Qz(patchun,std::vector<vreal>(patchvn))
	,m_Rz(patchun,std::vector<vreal>(patchvn))
	,m_Sz(patchun,std::vector<vreal>(patchvn))
{
	entitytype = 114;
	m_CTYPE = 3;//Spline boundary type: 1 linear , 2 quadratic , 3 cubic , 4 wilson-fowler ,5 modified wilson-fowler , 6 B spline
	m_PTYPE = 1;// Patch type: 1 Cartesian product , 0 unspecified
	m_M = patchun;// number of u segments
	int m_N = patchvn;// number of v segments
	m_Tu.resize (m_M+1);
	m_Tv.resize (m_N+1);
	for (int i=0;i<m_M+1;++i)
	{
		m_Tu[i] = i;
	}
	for (int j=0;j<m_N+1;++j)
	{
		m_Tv[j] = j;
	}
}

void ParametricSplineSurface::GetEntityIn (FileLines &mylines , int poffs)
{
	DoubleVector myvect;
	GetAllParam ( myvect , mylines );
	int vecsize = (int)myvect.size();
	if (vecsize <= 5)
		throw VException (( IDS_ParGetExcNotEno),1);
	m_CTYPE = (int)myvect[1];
	m_PTYPE = (int)myvect[2];
	m_M = (int)myvect[3];
	m_N = (int)myvect[4];

	if (vecsize < 6+m_M)
		throw VException (( IDS_ParGetExcNotEno),1);
	m_Tu.resize (m_M+1);
	for (int i=0;i<=m_M; ++i)
		m_Tu[i] = myvect[5+i];

	if (vecsize < 7+m_M+m_N)
		throw VException (( IDS_ParGetExcNotEno),1);
	m_Tv.resize (m_N + 1);
	for (int j=0;j<=m_N;++j)
		m_Tv[j] = myvect[6+m_M+j];

	if (vecsize < 7 + m_M + m_N + 48 * ( m_M*(m_N + 1) + (m_N + 1) ) )
		throw VException (( IDS_ParGetExcNotEno),1);
	
	m_Ax.resize (m_M+1);
	m_Bx.resize (m_M+1);
	m_Cx.resize (m_M+1);
	m_Dx.resize (m_M+1);
	m_Ex.resize (m_M+1);
	m_Fx.resize (m_M+1);
	m_Gx.resize (m_M+1);
	m_Hx.resize (m_M+1);
	m_Kx.resize (m_M+1);
	m_Lx.resize (m_M+1);
	m_Mx.resize (m_M+1);
	m_Nx.resize (m_M+1);
	m_Px.resize (m_M+1);
	m_Qx.resize (m_M+1);
	m_Rx.resize (m_M+1);
	m_Sx.resize (m_M+1);
	for (int i=0; i<=m_M; ++i)
	{
		m_Ax[i].resize (m_N+1);
		m_Bx[i].resize (m_N+1);
		m_Cx[i].resize (m_N+1);
		m_Dx[i].resize (m_N+1);
		m_Ex[i].resize (m_N+1);
		m_Fx[i].resize (m_N+1);
		m_Gx[i].resize (m_N+1);
		m_Hx[i].resize (m_N+1);
		m_Kx[i].resize (m_N+1);
		m_Lx[i].resize (m_N+1);
		m_Mx[i].resize (m_N+1);
		m_Nx[i].resize (m_N+1);
		m_Px[i].resize (m_N+1);
		m_Qx[i].resize (m_N+1);
		m_Rx[i].resize (m_N+1);
		m_Sx[i].resize (m_N+1);
	}

	m_Ay.resize (m_M+1);
	m_By.resize (m_M+1);
	m_Cy.resize (m_M+1);
	m_Dy.resize (m_M+1);
	m_Ey.resize (m_M+1);
	m_Fy.resize (m_M+1);
	m_Gy.resize (m_M+1);
	m_Hy.resize (m_M+1);
	m_Ky.resize (m_M+1);
	m_Ly.resize (m_M+1);
	m_My.resize (m_M+1);
	m_Ny.resize (m_M+1);
	m_Py.resize (m_M+1);
	m_Qy.resize (m_M+1);
	m_Ry.resize (m_M+1);
	m_Sy.resize (m_M+1);
	for (int i=0; i<=m_M; ++i)
	{
		m_Ay[i].resize (m_N+1);
		m_By[i].resize (m_N+1);
		m_Cy[i].resize (m_N+1);
		m_Dy[i].resize (m_N+1);
		m_Ey[i].resize (m_N+1);
		m_Fy[i].resize (m_N+1);
		m_Gy[i].resize (m_N+1);
		m_Hy[i].resize (m_N+1);
		m_Ky[i].resize (m_N+1);
		m_Ly[i].resize (m_N+1);
		m_My[i].resize (m_N+1);
		m_Ny[i].resize (m_N+1);
		m_Py[i].resize (m_N+1);
		m_Qy[i].resize (m_N+1);
		m_Ry[i].resize (m_N+1);
		m_Sy[i].resize (m_N+1);
	}

	m_Az.resize (m_M+1);
	m_Bz.resize (m_M+1);
	m_Cz.resize (m_M+1);
	m_Dz.resize (m_M+1);
	m_Ez.resize (m_M+1);
	m_Fz.resize (m_M+1);
	m_Gz.resize (m_M+1);
	m_Hz.resize (m_M+1);
	m_Kz.resize (m_M+1);
	m_Lz.resize (m_M+1);
	m_Mz.resize (m_M+1);
	m_Nz.resize (m_M+1);
	m_Pz.resize (m_M+1);
	m_Qz.resize (m_M+1);
	m_Rz.resize (m_M+1);
	m_Sz.resize (m_M+1);
	for (int i=0; i<=m_M; ++i)
	{
		m_Az[i].resize (m_N+1);
		m_Bz[i].resize (m_N+1);
		m_Cz[i].resize (m_N+1);
		m_Dz[i].resize (m_N+1);
		m_Ez[i].resize (m_N+1);
		m_Fz[i].resize (m_N+1);
		m_Gz[i].resize (m_N+1);
		m_Hz[i].resize (m_N+1);
		m_Kz[i].resize (m_N+1);
		m_Lz[i].resize (m_N+1);
		m_Mz[i].resize (m_N+1);
		m_Nz[i].resize (m_N+1);
		m_Pz[i].resize (m_N+1);
		m_Qz[i].resize (m_N+1);
		m_Rz[i].resize (m_N+1);
		m_Sz[i].resize (m_N+1);
	}

	for (int u = 0; u<=m_M; ++u)
	{
		for (int v=0; v<=m_N; ++v)
		{
			int si = 7 + m_M + m_N + (u*(m_N+1)*48 + v*48);	
			// X sequence
			m_Ax[u][v] = myvect[si];
			m_Bx[u][v] = myvect[si+1];
			m_Cx[u][v] = myvect[si+2];
			m_Dx[u][v] = myvect[si+3];

			m_Ex[u][v] = myvect[si+4];
			m_Fx[u][v] = myvect[si+5];
			m_Gx[u][v] = myvect[si+6];
			m_Hx[u][v] = myvect[si+7];

			m_Kx[u][v] = myvect[si+8];
			m_Lx[u][v] = myvect[si+9];
			m_Mx[u][v] = myvect[si+10];
			m_Nx[u][v] = myvect[si+11];

			m_Px[u][v] = myvect[si+12];
			m_Qx[u][v] = myvect[si+13];
			m_Rx[u][v] = myvect[si+14];
			m_Sx[u][v] = myvect[si+15];
	
			// Y sequence
			m_Ay[u][v] = myvect[si+16];
			m_By[u][v] = myvect[si+17];
			m_Cy[u][v] = myvect[si+18];
			m_Dy[u][v] = myvect[si+19];

			m_Ey[u][v] = myvect[si+20];
			m_Fy[u][v] = myvect[si+21];
			m_Gy[u][v] = myvect[si+22];
			m_Hy[u][v] = myvect[si+23];

			m_Ky[u][v] = myvect[si+24];
			m_Ly[u][v] = myvect[si+25];
			m_My[u][v] = myvect[si+26];
			m_Ny[u][v] = myvect[si+27];

			m_Py[u][v] = myvect[si+28];
			m_Qy[u][v] = myvect[si+29];
			m_Ry[u][v] = myvect[si+30];
			m_Sy[u][v] = myvect[si+31];
			
			// Z sequence
			m_Az[u][v] = myvect[si+32];
			m_Bz[u][v] = myvect[si+33];
			m_Cz[u][v] = myvect[si+34];
			m_Dz[u][v] = myvect[si+35];

			m_Ez[u][v] = myvect[si+36];
			m_Fz[u][v] = myvect[si+37];
			m_Gz[u][v] = myvect[si+38];
			m_Hz[u][v] = myvect[si+39];

			m_Kz[u][v] = myvect[si+40];
			m_Lz[u][v] = myvect[si+41];
			m_Mz[u][v] = myvect[si+42];
			m_Nz[u][v] = myvect[si+43];

			m_Pz[u][v] = myvect[si+44];
			m_Qz[u][v] = myvect[si+45];
			m_Rz[u][v] = myvect[si+46];
			m_Sz[u][v] = myvect[si+47];

		}
	}
}
void ParametricSplineSurface::WriteEntityOut (std::vector<VStr> &strvec)const
{
	WriteParam (entitytype , strvec);
	WriteParam (m_CTYPE , strvec);
	WriteParam (m_PTYPE , strvec);
	WriteParam (m_M , strvec);
	WriteParam (m_N , strvec);
	
	for ( int i=0; i<=m_M; i++ )
	{
		WriteParam (m_Tu[i],strvec);
	}

	for ( int j=0;j<=m_N; j++)
	{
		WriteParam (m_Tv[j] , strvec);
	}

	for ( int u = 0; u<=m_M; ++u)
	{
		for (int v=0;v<=m_N; ++v)
		{
			// X sequence
			WriteParam (m_Ax[u][v] , strvec);
			WriteParam (m_Bx[u][v] , strvec);
			WriteParam (m_Cx[u][v] , strvec);
			WriteParam (m_Dx[u][v] , strvec);

			WriteParam (m_Ex[u][v] , strvec);
			WriteParam (m_Fx[u][v] , strvec);
			WriteParam (m_Gx[u][v] , strvec);
			WriteParam (m_Hx[u][v] , strvec);

			WriteParam (m_Kx[u][v] , strvec);
			WriteParam (m_Lx[u][v] , strvec);
			WriteParam (m_Mx[u][v] , strvec);
			WriteParam (m_Nx[u][v] , strvec);

			WriteParam (m_Px[u][v] , strvec);
			WriteParam (m_Qx[u][v] , strvec);
			WriteParam (m_Rx[u][v] , strvec);
			WriteParam (m_Sx[u][v] , strvec);

			// Y sequence
			WriteParam (m_Ay[u][v] , strvec);
			WriteParam (m_By[u][v] , strvec);
			WriteParam (m_Cy[u][v] , strvec);
			WriteParam (m_Dy[u][v] , strvec);

			WriteParam (m_Ey[u][v] , strvec);
			WriteParam (m_Fy[u][v] , strvec);
			WriteParam (m_Gy[u][v] , strvec);
			WriteParam (m_Hy[u][v] , strvec);

			WriteParam (m_Ky[u][v] , strvec);
			WriteParam (m_Ly[u][v] , strvec);
			WriteParam (m_My[u][v] , strvec);
			WriteParam (m_Ny[u][v] , strvec);

			WriteParam (m_Py[u][v] , strvec);
			WriteParam (m_Qy[u][v] , strvec);
			WriteParam (m_Ry[u][v] , strvec);
			WriteParam (m_Sy[u][v] , strvec);

			// Z sequence
			WriteParam (m_Az[u][v] , strvec);
			WriteParam (m_Bz[u][v] , strvec);
			WriteParam (m_Cz[u][v] , strvec);
			WriteParam (m_Dz[u][v] , strvec);

			WriteParam (m_Ez[u][v] , strvec);
			WriteParam (m_Fz[u][v] , strvec);
			WriteParam (m_Gz[u][v] , strvec);
			WriteParam (m_Hz[u][v] , strvec);

			WriteParam (m_Kz[u][v] , strvec);
			WriteParam (m_Lz[u][v] , strvec);
			WriteParam (m_Mz[u][v] , strvec);
			WriteParam (m_Nz[u][v] , strvec);

			WriteParam (m_Pz[u][v] , strvec);
			WriteParam (m_Qz[u][v] , strvec);
			WriteParam (m_Rz[u][v] , strvec);
			WriteParam (m_Sz[u][v] , strvec);
		}
	}


}
void ParametricSplineSurface::GetChildren (std::vector<int> &childVec)const
{
	GetDirectoryChild (childVec);
}
void ParametricSplineSurface::AcceptVisitor (IgesEntityVisitor &visitor)const
{
	visitor.AcceptVirtual (*this);
}
//bool ParametricSplineSurface::IsPrimitiveEntity ()const
//{
//	return true;
//}
//double ParametricSplineSurface::GetSortNo()const
//{
//	return 15.0;
//}
IgesEntity * ParametricSplineSurface::MakeCopyAndAddToSet (const IgesEntitySet &mainSet , IgesEntitySet &newSet)const
{
	if (m_subsetIndex !=0)
		return newSet.GetEntityPointer(m_subsetIndex);
	ParametricSplineSurface *mycopy = new ParametricSplineSurface (*this);
	CopyDE (*mycopy,mainSet,newSet);
	newSet.InsertEntity (mycopy);
	m_subsetIndex = mycopy->directorylineno;
	return mycopy;
}

bool ParametricSplineSurface::CalculateAndSetOnePatchCoef ( const std::list<std::pair<VPoint2d , VPoint>> &parPointMap 
													, const std::list<std::pair<VPoint2d , VPoint>> &parSDervMap
													, const std::list<std::pair<VPoint2d , VPoint>> &parTDervMap
													, int patchui , int patchvi)
{
	size_t pointSize = parPointMap.size();
	size_t sDervSize = parSDervMap.size();
	size_t tDervSize = parTDervMap.size();
	if (pointSize != 8 || sDervSize != 4 || tDervSize != 4)
	{
		LogFileWriter::WriteUnknownExceptionLog ("ParametricSplineSurface::CalculateOnePatchCoef exception , map sizes unexpected!");
		return false;
	}
	
	Matrix A[3] = {Matrix(16,16),Matrix(16,16),Matrix(16,16)};
	Matrix C[3] = {Matrix(16,1),Matrix(16,1),Matrix(16,1)};
	std::list<std::pair<VPoint2d , VPoint>>::const_iterator pi = parPointMap.begin();
	std::list<std::pair<VPoint2d , VPoint>>::const_iterator pe = parPointMap.end();
	
	int counter = 1;
	for (pi;pi!=pe;++pi)
	{
		vreal s = pi->first.x();
		vreal s2 = s*s;
		vreal s3 = s2*s;
		vreal t = pi->first.y();
		vreal t2 = t*t;
		vreal t3 = t2*t;
		for (int i=0;i<3;++i)
		{
			A[i].SetElement (1.0,counter,1);
			A[i].SetElement (s , counter , 2);
			A[i].SetElement (s2 , counter ,3);
			A[i].SetElement (s3 , counter , 4);

			A[i].SetElement (t , counter , 5);
			A[i].SetElement (t*s , counter , 6);
			A[i].SetElement (t*s2 , counter , 7);
			A[i].SetElement (t*s3 , counter , 8);

			A[i].SetElement (t2 , counter , 9);
			A[i].SetElement (t2*s , counter , 10);
			A[i].SetElement (t2*s2 , counter , 11);
			A[i].SetElement (t2*s3 , counter , 12);

			A[i].SetElement(t3, counter, 13);
			A[i].SetElement (t3*s , counter , 14);
			A[i].SetElement (t3*s2 , counter , 15);
			A[i].SetElement (t3*s3 , counter , 16);

			C[i].SetElement(pi->second.operator[](i), counter, 1);
		}
		++counter;
	}

	std::list<std::pair<VPoint2d , VPoint>>::const_iterator sdi = parSDervMap.begin();
	std::list<std::pair<VPoint2d , VPoint>>::const_iterator sde = parSDervMap.end();
	for (sdi;sdi!=sde;++sdi)
	{
		vreal s = sdi->first.x();
		vreal s2 = s*s;
		vreal s3 = s2*s;
		vreal t = sdi->first.y();
		vreal t2 = t*t;
		vreal t3 = t2*t;

		for (int i=0;i<3;++i)
		{
			A[i].SetElement (0.0,counter,1);
			A[i].SetElement (1.0 , counter , 2);
			A[i].SetElement (2.*s , counter ,3);
			A[i].SetElement (3*s2 , counter , 4);

			A[i].SetElement (0.0 , counter , 5);
			A[i].SetElement (t , counter , 6);
			A[i].SetElement (t*s*2.0 , counter , 7);
			A[i].SetElement (t*s2*3.0 , counter , 8);

			A[i].SetElement (0.0 , counter , 9);
			A[i].SetElement (t2 , counter , 10);
			A[i].SetElement (t2*2.0*s , counter , 11);
			A[i].SetElement (t2*s2*3.0 , counter , 12);

			A[i].SetElement (0.0 , counter , 13);
			A[i].SetElement (t3 , counter , 14);
			A[i].SetElement (t3*s*2.0 , counter , 15);
			A[i].SetElement (t3*s2*3.0 , counter , 16);

			C[i].SetElement (sdi->second.operator[](i) , counter , 1);
		}
		++counter;
	}

	std::list<std::pair<VPoint2d , VPoint>>::const_iterator tdi = parTDervMap.begin();
	std::list<std::pair<VPoint2d , VPoint>>::const_iterator tde = parTDervMap.end();
	for (tdi;tdi!=tde;++tdi)
	{
		vreal s = tdi->first.x();
		vreal s2 = s*s;
		vreal s3 = s2*s;
		vreal t = tdi->first.y();
		vreal t2 = t*t;
		vreal t3 = t2*t;

		for (int i=0;i<3;++i)
		{
			A[i].SetElement (0.0,counter,1);
			A[i].SetElement (0.0 , counter , 2);
			A[i].SetElement (0.0 , counter ,3);
			A[i].SetElement (0.0 , counter , 4);

			A[i].SetElement (1.0 , counter , 5);
			A[i].SetElement (s , counter , 6);
			A[i].SetElement (s2 , counter , 7);
			A[i].SetElement (s3 , counter , 8);

			A[i].SetElement (t*2.0 , counter , 9);
			A[i].SetElement (t*2.0*s , counter , 10);
			A[i].SetElement (t*2.0*s2 , counter , 11);
			A[i].SetElement (t*2.0*s3 , counter , 12);

			A[i].SetElement (t2*3.0 , counter , 13);
			A[i].SetElement (t2*3.0*s , counter , 14);
			A[i].SetElement (t2*3.0*s2 , counter , 15);
			A[i].SetElement (t2*3.0*s3 , counter , 16);

			C[i].SetElement (tdi->second.operator[](i) , counter , 1);
		}
		++counter;
	}

	GaussJordanElimination gje;
	bool ck0 = gje.Solve (A[0],C[0]);
	if (ck0)
	{
		m_Ax[patchui][patchvi] = C[0].GetElement (1,1);
		m_Bx[patchui][patchvi] = C[0].GetElement (2,1);
		m_Cx[patchui][patchvi] = C[0].GetElement (3,1);
		m_Dx[patchui][patchvi] = C[0].GetElement (4,1);
	
		m_Ex[patchui][patchvi] = C[0].GetElement (5,1);
		m_Fx[patchui][patchvi] = C[0].GetElement (6,1);
		m_Gx[patchui][patchvi] = C[0].GetElement (7,1);
		m_Hx[patchui][patchvi] = C[0].GetElement (8,1);
	
		m_Kx[patchui][patchvi] = C[0].GetElement (9,1);
		m_Lx[patchui][patchvi] = C[0].GetElement (10,1);
		m_Mx[patchui][patchvi] = C[0].GetElement (11,1);
		m_Nx[patchui][patchvi] = C[0].GetElement (12,1);
	
		m_Px[patchui][patchvi] = C[0].GetElement (13,1);
		m_Qx[patchui][patchvi] = C[0].GetElement (14,1);
		m_Rx[patchui][patchvi] = C[0].GetElement (15,1);
		m_Sx[patchui][patchvi] = C[0].GetElement (16,1);	
	}
	else
	{
		LogFileWriter::WriteUnknownExceptionLog ("ParametricSplineSurface::CalculateAndSetOnePatchCoef exception","GaussJordanElimination failed to find X coef");
		return false;
	}
	bool ck1 = gje.Solve (A[1],C[1]);
	if (ck1)
	{
		m_Ay[patchui][patchvi] = C[1].GetElement (1,1);
		m_By[patchui][patchvi] = C[1].GetElement (2,1);
		m_Cy[patchui][patchvi] = C[1].GetElement (3,1);
		m_Dy[patchui][patchvi] = C[1].GetElement (4,1);
	
		m_Ey[patchui][patchvi] = C[1].GetElement (5,1);
		m_Fy[patchui][patchvi] = C[1].GetElement (6,1);
		m_Gy[patchui][patchvi] = C[1].GetElement (7,1);
		m_Hy[patchui][patchvi] = C[1].GetElement (8,1);
	
		m_Ky[patchui][patchvi] = C[1].GetElement (9,1);
		m_Ly[patchui][patchvi] = C[1].GetElement (10,1);
		m_My[patchui][patchvi] = C[1].GetElement (11,1);
		m_Ny[patchui][patchvi] = C[1].GetElement (12,1);
	
		m_Py[patchui][patchvi] = C[1].GetElement (13,1);
		m_Qy[patchui][patchvi] = C[1].GetElement (14,1);
		m_Ry[patchui][patchvi] = C[1].GetElement (15,1);
		m_Sy[patchui][patchvi] = C[1].GetElement (16,1);	
	}
	else
	{
		LogFileWriter::WriteUnknownExceptionLog ("ParametricSplineSurface::CalculateAndSetOnePatchCoef exception","GaussJordanElimination failed to find Y coef");	
		return false;
	}
	bool ck2 = gje.Solve(A[2],C[2]);
	if (ck2)
	{
		m_Az[patchui][patchvi] = C[2].GetElement (1,1);
		m_Bz[patchui][patchvi] = C[2].GetElement (2,1);
		m_Cz[patchui][patchvi] = C[2].GetElement (3,1);
		m_Dz[patchui][patchvi] = C[2].GetElement (4,1);

		m_Ez[patchui][patchvi] = C[2].GetElement (5,1);
		m_Fz[patchui][patchvi] = C[2].GetElement (6,1);
		m_Gz[patchui][patchvi] = C[2].GetElement (7,1);
		m_Hz[patchui][patchvi] = C[2].GetElement (8,1);
	
		m_Kz[patchui][patchvi] = C[2].GetElement (9,1);
		m_Lz[patchui][patchvi] = C[2].GetElement (10,1);
		m_Mz[patchui][patchvi] = C[2].GetElement (11,1);
		m_Nz[patchui][patchvi] = C[2].GetElement (12,1);
	
		m_Pz[patchui][patchvi] = C[2].GetElement (13,1);
		m_Qz[patchui][patchvi] = C[2].GetElement (14,1);
		m_Rz[patchui][patchvi] = C[2].GetElement (15,1);
		m_Sz[patchui][patchvi] = C[2].GetElement (16,1);		
	}
	else
	{
		LogFileWriter::WriteUnknownExceptionLog ("ParametricSplineSurface::CalculateAndSetOnePatchCoef exception","GaussJordanElimination failed to find Z coef");	
		return false;
	}

	return true;
}
bool ParametricSplineSurface::CalculateAndSetOnePatchCoef2 (const std::list<std::pair<VPoint2d,VPoint>> &parPointMapS
															,const std::list<std::pair<VPoint2d,VPoint>> &parPointMapT
															,const std::list<std::pair<VPoint2d,VPoint>> &parSDervMap
															,const std::list<std::pair<VPoint2d,VPoint>> &parTDervMap
															,const std::list<std::pair<VPoint2d,VPoint>> &parSDervDervMap
															,const std::list<std::pair<VPoint2d,VPoint>> &parTDervDervMap
															,int patchui , int patchvi)
{
	std::vector<VPoint>sol1(4,VPoint(0.0)),sol2(4,VPoint(0.0)),sol3(4,VPoint(0.0)),sol4(4,VPoint(0.0)),sol5(4,VPoint(0.0)),sol6(4,VPoint(0.0));
	bool ck1 = SolveFor4Points (parPointMapS,sol1,false);
	if (ck1 == false)
		return false;
	else
	{
		m_Ax.at(patchui).at(patchvi) = sol1.at(0).x();
		m_Ay.at(patchui).at(patchvi) = sol1.at(0).y();
		m_Az.at(patchui).at(patchvi) = sol1.at(0).z();

		m_Bx.at(patchui).at(patchvi) = sol1.at(1).x();
		m_By.at(patchui).at(patchvi) = sol1.at(1).y();
		m_Bz.at(patchui).at(patchvi) = sol1.at(1).z();

		m_Cx.at(patchui).at(patchvi) = sol1.at(2).x();
		m_Cy.at(patchui).at(patchvi) = sol1.at(2).y();
		m_Cz.at(patchui).at(patchvi) = sol1.at(2).z();

		m_Dx.at(patchui).at(patchvi) = sol1.at(3).x();
		m_Dy.at(patchui).at(patchvi) = sol1.at(3).y();
		m_Dz.at(patchui).at(patchvi) = sol1.at(3).z();
	}
	bool ck2 = SolveFor4Points (parPointMapT,sol2,true);
	if (ck2 == false)
		return false;
	else
	{
		vreal Ax2 = sol2.at(0).x();
		vreal Ay2 = sol2.at(0).y();
		vreal Az2 = sol2.at(0).z();

		m_Ex.at(patchui).at(patchvi) = sol2.at(1).x();
		m_Ey.at(patchui).at(patchvi) = sol2.at(1).y();
		m_Ez.at(patchui).at(patchvi) = sol2.at(1).z();

		m_Kx.at(patchui).at(patchvi) = sol2.at(2).x();
		m_Ky.at(patchui).at(patchvi) = sol2.at(2).y();
		m_Kz.at(patchui).at(patchvi) = sol2.at(2).z();

		m_Px.at(patchui).at(patchvi) = sol2.at(3).x();
		m_Py.at(patchui).at(patchvi) = sol2.at(3).y();
		m_Pz.at(patchui).at(patchvi) = sol2.at(3).z();
	}
	bool ck3 = SolveFor4Points (parSDervMap,sol3,true);
	if (ck3 == false)
		return false;
	else
	{
		vreal Bx2 = sol3.at(0).x();
		vreal By2 = sol3.at(0).y();
		vreal Bz2 = sol3.at(0).z();

		m_Fx.at(patchui).at(patchvi) = sol3.at(1).x();
		m_Fy.at(patchui).at(patchvi) = sol3.at(1).y();
		m_Fz.at(patchui).at(patchvi) = sol3.at(1).z();

		m_Lx.at(patchui).at(patchvi) = sol3.at(2).x();
		m_Ly.at(patchui).at(patchvi) = sol3.at(2).y();
		m_Lz.at(patchui).at(patchvi) = sol3.at(2).z();

		m_Qx.at(patchui).at(patchvi) = sol3.at(3).x();
		m_Qy.at(patchui).at(patchvi) = sol3.at(3).y();
		m_Qz.at(patchui).at(patchvi) = sol3.at(3).z();
	}
	bool ck4 = SolveFor4Points (parTDervMap,sol4,false);
	if (ck4 == false)
		return false;
	else
	{
		vreal Ex2 = sol4.at(0).x();
		vreal Ey2 = sol4.at(0).y();
		vreal Ez2 = sol4.at(0).z();

		vreal Fx2 = sol4.at(1).x();
		vreal Fy2 = sol4.at(1).y();
		vreal Fz2 = sol4.at(1).z();

		m_Gx.at(patchui).at(patchvi) = sol4.at(2).x();
		m_Gy.at(patchui).at(patchvi) = sol4.at(2).y();
		m_Gz.at(patchui).at(patchvi) = sol4.at(2).z();

		m_Hx.at(patchui).at(patchvi) = sol4.at(3).x();
		m_Hy.at(patchui).at(patchvi) = sol4.at(3).y();
		m_Hz.at(patchui).at(patchvi) = sol4.at(3).z();

	}

	bool ck5 = SolveFor4Points (parSDervDervMap,sol5,true);
	if (ck5 == false)
		return false;
	else
	{
		vreal Cx2 = sol5.at(0).x();
		vreal Cy2 = sol5.at(0).y();
		vreal Cz2 = sol5.at(0).z();

		vreal Gx2 = sol5.at(1).x();
		vreal Gy2 = sol5.at(1).y();
		vreal Gz2 = sol5.at(1).z();

		m_Mx.at(patchui).at(patchvi) = sol5.at(2).x();
		m_My.at(patchui).at(patchvi) = sol5.at(2).y();
		m_Mz.at(patchui).at(patchvi) = sol5.at(2).z();

		m_Rx.at(patchui).at(patchvi) = sol5.at(3).x();
		m_Ry.at(patchui).at(patchvi) = sol5.at(3).y();
		m_Rz.at(patchui).at(patchvi) = sol5.at(3).z();
	}

	bool ck6 = SolveFor4Points (parTDervDervMap,sol6,false);
	if (ck6 == false)
		return false;
	else
	{
		vreal Kx2 = sol6.at(0).x();
		vreal Ky2 = sol6.at(0).y();
		vreal Kz2 = sol6.at(0).z();

		vreal Lx2 = sol6.at(1).x();
		vreal Ly2 = sol6.at(1).y();
		vreal Lz2 = sol6.at(1).z();

		m_Mx.at(patchui).at(patchvi) = sol6.at(2).x();
		m_My.at(patchui).at(patchvi) = sol6.at(2).y();
		m_Mz.at(patchui).at(patchvi) = sol6.at(2).z();

		m_Nx.at(patchui).at(patchvi) = sol6.at(3).x();
		m_Ny.at(patchui).at(patchvi) = sol6.at(3).y();
		m_Nz.at(patchui).at(patchvi) = sol6.at(3).z();
	}

	return true;
}
bool ParametricSplineSurface::SolveFor4Points (const std::list<std::pair<VPoint2d , VPoint>> &parPointMap 
												, std::vector<VPoint> &ABCD , bool isForT)
{
	Matrix A[3] = {Matrix(4,4),Matrix(4,4),Matrix(4,4)};
	Matrix C[3] = {Matrix(4,1),Matrix(4,1),Matrix(4,1)};
	
	std::list<std::pair<VPoint2d,VPoint>>::const_iterator ppi = parPointMap.begin();
	std::list<std::pair<VPoint2d,VPoint>>::const_iterator ppe = parPointMap.end();
	int counter = 1;
	for (ppi;ppi!=ppe;++ppi)
	{
		vreal t = isForT ? ppi->first.y() : ppi->first.x();
		vreal t2 = t*t;
		vreal t3 = t2*t;

		for (int i=0;i<3;++i)
		{
			A[i].SetElement (1.,counter,1);
			A[i].SetElement (t,counter,2);
			A[i].SetElement (t2,counter,3);
			A[i].SetElement (t3,counter,4);
			C[i].SetElement(ppi->second.operator[](i),counter,1);
		}
		++counter;
	}

	GaussJordanElimination gje[3];
	bool ck1 = gje[0].Solve (A[0],C[0]);
	if (ck1)
	{
		ABCD.at(0).x(C[0].GetElement(1,1));
		ABCD.at(1).x(C[0].GetElement(2,1));
		ABCD.at(2).x(C[0].GetElement(3,1));
		ABCD.at(3).x(C[0].GetElement(4,1));
	}
	else
		return false;
	bool ck2 = gje[1].Solve(A[1],C[1]);
	if (ck2)
	{
		ABCD.at(0).y(C[1].GetElement(1,1));
		ABCD.at(1).y(C[1].GetElement(2,1));
		ABCD.at(2).y(C[1].GetElement(3,1));
		ABCD.at(3).y(C[1].GetElement(4,1));
	}
	else
		return false;
	bool ck3 = gje[2].Solve(A[2],C[2]);
	if (ck3)
	{
		ABCD.at(0).z(C[2].GetElement(1,1));
		ABCD.at(0).z(C[2].GetElement(2,1));
		ABCD.at(0).z(C[2].GetElement(3,1));
		ABCD.at(0).z(C[2].GetElement(4,1));
	}
	else
		return false;
	return true;
}

bool ParametricSplineSurface::SolveFor4Derv (const std::list<std::pair<VPoint2d,VPoint>> &parDervMap
											, std::vector<VPoint> &ABCD , bool isForT)
{
	Matrix A[3] = {Matrix(4,4),Matrix(4,4),Matrix(4,4)};
	Matrix C[3]= {Matrix(4,1),Matrix(4,1),Matrix(4,1)};
	std::list<std::pair<VPoint2d,VPoint>>::const_iterator pdi = parDervMap.begin();
	std::list<std::pair<VPoint2d,VPoint>>::const_iterator pde = parDervMap.end();
	for (pdi;pdi!=pde;++pdi)
	{
		
	}
	return true;
}