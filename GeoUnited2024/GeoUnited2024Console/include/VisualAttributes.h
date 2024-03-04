/******************************************************************************
*               File: VisualAttributes.h											  *
*******************************************************************************
*               Description:                                                  *
*                                                                             *
*******************************************************************************
*               History:                                                      *
*  02.07.2003 18:23:08 Created by:    Ata Hurdogan Demiray                    *
*******************************************************************************
*               (C) 2003 by Pozitif GmbH                                     *
******************************************************************************/

#ifndef __VisualAttributes_h__
#define __VisualAttributes_h__
#include "Stipple.h"

const int LINEFONT_NORMAL = 0;
const int LINEFONT_DASHDOT = 1;
const int LINEFONT_DOTTED = 2;

const int DRAW_BOTH = 2;
const int DRAW_ONLYWIRE = 1;
const int DRAW_ONLYSOLID = 0;

class VisualAttributes
{
public:
	friend class SerializerVisualAttributes;
	VisualAttributes();
	virtual ~VisualAttributes();
	vreal* GetShadeColor();
	void SetShadeColor ( bool modelselected );
	void ChangeShadeColor ( vreal newcolor[] );
	vreal* GetWireColor();	
	void SetWireAttributes ( bool modelselected );
	void ChangeWireColor ( vreal newcolor[] );
	vreal* GetPointColor();	
	void SetPointAttributes ( bool modelselected );
	void SetPointSize ( vreal newsize );
	vreal GetPointSize();
	void ChangePointColor ( vreal newcolor[] );
	void SetLineWidth ( vreal newwidth );
	vreal GetLineWidth ();
	void SetTranslucent ( bool newval );
	bool GetTranslucent ();
	void SetLineFont ( int newfont );
	int GetLineFont ();
	void SetDraw ( int newdraw );
	int GetDraw ();
private:
	vreal shadecolor[3];
	vreal wirecolor[3];
	vreal pointcolor[3];
	vreal selcolor[3];
	vreal linewidth;
	vreal pointsize;
	bool translucent;
	GLubyte halftone[128];
	int linefont;
	int draw;
	vreal tempcolor[3];
public:
	void Serialize(Bofstream &f)const;
	void Serialize(Bifstream &f);

	static void FillDefaults(PointVisualAtt &pvis, LineVisualAtt &lvis, FacetVisualAtt &fvis);
	static void FillDefaults2(PointVisualAtt &pvis, LineVisualAtt &lvis, FacetVisualAtt &fvis);
	static void FillDefaults3(PointVisualAtt &pvis, LineVisualAtt &lvis, FacetVisualAtt &fvis);
};


#endif //__LineSet_h__
