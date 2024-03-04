/******************************************************************************
*               File: PolarPoint2d.h										*
*******************************************************************************
*               Description:                                                  *
*                                                                             *
*******************************************************************************
*               History:                                                      *
*  14.02.2004 20:46:41 Created by:    Ata Hurdogan Demiray                    *
*******************************************************************************
*               (C) 2003 by Positive GmbH                                  *
******************************************************************************/

#ifndef __PolarPoint2d_h__
#define __PolarPoint2d_h__

struct VPoint2d;

struct PolarPoint2d
{
public:
	PolarPoint2d ( const VPoint2d &cartesien );
	PolarPoint2d ( const vreal &newr , const vreal &newq );
	~PolarPoint2d();
	vreal r() const;
	vreal q() const;
	vreal R() const;
	vreal Q() const;
	void r ( const vreal &newr );
	void q ( const vreal &newq );
	void R ( const vreal &newr );
	void Q ( const vreal &newq );
	void operator = ( const PolarPoint2d &Foo );
	bool operator == ( const PolarPoint2d &comp ) const;

private:
	vreal pr,pq;

};

#endif //__PolarPoint2d_h__
