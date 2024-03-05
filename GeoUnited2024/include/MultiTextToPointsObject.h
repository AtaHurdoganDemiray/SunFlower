// MultiTextToPointsObject.h

#ifndef __MultiTextToPointsObject_h__
#define __MultiTextToPointsObject_h__

#include "PointSet.h"
#include "OglWindowData.h"
#include "MultiObjectLabeler.h"
#include "DraftEntity.h"
#include "LineVisualAtt.h"
#include "Model.h"

class ViewVolume;
class Display;

class MultiTextToPointsObject : public DraftEntity
{
public:
	MultiTextToPointsObject 
		( const PointSet & _points 
		, const std::vector<CString> & _text 
		, const ViewVolume & view_volume 
		, const vreal & xwidth 
		, const vreal & ywidth 
		,const VStr &ename
		,std::vector<Display *> *asciiDisp);

	virtual ~MultiTextToPointsObject ();

	virtual void Draw ();
	virtual void OnResizeWindow ( OglWindowData & newwindow );
	virtual LineVisualAtt & GetLineVisualAtt();
	virtual const VColor &GetColor()const;
	virtual const VStr GetModelTypeEng()const;
protected:
	PointSet points;
	std::vector <CString> text;
	CString m_treeviewname;
	OglWindowData m_window;
	MultiObjectLabeler mol;
	LineVisualAtt m_visatt;
	Model m_model;
};

#endif