#ifndef MakeHull_h
#define MakeHull_h
#include "LineSet.h"
#include "SequentialLineSet.h"
#include "LineOrArc.h"
#include "CoordinateSystem.h"

class VMakeHull
{
public:
	VMakeHull();
	~VMakeHull();
 	
private:
	std::map<vreal, std::vector<VPoint>> m_xMap;
	std::map<vreal, std::vector<VPoint>> m_yMap;
	 
	LineSet m_lineSet; 
	std::vector<std::vector<LineOrArc>> m_larVec;
	CoordinateSystem m_relCS;
	CoordinateSystem m_absCS;
	VPoint m_minP;
	vreal m_tol;
public:
	void SetLineOrArc(const std::vector<std::vector<LineOrArc>> &larVec, vreal tol);
	void Calc(vreal digitiseStep);
	std::map<vreal, std::vector<VPoint>> GetResultPointsX();
	std::map<vreal, std::vector<VPoint>> GetResultPointsY();

private:
	void LineSetToRelativeCS();
	void LineSetToAbsoluteCS(); 
    
	void CalcX(vreal digitiseStep);
	void CalcY(vreal digitiseStep);



};

#endif