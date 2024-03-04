// VPlane.cpp

#include "stdafx.h"
#include "VPlane.h"
#include "VLine.h"
#include "VTriangleE.h"
#include "CoordinateSystem.h"
#include "VArc.h"
#include "VInterval.h"

VPlane::VPlane ( const VPoint &  _pointonplane , const VPoint & _normal )
:pointonplane ( _pointonplane ) , normal ( _normal )
{

}

VPlane::VPlane ( const vreal *P )	// coefficients
{
	if(P)
	{
		vreal s = P[0]*P[0] + P[1]*P[1] + P[2]*P[2];
		
		if(abs(s) < EPSILON)
			return;

		vreal a = -P[3] / s;

		pointonplane[0] = a * P[0];
		pointonplane[1] = a * P[1];
		pointonplane[2] = a * P[2];

		s = sqrt(s);

		if(abs(s) < EPSILON)
			return;

		normal[0] = P[0] / s;
		normal[1] = P[1] / s;
		normal[2] = P[2] / s;
	}
}

VPlane::VPlane ( vreal A,  vreal B,  vreal C,  vreal D )	// coefficients
{
	vreal s = A*A + B*B + C*C;
		
	if(abs(s) < EPSILON)
		return;

	vreal a = -D / s;

	pointonplane[0] = a * A;
	pointonplane[1] = a * B;
	pointonplane[2] = a * C;

	s = sqrt(s);

	if(abs(s) < EPSILON)
		return;

	normal[0] = A / s;
	normal[1] = B / s;
	normal[2] = C / s;
}

VPlane::VPlane ( const VPlane & Foo )
{
	(*this) = Foo;
}

VPlane & VPlane::operator = ( const VPlane & Foo )
{
	pointonplane = Foo.pointonplane;
	normal = Foo.normal;
	return (*this);
}

VPlane::VPlane ( VIFStream & infile )
{
	(*this)<<infile;
}

void VPlane::operator >> ( VofStream & outfile )
{
	pointonplane>>outfile;
	normal>>outfile;
}

void VPlane::operator << ( VIFStream & infile )
{
	pointonplane<<infile;
	normal<<infile;
}

bool VPlane::operator == (const VPlane &toCompare)const
{
	if (this==&toCompare)
		return true;

	if(pointonplane!=toCompare.pointonplane)
		return false;
	if(normal!=toCompare.normal)
		return false;

	return true;
}

bool VPlane::operator != (const VPlane &toCompare)const
{
	return !(this->operator ==(toCompare));
}

//2 plane intersection:
//plane1:p1 , normal1
//plane2:p2 , normal2
//p: an intersection point of 2 planes
//vn=normal1 x normal2
//d1=vn x n1
//d2=vn x n2
//[(p1+d1 s1) - p2].n2=0
//[(p2+d2 s2) - p1].n1=0
//s1=(p2-p1).n2/d1.n2
//s2=(p1-p2).n1/d2.n1

VPlane::IntType VPlane::Int (const VPlane &p,VLine &intline)const
{
	VPoint vn=normal^p.normal;
	if(vn.GetLength2()<1.e-24)
	{
		VPoint diff = pointonplane - p.pointonplane;
		vreal ck = diff % normal;
		if (ck < 1.e-12)
			return COINCIDENT;
		else
			return NOINT;
	}
	vn.NormThis();
	VPoint d1=vn^normal;
	//VPoint d2=vn^p.normal;
	VPoint diff=p.pointonplane-pointonplane;
	vreal s1=(diff%p.normal)/(d1%p.normal);
	//vreal s2=(-diff%normal)/(d2%normal);
	VPoint i=pointonplane+(d1*s1);
	intline.SetP1(i);
	intline.SetP2(i+vn);
	return REGULAR;

}

VPlane::IntType VPlane::Int (const VLine &l , vreal &intt)const
{
	VVector v = l.GetV();
	VVector diff = pointonplane - l.GetP1();
	//if (diff.GetLength2() < EPSILON)
	//{
		
	//}
	vreal div = v % normal;
	if (fabs(div) < EPSILON)
	{
		//line is parallel to plane
		if (fabs(diff % normal) > EPSILON)
		{
			return NOINT;
		}
		else
		{
			return COINCIDENT;
		}
	}
	intt = (diff % normal) / div;
	return REGULAR;		
}

void VPlane::ClipLine (const VLine &l , VLine &res , bool &totallyTrimmed)const
{
	//trims positive side of plane
	vreal t;
	IntType ck = Int (l , t);

	if (ck == COINCIDENT)
	{
		totallyTrimmed = false;
		return;
	}
	if (ck == NOINT)
	{
		VVector diff = l.GetP1() - pointonplane;
		vreal ck2 = diff % normal;
		if (ck2 > 0)
		{
			totallyTrimmed = true;
		}
		else
		{
			totallyTrimmed = false;
			res = l;
		}
		return;
	}
	
	if ( t<0 || t>1)
	{
		VVector diff = l.GetP1() - pointonplane;
		vreal ck2 = diff % normal;
		if (ck2 > 0)
		{
			totallyTrimmed = true;
		}
		else
		{
			totallyTrimmed = false;
			res = l;
		}
		return;
	}

	VVector diff = l.GetP1() - pointonplane;
	vreal ck3 = diff % normal;
	VPoint intp = l.GetP (t);
	if (ck3 < 0)
	{
		res.SetP1 (l.GetP1());
		res.SetP2 (intp);
		totallyTrimmed = false;
	}
	else
	{
		res.SetP1(intp);
		res.SetP2(l.GetP2());
		totallyTrimmed = false;
	}
	return;
}

VPoint VPlane::ProjectPoint (const VPoint &toProj)const
{
	// ((toProj + t normal) - pointonplane ) % normal = 0
	// toProj % normal + t - pointonplnae % normal = 0
	//  t = ( pointonplane - toProj ) % normal
	// pp = toProj + t normal
	vreal t = (pointonplane - toProj) % normal;
	VPoint pp = toProj + t * normal;
	return pp;
}

VPlane::IntType VPlane::LineInt(const VLine &l,double &t)const
{
	double denum = l.GetV() % normal;
	VPoint df = pointonplane - l.GetP1();
	double num = df % normal;
	if (fabs(denum)<EPSILON)
	{
		// line is parallel to plane
		if (fabs(num) < EPSILON)
			return VPlane::COINCIDENT;
		else 
			return VPlane::NOINT;
	}
	t = num / denum;
	return VPlane::REGULAR;
}

void VPlane::ClipTriangle(const VTriangleE &t, std::vector<VTriangleE> &remaining)const
{
	// Check sides for vertices
	const VPoint &v1 = t.GetCorner1();
	const VPoint &v2 = t.GetCorner2();
	const VPoint &v3 = t.GetCorner3();

	vreal p1 = (v1 - pointonplane) % normal;
	vreal p2 = (v2 - pointonplane) % normal;
	vreal p3 = (v3 - pointonplane) % normal;

	if (p1 > -EPSILON && p2 > -EPSILON && p3 > -EPSILON)
	{
		return;
	}
	else if (p1 < -EPSILON && p2 < -EPSILON && p3 < -EPSILON)
	{
		remaining.push_back(t);
		return;
	}

	return;

	// triangle should be splited
	VLine e1 (v1,v2);
	VLine e2 (v2,v3);
	VLine e3 (v3,v1);

	VLine r1,r2,r3;
	bool b1,b2,b3;
	ClipLine(e1,r1,b1);
	ClipLine(e2,r2,b2);
	ClipLine(e3,r3,b3);

	if (b1==true && b2 == false && b3 == false)
	{
		
	}
	else if (b1 == false && b2 == true && b3 == false)
	{

	}
	else if (b1 == false && b2 == false && b3 == true)
	{

	}
	else if (b1 == false && b2 == false && b3 == false)
	{
		// No triangle trimmed

	}
	else
		throw VException (( IDS_VPlCliExcCanNot),1);
}

bool VPlane::IsOverlapping(const VPlane &other) const
{
	double dp = this->normal % other.normal;
	if (fabs(1. - fabs(dp)) < 1.e-8)
	{
		VPoint pp1 = other.ProjectPoint(pointonplane);
		VPoint pp2 = ProjectPoint(other.pointonplane);
		if ( (pp1 - pointonplane).GetLength() > 1.e-8)
			return false;
		if ((pp2 - other.pointonplane).GetLength() > 1.e-8)
			return false;
		return true;
	}
	else
		return false;
}

CoordinateSystem VPlane::GetCoordSys() const
{
	VPoint dirz = normal.Unit();
	VVector dirx;
	double dp = dirz % UY;
	VVector dir1 = UY ^ dirz;
	VVector dir2 = dirz ^ UZ;
	dirx = dir1.GetLength2() > dir2.GetLength2() ? dir1 : dir2;
	dirx.NormThis();
	VVector diry = normal ^ dirx;
	CoordinateSystem cs (pointonplane,dirx,diry,dirz);
	return cs;
}

void VPlane::Serialize (Bofstream &f)const
{
	int version = 1;
	f.WriteInt (version);
	if (version >= 1)
	{
		pointonplane.Serialize (f);
		normal.Serialize (f);
	}
}

void VPlane::Serialize(Bifstream &f)
{
	int version;
	f.ReadInt (version);
	if (version >= 1)
	{
		pointonplane.Serialize (f);
		normal.Serialize (f);
	}
}

VPlane::IntType VPlane::Int (const VArc &a, std::vector<VPoint> &intPoints , std::vector<VPoint> &intPointsForCircleOfArc)const
{
	CoordinateSystem arcCs = a.GetCoordinateSystem();
	VPlane aplane (arcCs.GetOrigin(),arcCs.GetDirz());
	VLine iline;
	IntType ck = this->Int (aplane,iline);
	if (ck == REGULAR)
	{
		vreal swan = a.GetSweepAngle();
		VInterval ia (0.0 , swan);
		vreal angleofset = swan < 0.0 ? -360.0 : 0.0;
		std::vector<VPoint> allint = a.GetIntForInfiniteLineAndCircleOfArc (iline);
		size_t mysize = allint.size();
		for (size_t i = 0; i<mysize;++i)
		{
			const VPoint &curip = allint.at(i);
			intPointsForCircleOfArc.push_back (curip);
			vreal curangle = a.GetAngle (curip);
			curangle += angleofset;
			VInterval tempi (curangle,curangle);
			if (ia.IsContaining (tempi))
			{
				intPoints.push_back (curip);
			}
		}
	}
	return ck;
}

bool VPlane::IsPointOnPlane (const VPoint &p , vreal tol)const
{
	VPoint diff = p - this->pointonplane;
	vreal dp = diff % this->normal;
	if (fabs(dp) < tol)
		return true;
	else
		return false;
}