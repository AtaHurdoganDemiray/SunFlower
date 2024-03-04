// AnalyzeSurface.h

#ifndef __AnalyzeSurface_h__
#define __AnalyzeSurface_h__

class UntrimmedSurface;
struct VPoint;

class AnalyzeSurface
{
public:
	enum SurfaceType {none,planar,cylindirical,conical,sphere,torus,extruded,revolved,ruled};

	SurfaceType IsPlanarOrCylinder (const UntrimmedSurface &surf 
								 , const double &u0
								 , const double &u1
								 , const double &v0
								 , const double &v1
								, double &radius0
								, double &radius1
								, double &h
								, VPoint &center
								, VPoint &ax 
								, bool &isClosedCylinder
								, vreal tol);

	int GetUStep (const UntrimmedSurface &surf
					, const double &tol );
	int GetVStep (const UntrimmedSurface &surf
					, const double &tol);
	vreal GetParStepForU (const UntrimmedSurface &surf,const vreal u,const vreal v,const vreal tol);
	vreal GetParStepForV (const UntrimmedSurface &surf,const vreal u,const vreal v,const vreal tol);
	static vreal GetAprModelSpacePerimeterAlongU (const UntrimmedSurface &surf);
	static vreal GetAprModelSpacePerimeterAlongV (const UntrimmedSurface &surf); 
};

#endif