// ViewingState.h

#ifndef __ViewingState_h__
#define __ViewingState_h__

class ViewingState
{
public:
	ViewingState();
	enum State { dynamicrotate , pan , zoomtobox , zoompan , perspectivenavigate , dynamicroll , none }; 
	State GetState () const;
	void SetState ( State newstate );
	bool IsZoomPan () const;
	bool IsZoomToBox () const;
	bool IsDynamicRotate () const;
	bool IsPan () const;
	bool IsPerspectiveNavigate () const;
	bool IsDynamicRoll () const;
	bool IsClipped() const;
	bool DrawClipPlane() const;
	void SetClipped ( bool isclip );
	void SetDrawClipPlane ( bool drawplane );
private:
	State state;
	bool isclipped;
	bool drawclipplane;
	int m_colorMode;// 0 variable , 1 standard
public:
	void SetColorMode (int colorMode);
	int GetColorMode ()const;
private:
	bool m_isMouseInformerActive;
public:
	bool GetIsMouseInformerActive()const;
	void SetIsMouseInformerActive(bool isMouseInformerActive);
};

#endif