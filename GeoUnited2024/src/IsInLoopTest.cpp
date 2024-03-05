/******************************************************************************
*               File: IsInLoopTest.cpp											  *
*******************************************************************************
*               Description:                                                  *
*                                                                             *
*******************************************************************************
*               History:                                                      *
*  08.02.2004 20:37:20 Created by:    Ata Hurdogan Demiray                    *
*******************************************************************************
*               (C) 2003 by ModuleWorks GmbH                                  *
******************************************************************************/

#include "stdafx.h"
#include "IsInLoopTest.h"
#include "gmTrimLoop.h"
#include "gmVoronoi.h"

IsInLoopTest::IsInLoopTest( const gmTrimLoop &newloop)
:m_loop ( newloop )
{
	m_tris = new TriangleMesh;
	FormTriangleMesh();
}

//#############################################################################

IsInLoopTest::~IsInLoopTest()
{

}

//#############################################################################

bool IsInLoopTest::IsInLoop ( const VPoint2d &testpoint )
{
	VTriangle2d *currTri;
	for ( long i=0; i < looptrimesh.Size(); i++ )
	{
		currTri = looptrimesh[i];
		if ( currTri->IsInTriangle ( testpoint ) )
			return true;
	}

	return false;
}

//#############################################################################

void IsInLoopTest::FormTriangleMesh ()
{
	gmVoronoi tribuild ( m_loop.surfparampoints , looptrimesh );
	tribuild.BuildTriMesh ( m_loop.surfmodelpoints , m_loop.surfnormals 
		, *m_tris );	

}

//#############################################################################
