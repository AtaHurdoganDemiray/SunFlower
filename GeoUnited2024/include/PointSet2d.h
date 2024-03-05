/******************************************************************************
*               File: PointSet2d.h											  *
*******************************************************************************
*               Description:                                                  *
*                                                                             *
*******************************************************************************
*               History:                                                      *
*  02.07.2003 18:23:08 Created by:    Ata Hurdogan Demiray                    *
*******************************************************************************
*               (C) 2003 by Pozitif GmbH                                     *
******************************************************************************/

#ifndef __PointSet2d_h__
#define __PointSet2d_h__
#include "PrimitiveGeoVec.h"
#include "VPoint2d.h"
struct DoubleList;

class PointSet2d:public PrimitiveGeoVec<VPoint2d,2>
{
public:
	PointSet2d();
	PointSet2d ( int initialcapacity );
	PointSet2d ( DoubleList & mylist );
	PointSet2d ( const PointSet2d & Foo );
	PointSet2d & operator = (const PointSet2d &toCopy);
	virtual ~PointSet2d();
	void InsertPoint ( const VPoint2d & newpoint );
	void InsertPoint ( const vreal *coord );
	const VPoint2d * operator [] (const long &index ) const;
	VPoint2d * operator [] (const long& index );
	bool GetIndex ( const VPoint2d & p , int & index , vreal cointol) const;
	void RemoveDuplicate (std::vector<int> &removed);
	int InsertPointIf (const VPoint2d &p , bool &isexist);
private:
};


#endif //__PointSet2d_h__