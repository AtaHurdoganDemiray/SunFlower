/******************************************************************************
*               File: RoughPolarPSet2d.h											  *
*******************************************************************************
*               Description:                                                  *
*                                                                             *
*******************************************************************************
*               History:                                                      *
*  02.07.2003 18:23:08 Created by:    Ata Hurdogan Demiray                    *
*******************************************************************************
*               (C) 2003 by Pozitif GmbH                                     *
******************************************************************************/

#ifndef __RoughPolarPSet2d_h__
#define __RoughPolarPSet2d_h__

#include "VPoint2d.h"
//#include <vector>
//using namespace std;
#include "RoughPolarP2d.h"

class RoughPolarPSet2d
{
public:
	virtual ~RoughPolarPSet2d();
	long Size() const;
	void Clear();
	void InsertPoint ( const vreal &polar_r2 , const vreal &polar_q );
	void InsertPoint ( const RoughPolarP2d &newpoint );
	RoughPolarP2d * operator [] ( const int &index ) const;

private:
	std::vector <RoughPolarP2d*> points;
	void Destruct();	
};

#endif //__RoughPolarPSet2d_h__