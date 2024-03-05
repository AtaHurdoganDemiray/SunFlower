// AngleSupport.h

#ifndef __AngleSupport_h__
#define __AngleSupport_h__

#include "PointSet2d.h"
#include "PolarCalc2d.h"

class AngleSupport  
{
public:
	AngleSupport ( const PointSet2d &allpoints ) : m_allpoints(allpoints)
	{
		
	};

	vreal GetPolarAngle ( const long &origin , const long &secondpoint ) const
	{
		const VPoint2d *originpoint = m_allpoints[origin];
		const VPoint2d *otherpoint = m_allpoints[secondpoint];
		PolarCalc2d calc;
		vreal angle = calc.GetPolarAngle ( *originpoint , *otherpoint );
		return angle;
	};

private:
	const PointSet2d &m_allpoints;

};

#endif