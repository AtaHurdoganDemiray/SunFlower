/******************************************************************************
*               File: DrawShapes.h											  *
*******************************************************************************
*               Description:                                                  *
*                                                                             *
*******************************************************************************
*               History:                                                      *
*  01.08.2003 15:15:25 Created by:    Ata Hurdogan Demiray                    *
*******************************************************************************
*               (C) 2003 by Pozitif GmbH                                      *
******************************************************************************/

#ifndef __DrawShapes_h__
#define __DrawShapes_h__

#include "LineSet.h"
#include "TriangleMesh.h"
#include "VColor.h"

struct VPlane;
class ViewVolume;

class DrawShapes
{
public:
	DrawShapes();
	~DrawShapes();
	void DrawVector( VPoint &vecttodraw , VPoint &basepoint , vreal scalefactor );
	void CalculateArrowLines ( VPoint &vect , VPoint &basepoint , vreal scalefactor );
	void CreateTriOfPlane ( VPoint basepoint , VPoint normal , vreal scalefactor );
	void DrawPlane ( const VPlane & pln , const ViewVolume & vol );
	TriangleMesh planetri;
	VColor planecolor;
private:
	LineSet vectordr;
	VPoint m_vectorToDraw,m_basePoint;		
};



#endif // DrawShapes