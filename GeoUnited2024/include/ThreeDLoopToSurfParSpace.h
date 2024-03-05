// ThreeDLoopToSurfParSpace.h

#ifndef __ThreeDLoopToSurfParSpace_h__
#define __ThreeDLoopToSurfParSpace_h__

class VRefTriangleMesh;
class SequentialLineSet;
class PointSet;

class ThreeDLoopToSurfParSpace
{
public:
	static void Convert (const VRefTriangleMesh &surf
		, const SequentialLineSet &loop
		, const vreal tolstep
		, PointSet &pset );
	static void DigitiseLoop (const SequentialLineSet &poly 
							, const vreal mystep
							, PointSet &densp);
};

#endif