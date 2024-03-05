// BspLine2d.h

#ifndef __BspLine2d_h__
#define __BspLine2d_h__

#include "VLine2d.h"

namespace geo
{
	struct BspLine2d
	{
	public:
		BspLine2d();
		VLine2d m_line;
		VPoint2d m_normal;
	};
};
#endif