// VolumeBox.h

#ifndef __VolumeBox_h__
#define __VolumeBox_h__

#include "CoordinateSystem.h"

class VolumeBox
{
public:
	VolumeBox();
	VolumeBox (const CoordinateSystem &basec
				,const vreal &myx
				,const vreal &myy
				,const vreal &myz);
	VolumeBox (const VolumeBox &toCopy);
	VolumeBox &operator=(const VolumeBox &toCopy);
	bool IsInside (const VPoint &p)const;

//private:
	CoordinateSystem m_basec;
	vreal m_x,m_y,m_z;
};

#endif
