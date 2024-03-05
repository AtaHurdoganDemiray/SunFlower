// Mouse.h: interface for the Mouse class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_MOUSE_H__3362E957_93D5_4219_B5AC_DF3CB5F62D62__INCLUDED_)
#define AFX_MOUSE_H__3362E957_93D5_4219_B5AC_DF3CB5F62D62__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "VPoint.h"

class Mouse  
{
private:	
	VPoint prev_down_click_p;
	VPoint prev_up_click_p;
	VPoint down_click_p;
	VPoint prev_p;
	VPoint current_p;
	VPoint up_click_p;
	VPoint button_down_moving_p;
	bool button_down_flag;
	bool mbutton_down_flag;

	vreal prev_move_direction;

public:

	const VPoint & GetPrevLButtonDownP()const;
	const VPoint & GetPrevLButtonUpP()const;
	const VPoint & GetLButtonDownP()const;
	const VPoint & GetPrevP() const;
	const VPoint & GetCurrentP()const;
	const VPoint & GetLButtonUpP()const;
	VPoint GetDeltaP_Current_Prev()const;
	const VPoint & GetButtonDownMovingP()const;
	bool IsButtonDown()const;
	bool IsMButtonDown()const;
	void SetLButtonDownP(VPoint p);
	void SetCurrentP(VPoint p);
	void SetLButtonUpP(VPoint p);
	void SetButtonDownMovingP(VPoint p);
	void SetButtonDownFlag(bool state);
	void SetMButtonDownFlag(bool state);
	bool IsGoingDown()const;
	Mouse();
	virtual ~Mouse();

};

#endif // !defined(AFX_MOUSE_H__3362E957_93D5_4219_B5AC_DF3CB5F62D62__INCLUDED_)
