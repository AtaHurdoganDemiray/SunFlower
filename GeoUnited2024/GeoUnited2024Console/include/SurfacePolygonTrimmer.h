// SurfacePolygonTrimmer.h

#ifndef __SurfacePolygonTrimmer_h__
#define __SurfacePolygonTrimmer_h__

class dSurface;
class SequentialLineSet2d;
class VRefTriangleMesh;

class SurfacePolygonTrimmer
{
public:
	SurfacePolygonTrimmer (dSurface &surf
							,SequentialLineSet2d *outerloop
							,const std::vector<SequentialLineSet2d *> &innerLoopVec);
	VRefTriangleMesh * Trim ();
private:
	bool ShouldIReverseInnerLoop (const SequentialLineSet2d *sl , int i)const;
	bool ShouldIReverseOuterLoop (const SequentialLineSet2d *sl)const;

	dSurface &m_surf;
	SequentialLineSet2d * m_outerLoop;
	const std::vector <SequentialLineSet2d *> & m_innerLoopVec;
};

#endif