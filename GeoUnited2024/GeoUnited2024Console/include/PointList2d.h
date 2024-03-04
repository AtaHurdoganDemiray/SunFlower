// PointList2d.h

#ifndef __PointList2d_h__
#define __PointList2d_h__
#include "VPoint2d.h"
class PointList2d
{
public:
	typedef std::list<VPoint2d>::const_iterator cItr;
	typedef std::list<VPoint2d>::iterator Itr;
	void InsertPoint(const VPoint2d &newpoint);
	void InsertPoint(const vreal newpoint[]);
	void InsertBefore(Itr listmember,const VPoint2d &newpoint);
	//VPoint2d * operator[](int index);
	bool operator == (const PointList2d &toCompare)const;
	bool operator != (const PointList2d &toCompare)const;
	VPoint2d GetMin()const;
	VPoint2d GetMax()const;
	void SortInX();
	void SortInY();
	
	Itr Begin();
	Itr End();
protected:
	std::list<VPoint2d> m_list;

};

#endif