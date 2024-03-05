// OffsLineOrArc.h

#ifndef __OffsLineOrArc_h__
#define __OffsLineOrArc_h__

#include "LineOrArc.h"
#include "VPoint.h"

class OffsLineOrArc
{
public:
	OffsLineOrArc ();
	void SetGeo (const LineOrArc &g);
	void SetNextGeo (const LineOrArc &ng);
	const LineOrArc & GetGeo ()const;
	const LineOrArc & GetNextGeo ()const;
	LineOrArc & GetGeo ();
	LineOrArc & GetNextGeo ();
	bool IsNull()const;
	bool IsNextNull ()const;
	void SetNull (bool isnull);
	void SetNextNull (bool isnull);
	const VPoint & GetNullPoint ()const;
	void SetNullPoint (const VPoint &nullPoint);
	VPoint GetStartPoint ()const;
	VPoint GetEndPoint ()const;
	VPoint GetCenterPoint ()const;
	VPoint GetNextStartPoint ()const;
	VPoint GetNextCenterPoint ()const;
	VPoint GetNextEndPoint()const;
	static LineOrArc & GetFirstValidGeo (std::vector<OffsLineOrArc> &ovec);
	static LineOrArc & GetLastValidGeo (std::vector<OffsLineOrArc> &ovec);
	double GetLength ()const;
	static double GetLength (const std::vector<OffsLineOrArc> &vec);
	void SetParenti (int parenti);
	int GetParenti ()const;
	void SetStartNeighbori (int startNeighbori);
	int GetStartNeighbori ()const;
	void SetEndNeighbori (int endNeighbori);
	int GetEndNeighbori ()const;
	void SetNextStartNeighbori (int nextStartNeighbori);
	int GetNextStartNeighbori ()const;
	void SetNextEndNeighbori (int nextEndNeighbori);
	int GetNextEndNeighbori ()const;
private:
	LineOrArc m_g;
	LineOrArc m_ng;
	bool m_isNull;
	bool m_isNextNull;
	VPoint m_nullPoint;
	int m_parenti;
	int m_startNeighbori;
	int m_endNeighbori;
	int m_nextStartNeighbori;
	int m_nextEndNeighbori;
public:
	static void GetGeoVec ( const std::vector<OffsLineOrArc> &offsVec , std::vector<LineOrArc> &geoVec);
	static size_t GetValidElementCount(const std::vector<OffsLineOrArc> &offsVec);
};

#endif