/******************************************************************************
*               File: Camera.h											  *
*******************************************************************************
*               Description:                                                  *
*                                                                             *
*******************************************************************************
*               History:                                                      *
*  23.11.2003 05:55:26 Created by:    Ata Hurdogan Demiray                    *
*******************************************************************************
*               (C) 2003 by Pozitif GmbH		                              *
******************************************************************************/

#ifndef __Camera_h__
#define __Camera_h__

class Camera
{
public:
	Camera();
	~Camera();
	void UpdateLookDirAndUpVec ();
	void WalkRight();
	void WalkLeft();
	void WalkForward();
	void WalkBackward();
	void StepTiltAroundZPositive();
	void StepTiltAroundZNegative();
	void StepHeadUpDownPositive();
	void StepHeadUpDownNegative();
	void SetTiltIncrement ( vreal newval );
	void SetUpDownIncrement ( vreal newval );
	void SetWalkIncrement ( vreal newval );
	void SetUpLimit ( vreal newval );
	void SetDownLimit ( vreal newval );
	void LookAt ();	
	void FlyUp();
	void FlyDown();

	enum movedirection { GOFORWARD ,  GOBACKWARD , GORIGHT , GOLEFT , GOUP , GODOWN };

private:
	vreal eyecoordinate[3];
	vreal lookdir[3];
	vreal upvector[3];
	vreal tilt_around_z;
	vreal head_up_down_angle;
	vreal walk_increment;
	vreal tilt_increment;
	vreal up_down_increment;
	vreal uplimit;
	vreal downlimit;

};

#endif //__Camera_h__
