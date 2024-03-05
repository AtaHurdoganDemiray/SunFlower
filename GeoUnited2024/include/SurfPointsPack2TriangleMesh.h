/******************************************************************************
*               File: SurfPointsPack2TriangleMesh.h											  *
*******************************************************************************
*               Description:                                                  *
*                                                                             *
*******************************************************************************
*               History:                                                      *
*  04.03.2004 14:53:24 Created by:    Ata Hurdogan Demiray                    *
*******************************************************************************
*               (C) 2003 by Positive GmbH                                     *
******************************************************************************/

#ifndef __SurfPointsPack2TriangleMesh_h__
#define __SurfPointsPack2TriangleMesh_h__

#include "PosCubes2d.h"
#include "AngleSupport.h"
#include "DistanceSupport.h"

class TriangleMesh2d;
class TriangleMesh;
class SurfPointsPack;
class NeighborsData;

class SurfPointsPack2TriangleMesh
{
public:
	SurfPointsPack2TriangleMesh ( const SurfPointsPack &surfpack );
	~SurfPointsPack2TriangleMesh();	
	void CalculateMesh ( TriangleMesh2d &paramtri , TriangleMesh &modeltri ) const;
	void GetTrianglesAroundPoint ( const long &originpoint , LongList &triangles ) const;
	void RecordTriangles ( const long &originindex , const LongList &triangles 
	, TriangleMesh2d &paramtri , TriangleMesh &modeltri ) const;
private:
	const SurfPointsPack &msurfpack;
	PosCubes2d poscubes;
	AngleSupport anglesupport;
	DistanceSupport distancesupport;
	NeighborsData * history;
};

#endif //__SurfPointsPack2TriangleMesh_h__
