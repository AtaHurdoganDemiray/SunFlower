// VArc2d.h

#ifndef __VArc2d_h__
#define __VArc2d_h__

#include "VPoint2d.h"

class VArc2d
{
public:
	VArc2d();
	VArc2d ( const VPoint2d & _center , const vreal & _radius , const vreal & _startangle , const vreal & _endangle );
	VArc2d ( const VPoint2d & _center , double _radius , const VPoint2d & p0 , const VPoint2d &p1 , bool isCCW);
	// angle definitions are assumed to be positive and CCW with respect to X axis (arc axis is UZ)
	// 
	VArc2d ( const VArc2d & Foo );
	VArc2d & operator = ( const VArc2d & Foo );
	bool operator == ( const VArc2d & comp ) const;
	bool operator != ( const VArc2d & comp ) const;
	const vreal & GetRadius () const;
	const vreal & GetStartAngle () const;
	const vreal & GetEndAngle () const;
	const VPoint2d & GetCenter () const;
	bool IsFull()const;
	static bool Calculate (const vreal x1 
							,const vreal y1
							,const vreal x2
							,const vreal y2
							,const vreal x3
							,const vreal y3
							, vreal &solutionx
							, vreal &solutiony);
	bool IsAngleInArc (double angle)const;
	VPoint2d GetPoint (vreal angle)const;
	VPoint2d GetStartPoint ()const;
	VPoint2d GetEndPoint ()const;
protected:
	vreal radius;
	VPoint2d center;
	vreal startangle,endangle;

};


#endif