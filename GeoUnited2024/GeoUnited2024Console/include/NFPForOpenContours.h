#ifndef NFPForOpenContours_h
#define NFPForOpenContours_h
#include "LineOrArc.h"
#include "SequentialLineSet.h"

class NFPForOpenContours
{
public:
	NFPForOpenContours();
	~NFPForOpenContours();

private:
	std::vector<LineOrArc> m_closedTool;
	std::vector<LineOrArc> m_cuttingTool;
	std::vector<LineOrArc> m_openOrClosedContour; // ordered.
	VPoint m_toolRefPoint;
	int m_leftRightBoth; // 0 left, 1 right, 2 both
	vreal m_tol;

	SequentialLineSet *m_tool;
	SequentialLineSet *m_cutTool;
	SequentialLineSet *m_geo; 
	SequentialLineSet *m_result;
	std::vector<LineOrArc> * m_resultLA;
public:
	void SetTool(const std::vector<LineOrArc> & closedTool);
	void SetTool(SequentialLineSet * tool);
	void SetCuttingTool(const std::vector<LineOrArc> & cuttingTool);
	void SetContour(const std::vector<LineOrArc> & orderedOpneOrClosedContour);
	void SetToolRefPoint(const VPoint & p);
	void SetTol(vreal tol);
	void SetLeftRightBoth(int lrb);

	std::vector<LineOrArc> GetTool();
	SequentialLineSet * GetToolLS();
	std::vector <LineOrArc> GetCuttingTool();
	std::vector<LineOrArc> GetContour();
	VPoint GetToolRefPoint();
	vreal GetTol(); 
	int GetLeftRightBoth();
    
	void Calc();
	SequentialLineSet* GetResult();

private:
	void Move(const VPoint & moveP, const VPoint & targetPos, SequentialLineSet& ls);
public: 
	static int IsPointLeftRightOnLine(const VPoint& p, const VLine & line);
	bool IsToolInGeo(const SequentialLineSet & ls);
	static bool IsPointExist(const VPoint& p, const std::vector<VPoint> & pointVec);
	std::vector<LineOrArc>* GetResultLA();
};

#endif