// TriangleContainerBox.h

#ifndef __TriangleContainerBox_h__
#define __TriangleContainerBox_h__

class MultiTriangles;

class TriangleContainerBox
{
public:
	TriangleContainerBox();
	void Construct(MultiTriangles *multiTri , const std::list<int> &trianglesInside , const VPoint &min , const VPoint &max,vreal boxMaxLen);

private:
	MultiTriangles * m_multiTri;
	std::list<int> m_trianglesInside;
	VPoint m_min;
	VPoint m_max;
	TriangleContainerBox * m_subBoxes2;
	void DivideTo2(vreal boxMaxLen);
public:
	MultiTriangles * GetMultiTriangles();
	const MultiTriangles * GetMultiTriangles()const;
	std::list<int> & GetTrianglesInside();
	const std::list<int> & GetTrianglesInside()const;
	static void GetRelatedTriangles(const VPoint &boxmin , const VPoint &boxmax , const MultiTriangles *tri, const std::list<int> &initialIndexVec, std::list<int> &reducedResult);
	const VPoint & GetMin()const;
	const VPoint & GetMax()const;
	void GetLastLevelContainerBoxes(std::vector<TriangleContainerBox *> &lastLevelBoxes);
};

#endif