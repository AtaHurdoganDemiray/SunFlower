/******************************************************************************
*               File: TriangleMesh2d.h											  *
*******************************************************************************
*               Description:                                                  *
*                                                                             *
*******************************************************************************
*               History:                                                      *
*  02.07.2003 16:16:52 Created by:    Ata Hurdogan Demiray                    *
*******************************************************************************
*               (C) 2003 by Pozitif GmbH                                      *
******************************************************************************/

#ifndef __TriangleMesh2d_h__
#define __TriangleMesh2d_h__
#include "VTriangle2dE.h"
//#include "PointSet.h"
//#include <vector>
//using namespace std;
//#include "ExpandingTPointerArray.h"
//#include "ExpandingTArray.h"
struct DoubleList;
//class ModelSelectionFilters;
//struct VLine;

class TriangleMesh2d 
{
public:	
	TriangleMesh2d ( );
	TriangleMesh2d ( const int &initialsize );
	TriangleMesh2d ( DoubleList & mylist , unsigned int normalstate );	
	TriangleMesh2d ( const TriangleMesh2d & Foo );
	TriangleMesh2d & operator = (const TriangleMesh2d &toCopy);
	virtual ~TriangleMesh2d ();
	bool AddTriangle 
		( const VPoint2d & corner1 , const VPoint2d & corner2 , const VPoint2d &corner3 );
	bool AddTriangle ( const VTriangle2dE &newtri );
	void UpdateMinMax();
	void UpdateMinMaxForSingleTri ( const VTriangle2dE & newtri );
	long Size() const;
	const VPoint2d & GetMin() const;
	const VPoint2d & GetMax() const;
	void SetAttributes( vreal shadecolor[] );
	void Clear();
	VTriangle2dE * operator[] (const long &index) const;
	bool operator == (const TriangleMesh2d &toCompare)const;
	bool operator != (const TriangleMesh2d &toCompare)const;	
protected:
private:
	std::vector<VTriangle2dE>tri;
	VPoint2d m_min;
	VPoint2d m_max;
};

#endif //__TriangleMesh2d_h__
