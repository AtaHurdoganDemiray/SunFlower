/******************************************************************************
*               File: Camera.cpp											  *
*******************************************************************************
*               Description:                                                  *
*                                                                             *
*******************************************************************************
*               History:                                                      *
*  23.11.2003 05:58:26 Created by:    Ata Hurdogan Demiray                    *
*******************************************************************************
*               (C) 2003 by Pozitif GmbH	                                  *
******************************************************************************/

#include "stdafx.h"
#include "Camera.h"
#include "VPoint.h"

Camera::Camera()
{
	eyecoordinate[0] = 0.0;
	eyecoordinate[1] = 0.0;
	eyecoordinate[2] = 1.6;
	lookdir[0] = 0.0;
	lookdir[1] = 1.0;
	lookdir[2] = 0.0;
	upvector[0] = 0.0;
	upvector[1] = 0.0;
	upvector[2] = 1.0;

	tilt_around_z = 90.0;
	head_up_down_angle = 0.0;
	uplimit = 45;
	downlimit = -45;
	tilt_increment = 1.0;
	walk_increment = 1.0;
	up_down_increment = 1.0;

}

//#############################################################################

Camera::~Camera()
{

}

//#############################################################################

void Camera::SetWalkIncrement ( vreal newval )
{
	walk_increment = newval;
}

//#############################################################################

void Camera::SetTiltIncrement ( vreal newval )
{
	tilt_increment = newval;
}

//#############################################################################

void Camera::SetUpDownIncrement ( vreal newval )
{
	up_down_increment = newval;
}

//#############################################################################

void Camera::SetUpLimit ( vreal newval )
{
	uplimit = newval;
}

//#############################################################################

void Camera::SetDownLimit ( vreal newval )
{
	downlimit = newval;
}

//#############################################################################

void Camera::WalkRight ()
{
	VPoint upvec ( upvector[0] , upvector[1] , upvector[2] );
	VPoint lookvec ( lookdir[0] , lookdir[1] , lookdir[2] );
	VPoint rightvec = lookvec ^ upvec;

	eyecoordinate[0] -= ( rightvec.x() * walk_increment );
	eyecoordinate[1] -= ( rightvec.y() * walk_increment );

	//UpdateLookDirAndUpVec ();
}

//#############################################################################

void Camera::WalkLeft ()
{
	VPoint upvec ( upvector[0] , upvector[1] , upvector[2] );
	VPoint lookvec ( lookdir[0] , lookdir[1] , lookdir[2] );
	VPoint rightvec = lookvec ^ upvec;

	eyecoordinate[0] += ( rightvec.x() * walk_increment );
	eyecoordinate[1] += ( rightvec.y() * walk_increment );

	//UpdateLookDirAndUpVec ();
}

//#############################################################################

void Camera::WalkForward()
{
	VPoint forw ( lookdir[0] , lookdir[1] , 0.0 );
	forw = forw.Unit();

	eyecoordinate[0] = eyecoordinate[0] + ( forw.x() * walk_increment );
	eyecoordinate[1] = eyecoordinate[1] + ( forw.y() * walk_increment );

	//UpdateLookDirAndUpVec ();
}

//#############################################################################

void Camera::WalkBackward()
{
	VPoint forw ( lookdir[0] , lookdir[1] , 0.0 );
	forw = forw.Unit();

	eyecoordinate[0] = eyecoordinate[0] - ( forw.x() * walk_increment );
	eyecoordinate[1] = eyecoordinate[1] - ( forw.y() * walk_increment );
	
	//UpdateLookDirAndUpVec ();

}

//#############################################################################

void Camera::StepTiltAroundZPositive ()
{
	tilt_around_z += tilt_increment;
	
	if ( fabs(tilt_around_z) > 360 )
	{
		int temp = (int) tilt_around_z ;
		vreal decimal = tilt_around_z - temp;
		tilt_around_z = ( (vreal)(temp%360) + decimal );
	}
		
	UpdateLookDirAndUpVec ();

}

//#############################################################################

void Camera::StepTiltAroundZNegative ()
{
	tilt_around_z -= tilt_increment;
	
	if ( fabs(tilt_around_z) > 360 )
	{
		int temp = (int) tilt_around_z ;
		vreal decimal = tilt_around_z - temp;
		tilt_around_z = ( (vreal)(temp%360) + decimal );
	}

	UpdateLookDirAndUpVec ();

}

//#############################################################################

void Camera::StepHeadUpDownPositive ()
{
	if ( head_up_down_angle < uplimit )
		head_up_down_angle += up_down_increment;

	UpdateLookDirAndUpVec ();

}

//#############################################################################

void Camera::StepHeadUpDownNegative ()
{
	if ( head_up_down_angle > downlimit )
		head_up_down_angle -= up_down_increment;

	UpdateLookDirAndUpVec ();

}

//#############################################################################

void Camera::FlyUp ()
{
	eyecoordinate[2] += walk_increment;
	//UpdateLookDirAndUpVec ();
}

//#############################################################################

void Camera::FlyDown ()
{
	eyecoordinate[2] -= walk_increment;
	//UpdateLookDirAndUpVec ();
}

//#############################################################################

void Camera::UpdateLookDirAndUpVec ()
{
	VPoint zdir ( 0.0 , 0.0 , 1.0 );
	vreal rad_tilt = tilt_around_z * PI / 180.0;
	vreal rad_head = head_up_down_angle * PI / 180.0;

	VPoint temp ( cos ( rad_tilt ) , sin ( rad_tilt ) , 0.0 );
	VPoint perp = zdir^temp;

	vreal scalef = cos ( rad_head );
	perp *= scalef;
	perp.z ( sin ( rad_head ) );

	lookdir[0] = perp.x();
	lookdir[1] = perp.y();
	lookdir[2] = perp.z();

	VPoint upvec = perp ^ temp;

	upvector[0] = upvec.x();
	upvector[1] = upvec.y();
	upvector[2] = upvec.z();
		
}	

//#############################################################################

void Camera::LookAt ()
{
/*	gluLookAt ( eyecoordinate[0] , eyecoordinate[1] , eyecoordinate[2]
				, eyecoordinate[0] + lookdir[0]
				, eyecoordinate[1] + lookdir[1]
				, eyecoordinate[2] + lookdir[2] 
				, upvector[0] , upvector[1] , upvector[2] );
				
*/	
	glMatrixMode ( GL_MODELVIEW );
	glLoadIdentity ();

	//VPoint lookvec ( lookdir[0] , lookdir[1] , lookdir[2] );
	//VPoint upvec ( upvector[0] , upvector[1] , upvector[2] );

	//VPoint headvec = lookvec ^ upvec;

	glRotated ( -90 , 1.0 , 0.0 , 0.0 );// 4th T

	glRotated ( -head_up_down_angle , 1.0 , 0.0 , 0.0 );// 4th T

	glRotated ( -tilt_around_z , 0.0 , 0.0 , 1.0 );// 3rd T

	//glRotated ( -90 , 1.0 , 0.0 , 0.0 );// 2nd T

	glTranslated ( -eyecoordinate[0] , -eyecoordinate[1] , -eyecoordinate[2] ); // 1st T

}

//#############################################################################
