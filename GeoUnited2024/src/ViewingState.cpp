// ViewingState.cpp

#include "stdafx.h"
#include "ViewingState.h"

ViewingState::ViewingState ()
{
	state = dynamicrotate;
	isclipped = false;
	drawclipplane = false;// true;
	m_colorMode = 0;
	m_isMouseInformerActive = false;
}

void ViewingState::SetState ( State newstate )
{
	state = newstate;
}

ViewingState::State ViewingState::GetState () const
{
	return state;
}

bool ViewingState::IsDynamicRotate () const
{
	if ( state == dynamicrotate )
		return true;
	else
		return false;
}

bool ViewingState::IsPan () const
{
	if ( state == pan )
		return true;
	else
		return false;
}

bool ViewingState::IsPerspectiveNavigate () const
{
	if ( state == perspectivenavigate )
		return true;
	else
		return false;
}

bool ViewingState::IsZoomPan () const
{
	if ( state == zoompan )
		return  true;
	else
		return false;
}

bool ViewingState::IsZoomToBox () const
{
	if ( state == zoomtobox )
		return true;
	else
		return false;
}

bool ViewingState::IsDynamicRoll () const
{
	if ( state == dynamicroll )
		return true;
	else
		return false;
}

bool ViewingState::IsClipped () const
{
	return isclipped;
}

void ViewingState::SetClipped ( bool isclip )
{
	isclipped = isclip;
}

bool ViewingState::DrawClipPlane () const
{
	return drawclipplane;
}

void ViewingState::SetDrawClipPlane ( bool drawplane )
{
	drawclipplane = drawplane;
}

void ViewingState::SetColorMode (int colorMode)
{
	m_colorMode = colorMode;
}
		
int ViewingState::GetColorMode ()const
{
	return m_colorMode;
}

bool ViewingState::GetIsMouseInformerActive()const
{
	return m_isMouseInformerActive;
}

void ViewingState::SetIsMouseInformerActive(bool isMouseInformerActive)
{
	m_isMouseInformerActive = isMouseInformerActive;
}