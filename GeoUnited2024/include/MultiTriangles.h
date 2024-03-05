// MultiTriangles.h

#ifndef __MultiTriangles_h__
#define __MultiTriangles_h__
#include "MultiGV.h"

class MultiTriangles:public MultiGV<VTriangleE,Triangles>
{
public:
	MultiTriangles (const std::vector<const Triangles*> &trianglesVec);
	MultiTriangles (int totalSize , const std::vector<const Triangles *> &trianglesVec);
	MultiTriangles (const MultiTriangles &toCopy);
	MultiTriangles & operator = (const MultiTriangles &toCopy);
	//const VTriangleE & operator[] (int i)const;
	bool IsCollidingToBox(const VPoint &boxmin, const VPoint &boxmax)const;
	bool IsCollidingToBox(const VPoint &boxmin, const VPoint &boxmax,const std::list<int> &someTri)const;
	class EncBox
	{
	public:
		EncBox();
		VPoint m_mn;
		VPoint m_mx;
		void Make8SubBoxes(std::list<EncBox *> &boxlist);
		//bool m_isActive;
	};
	static int m_maxRecursiveDivideCount;
	void RecursiveDivide(EncBox *realBox, EncBox *rootBox, std::list<EncBox *> &boxlist, int &recCount, int recDepthLimit);
};

#endif