// SilhoutteCalc.h

#ifndef __SilhoutteCalc_h__
#define __SilhoutteCalc_h__

#include "CoordinateSystem.h"
#include "VPoint.h"

class Triangles;
class SequentialLineSet;
class SequentialLineSet2d;
struct VTriangle;
namespace geo
{
	class BspTree;
};

class SilhoutteCalc
{
public:
	SilhoutteCalc (const Triangles *mesh , std::vector<VPoint> &sortedSilhPoints , CoordinateSystem cs , double resolution);
	~SilhoutteCalc ();
	static void Calc (const Triangles *mesh , const CoordinateSystem &cs , std::vector<SequentialLineSet *> &resVec);
private:
	void DecideDigitiseAxis();
	void MakeArrays (int nx,int ny);
	void DestroyArrays ();
	void Digitise ();
	void DigitiseAlongX (const VPoint &c1 , const VPoint &c2);
	void DigitiseAlongY (const VPoint &c1 , const VPoint &c2);
	double GetCoordX (int val);
	int GetIntX (double coord);
	double GetCoordY (int val);
	int GetIntY (double coord);
	void CalculatePoints ();
	const Triangles *m_mesh;
	std::vector<VPoint> &m_sortedSilhPoints;

	VPoint m_min,m_max;
	double m_dx,m_dy;
	double m_minx,m_miny;

	double m_resolution;
	double m_inverseResolution;
	double *m_mnX;
	double *m_mxX;
	double *m_mnY;
	double *m_mxY;
	int m_nx,m_ny;
	CoordinateSystem m_cs;

	static geo::BspTree * GetProjectedCCWLoop (const VTriangle &tri , const CoordinateSystem &cs);
};

#endif