// ScaleMatrix.cpp

#include "stdafx.h"
#include "ScaleMatrix.h"
#include "ReorientMatrix.h"

ScaleMatrix::ScaleMatrix ()
:TransformationMatrix(),m_sx(1.),m_sy(1.),m_sz(1.)
{

}
	
ScaleMatrix::ScaleMatrix (const vreal s)
:TransformationMatrix(),m_sx(s),m_sy(s),m_sz(s)
{
	SetRow (1,m_sx,0.,0.,0.);
	SetRow (2,0.,m_sy,0.,0.);
	SetRow (3,0.,0.,m_sz,0.);
	SetRow (4,0.,0.,0.,1.);
}
	
ScaleMatrix::ScaleMatrix (const ScaleMatrix &tocopy)
:TransformationMatrix (tocopy),m_sx(tocopy.m_sx)
,m_sy(tocopy.m_sy),m_sz(tocopy.m_sz)
{
	SetRow (1,m_sx,0.,0.,0.);
	SetRow (2,0.,m_sy,0.,0.);
	SetRow (3,0.,0.,m_sz,0.);
	SetRow (4,0.,0.,0.,1.);

}
ScaleMatrix::ScaleMatrix(const vreal sx , const vreal sy, const vreal sz)
:TransformationMatrix () , m_sx(sx),m_sy(sy),m_sz(sz)
{
	SetRow (1,m_sx,0.,0.,0.);
	SetRow (2,0.,m_sy,0.,0.);
	SetRow (3,0.,0.,m_sz,0.);
	SetRow (4,0.,0.,0.,1.);
}
	
ScaleMatrix& ScaleMatrix::operator = (const ScaleMatrix &tocopy)
{
	TransformationMatrix::operator = (tocopy);
	m_sx = tocopy.m_sx;
	m_sy = tocopy.m_sy;
	m_sz = tocopy.m_sz;
	return *this;
}

ScaleMatrix::~ScaleMatrix ()
{

}
	
void ScaleMatrix::Serialize (Bofstream &f)const
{
	TransformationMatrix::Serialize (f);
	f.WriteDouble (m_sx);
	f.WriteDouble (m_sy);
	f.WriteDouble (m_sz);
	m_cs.Serialize(f);
}

void ScaleMatrix::Serialize (Bifstream &f)
{
	TransformationMatrix::Serialize (f);
	f.ReadDouble (m_sx);
	f.ReadDouble(m_sy);
	f.ReadDouble(m_sz);
	m_cs.Serialize(f);
}

ScaleMatrix::ScaleMatrix (const CoordinateSystem &cs , vreal sx , vreal sy , vreal sz)
	:m_cs(cs),m_sx(sx),m_sy(sy),m_sz(sz)
{
	// reorient from cs to absCs
	CoordinateSystem absCs;
	ReorientMatrix Re (cs,absCs);
	// Scale mat
	TransformationMatrix S;
	S.SetElement(m_sx,1,1);
	S.SetElement(m_sy,2,2);
	S.SetElement(m_sz,3,3);
	// back to original position
	ReorientMatrix ReRe (absCs,cs);
	(*this)*=Re;
	(*this)*=S;
	(*this)*=ReRe;
}

const CoordinateSystem & ScaleMatrix::GetCs()const
{
	return m_cs;
}