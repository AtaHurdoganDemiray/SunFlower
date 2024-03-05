// MultiTrianglesSectionMaker.h

#ifndef __MultiTrianglesSectionMaker_h__
#define __MultiTrianglesSectionMaker_h__

struct VLine;
class PosCubes;
class MultiTriangles;
class LineSet;
struct VPlane;

class MultiTrianglesSectionMaker
{
public:
	static void MakeSectionOfOnePlane (const VLine &currline 
									, PosCubes & mycube
									, MultiTriangles &multiTri
									, LineSet &sectionLines );
	static void MakeSectionOfZPlane (const vreal z
										, PosCubes &mycube
										, MultiTriangles &multitri
										, LineSet &sectionLines);

};

#endif