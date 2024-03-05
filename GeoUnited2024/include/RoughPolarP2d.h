/******************************************************************************
*               File: RoughPolarP2d.h										*
*******************************************************************************
*               Description:                                                  *
*                                                                             *
*******************************************************************************
*               History:                                                      *
*  14.02.2004 20:46:41 Created by:    Ata Hurdogan Demiray                    *
*******************************************************************************
*               (C) 2003 by Positive GmbH                                  *
******************************************************************************/

#ifndef __RoughPolarP2d_h__
#define __RoughPolarP2d_h__

struct VPoint2d;

struct RoughPolarP2d
{
public:
	RoughPolarP2d ( const VPoint2d &cartesien );
	RoughPolarP2d ( const vreal &newr2 , const vreal &newq );
	~RoughPolarP2d();
	vreal r2() const;
	vreal q() const;
	vreal R2() const;
	vreal Q() const;
	void r2 ( const vreal &newr2 );
	void q ( const vreal &newq );
	void R2 ( const vreal &newr2 );
	void Q ( const vreal &newq );
	void operator = ( const RoughPolarP2d &Foo );
	bool operator == ( const RoughPolarP2d &comp ) const;

private:
	vreal pr2,pq;

};

#endif //__RoughPolarP2d_h__
