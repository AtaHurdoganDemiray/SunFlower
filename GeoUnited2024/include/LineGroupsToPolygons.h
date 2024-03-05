// LineGroupsToPolygons.h

#ifndef __LineGroupsToPolygons_h__
#define __LineGroupsToPolygons_h__

struct VLine2dE;
struct VPoint2d;

class LineGroupsToPolygons
{
public:
	struct Angle_Index
	{
	public:
		double a;
		int i;
	};

	static void Convert (const std::vector<std::vector <VLine2dE> > &linegroups
					, std::vector<std::vector<VPoint2d>> &polygons);
	
private:
	static void ConvertSinglePolygon (const std::vector<VLine2dE> &linegroup
									, std::vector<VPoint2d> &polygon );

};

#endif