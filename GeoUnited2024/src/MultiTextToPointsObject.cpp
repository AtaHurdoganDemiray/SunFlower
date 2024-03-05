// MultiTextToPointsObject.cpp

#include "stdafx.h"
#include "MultiTextToPointsObject.h"
#include "SetMerger.h"
#include "ViewVolume.h"
#include "ViewPointItemTypes.h"

MultiTextToPointsObject::MultiTextToPointsObject 
( const PointSet & _points 
 , const std::vector<CString> & _text 
 , const ViewVolume & view_volume 
 , const vreal & xwidth 
 , const vreal & ywidth
 , const VStr &ename
 , std::vector<Display *> *asciiDisp)
 :	DraftEntity (0)
	, m_model (ViewPointItemTypes::m_multitext,ename)
	, points ( _points.Size() ) , text ( _text ) 
	, m_window ( view_volume.corner_x 
				, view_volume.corner_y
				, view_volume.m_near
				, view_volume.m_far )
	, mol ( xwidth , ywidth , m_window , asciiDisp)
{
	SetMerger merge;
	merge.Merge ( _points , points );	
	SetDraftEntityModel (&m_model);
}

MultiTextToPointsObject::~MultiTextToPointsObject()
{
	
}

void MultiTextToPointsObject::Draw ()
{
	mol.InitCellsSelected();
	//GLfloat showcolor [3] = { 0 , 0 , 0 };
	mol.LabelCMMPoints ( points , text );//, showcolor );
}

void MultiTextToPointsObject::OnResizeWindow ( OglWindowData & newwindow )
{
	mol.ResetOglWindowData ( newwindow );
}

LineVisualAtt & MultiTextToPointsObject::GetLineVisualAtt()
{
	return m_visatt;
}

const VColor & MultiTextToPointsObject::GetColor() const
{
	return m_visatt.GetWireColor();
}

const VStr MultiTextToPointsObject::GetModelTypeEng() const
{
	return ViewPointItemTypes::MultiTextEng();	
}
