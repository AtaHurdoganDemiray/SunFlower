// ImprovedTriangleContainerBox.h

#ifndef __ImprovedTriangleContainerBox_h__
#define __ImprovedTriangleContainerBox_h__

#include "CoordinateSystem.h"
#include "TransformationMatrix.h"
class MultiTriangles;

class ImprovedTriangleContainerBox
{
public:
	ImprovedTriangleContainerBox();
	void Construct(MultiTriangles *multiTri , const std::list<int> &trianglesInside , vreal bestBoxSearchAngleStepRough , vreal bestBoxSearchAngleStepFine);

private:
	MultiTriangles *m_multiTri;
	std::list<int> m_trianglesInside;
	CoordinateSystem m_cs;
	VPoint m_minInCs;
	VPoint m_maxInCs;
	TransformationMatrix m_tm;
	void SearchBestBox(vreal angleStepRough,vreal angleStepFine);
public:
	const MultiTriangles * GetMultiTri()const;
	MultiTriangles * GetMultiTri();
	const std::list<int> & GetTrianglesInside()const;
	std::list<int> & GetTrianglesInside();
	const CoordinateSystem & GetCs()const;
	const VPoint & GetMinInCs()const;
	const VPoint & GetMaxInCs()const;
	const TransformationMatrix & GetTm()const;
	void SetTransformationMatrix(const TransformationMatrix &tm);
	bool IsIntersectingOnlyBox(const ImprovedTriangleContainerBox *other)const;
	bool IsIntersectingDetailed(const ImprovedTriangleContainerBox *other)const;

};

#endif