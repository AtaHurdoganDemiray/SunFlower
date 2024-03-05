// BSplineM4.h

#ifndef __BSplineM4_h__
#define __BSplineM4_h__

#include "BSplineM.h"

class BSplineM4 : public BSplineM
{
public:
	BSplineM4 ()
		: BSplineM ( 4, 4 )
	{
		vreal elements[16] = { -1/6 , 0.5 , -0.5 , 1/6 ,
								 0.5 , 1.0 ,  0.5 , 0.0 ,
								-0.5 , 0.0 ,  0.5 , 0.0 ,
								 1/6 , 2/3 ,  1/6 , 0.0  };

		for ( int row = 1; row <=4; row++ )
			for ( int col = 1; col <= 4; col++ )
				m.SetElement ( elements[ row-1 * 4 + ( col - 1 ) ] , row , col );
	};

};

#endif //__BSplineM4_h__
