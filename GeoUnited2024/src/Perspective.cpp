/******************************************************************************
*               File: Perspective.cpp											  *
*******************************************************************************
*               Description:                                                  *
*                                                                             *
*******************************************************************************
*               History:                                                      *
*  23.11.2003 05:39:53 Created by:    Ata Hurdogan Demiray                    *
*******************************************************************************
*               (C) 2003 by Pozitif GmbH	                                 *
******************************************************************************/

#include "stdafx.h"
#include "Perspective.h"
//#include "glu.h"

Perspective::Perspective()
{
	m_aspect = 1.0;
	m_far = 10000.0;
	m_near = 0.1;
	m_fovy = 45.0*3.14/180.0;
	//m_fovy = 45.0;
}

//#############################################################################

Perspective::~Perspective()
{

}

//#############################################################################

vreal Perspective::GetFovy()
{
	return m_fovy;
}

//#############################################################################

vreal Perspective::GetAspect()
{
	return m_fovy;
}

//#############################################################################

vreal Perspective::GetNear()
{
	return m_near;
}

//#############################################################################

vreal Perspective::GetFar()
{
	return m_far;
}

//#############################################################################

void Perspective::SetFovy ( vreal newval )
{
	m_fovy = newval;
}

//#############################################################################

void Perspective::SetAspect ( vreal newval )
{
	m_aspect = newval;
}

//#############################################################################

void Perspective::SetNear ( vreal newval )
{
	m_near = newval;
}

//#############################################################################

void Perspective::SetFar ( vreal newval )
{
	m_far = newval;
}

//#############################################################################

void Perspective::SetPerspective ()
{
	glMatrixMode ( GL_PROJECTION );
	glLoadIdentity ();
	//gluPerspective ( m_fovy , m_aspect , m_near , m_far );
	vreal thetop = m_near * tan ( m_fovy / 2.0 );
	vreal thebottom = -thetop;
	vreal theright = thetop * m_aspect;
	vreal theleft = -theright;
	
	glFrustum ( theleft , theright , thebottom , thetop , m_near , m_far );
	
	glMatrixMode ( GL_MODELVIEW );

}

//#############################################################################

