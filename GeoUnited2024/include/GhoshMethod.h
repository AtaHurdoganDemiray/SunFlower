#ifndef GhoshMethod_h
#define GhoshMethod_h
#include "SequentialLineSet.h"
#include "LineOrArc.h"
#include "VLine.h"
#include "VPoint.h"
#include "GhoshItem.h" 
 
class GhoshMethod
{
public:
	GhoshMethod();
	~GhoshMethod();

private:
	SequentialLineSet m_geo;
	SequentialLineSet m_tool; 
	vreal m_digitiseStep;
	VPoint m_toolRefPoint;
public:
	void SetGeo(const std::vector<LineOrArc>&geo);
	void SetGeo(const SequentialLineSet &goeLS);
	void SetTool(const std::vector<LineOrArc> &tool);
	void SetTool(const SequentialLineSet& toolLS);
	void SetToolRefPoint(const VPoint& p);
	void SetDigitiseStep(vreal digitiseStep);
	VPoint GetToolRefPoint();
	SequentialLineSet GetGeo();
	SequentialLineSet GetTool();
	vreal GetDigitiseStep();
private:
	void CreateMergeList();
	void GhoshFunc();


	std::vector<GhoshItem> m_ghosList;
	std::vector<GhoshItem> m_mergeVec;
	SequentialLineSet m_resultLS;
public:
	std::vector<GhoshItem> &GetGhoshResult();
	void Calc();
private :
	void AddGhsItemToList(GhoshItem& ghs);
public:
	// if not find return -1 other wise return index
	static int FindVecIndexByItemIndex(int ghsItemIndex, const std::vector<GhoshItem> &vec);
	static void GetLineSet(const std::vector<GhoshItem>& ghsVec, SequentialLineSet &ls);
 
	std::vector<LineOrArc>* GetLineResult();
private:
	void GhoshFunc2();
	SequentialLineSet GetResultLs();
	
};


#endif