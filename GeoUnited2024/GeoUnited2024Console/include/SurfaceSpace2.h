// SurfaceSpace2.h

#ifndef __SurfaceSpace2_h__
#define __SurfaceSpace2_h__

class UntrimmedSurface;

class SurfaceSpace2
{
public:
	SurfaceSpace2(int m, int n , UntrimmedSurface &surf);
	int GetM()const;
	int GetN()const;
	const UntrimmedSurface & GetSurf()const;
	UntrimmedSurface & GetSurf();

private:
	int m_m;
	int m_n;
	UntrimmedSurface &m_surf;
	mutable std::vector<std::vector<VPoint>> m_modelSpace;
	mutable std::vector<std::vector<VPoint2d>> m_paramSpace;
	static void FillSpaces(UntrimmedSurface &surf
		, vreal u0 , vreal u1
		, vreal v0 , vreal v1
		, int m , int n
		, std::vector<std::vector<VPoint2d>> &parSpace
		, std::vector<std::vector<VPoint>> &modSpace);
public:
	// returns -1 if unsuccessfull , returns xi otherwise
	int SearchNearest(const VPoint &modelPoint
		, const std::vector<std::vector<VPoint2d>> &parSpace
		, const std::vector<std::vector<VPoint>> &modSpace
		, int m , int n
		, int &xi, int &yi)const;
	// returns -1 if unsuccessfull , returrns subxi otherwise
	int RefineSolution(const VPoint &modelPoint
		, const std::vector <std::vector<VPoint2d>> &parSpace
		, const std::vector <std::vector<VPoint >> &modSpace
		, int xi , int yi
		, int xdivide, int ydivide
		, std::vector<std::vector<VPoint2d>> &subParSpace
		, std::vector<std::vector<VPoint>> &subModSpace
		, int &subxi , int &subyi
		)const;
	bool Unproject(const VPoint &modelPoint, vreal tol, VPoint2d &par)const;
	static int m_refineCount;
	static int m_refineXm;
	static int m_refineYn;
	static int m_divergeException;
};

#endif