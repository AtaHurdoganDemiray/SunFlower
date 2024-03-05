// VolumeBox.cpp

#include "stdafx.h"
#include "VolumeBox.h"


VolumeBox::VolumeBox()
{

}

VolumeBox::VolumeBox (const CoordinateSystem &basec
					,const vreal &myx
					,const vreal &myy
					,const vreal &myz)
:m_basec(basec),m_x(myx),m_y(myy),m_z(myz)
{
	// basec origin is at lower left corner of box
	//basec xDir is along box width
	//basec yDir is along box length
	//basec zDir is along box height
}

VolumeBox::VolumeBox (const VolumeBox &toCopy)
:m_basec(toCopy.m_basec),m_x(toCopy.m_x),m_y(toCopy.m_y),m_z(toCopy.m_z)
{
	
}

VolumeBox & VolumeBox::operator = (const VolumeBox &toCopy)
{
	m_basec=toCopy.m_basec;
	m_x=toCopy.m_x;
	m_y=toCopy.m_y;
	m_z=toCopy.m_z;
	return (*this);
}

bool VolumeBox::IsInside (const VPoint &p)const
{
	VPoint delta1=p-m_basec.GetOrigin();
	VPoint upperright=m_basec.GetOrigin()
						+m_x*m_basec.GetDirx()
						+m_y*m_basec.GetDiry()
						+m_z*m_basec.GetDirz();
	VPoint delta2=p-upperright;

	//is between x normaled box planes
	vreal dp1=delta1%m_basec.GetDirx();
	vreal dp2=delta2%m_basec.GetDirx();
	if(dp1<0.||dp2>0.)
		return false;
	//is between y normaled box planes
	dp1=delta1%m_basec.GetDiry();
	dp2=delta2%m_basec.GetDiry();
	if(dp1<0.||dp2>0.)
		return false;
	//is between z normaled box planes
	dp1=delta1%m_basec.GetDirz();
	dp2=delta2%m_basec.GetDirz();
	if(dp1<0.||dp2>0.)
		return false;

	//else point is between all plane couples of box
	return true;
}
