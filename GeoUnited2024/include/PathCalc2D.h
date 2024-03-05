
#ifndef PathCalc2D_h
#define PathCalc2D_h
#include "LineOrArc.h"
#include "SequentialLineSet.h" 
class PathCalc2D
{
public:
	PathCalc2D();
	PathCalc2D(const std::vector<LineOrArc> &tool, const VPoint &toolRefPoint, const std::vector<LineOrArc> &geo, const vreal &toolDiigitise, const vreal &geoDigitise); 
	~PathCalc2D();
 
	std::vector<LineOrArc> m_tool;
	std::vector<LineOrArc> m_geo;
	VPoint m_toolRefPoint; 
	SequentialLineSet *m_toolSeqLineSet;
	SequentialLineSet *m_geoSeqLineSet;
 
	vreal m_geoDigitise;
	vreal m_toolDigitise;
private:
	void MakeSeqLineSet(const std::vector <LineOrArc>& lineOrArc, SequentialLineSet & seqLineSet,vreal digitiseStep);

public:
	bool Calc(SequentialLineSet & path, std::vector<LineOrArc>& laVec);
	void SetTool(const std::vector<LineOrArc> &tool);
	void SetGeo(const std::vector<LineOrArc> & geo);
	void SetToolRefPoint(const VPoint& p);
	void SetToolDigitiseStepLength(vreal stepLen);
	void SetGeoDigitiseStepLength(vreal stepLen);
	VPoint GetToolRefP();
	vreal GetToolDigitiseSteLength();
	vreal GetGeoDigitiseSteLength();
	
	static int IsPointRightLeftOnLine(const VPoint& p, const VLine &ls);
	bool IsPointExist(const VPoint& p, const std::vector<VPoint> & pointVec);
};

#endif