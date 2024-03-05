// dglLine.cpp

#include "stdafx.h"
#include "dglLine.h"
#include "TransformationMatrix.h"

dglLine::dglLine()
:m_showStart (true),m_showEnd (true)
{
	m_visatt.ChangeWireColor(VColor (0.33,0.5,1.0));
	m_visatt.SetLineWidth(LineVisualAtt::thick);
}

dglLine::dglLine(const VLine &l)
:m_l(l) , m_showStart (true) , m_showEnd(true)
{
	m_visatt.ChangeWireColor(VColor (0.33,0.5,1.0));
	m_visatt.SetLineWidth(LineVisualAtt::thick);
}

const VLine & dglLine::GetLine()const
{
	return m_l;
}

void dglLine::SetLine(const VLine &l)
{
	m_l = l;
}

void dglLine::Draw()const
{
	glBegin(GL_LINES);
	glVertex3dv (m_l.GetP1().GetArray());
	glVertex3dv (m_l.GetP2().GetArray());
	glEnd();
	if (m_showStart)
	{
		glBegin(GL_POINTS);
		glVertex3dv (m_l.GetP1().GetArray());
		glEnd();
	}
	if (m_showEnd)
	{
		glBegin(GL_POINTS);
		glVertex3dv (m_l.GetP2().GetArray());
		glEnd();
	}
}

bool dglLine::GetShowStart()const
{
	return m_showStart;
}
void dglLine::SetShowStart(bool showStart)
{
	m_showStart = showStart;
}
bool dglLine::GetShowEnd()const
{
	return m_showEnd;
}
void dglLine::SetShowEnd(bool showEnd)
{
	m_showEnd =showEnd;
}
const LineVisualAtt & dglLine::GetVisAtt ()const
{
	return m_visatt;
}
void dglLine::SetVisAtt (const LineVisualAtt &visatt)
{
	m_visatt = visatt;
}

void dglLine::HideShow (bool isHide)
{
	if (isHide)
	{
		m_visatt.SetDraw (false);
	}
	else
	{
		m_visatt.SetDraw (true);
	}
}

void dglLine::Transform (const TransformationMatrix &tm)
{
	m_l = tm.Transform(m_l);
}

VLine & dglLine::GetLine()
{
	return m_l;
}
