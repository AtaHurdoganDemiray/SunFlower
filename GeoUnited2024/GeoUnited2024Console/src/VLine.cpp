/******************************************************************************
*               File: VLine.cpp											  *
*******************************************************************************
*               Description:                                                  *
*                                                                             *
*******************************************************************************
*               History:                                                      *
*  02.07.2003 18:30:20 Created by:    Ata Hurdogan Demiray                    *
*******************************************************************************
*               (C) 2003 by Pozitif GmbH                                      *
******************************************************************************/

#include "stdafx.h"
#include "VLine.h"
#include "Bifstream.h"
#include "Bofstream.h"

#include "VLine2d.h"

VLine::VLine()
{

}

//#############################################################################

VLine::VLine ( vreal pn1[] , vreal pn2[] )
{
	p1.x ( pn1[0] );
	p1.y ( pn1[1] );	
	p1.z ( pn1[2] );

	p2.x ( pn2[0] );
	p2.y ( pn2[1] );	
	p2.z ( pn2[2] );

}

//#############################################################################

VLine::VLine ( const VPoint &pn1 , const VPoint &pn2 )
{
	p1 = pn1;
	p2 = pn2;
}

//#############################################################################

VLine::VLine (const VLine &toCopy)
{
	p1 = toCopy.p1;
	p2 = toCopy.p2;
}	

//#############################################################################

VLine & VLine::operator =(const VLine &toCopy)
{
	p1 = toCopy.p1;
	p2 = toCopy.p2;
	return *this;
}

//#############################################################################

VLine::~VLine()
{

}

//#############################################################################

void VLine::SetP1 ( const VPoint &apoint )
{
	p1 = apoint;
}

//#############################################################################

void VLine::SetP2 ( const VPoint &apoint )
{
	p2 = apoint;
}

//#############################################################################

const VPoint & VLine::GetP1 () const
{
	return p1;
}

//#############################################################################

const VPoint & VLine::GetP2 () const
{
	return p2;
}

//#############################################################################

VPoint & VLine::GetP1 ()
{
	return p1;
}

//#############################################################################

VPoint & VLine::GetP2 ()
{
	return p2;
}

//#############################################################################

VPoint VLine::GetP ( const vreal & parameter ) const
{
	return ( p1 + ( GetV() * parameter ) );
}

double VLine::GetT (const VPoint &p)const
{
	return ((p - p1) % GetV()) / GetV().GetLength2();
}

VLine::VLine ( VIFStream & infile )
{
	(*this)<<infile;
}

void VLine::operator << ( VIFStream & infile )
{
	p1<<infile;
	p2<<infile;
}

void VLine::operator >> ( VofStream & outfile ) const
{
	p1>>outfile;
	p2>>outfile;
}

bool VLine::operator == (const VLine &toCompare)const
{
	if (this==&toCompare)
		return true;
	if(p1!=toCompare.p1)
		return false;
	if(p2!=toCompare.p2)
		return false;
	return true;
}

bool VLine::operator != (const VLine &toCompare)const
{
	return !(this->operator ==(toCompare));
}

VLine::VLine (Bifstream &binfile)
{
	Serialize (binfile);
}

void VLine::Serialize (Bifstream &binfile)
{
	p1.Serialize (binfile);
	p2.Serialize (binfile);
}

void VLine::Serialize (Bofstream &binfile) const
{
	p1.Serialize (binfile);
	p2.Serialize (binfile);
}

VLine::IntType VLine::InfiniteLinesInt	(const VLine &other
										,VPoint &intp1,vreal &ints1
										,VPoint &intp2,vreal &ints2)const
{
	VPoint v1=p2-p1;
	VPoint v2=other.p2-other.p1;

	vreal myA = -2 * (v1 % v2);
	vreal myB = 2 * (v1 % v1);
	vreal myC = 2 * ( ( p1 % v1 ) - ( v1 % other.p1 ) );
	vreal myD = 2 * ( v2 % v2 );
	vreal myE = myA;//
	vreal myF = 2 * ( ( other.p1 % v2 ) - ( p1 % v2 ) );

	vreal mydivisor = myE * myA - myB * myD;

	if ( fabs(mydivisor) < EPSILON )
	{	
		// lines are paralel
		if (fabs((other.p1-p1)%v1.Unit())-1.0<1.e-12)
			return parallel_coincident;
		else
			return parallel_seperate;		
	}

	ints1 = ( myD * myC - myA * myF ) / mydivisor;
	ints2 = (myB * myF - myE * myC) / mydivisor;
	intp1 = GetP(ints1);
	intp2 = other.GetP(ints2);
	if (intp1.IsCoincident(intp2))
		return intersecting;
	else
		return nointersection;
}

void VLine::UpdateMinMax(VPoint minp,VPoint maxp)const
{
	minp.ReplaceMin (p1);
	minp.ReplaceMin (p2);
	
	maxp.ReplaceMax (p1);
	maxp.ReplaceMax (p2);
}

sVector<double,3> VLine::GetMin()const
{
	sVector<double,3> tmp (MAX_NUMBER_LIMIT);
	tmp.ReplaceMin(p1.GetMin());
	tmp.ReplaceMin(p2.GetMin());
	return tmp;
}
sVector<double,3> VLine::GetMax()const
{	
	sVector<double,3> tmp (MIN_NUMBER_LIMIT);
	tmp.ReplaceMax(p1.GetMax());
	tmp.ReplaceMax(p2.GetMax());
	return tmp;
}

vreal VLine::GetLength()const
{
	vreal length = GetV().GetLength();
	return length;
}

VPoint VLine::GetMidPoint()const
{
	return (p1+p2)*0.5;
}

void VLine::Reverse ()
{
	VPoint tmp (p1);
	p1 = p2;
	p2 = tmp;
}

std::vector<VLine> VLine::Split (const double &t , const double &lengththreshold)const
{
	std::vector<VLine> res;
	if (t<0 || t>1 || fabs(t) < EPSILON || fabs(1.-t) < EPSILON)
	{
		res.push_back(*this);
	}
	else
	{
		VLine l1 (p1,GetP(t));
		VLine l2 (GetP(t),p2);
		if (l1.GetLength() > lengththreshold && l2.GetLength() > lengththreshold)
		{
			res.push_back(l1);
			res.push_back(l2);
		}
		else
			res.push_back(*this);
	}
	return res;
}

VLine VLine::GetReversed ()const
{
	VLine r (p2,p1);
	return r;
}

std::vector<VLine> VLine::SplitLineByLength (double l)const
{
	std::vector<VLine> res;
	double parstep = l / GetLength();
	if (parstep < EPSILON )
		throw VException (( IDS_VLiSplExcLenIsZ),1);
	if (parstep > 1.)
	{
		// l is greater than line length so can not split but give the line it self
		res.push_back (*this);
		return res;
	}
	VPoint myp0 = GetP(0.0);
	VPoint myp1;
	for (double t = parstep;t<1.0; t+=parstep)
	{
		myp1 = GetP (t);
		res.push_back(VLine(myp0,myp1));
		myp0 = myp1;
	}
	myp1 = GetP(1.0);
	VLine tmp (myp0,myp1);
	if (tmp.GetLength() > 1.e-8)
		res.push_back(tmp);
	return res;
}

VPoint VLine::GetNormalOnXYPlane ()const
{
	//if (fabs (p1.z() - p2.z()) > EPSILON)
	//	throw VException (("VLine::GetNormal exception , line is not XY plane"),1);

	VLine2d l2 (VPoint2d(p1.x(),p1.y()) , VPoint2d (p2.x(),p2.y()));
	VPoint2d nl2 = l2.GetNormal();
	VPoint n (nl2.x(), nl2.y() ,  0.0);
	return n;
}

bool VLine::IsDegenerate (bool ckMinLength
						  , double minLengthAllowed
						  , bool ckMaxLength
						  , double maxLengthAllowed)const
{
	if (ckMinLength || ckMaxLength)
	{
		double len = GetLength();
		if (ckMinLength)
		{
			if (len < minLengthAllowed)
				return true;
		}
		if (ckMaxLength)
		{
			if (len > maxLengthAllowed)
				return true;
		}
	}
	return false;
}

bool VLine::Compare(const VLine &l2,double tol) const
{
	if (GetP1().Compare (l2.GetP1(),tol) == false)
		return false;
	if (GetP2().Compare (l2.GetP2(),tol) == false)
		return false;
	return true;
}
bool VLine::IsOnLine (const VPoint p3 , vreal &t , vreal tol) const
{
	t = GetT(p3);
	VPoint pp = GetP(t);
	
	if (pp.Compare(p3,tol))
		return true;

	return false;
}

VLine VLine::GetLineOnXY()const
{
	VLine myline(VPoint(GetP1().x(),GetP1().y(), 0.0), VPoint(GetP2().x(),GetP2().y(),0.0));
	return myline;
}
