// VLine2dE.h

#ifndef __VLine2dE_h__
#define __VLine2dE_h__
#include "VLine2d.h"

struct VLine2dE
{
public:
	enum DegenerateType {emptyset,infiniteset};

	VLine2dE();
	VLine2dE (DegenerateType t);
	VLine2dE (const VLine2d &l);
	VLine2dE(VPoint2d p1,VPoint2d p2);
	VLine2dE(const VLine2dE &toCopy);
	VLine2dE operator = (const VLine2dE &toCopy);
	~VLine2dE();

	const VPoint2d & GetP1() const;
	const VPoint2d & GetP2() const;
	VPoint2d & GetP1();
	VPoint2d & GetP2();
	VPoint2d GetP ( const vreal & sofline ) const;
	VPoint2d GetV() const;
	void SetP1 ( const VPoint2d & apoint );
	void SetP2 ( const VPoint2d & apoint );	
	void Serialize (Bofstream &binfile) const;
	void Serialize (Bifstream &binfile);
	bool operator == (const VLine2dE &toCompare)const;
	bool operator != (const VLine2dE &toCompare)const;
	bool IsCorrupted () const;
	const UnitVector2d &GetNormal()const;
	const UnitVector2d & N()const;
	const vreal & C()const;
	const VLine2d & L()const;
	UnitVector2d & N();
	vreal & C();
	VLine2d & L();
	VLine2dE GetReversed ()const;
	void Reverse ();
	void Negate();

	// if two line segments intersect return value is int param of this line ,between[0,1]
	// if no intersection or lines are parallel return value is -1
	// if lines are parallel and coincide return value is -2
	vreal FastIntCalc (const VLine2dE &l2)const;
private:
	VLine2d m_L;
	UnitVector2d m_N;
	vreal m_C;//C=Dot(L.P1,N)=Dot(L.P2,N)
};

#endif
