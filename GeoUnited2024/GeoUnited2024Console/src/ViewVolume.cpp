// ViewVolume.cpp: implementation of the ViewVolume class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "ViewVolume.h"
#include "Bifstream.h"
#include "Bofstream.h"

//#ifdef _DEBUG
//#undef THIS_FILE
//static VChr THIS_FILE[]=__FILE__;
//#define new DEBUG_NEW
//#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

ViewVolume::ViewVolume()
{
	m_left = -50.0;
	m_right = 50.0;
	m_bottom = -50.0;
	m_top = 50.0;
	m_far = -50.0;
	m_near = 50.0;
	corner_x=100;
	corner_y=70;
}
ViewVolume::ViewVolume (const vreal L
						,const int x
						,const int y)
						:m_far(L),m_near(-L),m_right(L),m_left(-L),m_top(L),m_bottom(-L)
						,corner_x(x),corner_y(y)
{

}
ViewVolume::ViewVolume (const vreal near,
					const vreal far,
					const vreal bottom,
					const vreal top,
					const vreal left,
					const vreal right,
					const int x,
					const int y)
					:m_near(near),m_far(far),m_bottom(bottom),m_top(top),m_left(left)
					,m_right(right),corner_x(x),corner_y(y)
{

}
ViewVolume::~ViewVolume()
{

}

void ViewVolume::Serialize(Bifstream &ar)
{
	ar.ReadDouble (m_bottom);
	ar.ReadDouble (m_far);
	ar.ReadDouble (m_left);
	ar.ReadDouble (m_near);
	ar.ReadDouble (m_right);
	ar.ReadDouble (m_top);
	ar.ReadInt (corner_x);
	ar.ReadInt (corner_y);
}
void ViewVolume::Serialize(Bofstream &ar)const
{
	ar.WriteDouble (m_bottom);
	ar.WriteDouble (m_far);
	ar.WriteDouble (m_left);
	ar.WriteDouble (m_near);
	ar.WriteDouble (m_right);
	ar.WriteDouble (m_top);
	ar.WriteInt(corner_x);
	ar.WriteInt(corner_y);
}

void ViewVolume::SetViewVolume()
{
	glMatrixMode ( GL_PROJECTION );
	glLoadIdentity();
	glOrtho( m_left ,
			 m_right ,
			 m_bottom ,
			 m_top ,
			 m_near ,
			 m_far );

}

void ViewVolume::SetLeft( vreal newleft )
{
	m_left = newleft;
}

void ViewVolume::SetRight( vreal newright )
{
	m_right = newright;
}

void ViewVolume::SetTop( vreal newtop )
{
	m_top = newtop;
}

void ViewVolume::SetBottom( vreal newbottom )
{
	m_bottom = newbottom;
}

void ViewVolume::SetFar ( vreal newfar )
{
	m_far = newfar;
	//m_far *= 10.;
}

void ViewVolume::SetNear( vreal newnear )
{
	m_near = newnear;
	//m_near *= 10.;
}

vreal ViewVolume::ConvertOneUnitOnWindowToVolume () const
{
	return (( m_right - m_left ) / corner_x);
}

void ViewVolume::ScreenCoordToViewVolume ( vreal x , vreal y , vreal & vx
										 , vreal & vy , double rotcenterx , double rotcentery) const
{	
	vreal ym = -y + corner_y;
	
	vreal deltax , deltay;

	deltax = m_right - m_left;
	deltay = m_top - m_bottom;

	vreal dx = x * deltax / static_cast <vreal> (corner_x);
	vreal dy = ym * deltay / static_cast <vreal> (corner_y);

	vx = m_left + dx;
	vy = m_bottom + dy;
	//vx += rotcenterx;
	//vy += rotcentery;
	return;
}

vreal ViewVolume::GetAspectRatio () const
{
	return ( static_cast <vreal> (corner_x / corner_y) );
}

vreal ViewVolume::GetDiaogonalRadius() const
{
	vreal dx = m_right - m_left;
	vreal dy = m_top - m_bottom;
	vreal dz = m_far - m_near;

	vreal radius = (sqrt ( dx*dx+dy*dy+dz*dz )) / 2.;
	return radius;
}

const ViewVolume operator - (const ViewVolume &v1 , const ViewVolume &v2)
{
	ViewVolume dif (v2);
	dif.m_bottom -= v1.m_bottom;
	dif.m_far -= v1.m_far;
	dif.m_left -= v1.m_left;
	dif.m_near -= v1.m_near;
	dif.m_right -= v1.m_right;
	dif.m_top -= v1.m_top;
	return dif;
}

const ViewVolume operator + (const ViewVolume &v1 , const ViewVolume &v2)
{
	ViewVolume sum (v2);
	sum.m_bottom += v1.m_bottom;
	sum.m_far += v1.m_far;
	sum.m_left += v1.m_left;
	sum.m_near += v1.m_near;
	sum.m_right += v1.m_right;
	sum.m_top += v1.m_top;
	return sum;
}

const ViewVolume operator * (const ViewVolume &v , const double &f)
{
	ViewVolume r (v);
	r.m_bottom *=f;
	r.m_far *=f;
	r.m_left *=f;
	r.m_near *=f;
	r.m_right*=f;
	r.m_top *=f;
	return r;
}

const ViewVolume operator * (const double f , const ViewVolume &v)
{
	ViewVolume r (v);
	r.m_bottom *=f;
	r.m_far *=f;
	r.m_left *=f;
	r.m_near *=f;
	r.m_right*=f;
	r.m_top *=f;
	return r;
}
