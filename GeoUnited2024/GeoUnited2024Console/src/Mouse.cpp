// Mouse.cpp: implementation of the Mouse class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "Mouse.h"


#ifdef _DEBUG
#undef THIS_FILE
static VChr THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

Mouse::Mouse()
{
	button_down_flag = false;
	mbutton_down_flag = false;
}

Mouse::~Mouse()
{

}

const VPoint & Mouse::GetPrevLButtonDownP() const
{
	return prev_down_click_p;
}

const VPoint & Mouse::GetPrevLButtonUpP() const
{
	return prev_up_click_p;
}

const VPoint & Mouse::GetLButtonDownP() const
{
	return down_click_p;
}

const VPoint & Mouse::GetPrevP() const
{
	return prev_p;
}

const VPoint & Mouse::GetCurrentP() const
{
	return current_p;
}

const VPoint & Mouse::GetLButtonUpP() const
{
	return up_click_p;
}

const VPoint & Mouse::GetButtonDownMovingP() const
{
	return button_down_moving_p;
}

VPoint Mouse::GetDeltaP_Current_Prev() const
{
	return (current_p - prev_p);
}

bool Mouse::IsButtonDown() const  
{
	return button_down_flag;
}

void Mouse::SetLButtonDownP(VPoint p)
{
	prev_down_click_p = down_click_p;
	down_click_p = p;
	button_down_moving_p = p;
}

void Mouse::SetCurrentP(VPoint p)
{
	prev_p = current_p;
	current_p = p;
}

void Mouse::SetLButtonUpP(VPoint p)
{
	prev_up_click_p = up_click_p;
	up_click_p = p;
}

void Mouse::SetButtonDownMovingP(VPoint p)
{
	button_down_moving_p = p;
}

void Mouse::SetButtonDownFlag(bool state)
{
	button_down_flag = state;
}

void Mouse::SetMButtonDownFlag(bool state)
{
	mbutton_down_flag = state;
}

bool Mouse::IsGoingDown() const
{
	if ( (current_p.y() - prev_p.y() ) > 0.0)
		return false;
	else
		return true;
}

bool Mouse::IsMButtonDown()const
{
	return this->mbutton_down_flag;
}
