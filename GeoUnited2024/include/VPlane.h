// VPlane.h

#ifndef __VPlane_h__
#define __VPlane_h__

#include "VPoint.h"
struct VLine;
struct CoordinateSystem;
struct VTriangleE;
class VArc;

struct VPlane
{
public:
	enum IntType {NOINT , COINCIDENT , REGULAR};
	VPlane ( const VPoint & _pointonplane , const VPoint & _normal );
	VPlane ( const vreal *P );	// coefficients
	VPlane ( vreal A,  vreal B,  vreal C,  vreal D );	// coefficients
	VPlane ( const VPlane & Foo );
	VPlane ( VIFStream & infile );
	VPlane & operator = ( const VPlane & Foo );
	void operator >> ( VofStream & outfile );
	void operator << ( VIFStream & infile );
	bool operator == (const VPlane &toCompare)const;
	bool operator != (const VPlane &toCompare)const;
	//calcs intersection of 2 planes&returns true if planes are not parallel 
	IntType Int (const VPlane &p,VLine &intline)const;
	IntType Int (const VLine &l , vreal &intt)const;
	//trims positive side of plane
	void ClipLine (const VLine &l , VLine &res , bool &totallyTrimmed)const;
	void ClipTriangle (const VTriangleE &t , std::vector<VTriangleE> &remaining)const;
	VPoint ProjectPoint (const VPoint &toProj)const;
	IntType LineInt (const VLine &l,double &t)const;
	VPoint pointonplane , normal;
	CoordinateSystem GetCoordSys ()const;
	bool IsOverlapping (const VPlane &other)const;
	void Serialize (Bofstream &f)const;
	void Serialize (Bifstream &f);
	IntType Int (const VArc &a , std::vector<VPoint> &intPoints , std::vector<VPoint> &intPointsForCircleOfArc)const;
	bool IsPointOnPlane (const VPoint &p , vreal tol)const;
};

#endif