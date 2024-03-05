/******************************************************************************
*               File: OGLNurbsSupport.h										  *
*******************************************************************************
*               Description:                                                  *
*                                                                             *
*******************************************************************************
*               History:                                                      *
*  01.08.2003 15:15:25 Created by:    Ata Hurdogan Demiray                    *
*******************************************************************************
*               (C) 2003 by Pozitif GmbH                                      *
******************************************************************************/

#ifndef __OGLNurbsSupport_h__
#define __OGLNurbsSupport_h__

#include <vector>
#include "oglNurbsSurface.h"
#include "oglNurbsCurve.h"

class gmE128;
class gmE126;
class TriangleMesh;
class LineSet;
typedef std::vector < gmE126* > gmE126List;
typedef class GLUnurbs GLUnurbsObj;
class IgesEntitySet;
class BSplineSurface;

typedef std::vector<TriangleMesh *> TriangleMeshSet;

class OGLNurbsSupport
{
public:
	OGLNurbsSupport();
	~OGLNurbsSupport();
	void SetProperties( double tolerance , bool outline , bool displaylist );
	TriangleMesh * TriangulateNurbs 
							( gmE128 & nurbs , gmE126List & trimmingloops , double tolerance );

	TriangleMeshSet * TriangulateNurbs 
							( IgesEntitySet & allentities , double tolerance );

	TriangleMesh * OGLNurbsSupport::TriangulateNurbs 
							( BSplineSurface * mybspline , double tolerance );

	unsigned int CreateDispListForNurbs ( double tolerance );
	unsigned int CreateWireDispListForNurbs ( double tolerance );
	LineSet * TessellateOutlineOfNurbs ( double tolerance );
	TriangleMesh * DrawNurbsSurface 
		( oglNurbsSurface &mysurface , oglNurbsCurveVec &trimvec , double tolerance);
	void ApplyATrimLoop ( oglNurbsCurve &mytrimcurve );
	void ReNewTheNurb ();

private:
	GLUnurbsObj * theNurb;
	
};

#endif //OGLNurbsSupport