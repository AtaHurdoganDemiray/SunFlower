// InfiniteCylinderLineIntersection.cpp

#include "stdafx.h"
#include "InfiniteCylinderLineIntersection.h"
#include "QdrEqSolver.h"

int InfiniteCylinderLineIntersection::Int(const VPoint &cylcenter, const VPoint &cylax, vreal  cylrad , const VLine &l, std::vector<vreal> &intt)
{
	const VPoint &l0 = l.GetP1();
	VPoint lv = l.GetV();
	vreal lineLen = lv.GetLength();
	//lv.NormThis();
	vreal invLineLen = 1. / lineLen;
	lv *= invLineLen;
	VPoint A = l0 - cylcenter;
	VPoint B = A ^ cylax;
	VPoint C = lv ^ cylax;
	vreal b = B % B;
	vreal c = C % C;
	vreal d = 2.0 * B % C;
	vreal roots[2];
	vreal R2 = cylrad*cylrad;
	int rootno = QdrEqSolver<vreal>::FindRoots(c, d, b - R2, roots);
	for (int i = 0; i < rootno; ++i)
		intt.push_back(roots[i]*invLineLen);
	return rootno;
}