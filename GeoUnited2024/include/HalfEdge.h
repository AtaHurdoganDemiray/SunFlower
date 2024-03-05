// HalfEdge.h

#ifndef __HalfEdge_h__
#define __HalfEdge_h__

struct VPoint;

class HalfEdge
{
public:
	HalfEdge (int v0 , int v1);
	int GetV0()const;
	int GetV1()const;
	void SetV0(int v0);
	void SetV1(int v1);
	HalfEdge * GetOpposite()const;
	void SetOpposite (HalfEdge *opposite);
	HalfEdge * GetNext()const;
	void SetNext (HalfEdge *next);
	static double m_epsilon;
	HalfEdge * GetContaining (const VPoint &p)const;
	int GetLoopId ()const;
	void SetLoopId (int loopid);
private:
	int m_v0;
	int m_v1;
	HalfEdge *m_opposite;
	HalfEdge *m_next;
	int m_loopid;
};

#endif