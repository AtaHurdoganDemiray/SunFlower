/******************************************************************************
*               File: VLine2d.h											  *
*******************************************************************************
*               Description:                                                  *
*                                                                             *
*******************************************************************************
*               History:                                                      *
*  02.07.2003 18:16:47 Created by:    Ata Hurdogan Demiray                    *
*******************************************************************************
*               (C) 2003 by Pozitif GmbH                                      *
******************************************************************************/

#ifndef __VLine2d_h__
#define __VLine2d_h__

#include "VPoint2d.h"

struct VLine2d
{
public:
	VLine2d();
	VLine2d( const VPoint2d & pn1 , const VPoint2d & pn2 );
	VLine2d ( vreal pn1[3] , vreal pn2[3] );
	VLine2d ( VIFStream & infile );	
	VLine2d (const VLine2d &toCopy);
	VLine2d & operator = (const VLine2d &toCopy);
	~VLine2d();
	const VPoint2d & GetP1() const;
	const VPoint2d & GetP2() const;
	VPoint2d & GetP1();
	VPoint2d & GetP2();
	VPoint2d GetP ( const vreal & sofline ) const;
	VPoint2d GetV() const { return (p2-p1); }
	void SetP1 ( const VPoint2d & apoint );
	void SetP2 ( const VPoint2d & apoint );
	void operator << ( VIFStream & infile );
	void operator >> ( VofStream & outfile ) const;
	
	void Serialize (Bofstream &binfile) const;
	void Serialize (Bifstream &binfile);
	VLine2d (Bifstream &binfile);
	bool operator == (const VLine2d &toCompare)const;
	bool operator != (const VLine2d &toCompare)const;
	
	inline bool IsCorrupted () const
	{
		if ( p1.IsCorrupted() || p2.IsCorrupted() )
		{
			return true;
		}
		
		if ( GetV().GetLength() < EPSILON )
		{
			return true;
		}

		return false;
	};
	
	VPoint2d GetNormal()const;
	void UpdateMinMax(VPoint2d minp,VPoint2d maxp)const;
	sVector<double,2> GetMin()const;
	sVector<double,2> GetMax()const;
	vreal GetLength()const;
	VPoint2d GetMidPoint()const;
	vreal GetPar01 (const VPoint2d &p);
private:
	VPoint2d p1;
	VPoint2d p2;
public:
	struct IntersectionData
	{
	public:
		enum Type{ NOTPARALLEL = 0 , PARALLEL = 1};
		vreal mL1IntersectionPar;
		vreal mL2IntersectionPar;
		Type mType; 
		vreal mDistIfParallel;
	};
	struct DistanceData
	{
	public:
		VPoint2d mL1P1, mL1P2,mL2P1,mL2P2;
		VPoint2d mL1DirX;
		VPoint2d mL1DirY;
		vreal mL1Len;
		VPoint2d mL2DirX;
		VPoint2d mL2DirY;
		vreal mL2Len;

		VPoint2d mL1V1inL2CS;
		VPoint2d mL1V2inL2CS;
		VPoint2d mL2V1inL1CS;
		VPoint2d mL2V2inL1CS;
		enum Location { ON_LINE = 0, ON_VERTEX1 = 1, ON_VERTEX2 = 1 };
		Location GetL1V1MinPointOnL2(vreal &dist , VPoint2d &l2p);
		Location GetL1V2MinPointOnL2(vreal &dist , VPoint2d &l2p);
		Location GetL2V1MinPointOnL1(vreal& dist , VPoint2d &l1p);
		Location GetL2V2MinPointOnL1(vreal& dist , VPoint2d &l1p);
	};
	static IntersectionData CalculateIntersection(const VLine2d& L1, const VLine2d& L2);
	static DistanceData CalculateDistances(const VLine2d &L1 , const VLine2d &L2);
	vreal GetLineCSDirAndLength(VPoint2d& csDirX , VPoint2d &csDirY)const;

	static bool TestLine2dIntersectionsAndDistances();
	static bool TestLine2dRegularIntersection1();
	static bool TestLine2dRegularIntersection2();
	static bool TestLine2dIntersectionParallel1();
	static bool TestLine2dIntersectionParallel2();
	static bool TestLine2dDistances1();
};

#endif //__VLine2d_h__

