/******************************************************************************
*               File: VLine.h											  *
*******************************************************************************
*               Description:                                                  *
*                                                                             *
*******************************************************************************
*               History:                                                      *
*  02.07.2003 18:16:47 Created by:    Ata Hurdogan Demiray                    *
*******************************************************************************
*               (C) 2003 by Pozitif GmbH                                      *
******************************************************************************/

#ifndef __VLine_h__
#define __VLine_h__

#include "VPoint.h"

struct VLine
{
public:
	VLine();
	VLine( const VPoint & pn1 , const VPoint & pn2 );
	VLine ( vreal pn1[3] , vreal pn2[3] );
	VLine (const VLine &toCopy);
	VLine & operator =(const VLine &toCopy);
	VLine ( VIFStream & infile );
	~VLine();
	const VPoint & GetP1() const;
	const VPoint & GetP2() const;
	VPoint & GetP1();
	VPoint & GetP2();
	VPoint GetV() const { return (p2-p1); }
	void SetP1 ( const VPoint & apoint );
	void SetP2 ( const VPoint & apoint );
	VPoint GetP ( const vreal & parameter ) const;
	double GetT (const VPoint &p)const;

	void operator << ( VIFStream & infile );
	void operator >> ( VofStream & outfile ) const;
	void Serialize (Bofstream &binfile)const;
	void Serialize (Bifstream &binfile);
	VLine ( Bifstream & binfile );
	bool operator == (const VLine &toCompare)const;
	bool operator != (const VLine &toCompare)const;
	
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
	
	enum IntType {parallel_seperate,parallel_coincident,intersecting,nointersection};
	IntType InfiniteLinesInt(const VLine &other
							,VPoint &intp1,vreal &ints1
							,VPoint &intp2,vreal &ints2)const;
	void UpdateMinMax(VPoint minp,VPoint maxp)const;
	sVector<double,3> GetMin()const;
	sVector<double,3> GetMax()const;
	vreal GetLength()const;
	VPoint GetMidPoint()const;
	void Reverse();
	std::vector<VLine> Split (const double &t,const double &lengththreshold)const;
	VLine GetReversed ()const;
	std::vector<VLine> SplitLineByLength (double l)const;
	VPoint GetNormalOnXYPlane ()const;
private:
	VPoint p1;
	VPoint p2;
public:
	bool IsDegenerate ( bool ckMinLength
					  , double minLengthAllowed
					  , bool ckMaxLength
					  , double maxLengthAllowed)const;
public:
	// compare returns true if two geometry is same within tolerance else false
	bool Compare (const VLine &l2,double tol)const;
	bool IsOnLine (const VPoint p3 , vreal &t,vreal tol) const;
	VLine GetLineOnXY()const;
};

#endif //__VLine_h__

