// BoundingBox.h

#ifndef __BoundingBox_h__
#define __BoundingBox_h__

#include "VPoint.h"

class BoundingBox 
{
public:
	BoundingBox ( const VPoint &min , const VPoint &max );
	BoundingBox ( const BoundingBox &Foo );
	bool operator == ( const BoundingBox &comp ) const;
	bool operator != ( const BoundingBox &comp ) const;
	const BoundingBox & operator = ( const BoundingBox &Foo );
	const VPoint &GetMin()const;
	const VPoint &GetMax()const;

private:
	VPoint m_min , m_max;

};

#endif //__BoundingBox_h__
