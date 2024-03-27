/******************************************************************************
*               File:											  *
*******************************************************************************
*               Description:                                                  *
*                                                                             *
*******************************************************************************
*               History:                                                      *
*  01.02.2004 05:42:07 Created by:    Ata Hurdogan Demiray                    *
*******************************************************************************
*               (C) 2003 by Positive GmbH		                                *
******************************************************************************/

#ifndef __TriangleMeshCollCheck2d_h__
#define __TriangleMeshCollCheck2d_h__

//#include "DynamicPosCubes2d.h"
#include "LongList.h"
#include "TriangleMesh2d.h"
#include "PosCubes2d.h"
#include "DynamicPosCubes2d.h"

class TriangleMeshCollCheck2d
{
public:
	TriangleMeshCollCheck2d( const TriangleMesh2d &newtrimesh2d );
	~TriangleMeshCollCheck2d ();

	bool TriangleMeshCollCheck2d::CheckColl
	( const long &triindice , const LongList &notincludedindices 
	, PosCubes2d &poscubes ) const;

	bool TriangleMeshCollCheck2d::CheckColl
	( const VPoint2d &cor1 , const VPoint2d &cor2 , const VPoint2d &cor3 
	, DynamicPosCubes2d &poscubes ) const;

	bool CheckCollToLine ( const VLine2d &newline , DynamicPosCubes2d &poscubes ) const;


private:
	const TriangleMesh2d &m_trimesh2d;
	//DynamicPosCubes2d poscubes;
};

#endif //__TriangleMeshCollCheck2d_h__
