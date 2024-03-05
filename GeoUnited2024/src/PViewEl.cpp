// PViewEl.cpp

#include "stdafx.h"
#include "PViewEl.h"

PViewEl::PViewEl()
	:m_model("PViewEl", ""), m_source(0)
{

}
PViewEl::PViewEl(Model *source, TransformationMatrix &tm, const VPoint &mn, const VPoint &mx , const VStr &name)
	:m_source(source), m_min(mn), m_max(mx), m_tm(tm), m_model("PViewEl",name)
{
	m_model.GetDisplay().SetDispListIdLines(m_source->GetDisplay().GetDispListIdLines());
	m_model.GetDisplay().GetVisualAtt().ChangeWireColor(VGlobalValues::viewpointdefaults.m_surfaceWireColor2);
	m_model.GetDisplay().GetVisualAtt().SetLineWidth(LineVisualAtt::thin);
}
void PViewEl::UpdateOpenBox(std::vector<const Display *> linesDispVec , std::vector<OglMatrix> &oglTmVec,const VPoint &mn , const VPoint &mx , const VStr &name)
{
	m_source = 0;
	m_min = mn;
	m_max = mx;
	m_model.SetName(name);
	std::vector<const Display *> emptyFacetDispVec;
	std::vector<const Display *> emptyPointDispVec;
	m_model.GetDisplay().MakeDisplayList(emptyFacetDispVec, linesDispVec, emptyPointDispVec, oglTmVec);
	m_model.GetDisplay().GetVisualAtt().ChangeWireColor(VGlobalValues::viewpointdefaults.m_wireColor2);
	m_model.GetDisplay().GetVisualAtt().SetLineWidth(LineVisualAtt::thin);
}
// virtuals of VTransformable
void PViewEl::OnTransform(const TransformationMatrix &tm)
{
	m_tm *= tm;
}
const VPoint PViewEl::GetMinPoint()const
{
	return m_min;
}
const VPoint PViewEl::GetMaxPoint()const
{
	return m_max;
}
const VPoint PViewEl::GetMinPointInCs(const CoordinateSystem &cs)const
{
	return cs.AbsoluteCoordToRelativeCoord(m_min);
}
const VPoint PViewEl::GetMaxPointInCs(const CoordinateSystem &cs)const
{
	return cs.AbsoluteCoordToRelativeCoord(m_max);
}
Model * PViewEl::GetTrModel()
{
	return &m_model;
}

void PViewEl::Delete()
{
	m_model.SetVisible(false);
	m_model.SetDeleted(true);
}