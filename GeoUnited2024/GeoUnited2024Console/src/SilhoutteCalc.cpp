// SilhoutteCalc.cpp

#include "stdafx.h"
#include "SilhoutteCalc.h"
#include "Triangles.h"
#include "VTriangle.h"
#include "BspTree.h"
#include "BspTreeBooleanOp.h"
#include "BspTreeConstructor.h"
#include "SequentialLineSet2d.h"
#include "SequentialLineSet.h"
#include "LineSet2d.h"
#include "LineSet2d2SequentialLineSet2d.h"
#include "BspTreeToLineSet2d.h"

using namespace geo;

SilhoutteCalc::SilhoutteCalc (const Triangles *mesh , std::vector<VPoint> &sortedSilhPoints , CoordinateSystem cs , double resolution)
:m_cs (cs) , m_resolution (resolution) , m_mesh (mesh) , m_sortedSilhPoints (sortedSilhPoints)
{
	m_inverseResolution = 1.0 / m_resolution;
	VPoint mymin (MAX_NUMBER_LIMIT);
	VPoint mymax (MIN_NUMBER_LIMIT);
	int mysize = m_mesh->Size();
	for (int i=0;i<mysize;++i)
	{
		VTriangle ct = m_mesh->GetTri(i);
		VPoint tc1 = m_cs.AbsoluteCoordToRelativeCoord(ct.GetCorner1());
		VPoint tc2 = m_cs.AbsoluteCoordToRelativeCoord(ct.GetCorner2());
		VPoint tc3 = m_cs.AbsoluteCoordToRelativeCoord(ct.GetCorner3());
		mymin.ReplaceMin(tc1);
		mymin.ReplaceMin(tc2);
		mymin.ReplaceMin(tc3);
		mymax.ReplaceMax(tc1);
		mymax.ReplaceMax(tc2);
		mymax.ReplaceMax(tc3);
	}
	m_min = mymin;
	m_max = mymax;
	m_dx = m_max.x() - m_min.x();
	m_dy = m_max.y() - m_min.y();	

	m_minx = m_min.x();
	m_miny = m_min.y();
	m_nx = (int) (m_dx * m_inverseResolution + 1);
	m_ny = (int) (m_dy * m_inverseResolution + 1);
	MakeArrays(m_nx,m_ny);
	Digitise();
	CalculatePoints();
}

SilhoutteCalc::~SilhoutteCalc()
{
	DestroyArrays();
}

void SilhoutteCalc::MakeArrays(int nx, int ny)
{
	m_mnX = new double[nx];
	m_mxX = new double[nx];
	m_mnY = new double[ny];
	m_mxY = new double[ny];
	for (int i=0;i<nx;++i)
	{
		m_mnX[i] = MAX_NUMBER_LIMIT;
		m_mxX[i] = MIN_NUMBER_LIMIT;
	}
	for (int i=0;i<ny;++i)
	{
		m_mnY[i] = MAX_NUMBER_LIMIT;
		m_mxY[i] = MIN_NUMBER_LIMIT;
	}
}

void SilhoutteCalc::DestroyArrays()
{
	delete [] m_mnX;
	delete [] m_mxX;
	delete [] m_mnY;
	delete [] m_mxY;
}

void SilhoutteCalc::Digitise()
{
	int mysize = m_mesh->Size();
	for (int i=0;i<mysize;++i)
	{
		VTriangle curtri = m_mesh->GetTri(i);
		VPoint tc1 = m_cs.AbsoluteCoordToRelativeCoord(curtri.GetCorner1());
		VPoint tc2 = m_cs.AbsoluteCoordToRelativeCoord(curtri.GetCorner2());
		VPoint tc3 = m_cs.AbsoluteCoordToRelativeCoord(curtri.GetCorner3());
		DigitiseAlongX(tc1,tc2);
		DigitiseAlongX(tc2,tc3);
		DigitiseAlongX(tc3,tc1);
		//DigitiseAlongY(tc1,tc2);
		//DigitiseAlongY(tc2,tc3);
		//DigitiseAlongY(tc3,tc1);
	}
}

void SilhoutteCalc::DigitiseAlongX(const VPoint &c1, const VPoint &c2)
{
	//int x0 = GetIntX(c1.x());
	//int x1 = GetIntX(c2.x());
	//if (x0 == x1)
	//	return;
	//double dy = c2.y() - c1.y();
	//double y0 = c1.y();
	//if (x0 > x1)
	//{
	//	int tmp = x0;
	//	x0 = x1;
	//	x1 = tmp;
	//	dy = -dy;
	//	y0 = c2.y();
	//}
	//if (x0 < 0 || x0 >= m_nx || x1 < 0 || x1 >= m_nx)
	//{
	//	throw VException (("SilhoutteCalc::DigitiseAlongX exception , x exceed array limits!"),1);
	//} 
	//double invdx = 1 / (x1 - x0);
	//int x1p = x1 + 1;


	//for (int x=x0;x<x1p;++x)
	//{
	//	double y = y0 + (x-x0) * dy * invdx;
	//	if (m_mnX[x] > y)
	//		m_mnX[x] = y;
	//	if (m_mxX[x] < y)
	//		m_mxX[x] = y;
	//}

	VPoint dif = c2 - c1;
	double diflen = dif.GetLength();
	VPoint udif = dif * (1 / diflen);
	double dt = m_resolution / diflen;
	for (double t=0.0;t<1;t+=dt)
	{
		VPoint curp = c1 + udif * t;
		int cx = GetIntX(curp.x());
		if (m_mnX[cx] > curp.y())
			m_mnX[cx] = curp.y();
		if (m_mxX[cx] < curp.y())
			m_mxX[cx] = curp.y();	
	}
}

void SilhoutteCalc::DigitiseAlongY(const VPoint &c1, const VPoint &c2)
{
	int y0 = GetIntY(c1.y());
	int y1 = GetIntY(c2.y());
	double invdy = 1 / (y1 - y0);
	int y1p = y1 + 1;
	double dx = c2.x() - c1.x();
	double x0 = c1.x();

	for (int y = y0; y < y1p; ++y)
	{
		double x = x0 + (y-y0) * dx * invdy;
		if (m_mnY[y] > x)
			m_mnY[y] = x;
		if (m_mxY[y] < x)
			m_mxY[y] = x;
	}
}

double SilhoutteCalc::GetCoordX(int val)
{
	return val * m_resolution + m_minx;
}

int SilhoutteCalc::GetIntX(double coord)
{
	return (int) ((coord-m_minx) * m_inverseResolution);
}
double SilhoutteCalc::GetCoordY(int val)
{
	return val * m_resolution + m_miny;
}
int SilhoutteCalc::GetIntY(double coord)
{
	return (int) ((coord - m_miny) * m_inverseResolution);
}
void SilhoutteCalc::CalculatePoints()
{
	for (int x=0;x<m_nx;++x)
	{
		double myx = GetCoordX(x);
		double ymn = m_mnX[x];
		double ymx = m_mxX[x];
		if (ymn != MAX_NUMBER_LIMIT)
		{
			VPoint pp (myx , ymn , 0.0);
			VPoint ap = m_cs.RelativeCoordToAbsoluteCoord(pp);
			m_sortedSilhPoints.push_back(ap);
		}
		if (ymx != MIN_NUMBER_LIMIT)
		{
			VPoint pp (myx , ymx , 0.0);
			VPoint ap = m_cs.RelativeCoordToAbsoluteCoord(pp);
			m_sortedSilhPoints.push_back(ap);
		}
	}
}

void SilhoutteCalc::Calc(const Triangles *mesh, const CoordinateSystem &cs, std::vector<SequentialLineSet *> &resVec)
{
	int mysize = mesh->Size();
	if (mysize == 0)
		return;
	VTriangle curtri = mesh->GetTri(0);
	BspTree *bloop = GetProjectedCCWLoop(curtri,cs);
	for (int i=1;i<mysize;++i)
	{
		VTriangle curtri = mesh->GetTri(i);
		BspTree *curloop = GetProjectedCCWLoop(curtri,cs);
		bloop = BspTreeBooleanOp::Unite(*bloop,*curloop);
	}
	BspTreeToLineSet2d mylinesetmaker;
	LineSet2d *ls = mylinesetmaker.Make(*bloop);
	LineSet2d2SequentialLineSet2d myconv;
	std::vector<SequentialLineSet2d *> slVec = myconv.Make(*ls);
	delete ls;

	size_t mysize2 = slVec.size();
	for (size_t i=0;i<mysize2;++i)
	{
		SequentialLineSet2d *sl2d = slVec.at(i);
		SequentialLineSet *cursl = new SequentialLineSet (sl2d->Size());
		int mysize3 = sl2d->GetPointSize();
		for (int j=0;j<mysize3;++j)
		{
			VPoint2d *curp = sl2d->GetPoint(j);
			VPoint curpp (curp->x(),curp->y(),0.0);
			curpp = cs.RelativeCoordToAbsoluteCoord(curpp);
			cursl->InsertPoint(curpp);
		}
		resVec.push_back(cursl);
		delete sl2d;
	}
}

geo::BspTree * SilhoutteCalc::GetProjectedCCWLoop(const VTriangle &tri, const CoordinateSystem &cs)
{
	VPoint tp0 = cs.AbsoluteCoordToRelativeCoord (tri.GetCorner1());
	VPoint tp1 = cs.AbsoluteCoordToRelativeCoord(tri.GetCorner2());
	VPoint tp2 = cs.AbsoluteCoordToRelativeCoord(tri.GetCorner3());

	VPoint tpp0 (tp0.x(),tp0.y(),0.0);
	VPoint tpp1 (tp1.x(),tp1.y(),0.0);
	VPoint tpp2 (tp2.x(),tp2.y(),0.0);
	VTriangleE mytri (tpp0 , tpp1 , tpp2);
	LineSet2d ls(4);
	VPoint2d c1(tp0.x(),tp0.y());
	VPoint2d c2(tp1.x(),tp1.y());
	VPoint2d c3(tp2.x(),tp2.y());
	if (mytri.GetNormal() % UZ > 0.0)
	{
		ls.AddLine(c1,c2);
		ls.AddLine(c2,c3);
		ls.AddLine(c3,c1);
	}
	else
	{
		ls.AddLine(c1,c3);
		ls.AddLine(c3,c2);
		ls.AddLine(c2,c1);
	}
	BspTreeConstructor mycons;
	return mycons.ConstructTree(ls);
}
