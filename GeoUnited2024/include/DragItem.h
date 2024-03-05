// DragItem.h

#ifndef __DragItem_h__
#define __DragItem_h__

class Mouse;

class DragItem
{
public:
	DragItem ()
	:m_isOnRun (false)
	{
	}
	virtual bool IsHit (Mouse &mouse)const = 0;
	virtual void Run (Mouse &mouse) = 0;
	bool IsOnRun ()const
	{
		return m_isOnRun;
	}
	void SetIsOnRun (bool newval)
	{
		m_isOnRun = newval;
	}

protected:
	bool m_isOnRun;
};

#endif