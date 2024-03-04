// LabelDisplay.h

#ifndef __LabelDisplay_h__
#define __LabelDisplay_h__
#include "DraftEntity.h"
#include "LineVisualAtt.h"
#include "DragItem.h"
class ViewVolume;
struct VPoint;
class Mouse;
struct VColor;
struct OglWindowData;
class Display;

class LabelDisplay:public DraftEntity,public DragItem
{
public:
	LabelDisplay (const std::vector<Display *> * asciiDisp , Model *model,vreal textX = 120., vreal textY = 20.);
	LabelDisplay (const std::vector<Display *> * asciiDisp ,  Model *model
				, const VPoint &p 
				, const VStr &txt
				, const VStr &name
				, vreal textX = 120.
				, vreal textY = 20.);
	virtual void Draw();
	virtual LineVisualAtt & GetLineVisualAtt();
	virtual void OnResizeWindow ( OglWindowData & mywindow );
	virtual bool IsHit (Mouse &mouse)const;
	virtual void Run (Mouse &mouse);
	virtual const VColor & GetColor()const;
	void SetViewVolume (const ViewVolume *vv);
	virtual const VStr GetModelTypeEng ()const;
	void UpdateModelPoint (const VPoint &p);
	void SetTextBoxXY (vreal textX , vreal textY);
	void SetModelPoint (const VPoint &p);
//protected:
	const ViewVolume *m_vv;
	VStr m_txt;
	GLfloat modelpoint[4];
	GLfloat base[2];
	LineVisualAtt m_visatt;
	vreal  m_textX;
	vreal m_textY;
	const std::vector<Display *> * m_asciiDisp;
	GLfloat m_xBase, m_yBase;
};

#endif