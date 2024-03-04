/******************************************************************************
*               File: IsInLoopTest.h											  *
*******************************************************************************
*               Description:                                                  *
*                                                                             *
*******************************************************************************
*               History:                                                      *
*  08.02.2004 20:32:02 Created by:    Ata Hurdogan Demiray                    *
*******************************************************************************
*               (C) 2003 by ModuleWorks GmbH                                  *
******************************************************************************/

#ifndef __IsInLoopTest_h__
#define __IsInLoopTest_h__
#include "TriangleMesh2d.h"
#include "TriangleMesh.h"

class gmTrimLoop;

class IsInLoopTest
{
public:
	IsInLoopTest( const gmTrimLoop &newloop );
	~IsInLoopTest();

	bool IsInLoop ( const VPoint2d &testpoint );
	void FormTriangleMesh ();


	const gmTrimLoop &m_loop;
	TriangleMesh2d looptrimesh;
	TriangleMesh *m_tris;
};

#endif //__IsInLoopTest_h__
