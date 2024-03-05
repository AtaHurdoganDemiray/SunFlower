// MirrorMatrix.cpp

#include "stdafx.h"
#include "MirrorMatrix.h"
#include "ReorientMatrix.h"

MirrorMatrix::MirrorMatrix (const CoordinateSystem &cs , int mirrorAxisNo)
	:m_cs(cs),m_mirrorAxisNo(mirrorAxisNo)
{
	// first move parts from cs to abs cs
	CoordinateSystem absCs;
	ReorientMatrix Re (m_cs , absCs);
	// mirror
	TransformationMatrix Mi;
	vreal mirrormagX = 1.;
	vreal mirrormagY = 1.;
	vreal mirrormagZ = 1.;
	if (m_mirrorAxisNo == 0)
		mirrormagX *= -1;
	else if (m_mirrorAxisNo == 1)
		mirrormagY *= -1;
	else if (m_mirrorAxisNo == 2)
		mirrormagZ *= -1;
	Mi.SetElement ( mirrormagX , 1 , 1 );
	Mi.SetElement ( mirrormagY , 2 , 2 );
	Mi.SetElement ( mirrormagZ , 3 , 3 );
	// transform back from abs cs to cs
	ReorientMatrix ReRe (absCs , m_cs);

	(*this) *= Re;
	(*this) *= Mi;
	(*this) *= ReRe;
}

const CoordinateSystem & MirrorMatrix::GetCs()const
{
	return m_cs;
}

int MirrorMatrix::GetMirrorAxisNo()const
{
	return m_mirrorAxisNo;
}

void MirrorMatrix::Serialize(Bofstream &f)const
{
	int version = 1;
	f.WriteInt(version);
	TransformationMatrix::Serialize (f);
	f.WriteInt(m_mirrorAxisNo);
	m_cs.Serialize(f);
}

void MirrorMatrix::Serialize (Bifstream &f)
{
	int version;
	f.ReadInt(version);
	if (version >= 1)
	{
		TransformationMatrix::Serialize(f);
		f.ReadInt(m_mirrorAxisNo);
		m_cs.Serialize(f);
	}
}