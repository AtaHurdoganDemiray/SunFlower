// OglServ.h

#ifndef __OglServ_h__
#define __OglServ_h__


struct VPoint;

class OglServ
{
public:
	bool OglServ::GetScreenCoordForPoint ( const VPoint & mypoint , VPoint &screenpoint ) const;
};

#endif