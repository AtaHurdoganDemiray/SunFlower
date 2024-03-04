// MultiTriangles.cpp

#include "stdafx.h"
#include "MultiTriangles.h"
#include "VTriangleE.h"
#include "TriangleIntersection.h"

MultiTriangles::MultiTriangles (const std::vector<const Triangles*> &trianglesVec)
:MultiGV<VTriangleE,Triangles> (trianglesVec)
{

}
MultiTriangles::MultiTriangles (int totalSize , const std::vector<const Triangles *> &trianglesVec)
:MultiGV<VTriangleE,Triangles>(totalSize,trianglesVec)
{

}
MultiTriangles::MultiTriangles (const MultiTriangles &toCopy)
:MultiGV<VTriangleE,Triangles> (toCopy)
{

}
MultiTriangles & MultiTriangles::operator = (const MultiTriangles &toCopy)
{
	if (this != &toCopy)
	{
		MultiGV<VTriangleE,Triangles>::operator = (toCopy);
	}
	return *this;
}
//const VTriangleE & MultiTriangles::operator [] (int i)const
//{
//
//	const VTriangleE &result = MultiGV<VTriangleE,Triangles>::operator [] (i);
//	if (result.IsCorrupted())
//	{
//		VPoint f1 (-50000,-50000,-50000);
//		VPoint f2 (-50001,-50000,-50000);
//		VPoint f3 (-50000,-50001,-50000);
//		VTriangleE fake (f1,f2,f3);
//		//throw VException (("MultiTriangles::operator [] exception"),1);
//		return fake;
//	}
//	
//	return result;
//}

bool MultiTriangles::IsCollidingToBox(const VPoint &boxmin, const VPoint &boxmax)const
{
	int mysize = Size();
	for (int i = 0; i < mysize; ++i)
	{
		VTriangleE curTri = operator[](i);
		if (TriangleIntersection::IsTriCollidingToBox(curTri, boxmin, boxmax))
			return true;
	}
	return false;
}
bool MultiTriangles::IsCollidingToBox(const VPoint &boxmin, const VPoint &boxmax, const std::list<int> &someTri)const
{
	std::list<int>::const_iterator i = someTri.begin();
	std::list<int>::const_iterator e = someTri.end();
	for (i; i != e; ++i)
	{
		VTriangleE curTri = operator[](*i);
		if (TriangleIntersection::IsTriCollidingToBox(curTri, boxmin, boxmax))
			return true;
	}
	return false;
}
MultiTriangles::EncBox::EncBox()
//	:m_isActive(true)
{

}

int MultiTriangles::m_maxRecursiveDivideCount = 100000;


void MultiTriangles::RecursiveDivide(EncBox *realBox, EncBox *rootBox, std::list<EncBox *> &boxlist, int &recCount, int recDepthLimit)
{
	++recCount;
	vreal deltaxfirst = rootBox->m_mx.x() - rootBox->m_mn.x();
	vreal deltaxlast = realBox->m_mx.x() - realBox->m_mn.x();
	int curRecDepth = (int)((deltaxfirst / deltaxlast) + 0.1);
	bool ck = IsCollidingToBox(realBox->m_mn, realBox->m_mx);

	if (recCount < m_maxRecursiveDivideCount && curRecDepth < recDepthLimit)
	{
		//realBox->m_isActive = false;
		if (ck)
		{
			std::list<EncBox *> tmplist;
			realBox->Make8SubBoxes(tmplist);
			std::list<EncBox *>::iterator i = tmplist.begin();
			std::list<EncBox *>::iterator e = tmplist.end();
			for (i; i != e; ++i)
			{
				RecursiveDivide(*i, rootBox, boxlist, recCount, recDepthLimit);
			}
		}
	}
	else
	{
		if (ck)
		{
			//realBox->m_isActive = true;
			boxlist.push_back(realBox);
		}
	}
}

void MultiTriangles::EncBox::Make8SubBoxes(std::list<EncBox *> &boxes)
{
	VPoint mid = (m_mn + m_mx)*0.5;
	EncBox *basebox1 = new EncBox;
	basebox1->m_mn = m_mn;
	basebox1->m_mx = mid;
	EncBox *basebox2 = new EncBox;
	basebox2->m_mn = VPoint(mid.x(), m_mn.y(), m_mn.z());
	basebox2->m_mx = VPoint(m_mx.x(), mid.y(), mid.z());
	EncBox *basebox3 = new EncBox;
	basebox3->m_mn = VPoint(m_mn.x(), mid.y(), m_mn.z());
	basebox3->m_mx = VPoint(mid.x(), m_mx.y(), mid.z());
	EncBox *basebox4 = new EncBox;
	basebox4->m_mn = VPoint(mid.x(), mid.y(), m_mn.z());
	basebox4->m_mx = VPoint(m_mx.x(), m_mx.y(), mid.z());

	EncBox *topbox1 = new EncBox;
	topbox1->m_mn = VPoint(m_mn.x(), m_mn.y(), mid.z());
	topbox1->m_mx = VPoint(mid.x(), mid.y(), m_mx.z());
	EncBox *topbox2 = new EncBox;
	topbox2->m_mn = VPoint(mid.x(), m_mn.y(), mid.z());
	topbox2->m_mx = VPoint(m_mx.x(), mid.y(), m_mx.z());
	EncBox *topbox3 = new EncBox;
	topbox3->m_mn = VPoint(m_mn.x(), mid.y(), mid.z());
	topbox3->m_mx = VPoint(mid.x(), m_mx.y(), m_mx.z());
	EncBox *topbox4 = new EncBox;
	topbox4->m_mn = mid;
	topbox4->m_mx = m_mx;

	boxes.push_back(basebox1);
	boxes.push_back(basebox2);
	boxes.push_back(basebox3);
	boxes.push_back(basebox4);
	boxes.push_back(topbox1);
	boxes.push_back(topbox2);
	boxes.push_back(topbox3);
	boxes.push_back(topbox4);
}