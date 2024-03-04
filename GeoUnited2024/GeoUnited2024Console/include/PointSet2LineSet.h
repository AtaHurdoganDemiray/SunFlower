// PointSet2LineSet.h

#ifndef __PointSet2LineSet_h__
#define __PointSet2LineSet_h__

class PointSet;
class LineSet;
class PointSet2d;
class LineSet2d;

class PointSet2LineSet
{
public:
	PointSet2LineSet();
	~PointSet2LineSet();

	void FillLineSetByPointSet ( const PointSet &pointset , LineSet &lineset ) const;
	void FillLineSetByPointSet ( const PointSet2d &pointset , LineSet2d &lineset ) const;

};

#endif //__PointSet2LineSet_h__