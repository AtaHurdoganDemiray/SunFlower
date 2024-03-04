/******************************************************************************
*               File: VLine2d.cpp											  *
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
#include "VLine2d.h"
#include "LinearEquations.h"

VLine2d::VLine2d()
{

}

//#############################################################################

VLine2d::VLine2d ( vreal pn1[] , vreal pn2[] )
{
	p1.x ( pn1[0] );
	p1.y ( pn1[1] );	

	p2.x ( pn2[0] );
	p2.y ( pn2[1] );	

}

//#############################################################################

VLine2d::VLine2d ( const VPoint2d &pn1 , const VPoint2d &pn2 )
{
	p1 = pn1;
	p2 = pn2;
}

//#############################################################################

VLine2d::VLine2d ( const VLine2d &toCopy )
{
	p1 = toCopy.p1;
	p2 = toCopy.p2;
}

//#############################################################################

VLine2d & VLine2d::operator = ( const VLine2d &toCopy )
{
	p1 = toCopy.p1;
	p2 = toCopy.p2;
	return *this;
}

//#############################################################################

VLine2d::~VLine2d()
{

}

//#############################################################################

void VLine2d::SetP1 ( const VPoint2d &apoint )
{
	p1 = apoint;
}

//#############################################################################

void VLine2d::SetP2 ( const VPoint2d &apoint )
{
	p2 = apoint;
}

//#############################################################################

const VPoint2d & VLine2d::GetP1 () const
{
	return p1;
}

//#############################################################################

const VPoint2d & VLine2d::GetP2 () const
{
	return p2;
}

//#############################################################################

VPoint2d & VLine2d::GetP1 ()
{
	return p1;
}

//#############################################################################

VPoint2d & VLine2d::GetP2 ()
{
	return p2;
}

//#############################################################################

VPoint2d VLine2d::GetP ( const vreal & sofline ) const
{
	return ( p1 + GetV() * sofline );
}

//#############################################################################

VLine2d::VLine2d ( VIFStream & infile )
{
	(*this)<<infile;
}

void VLine2d::operator << ( VIFStream & infile )
{
	p1<<infile;
	p2<<infile;
}
	
void VLine2d::operator >> ( VofStream & outfile ) const
{
	p1>>outfile;
	p2>>outfile;
}

VLine2d::VLine2d (Bifstream &binfile)
{
	Serialize (binfile);
}

void VLine2d::Serialize (Bifstream &binfile)
{
	p1.Serialize (binfile);
	p2.Serialize (binfile);
}

void VLine2d::Serialize (Bofstream &binfile)const
{
	p1.Serialize (binfile);
	p2.Serialize (binfile);
}

bool VLine2d::operator == (const VLine2d &toCompare)const
{
	if (this==&toCompare)
		return true;
	if(p1!=toCompare.p1)
		return false;
	if(p2!=toCompare.p2)
		return false;
	return true;
}

bool VLine2d::operator != (const VLine2d &toCompare)const
{
	return !(this->operator ==(toCompare));
}

VPoint2d VLine2d::GetNormal()const
{
	VPoint2d linenormal=GetV();
	linenormal.NormThis();
	vreal tmp=linenormal.x();
	linenormal.x(-linenormal.y());
	linenormal.y(tmp);
	return linenormal;
}
vreal VLine2d::GetLineCSDirAndLength(VPoint2d &csDirX , VPoint2d &csDirY)const
{
	csDirX = GetV();
	vreal lineLen = GetLength();
	csDirX *= (1./lineLen);
	csDirY.x(-csDirX.y());
	csDirY.y(csDirX.x());
	return lineLen;
}
void VLine2d::UpdateMinMax(VPoint2d minp,VPoint2d maxp)const
{
	minp.ReplaceMin (p1);
	minp.ReplaceMin (p2);
	
	maxp.ReplaceMax (p1);
	maxp.ReplaceMax (p2);
}

sVector<double,2> VLine2d::GetMin()const
{
	sVector<double,2> tmp(MAX_NUMBER_LIMIT);
	tmp.ReplaceMin(p1.GetMin());
	tmp.ReplaceMin(p2.GetMin());
	return tmp;
}
sVector<double,2> VLine2d::GetMax()const
{
	sVector<double,2> tmp(MIN_NUMBER_LIMIT);
	tmp.ReplaceMax (p1.GetMax());
	tmp.ReplaceMax(p2.GetMax());
	return tmp;
}

vreal VLine2d::GetLength()const
{
	vreal length = GetV().GetLength();
	return length;
}

VPoint2d VLine2d::GetMidPoint()const
{
	return (p1+p2)*0.5;
}

vreal VLine2d::GetPar01 (const VPoint2d &p)
{
	return (p-p1).GetLength() / (p2-p1).GetLength();
}

VLine2d::IntersectionData VLine2d::CalculateIntersection(const VLine2d& L1, const VLine2d& L2)
{
	IntersectionData d;
	

	// Function solves linear eq. system
	// a1 x + b1 y + c1 = 0
	// a2 x + b2 y + c2 = 0
	// if there is a solution returns true and fills x , y solutions,
	// otherwise it returns false
	LinearEquations l;

	// L1Vx*X - L2Vx*Y + L1Px - L2Px = 0
	// L1Vy*X - L2Vy*Y + L1Py - L2Py = 0

	VPoint2d v1 = L1.GetV();
	VPoint2d v2 = L2.GetV();
	bool ck = l.SolveLinear2Unknown(v1.x(),-v2.x(),L1.p1.x()-L2.p1.x()
									,v1.y(),-v2.y(),L1.p1.y()-L2.p1.y()
									,d.mL1IntersectionPar,d.mL2IntersectionPar);
	d.mType = ck ? d.NOTPARALLEL : d.PARALLEL;
	if (ck)
	{
		d.mType = d.NOTPARALLEL;
	}
	else
	{
		d.mType = d.PARALLEL;
		VPoint2d l2Dirx, l2Diry;
		vreal l2Len = L2.GetLineCSDirAndLength(l2Dirx, l2Diry);
		VPoint2d dif = L1.GetP1() - L2.GetP1();
		d.mDistIfParallel = fabs(dif%l2Diry);
	}
	return d;
}

VLine2d::DistanceData VLine2d::CalculateDistances(const VLine2d& L1, const VLine2d& L2)
{
	DistanceData d;
	d.mL1P1 = L1.GetP1();
	d.mL1P2 = L1.GetP2();
	d.mL2P1 = L2.GetP1();
	d.mL2P2 = L2.GetP2();
	d.mL2Len = L2.GetLineCSDirAndLength(d.mL2DirX,d.mL2DirY);
	d.mL1Len = L1.GetLineCSDirAndLength(d.mL1DirX,d.mL1DirY);
	VPoint2d toL1V1 = L1.GetP1() - L2.GetP1();
	VPoint2d toL1V2 = L1.GetP2() - L2.GetP1();
	VPoint2d toL2V1 = L2.GetP1() - L1.GetP1();
	VPoint2d toL2V2 = L2.GetP2() - L1.GetP1();
	d.mL1V1inL2CS.x(d.mL2DirX % toL1V1);
	d.mL1V1inL2CS.y(d.mL2DirY % toL1V1);
	d.mL1V2inL2CS.x(d.mL2DirX % toL1V2);
	d.mL1V2inL2CS.y(d.mL2DirY % toL1V2);
	d.mL2V1inL1CS.x(d.mL1DirX % toL2V1);
	d.mL2V1inL1CS.y(d.mL1DirY % toL2V1);
	d.mL2V2inL1CS.x(d.mL1DirX % toL2V2);
	d.mL2V2inL1CS.y(d.mL1DirY % toL2V2);
	return d;
}

VLine2d::DistanceData::Location VLine2d::DistanceData::GetL1V1MinPointOnL2(vreal& dist, VPoint2d& l2p)
{
	Location L;
	if (mL1V1inL2CS.x() >= 0.0 && mL1V1inL2CS.x() <= mL2Len)
	{
		L = Location::ON_LINE;
		l2p = mL2P1 + mL2DirX * mL1V1inL2CS.x();
		dist = fabs(mL1V1inL2CS.y());
	}
	else if(mL1V1inL2CS.x() < 0.0)
	{
		L = Location::ON_VERTEX1;
		l2p = mL2P1;
		dist = (mL1P1 - mL2P1).GetLength();
	}
	else
	{
		L = Location::ON_VERTEX2;
		l2p = mL2P2;
		dist = (mL1P1 - mL2P2).GetLength();
	}
	return L;
}
VLine2d::DistanceData::Location VLine2d::DistanceData::GetL1V2MinPointOnL2(vreal& dist, VPoint2d& l2p)
{
	Location L;
	if (mL1V2inL2CS.x() >= 0.0 && mL1V2inL2CS.x() <= mL2Len)
	{
		L = Location::ON_LINE;
		l2p = mL2P1 + mL2DirX * mL1V2inL2CS.x();
		dist = fabs(mL1V2inL2CS.y());
	}
	else if (mL1V2inL2CS.x() < 0.0)
	{
		L = Location::ON_VERTEX1;
		l2p = mL2P1;
		dist = (mL1P2 - mL2P1).GetLength();
	}
	else
	{
		L = Location::ON_VERTEX2;
		l2p = mL2P2;
		dist = (mL1P2 - mL2P2).GetLength();
	}
	return L;
}
VLine2d::DistanceData::Location VLine2d::DistanceData::GetL2V1MinPointOnL1(vreal& dist, VPoint2d& l1p)
{
	Location L;
	if (mL2V1inL1CS.x() >= 0 && mL2V1inL1CS.x() <= mL1Len)
	{
		L = Location::ON_LINE;
		l1p = mL1P1 + mL1DirX * mL2V1inL1CS.x();
		dist = fabs(mL2V1inL1CS.y());
	}
	else if (mL2V1inL1CS.x() < 0.0)
	{
		L = Location::ON_VERTEX1;
		l1p = mL1P1;
		dist = (mL2P1 - mL1P1).GetLength();
	}
	else
	{
		L = Location::ON_VERTEX2;
		l1p = mL1P2;
		dist = (mL2P1 - mL1P2).GetLength();
	}
	return L;
}
VLine2d::DistanceData::Location VLine2d::DistanceData::GetL2V2MinPointOnL1(vreal& dist, VPoint2d& l1p)
{
	Location L;
	if (mL2V2inL1CS.x() >= 0 && mL2V2inL1CS.x() <= mL1Len)
	{
		L = Location::ON_LINE;
		l1p = mL1P1 + mL2V2inL1CS.x() * mL1DirX;
		dist = fabs(mL2V2inL1CS.y());
	}
	else if (mL2V2inL1CS.x() < 0.0)
	{
		L = Location::ON_VERTEX1;
		l1p = mL1P1;
		dist = (mL1P1 - mL2P2).GetLength();
	}
	else
	{
		L = Location::ON_VERTEX2;
		l1p = mL1P2;
		dist = (mL1P2 - mL2P2).GetLength();
	}
	return L;
}

bool VLine2d::TestLine2dIntersectionsAndDistances()// ../CAMDisk/Samples/Test/VLine2d.igs
{
	bool ck1 = TestLine2dRegularIntersection1();
	if (ck1)
	{
		LogFileWriter::WriteUnknownExceptionLog("TestLine2dRegularIntersection1:","passed test");
	}
	else
	{
		LogFileWriter::WriteUnknownExceptionLog("TestLine2dRegularIntersection1:", "failed test");
	}
	bool ck2 = TestLine2dRegularIntersection2();
	if (ck2)
	{
		LogFileWriter::WriteUnknownExceptionLog("TestLine2dRegularIntersection2:","passed test");
	}
	else
	{
		LogFileWriter::WriteUnknownExceptionLog("TestLine2dRegularIntersection2:", "failed test");
	}
	bool ck3 = TestLine2dIntersectionParallel1();
	if (ck3)
	{
		LogFileWriter::WriteUnknownExceptionLog("TestLine2dIntersectionParallel1:","passed test");
	}
	else
	{
		LogFileWriter::WriteUnknownExceptionLog("TestLine2dIntersectionParallel1:", "failed test");
	}
	bool ck4 = TestLine2dIntersectionParallel2();
	if (ck4)
	{
		LogFileWriter::WriteUnknownExceptionLog("TestLine2dIntersectionParallel2:", "passed test");
	}
	else
	{
		LogFileWriter::WriteUnknownExceptionLog("TestLine2dIntersectionParallel2:", "failed test");
	}
	bool ck5 = TestLine2dDistances1();
	if (ck5)
	{
		LogFileWriter::WriteUnknownExceptionLog("TestLine2dDistances1:", "passed test");
	}
	else
	{
		LogFileWriter::WriteUnknownExceptionLog("TestLine2dDistances1:", "failed test");
	}
	
	return ck1 && ck2 && ck3 && ck4 && ck5;
}
bool VLine2d::TestLine2dRegularIntersection1()
{
	VLine2d L1(VPoint2d(74.550752 , 115.785629),VPoint2d(168.735731,242.237684));
	VLine2d L2(VPoint2d(123.823450,217.383315),VPoint2d(198.822600,194.709153));
	VPoint2d intP(145.371346,210.868834);
	VLine2d::IntersectionData d = VLine2d::CalculateIntersection(L1, L2);
	vreal compTol = 1.e-4;
	if (d.mType == VLine2d::IntersectionData::NOTPARALLEL)
	{
		VPoint2d ip1 = L1.GetP1() + d.mL1IntersectionPar * L1.GetV();
		if (ip1.Compare(intP, compTol) == false)
			return false;
		VPoint2d ip2 = L2.GetP1() + d.mL2IntersectionPar * L2.GetV();
		if (ip2.Compare(intP, compTol) == false)
			return false;
	}
	else
	{
		return false;
	}
	return true;
}
bool VLine2d::TestLine2dRegularIntersection2()
{
	VPoint2d V1(225.944386,298.521930);
	VPoint2d V2(239.130283,160.383960);
	VPoint2d V3(245.409281,109.524072);
	VPoint2d V4(376.640353,218.778648);
	VPoint2d ip(244.089938,108.425671);
	VLine2d L1(V1,V2);
	VLine2d L2(V3,V4);
	vreal compTol = 1.e-4;
	int counter = 0;
	while (true)
	{
		IntersectionData d = CalculateIntersection(L1, L2);
		if (d.mType == IntersectionData::PARALLEL)
			return false;
		VPoint2d dip1 = L1.GetP1() + d.mL1IntersectionPar * L1.GetV();
		if (dip1.Compare(ip, compTol) == false)
			return false;
		VPoint2d dip2 = L2.GetP1() + d.mL2IntersectionPar * L2.GetV();
		if (dip2.Compare(ip, compTol) == false)
			return false;
		++counter;
		if (counter == 4)
			break;
		if (counter == 1)
			L1 = VLine2d(V2, V1);
		else if (counter == 2)
			L2 = VLine2d(V4, V3);
		else if (counter == 3)
			L1 = VLine2d(V1,V2);
	}
	return true;
}
bool VLine2d::TestLine2dIntersectionParallel1()
{
	VPoint2d V1(502.215828,89.195414);
	VPoint2d V2(631.442669,54.734923);
	VPoint2d V3(492.427754,47.616324);
	VPoint2d V4(627.114884,11.699756);
	int counter = 0;
	VLine2d L1(V1,V2);
	VLine2d L2(V3,V4);
	while (true)
	{
		IntersectionData d = CalculateIntersection(L1, L2);
		if (d.mType == IntersectionData::NOTPARALLEL)
		{
			//return false;
			// this case was produced to be 2 parallel line case 
			// almost parallel but numeric zero tolerance is EPSILON which is around 1.e-12 , determinant was around 1.e-7
			// so solver think it is not parallel 
			// the intersection points are far far from lines , almost infinity 
		}
		++counter;
		if (counter == 1)
			L1 = VLine2d(V2, V1);
		else if (counter == 2)
			L2 = VLine2d(V4, V3);
		else if (counter == 3)
			L1 = VLine2d(V1, V2);
		else
			break;
	}
	return true;
}
bool VLine2d::TestLine2dIntersectionParallel2()
{
	VPoint2d V1(449.251076, -147.387252);
	VPoint2d V2(449.251076, -447.574803);
	VPoint2d V3(253.7050, -526.113913);
	VPoint2d V4(655.690017, -526.113913);
	VPoint2d ip(449.251076, -526.113913);
	vreal compTol = 1.e-4;
	VLine2d L1(V1, V2);
	VLine2d L2(V3, V4);
	int counter = 0;
	while (true)
	{
		IntersectionData d = CalculateIntersection(L1, L2);
		if (d.mType == IntersectionData::PARALLEL)
			return false;
		VPoint2d l1p = L1.GetP1() + d.mL1IntersectionPar * L1.GetV();
		VPoint2d l2p = L2.GetP1() + d.mL2IntersectionPar * L2.GetV();
		if (l1p.Compare(ip, compTol) == false)
			return false;
		if (l2p.Compare(ip, compTol) == false)
			return false;
		++counter;
		if (counter == 1)
			L1 = VLine2d(V2, V1);
		else if (counter == 2)
			L2 = VLine2d(V4, V3);
		else if (counter == 3)
			L1 = VLine2d(V1, V2);
		else
			break;
	}
	return true;
}
bool VLine2d::TestLine2dDistances1()
{
	VPoint2d V1(225.9440, 298.5220);
	VPoint2d V2(239.1300, 160.3840);
	VPoint2d V3(376.6400, 218.7790);
	VPoint2d V4(244.0900, 108.4260);
	VPoint2d onL2(266.7090, 127.2570);
	VPoint2d onL22(326.8460, 177.3230);
	VPoint2d onL1(234.8480, 205.2440);
	VPoint2d onL11 = V2;
	VLine2d L1(V1, V2);
	VLine2d L2(V3,V4);
	vreal compTol = 1.e-3;
	DistanceData d = VLine2d::CalculateDistances(L1, L2);
	vreal dist;
	VPoint2d minp;
	int counter = 0;
	while (true)
	{
		d.GetL1V2MinPointOnL2(dist, minp);
		if (minp.Compare(onL2, compTol) == false)
			return false;
		if (fabs(dist - 43.104) > 1.e-2)
			return false;
		d.GetL1V1MinPointOnL2(dist, minp);
		if (minp.Compare(onL22, compTol) == false)
			return false;
		if (fabs(dist - 157.703) > 1.e-2)
			return false;
		d.GetL2V1MinPointOnL1(dist, minp);
		if (minp.Compare(onL1, compTol) == false)
			return false;
		if (fabs(dist - 142.437) > 1.e-2)
			return false;
		d.GetL2V2MinPointOnL1(dist, minp);
		if (minp.Compare(onL11, compTol) == false)
			return false;
		if (fabs(dist - 52.194463) > compTol)
			return false;
		++counter;
		if (counter == 1)
			L1 = VLine2d(V2, V1);
		else if (counter == 2)
			L2 = VLine2d(V4, V3);
		else if (counter == 3)
			L1 = VLine2d(V1, V2);
		else
			break;
	}
	return true;
}
