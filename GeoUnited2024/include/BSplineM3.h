// BSplineM3.h

#ifndef __BSplineM3_h__
#define __BSplineM3_h__

#include "BSplineM.h"

vreal temp[3] = {0,0,0};

class BSplineM3 : public BSplineM
{
public:
	BSplineM3 ()
	: BSplineM ( 3 , 3  )
	{
		vreal elements[9] = 		
				{  0.5 , -1.0 , 0.5 
				, -1.0 ,  1.0 , 0.0
				,  0.5 ,  0.5 , 0.0  };

		for ( int row = 1; row <=3; row++ )
			for ( int col = 1; col <=3; col++ )
				m.SetElement ( elements [(row-1)*3 + (col-1) ] , row , col ); 	
	};
};

#endif //__BSplineM3_h__
