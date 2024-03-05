/******************************************************************************
*               File: oglTrimmedNurbs.h										  *
*******************************************************************************
*               Description:                                                  *
*                                                                             *
*******************************************************************************
*               History:                                                      *
*  20.12.2003 17:19:55 Created by:    Ata Hurdogan Demiray                    *
*******************************************************************************
*               (C) 2003 by Pozitif GmbH	                                 *
******************************************************************************/

#ifndef __oglTrimmedNurbs_h__
#define __oglTrimmedNurbs_h__

#include "oglNurbsSurface.h"
#include "oglNurbsCurve.h"
class BSplineSurface;

struct oglTrimmedNurbs
{
public:
	oglTrimmedNurbs();
	~oglTrimmedNurbs();
	
	oglNurbsSurface * thebody;
	glAllTrimLoops * alltrimloops;
	TrimLoop * currtrimloop;

	void FormTheBody ( const BSplineSurface &newnurbsbody );
	void AddATrimLoop();
	void AddoglNurbsCurveToCurrTrimLoop ( const BSplineCurve &newtrimcurve );
		

};

typedef std::vector <oglTrimmedNurbs*> oglTrimmedNurbsVec;

#endif //__oglTrimmedNurbs_h__
