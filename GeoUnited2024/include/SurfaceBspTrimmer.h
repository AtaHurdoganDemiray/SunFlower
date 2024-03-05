// SurfaceBspTrimmer.h

#ifndef __SurfaceBspTrimmer_h__
#define __SurfaceBspTrimmer_h__

class SequentialLineSet2d;
struct VLine2d;
struct VPoint2d;
struct VLine2dE;
struct VTriangle2d;
class dSurface;

class SurfaceBspTrimmer
{
public:
	SurfaceBspTrimmer ( dSurface &surf 
						, const SequentialLineSet2d &outerLoop
						, const std::vector<SequentialLineSet2d *> &innerLoopVec );
	~SurfaceBspTrimmer();
	std::vector<VTriangle2d> * Trim ();

	void DivideUntil (const std::vector<VLine2dE> &toDivide 
						, std::vector<std::vector<VLine2dE>> &linegroups);
	bool Divide (const std::vector<VLine2dE>::const_iterator &l
				, const std::vector<VLine2dE> &toDivide
				, std::vector <VLine2dE> &posChildVec 
				, std::vector<VLine2dE> &negChildVec);
	enum Type {POSITIVE,NEGATIVE,CROSSING,COINCIDENT};
	Type Classify (const VLine2dE &l1, const VLine2dE &l2
					, VLine2dE &posChild 
					, VLine2dE &negChild);


private:
	void MakeLineVec ();
	void MakeConvexPolygons (const std::vector<std::vector<VLine2dE>> &linegroups
		, std::vector <std::vector <VPoint2d>> &polygons);
	
	void FillTriangleVec (const std::vector<std::vector<VPoint2d>> &polygons
						, std::vector<VTriangle2d> &triVec);
	void AddSinglePolygon (const std::vector<VPoint2d> &polygons
						, std::vector<VTriangle2d> &triVec);
	void FillMyLineVec (const SequentialLineSet2d *sl,bool rev);
	bool ShouldIReverseInnerLoop (const SequentialLineSet2d *sl , int i)const;
	bool ShouldIReverseOuterLoop (const SequentialLineSet2d *sl)const;
	const SequentialLineSet2d &m_outerLoop;
	const std::vector<SequentialLineSet2d *> &m_innerLoopVec;
	std::vector<VLine2dE> m_lineVec;
	dSurface &m_surf;
};

#endif