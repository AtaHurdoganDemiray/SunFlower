/******************************************************************************
*               File: SetMerger.h											  *
*******************************************************************************
*               Description:                                                  *
*                                                                             *
*******************************************************************************
*               History:                                                      *
*  04.03.2004 13:39:40 Created by:    Ata Hurdogan Demiray                    *
*******************************************************************************
*               (C) 2003 by Positive GmbH	                                  *
******************************************************************************/

#ifndef __SetMerger_h__
#define __SetMerger_h__

struct LongVec;
struct DoubleVec;
class PointSet2d;
class PointSet;
class LineSet2d;
class LineSet;
class TriangleMesh2d;
class TriangleMesh;
class RoughPolarPSet2d;

class SetMerger
{
public:
	void Merge ( const LongVec &source , LongVec &destiny );
	void Merge ( const DoubleVec &source , DoubleVec &destiny );
	void Merge ( const RoughPolarPSet2d &source , RoughPolarPSet2d &destiny );
	void Merge ( const PointSet2d &source , PointSet2d &destiny );
	void MergeNoRepeat ( const PointSet2d & source , PointSet2d & destiny );
	void Merge ( const PointSet &source , PointSet &destiny );
	void Merge ( const LineSet2d &source , LineSet2d &destiny );
	void Merge ( const LineSet &source , LineSet &destiny );
	void Merge ( const TriangleMesh2d &source , TriangleMesh2d &destiny );
	void Merge ( const TriangleMesh &source , TriangleMesh &destiny );
	
};

#endif //__SetMerger_h__
