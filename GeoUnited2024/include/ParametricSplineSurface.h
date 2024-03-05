// ParametricSplineSurface.h

#ifndef __ParametricSplineSurface_h__
#define __ParametricSplineSurface_h__

#include "IgesEntity.h"

class ParametricSplineSurface : public IgesEntity
{
public:
	ParametricSplineSurface ();
	ParametricSplineSurface (int patchun , int patchvn);
	virtual void GetEntityIn (FileLines &mylines , int poffs);
	virtual void WriteEntityOut (std::vector<VStr> &strvec)const;
	virtual void GetChildren (std::vector<int> &childVec)const;
	virtual void AcceptVisitor (IgesEntityVisitor &visitor)const;
	//virtual bool IsPrimitiveEntity ()const;
	//virtual double GetSortNo()const;
	virtual IgesEntity * MakeCopyAndAddToSet (const IgesEntitySet &mainSet , IgesEntitySet &newSet)const;

	int m_CTYPE;
	int m_PTYPE;
	int m_M;
	int m_N;
	std::vector<vreal> m_Tu;
	std::vector<vreal> m_Tv;

	// X sequence
    std::vector<std::vector<vreal> > m_Ax;
    std::vector<std::vector<vreal> > m_Bx;
    std::vector<std::vector<vreal> > m_Cx;
    std::vector<std::vector<vreal> > m_Dx;
	
    std::vector<std::vector<vreal> > m_Ex;
    std::vector<std::vector<vreal> > m_Fx;
    std::vector<std::vector<vreal> > m_Gx;
    std::vector<std::vector<vreal> > m_Hx;
	
    std::vector<std::vector<vreal> > m_Kx;
    std::vector<std::vector<vreal> > m_Lx;
    std::vector<std::vector<vreal> > m_Mx;
    std::vector<std::vector<vreal> > m_Nx;
	
    std::vector<std::vector<vreal> > m_Px;
    std::vector<std::vector<vreal> > m_Qx;
    std::vector<std::vector<vreal> > m_Rx;
    std::vector<std::vector<vreal> > m_Sx;

	// Y sequence
    std::vector<std::vector<vreal> > m_Ay;
    std::vector<std::vector<vreal> > m_By;
    std::vector<std::vector<vreal> > m_Cy;
    std::vector<std::vector<vreal> > m_Dy;
	
    std::vector<std::vector<vreal> > m_Ey;
    std::vector<std::vector<vreal> > m_Fy;
    std::vector<std::vector<vreal> > m_Gy;
    std::vector<std::vector<vreal> > m_Hy;
	
    std::vector<std::vector<vreal> > m_Ky;
    std::vector<std::vector<vreal> > m_Ly;
    std::vector<std::vector<vreal> > m_My;
    std::vector<std::vector<vreal> > m_Ny;
	
    std::vector<std::vector<vreal> > m_Py;
    std::vector<std::vector<vreal> > m_Qy;
    std::vector<std::vector<vreal> > m_Ry;
    std::vector<std::vector<vreal> > m_Sy;

	// Z sequence
    std::vector<std::vector<vreal> > m_Az;
    std::vector<std::vector<vreal> > m_Bz;
    std::vector<std::vector<vreal> > m_Cz;
    std::vector<std::vector<vreal> > m_Dz;
	
    std::vector<std::vector<vreal> > m_Ez;
    std::vector<std::vector<vreal> > m_Fz;
    std::vector<std::vector<vreal> > m_Gz;
    std::vector<std::vector<vreal> > m_Hz;
	
    std::vector<std::vector<vreal> > m_Kz;
    std::vector<std::vector<vreal> > m_Lz;
    std::vector<std::vector<vreal> > m_Mz;
    std::vector<std::vector<vreal> > m_Nz;
	
    std::vector<std::vector<vreal> > m_Pz;
    std::vector<std::vector<vreal> > m_Qz;
    std::vector<std::vector<vreal> > m_Rz;
    std::vector<std::vector<vreal> > m_Sz;

	bool CalculateAndSetOnePatchCoef ( const std::list<std::pair<VPoint2d , VPoint>> &parPointMap // VPoint2d x->u or s , y->v or t
										, const std::list<std::pair<VPoint2d , VPoint>> &parSDervMap
										, const std::list<std::pair<VPoint2d , VPoint>> &parTDervMap
										, int patchui , int patchvi);
	bool CalculateAndSetOnePatchCoef2 (const std::list<std::pair<VPoint2d,VPoint>> &parPointMapS
		,const std::list<std::pair<VPoint2d,VPoint>> &parPointMapT
		,const std::list<std::pair<VPoint2d,VPoint>> &parSDervMap
		,const std::list<std::pair<VPoint2d,VPoint>> &parTDervMap
		,const std::list<std::pair<VPoint2d,VPoint>> &parSDervDervMap
		,const std::list<std::pair<VPoint2d,VPoint>> &parTDervDervMap
		,int patchui , int patchvi);
	bool SolveFor4Points (const std::list<std::pair<VPoint2d , VPoint>> &parPointMap
							, std::vector<VPoint> &ABCD , bool isForT);
	bool SolveFor4Derv (const std::list<std::pair<VPoint2d,VPoint>> &parDervMap
						, std::vector<VPoint> &ABCD , bool isForT);

};
#endif
