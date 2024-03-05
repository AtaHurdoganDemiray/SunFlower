// ThinTransformShell.cpp

#include "stdafx.h"
#include "ThinTransformShell.h"
#include "SequentialLineSet.h"
#include "Model.h"
#include "CoordinateSystem.h"
#include "LineOrArc.h"

ThinTransformShell::ThinTransformShell (const VPoint &mn , const VPoint &mx ,Model *mymodel)
: m_min (mn),m_max(mx),m_model(mymodel)
{
	m_model->SetTransformable (this);
}
void ThinTransformShell::OnTransform (const TransformationMatrix &tm)
{
	//  Do nothing !!!
}
const VPoint ThinTransformShell::GetMinPoint()const
{
	return m_min;
}
const VPoint ThinTransformShell::GetMaxPoint()const
{
	return m_max;
}
const VPoint ThinTransformShell::GetMinPointInCs (const CoordinateSystem &cs)const
{
	VPoint csMin,csMax;
	GetMinMaxInCs(cs,csMin,csMax);
	return csMin;
}
const VPoint ThinTransformShell::GetMaxPointInCs (const CoordinateSystem &cs)const
{
	VPoint csMin,csMax;
	GetMinMaxInCs(cs,csMin,csMax);
	return csMax;
}
void ThinTransformShell::GetMinMaxInCs (const CoordinateSystem &cs , VPoint &csMin , VPoint &csMax)const
{
	std::vector<LineOrArc> lVec;
	LineOrArc::Get2dBox (lVec,m_min,m_max,m_min.z(),0.0,UZ);
	LineOrArc::GetHeightLines(lVec,m_min,m_max,0.0,UZ);
	std::vector<LineOrArc> cslVec;
	LineOrArc::TransformFromAbsToLocalCs (lVec,cs,cslVec);
	LineOrArc::GetMinMax (cslVec,csMin,csMax);
}
Model * ThinTransformShell::GetTrModel()
{
	return m_model;
}
