/******************************************************************************
*               File: gmVectorUtility.h										  *
*******************************************************************************
*               Description:                                                  *
*                                                                             *
*******************************************************************************
*               History:                                                      *
*  02.07.2003 18:16:47 Created by:    Ata Hurdogan Demiray                    *
*******************************************************************************
*               (C) 2003 by Pozitif GmbH                                      *
******************************************************************************/

#include "stdafx.h"
#include "gmVectorUtility.h"
#include "BasicDefinitions.h"
#include "VLine2d.h"
#include "VLine.h"
#include "PolarCalc2d.h"

// old p_3d functions
typedef vreal real;
typedef vreal p_2d [2];
typedef vreal p_3d [3];
#define X 0
#define Y 1
#define Z 2

inline void v_add ( const p_2d v1 , const p_2d v2 , p_2d result )
{
	result[X] = v1[X] + v2[X];
	result[Y] = v1[Y] + v2[Y];
}

inline void v_add_3d ( const p_3d v1 , const p_3d v2 , p_3d result )
{
	result[X] = v1[X] + v2[X];
	result[Y] = v1[Y] + v2[Y];
	result[Z] = v1[Z] + v2[Z];
}

inline void v_sub ( const p_2d v1 , const p_2d v2 , p_2d result )
{
	result[X] = v1[X] - v2[X];
	result[Y] = v1[Y] - v2[Y];
}

inline void v_sub_3d ( const p_3d v1 , const p_3d v2 , p_3d result )
{
	result[X] = v1[X] - v2[X];
	result[Y] = v1[Y] - v2[Y];
	result[Z] = v1[Z] - v2[Z];
}

inline void v_scale ( vreal k , const p_2d v1 , p_2d result )
{
	result[X] = v1[X] * k;
	result[Y] = v1[Y] * k;
}

inline void v_scale_3d ( vreal k , const p_3d v1 , p_3d result )
{
	result[X] = v1[X] * k;
	result[Y] = v1[Y] * k;
	result[Z] = v1[Z] * k;
}

inline vreal v_length ( const p_2d v1 )
{
	return ( sqrt ( v1[X] * v1[X] + v1[Y] * v1[Y] ));
}

inline vreal v_length_3d ( const p_3d v1 )
{
	return ( sqrt ( v1[X] * v1[X] + v1[Y] * v1[Y] + v1[Z] * v1[Z] ));
}

inline vreal v_length2 ( const p_2d v1 )
{
	return ( v1[X] * v1[X] + v1[Y] * v1[Y] );
}

inline vreal v_length2_3d ( const p_3d v1 )
{
	return ( v1[X] * v1[X] + v1[Y] * v1[Y] + v1[Z] * v1[Z] );
}

inline vreal v_dot ( const p_2d v1 , const p_2d v2 )
{
	return ( v1[X] * v2[X] + v1[Y] * v2[Y] );
}

inline vreal v_dot_3d ( const p_3d v1 , const p_3d v2 )
{
	return ( v1[X] * v2[X] + v1[Y] * v2[Y] + v1[Z] * v2[Z] );
}

inline void copy_p ( p_2d dest , const p_2d source )
{
	dest[X] = source[X];
	dest[Y] = source[Y];
}

inline void copy_p_3d ( p_3d dest , const p_3d source )
{
	dest[X] = source[X];
	dest[Y] = source[Y];
	dest[Z] = source[Z];
}

inline void v_norm ( const p_2d v1 , p_2d normed )
{
	vreal mag = v_length ( v1 );
	if ( fabs(mag) > 0 )
	{
		normed[X] = v1[X] / mag;
		normed[Y] = v1[Y] / mag;
	}
	return;
}

inline void v_norm_3d ( const p_3d v1 , p_3d normed )
{
	vreal mag = v_length_3d ( v1 );
	if ( fabs(mag) > 0 )
	{
		normed[X] = v1[X] / mag;
		normed[Y] = v1[Y] / mag;
		normed[Z] = v1[Z] / mag;
	}
	return;
}

inline vreal v_cross ( const p_2d v1 , const p_2d v2 )
{
	vreal temp;

	temp = v1[X] * v2[Y] - v1[Y] * v2[X];

	return temp;
}

inline void v_cross_3d ( const p_3d v1 , const p_3d v2 , p_3d result )
{
	p_3d temp;

	temp[X] = v1[Y] * v2[Z] - v1[Z] * v2[Y];
	temp[Y] = v1[Z] * v2[X] - v1[X] * v2[Z];
	temp[Z] = v1[X] * v2[Y] - v1[Y] * v2[X];

	copy_p_3d ( result , temp );
}

// utility functions

gmVectorUtility::gmVectorUtility()
{

}

gmVectorUtility::~gmVectorUtility()
{

}

vreal gmVectorUtility::GetAngleBetweenVectors ( const VPoint &v1 , const VPoint &v2 )
{
	VPoint vn1 = v1.Unit();
	VPoint vn2 = v2.Unit();

	vreal dotp = vn1%vn2;
	vreal rad_angle = vp_acos ( dotp );

	rad_angle = fabs(rad_angle);
	vreal rad_angle2 = 2*PI - rad_angle;

	if ( rad_angle < rad_angle2 )
		return rad_angle;
	else
		return rad_angle2;
		
}

void gmVectorUtility::RotatePointAroundLine ( const vreal point[3] , const vreal line[3] 
											 , const vreal origin[3] ,
								    vreal pointrotated[3] , vreal angle )//radians 
{
	vreal v[3] , md[3] , od[3] , pnew[3] , odnormed[3] , mdnormed[3] , linenormed[3];
	vreal mag , anglerd;

	//anglerd = ( angle / 180 ) * PI;
	anglerd = angle;

	v_sub_3d ( point , origin , v );
	v_norm_3d ( line , linenormed );

	v_cross_3d ( v , linenormed , od );
	v_cross_3d ( linenormed , od , md );
	v_norm_3d ( od , odnormed );
	v_norm_3d ( md , mdnormed );

	v_sub_3d ( point , md , pnew );

	mag = v_length_3d ( md );
	p_3d d1 , d2 ;

	v_scale_3d ( mag * cos (anglerd) , mdnormed , d1 );
	v_scale_3d ( -mag * sin (anglerd) , odnormed , d2 );

	v_add_3d ( pnew , d1 , pointrotated );
	v_add_3d ( pointrotated , d2 , pointrotated );
}	


