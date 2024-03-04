// dglArc.cpp

#include "stdafx.h"
#include "dglArc.h"
#include "VLine.h"

dglArc::dglArc()
:m_sl (50),m_da (3.0)
{

}

dglArc::dglArc(const VArc &a , double deltaAngle)
:m_a(a) , m_sl (50) , m_da (deltaAngle)
{
	Digitise();
}

const VArc & dglArc::GetArc() const
{
	return m_a;
}

void dglArc::SetArc(const VArc &a)
{
	m_a = a;
	Digitise();
}

void dglArc::Draw() const
{
	glBegin ( GL_LINES );

	for ( long i = 0; i < m_sl.Size(); i++ )
	{
		VLine currline = m_sl[i];
		glVertex3f ( static_cast <GLfloat> (currline.GetP1().x())
			,static_cast <GLfloat> (currline.GetP1().y())
			,static_cast <GLfloat> (currline.GetP1().z()) );

		glVertex3f ( static_cast <GLfloat> (currline.GetP2().x())
			,static_cast <GLfloat> (currline.GetP2().y())
			,static_cast <GLfloat> (currline.GetP2().z()) );

	}
	glEnd ();
	VPoint midPoint = m_a.GetP(m_a.GetSweepAngle()*0.5);
	glPointSize (6.f);
	glBegin ( GL_POINTS);
		glVertex3d (m_sl.Start()->x(),m_sl.Start()->y(),m_sl.Start()->z());
		glVertex3d (midPoint.x(),midPoint.y(),midPoint.z());
		glVertex3d (m_sl.End()->x(),m_sl.End()->y(),m_sl.End()->z());
	glEnd ();

}

void dglArc::Digitise()
{
	m_sl.GetPointSet().Clear();
	double a = 0.0;
	double ea = m_a.GetSweepAngle();
	if (ea * m_da < 0.0)
		m_da *= -1;
	
	if (m_da > 0)
	{
		for (a;a<ea+1.e-4;a+=m_da)
		{
			VPoint cp = m_a.GetP (a);
			m_sl.InsertPoint(cp);
		}
	}
	else
	{
		for (a; a > ea + 1.e-4; a += m_da)
			m_sl.InsertPoint(m_a.GetP(a));
	}

	if (fabs(ea-a)>1.e-4)// Add end point if it is not added in loop
	{
		VPoint cp = m_a.GetP (ea);
		m_sl.InsertPoint(cp);
	}
}

double dglArc::GetDeltaAngle() const
{
	return m_da;
}

void dglArc::SetDeltaAngle(double deltaAngle)
{
	m_da = deltaAngle;
}

const LineVisualAtt & dglArc::GetVisAtt() const
{
	return m_visAtt;
}

void dglArc::SetVisAtt(const LineVisualAtt &visatt)
{
	m_visAtt = visatt;
}

void dglArc::HideShow (bool isHide)
{
	if (isHide)
	{
		m_visAtt.SetDraw (false);
	}
	else
	{
		m_visAtt.SetDraw (true);
	}
}

void dglArc::Transform (const TransformationMatrix &tm)
{
	m_a.Transform (tm);
	m_sl.Transform (tm);
}

const VPoint dglArc::GetMinPoint()const
{
	return m_sl.GetMin();
}
const VPoint dglArc::GetMaxPoint()const
{
	return m_sl.GetMax();
}
const VPoint dglArc::GetMinPointInCs(const CoordinateSystem &cs)const
{
	return m_sl.GetMinPointInCs(cs);
}
const VPoint dglArc::GetMaxPointInCs(const CoordinateSystem &cs)const
{
	return m_sl.GetMaxPointInCs(cs);
}

void dglArc::Reverse()
{
	VArc tmp = this->GetArc();
	this->SetArc(tmp.GetReversed());
}