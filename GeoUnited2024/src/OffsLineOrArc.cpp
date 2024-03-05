// OffsLineOrArc.cpp

#include "stdafx.h"
#include "OffsLineOrArc.h"
#include "VArc.h"
#include "OffsLineOrArc.h"
//#include "WirePlugInProject.h"
//#include "WireOperation.h"

OffsLineOrArc::OffsLineOrArc()
:m_parenti (-1) , m_startNeighbori(-1) , m_endNeighbori(-1) , m_nextStartNeighbori(-1) , m_nextEndNeighbori(-1)
{
	m_isNull = true;
	m_isNextNull = true;
}
void OffsLineOrArc::SetGeo (const LineOrArc &g)
{
	m_g = g;
	m_isNull = false;
}
void OffsLineOrArc::SetNextGeo (const LineOrArc &ng)
{
	m_ng = ng;
	m_isNextNull = false;
}
const LineOrArc & OffsLineOrArc::GetGeo ()const
{
	return m_g;
}
const LineOrArc & OffsLineOrArc::GetNextGeo ()const
{
	return m_ng;
}
LineOrArc & OffsLineOrArc::GetGeo ()
{
	return m_g;
}
LineOrArc & OffsLineOrArc::GetNextGeo ()
{
	return m_ng;
}
bool OffsLineOrArc::IsNull()const
{
	return m_isNull;
}
bool OffsLineOrArc::IsNextNull ()const
{
	return m_isNextNull;
}
void OffsLineOrArc::SetNull (bool isnull)
{
	m_isNull = isnull;
}
void OffsLineOrArc::SetNextNull(bool isnull)
{
	m_isNextNull = isnull;
}

const VPoint & OffsLineOrArc::GetNullPoint() const
{
	return m_nullPoint;
}

void OffsLineOrArc::SetNullPoint(const VPoint &nullPoint)
{
	m_nullPoint = nullPoint;
}

VPoint OffsLineOrArc::GetEndPoint()const
{
	if (IsNull()==false)
		return GetGeo().GetEndPoint();
	else
		return GetNullPoint();
}

VPoint OffsLineOrArc::GetCenterPoint ()const
{
	if (IsNull() == false)
	{
		if (GetGeo().GetType() != LineOrArc::arc)
			throw VException (( IDS_OffGetExcGeoIsN),1);
		else
			return GetGeo().GetArc()->GetCenter();
	}
	else
	{
		return GetNullPoint();
	}
}

VPoint OffsLineOrArc::GetStartPoint ()const
{
	if (IsNull() == false)
		return GetGeo().GetStartPoint();
	else
		return GetNullPoint();
}

VPoint OffsLineOrArc::GetNextEndPoint ()const
{
	if (IsNull())
		return GetNullPoint();
	if (IsNextNull() == true)
		return GetGeo().GetEndPoint();
	else
		return GetNextGeo().GetEndPoint();
}
VPoint OffsLineOrArc::GetNextCenterPoint ()const
{
	if (IsNull())
		return GetNullPoint();
	if (IsNextNull())
		return GetGeo().GetEndPoint();
	else
	{
		if (GetNextGeo().GetType() != LineOrArc::arc)
			throw VException (( IDS_OffGetExcGeoIsN1),1);
		return GetNextGeo().GetArc()->GetCenter();
	}
}
VPoint OffsLineOrArc::GetNextStartPoint ()const
{
	if (IsNull())
		return GetNullPoint();
	if (IsNextNull())
		return GetGeo().GetStartPoint();
	else
		return GetNextGeo().GetStartPoint();
}

LineOrArc & OffsLineOrArc::GetFirstValidGeo (std::vector<OffsLineOrArc> &ovec)
{
	size_t mysize = ovec.size();
	for (size_t i=0;i<mysize;++i)
	{
		OffsLineOrArc &og = ovec.at (i);
		if (og.IsNull() == false)
		{
			return og.GetGeo();
		}
		if (og.IsNextNull()== false)
		{
			return og.GetNextGeo();
		}
	}
	throw VException (( IDS_OffGetExcTheIsN),1);
}
LineOrArc & OffsLineOrArc::GetLastValidGeo (std::vector<OffsLineOrArc> &ovec)
{
	size_t mysize = ovec.size() - 1;
	for (size_t i=mysize; i>=0; i--)
	{
		OffsLineOrArc &og = ovec.at (i);
		if (og.IsNextNull() == false)
		{
			return og.GetNextGeo();
		}
		if (og.IsNull() == false)
		{
			return og.GetGeo();
		}
	}
	throw VException (( IDS_OffGetExcTheIsN1),1);
}

double OffsLineOrArc::GetLength ()const
{
	double len = 0;
	if (IsNull() == false)
	{
		len += GetGeo().GetLength();
	}
	if (IsNextNull() == false)
	{
		len += GetNextGeo().GetLength();
	}
	return len;
}
	
double OffsLineOrArc::GetLength (const std::vector<OffsLineOrArc> &vec)
{
	double len (0);
	size_t mysize = vec.size();
	for (size_t i=0;i<mysize;++i)
	{
		len += vec.at(i).GetLength();
	}
	return len;
}

void OffsLineOrArc::SetParenti (int parenti)
{
	m_parenti = parenti;
}
int OffsLineOrArc::GetParenti ()const
{
	return m_parenti;
}
void OffsLineOrArc::SetStartNeighbori (int startNeighbori)
{
	m_startNeighbori = startNeighbori;
}
int OffsLineOrArc::GetStartNeighbori ()const
{
	return m_startNeighbori;
}
void OffsLineOrArc::SetEndNeighbori (int endNeighbori)
{
	m_endNeighbori = endNeighbori;
}
int OffsLineOrArc::GetEndNeighbori ()const
{
	return m_endNeighbori;
}
void OffsLineOrArc::SetNextStartNeighbori (int nextStartNeighbori)
{
	m_nextStartNeighbori = nextStartNeighbori;
}
int OffsLineOrArc::GetNextStartNeighbori ()const
{
	return m_nextStartNeighbori;
}
void OffsLineOrArc::SetNextEndNeighbori (int nextEndNeighbori)
{
	m_nextEndNeighbori = nextEndNeighbori;
}
int OffsLineOrArc::GetNextEndNeighbori ()const
{
	return m_nextEndNeighbori;
}

void OffsLineOrArc::GetGeoVec (const std::vector<OffsLineOrArc> &offsVec , std::vector<LineOrArc> &geoVec)
{
	size_t mysize = offsVec.size();
	for (size_t i=0;i<mysize;++i)
	{
		const OffsLineOrArc &curOffsGeo = offsVec.at(i);
		if (curOffsGeo.IsNull() == false)
		{
			geoVec.push_back (curOffsGeo.GetGeo());
		}
	}
}

size_t OffsLineOrArc::GetValidElementCount(const std::vector<OffsLineOrArc> &offsVec)
{
	size_t mysize = offsVec.size();
	size_t validsize = 0;
	for (size_t i=0;i<mysize;++i)
	{
		if (offsVec.at(i).IsNull() == false)
			++validsize;
	}
	return validsize;
}