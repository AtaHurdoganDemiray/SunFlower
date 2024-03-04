// PointList.h

#ifndef __PointList_h__
#define __PointList_h__
#include "VPoint.h"

class PointList
{
public:
	typedef std::list<VPoint>::const_iterator cItr;
	typedef std::list<VPoint>::iterator Itr;
	void InsertPoint(const VPoint &newpoint);
	void InsertPoint(const vreal newpoint[]);
	void InsertBefore(Itr listmember,const VPoint &newpoint);
	//VPoint * operator[](int index);
	bool operator == (const PointList &toCompare)const;
	bool operator != (const PointList &toCompare)const;
	Itr Begin();
	Itr End();
	void Serialize(Bofstream &outfile)const;
	void Serialize(Bifstream &infile);
	VPoint GetMin()const;
	VPoint GetMax()const;
	void SortInX();
	void SortInY();
	void SortInZ();

protected:
	std::list<VPoint> m_list;
};

#endif
