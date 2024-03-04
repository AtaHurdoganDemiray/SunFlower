/******************************************************************************
*               File: SurfPointsPack.h										  *
*******************************************************************************
*               Description:                                                  *
*                                                                             *
*******************************************************************************
*               History:                                                      *
*  04.03.2004 14:49:08 Created by:    Ata Hurdogan Demiray                    *
*******************************************************************************
*               (C) 2003 by Positive GmbH	                                 *
******************************************************************************/

#ifndef __SurfPointsPack_h__
#define __SurfPointsPack_h__

#include "PointSet2d.h"
#include "PointSet.h"

class SurfPointsPack
{
public:
	SurfPointsPack ( const long &initialcapacity );
	SurfPointsPack ( const SurfPointsPack & Foo );
	bool operator == (const SurfPointsPack &toCompare)const;
	bool operator != (const SurfPointsPack &toCompare)const;	
	const VPoint & GetModelPoint ( const long & i)const;
	const VPoint2d & GetParamPoint ( const long & i)const;
	const VPoint & GetNormal ( const long & i)const;
	int InsertPoint (const VPoint2d &par,const VPoint &mod,const VPoint &nor);
	void Serialize (Bofstream &outfile)const;
	void Serialize (Bifstream &infile);
	void RemoveDuplicate ();
	void Transform (const TransformationMatrix &tm);
	PointSet2d parampoints;
	PointSet modelpoints;
	PointSet normals;
	void Clear();
};

#endif //__SurfPointsPack_h__
