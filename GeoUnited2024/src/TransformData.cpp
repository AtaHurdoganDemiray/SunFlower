// TransformData.cpp
#include "stdafx.h"
#include "TransformData.h"
#include "Bofstream.h"
#include "Bifstream.h"

TransformData::TransformData()
:m_om(m_tm),isactive(true)
{

}
TransformData::TransformData (const TransformationMatrix &tm)
:m_om(tm),m_tm(tm)
{
	isactive = true;
}
void TransformData::Serialize (Bofstream &f)const
{
	int version = 1;
	f.WriteInt (version);
	if (version >=1)
	{
		m_tm.Serialize (f);
		f.WriteBool (isactive);
	}
}
void TransformData::Serialize (Bifstream &f)
{
	int version;
	f.ReadInt (version);
	if (version >=1)
	{
		m_tm.Serialize (f);
		m_om = OglMatrix (m_tm);
		f.ReadBool (isactive);
	}
}
