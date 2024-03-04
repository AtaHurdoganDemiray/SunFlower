// LeastSquaresPlaneFit.h

#ifndef __LeastSquaresPlaneFit_h__
#define __LeastSquaresPlaneFit_h__

struct VPlane;
struct CoordinateSystem;

class LeastSquaresPlaneFit
{
public:
	static bool Fit(const std::vector<VPoint> &pvec, VPlane &p,const VPoint &preferredPositiveNormalDirectionIf); // returns false if calculation fails , otherwise p is filled with best fit plane
	static bool Fit(const std::list<const std::vector<VPoint>*> &allp, VPlane &p,const VPoint &preferredPositiveNormalDirectionIf);
	static bool FitSearchAClosePlaneInitially(const std::list<const std::vector<VPoint>*> &allp, CoordinateSystem &solCs , vreal &maxError , const CoordinateSystem &preferedPositiveDirectionsIf);// returns true if a plane is found successfully and max error is filled
	static bool GetUnusedPoint(const std::vector<VPoint> &allp2, std::map<int,int> &usedp, int &pp1, int &pp2, int &pp3);
	static vreal GetMaxError(const std::vector<VPoint> &allp2, const CoordinateSystem &cs);
};

#endif