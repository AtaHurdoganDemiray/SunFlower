
#ifndef __ArcFit_h__
#define __ArcFit_h__

#include "LineOrArc.h"
#include "VPoint.h"
struct CoordinateSystem;
struct VPlane;

class ArcFit
{
public:
	ArcFit();
	
	int FitTr (const std::vector<VPoint> &Points , vreal tol , std::vector<LineOrArc> &result , const CoordinateSystem &cs);

	int Fit(const std::vector<VPoint> &Points, vreal tol, std::vector<LineOrArc> &result);

	static int FitRecursive (const std::vector<VPoint> &Points , vreal tol , std::vector<LineOrArc> &result);
	static bool FitLineOrArcRecursive (const std::vector<VPoint> &Points , vreal tol , std::vector<LineOrArc> &results , int startIndice , int endIndice , std::map<int,int> &sortHelper);//first int of sortHelper is start indice of point in Points , second int is LineOrArc indice in results
	//return 0: not a line or arc
	//return 1: line, loa is set to that line
	//return 2: arc, loa is set to that arc
	int IsLineOrArcTr (const std::vector<VPoint> &Points, vreal tol, LineOrArc &loa , const CoordinateSystem &cs);
	int IsLineOrArc(const std::vector<VPoint> &Points, vreal tol, LineOrArc &loa);
	void CleanCorruptedArc (std::vector<LineOrArc> &result);

	/* Helper Functions */

	//computes the up vector and forms the arc
	static bool MakeArc(const std::vector<VPoint> &Points, int i0, int i1, VPoint &center, vreal radius, VArc &arc);

	//line or arc passes through start and end points (i0 and i1)
	static bool FitLineOrArc(const std::vector<VPoint> &Points, LineOrArc &result, vreal tol, int i0 = -1, int i1 = -1);

	//error is the maximum perpendicular distance of all points
	static vreal CalcLineErr(const std::vector<VPoint> &Points, int i0, int i1);

	//error is the maximum |ri - r| of all points
	static vreal CalcArcErr(const std::vector<VPoint> &Points, int i0, int i1, const VArc &arc);

	//error of the line or arc based on the CalcLineErr and CalcArcErr functions
	static vreal CalcErr(const std::vector<VPoint> &Points, int i0, int i1, const LineOrArc &la);

	//fit plane to given points
	//static vreal FitPlane(const std::vector<VPoint> &Points, VPlane &plane);

protected:
	static void CloseArcIf (bool isclosed , VArc &a);
	static void HandleFitLineOrArcException (const std::vector<VPoint> &Points , int i0 , int i1 , LineOrArc &result);
	static int HandleArcFitFitException (const std::vector<VPoint> &Points , std::vector<LineOrArc> &result);
	static void ReplaceArcsWithLinesIf(std::vector<LineOrArc> &result, vreal tol);
	static bool ReplaceArcIf(const LineOrArc &a, vreal tol, VLine &l);
};

#endif
