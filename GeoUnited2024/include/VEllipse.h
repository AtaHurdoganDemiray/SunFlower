// VEllipse.h
#ifndef __VEllipse_h__
#define __VEllipse_h__
#include "CoordinateSystem.h"
class TransformationMatrix;
class ConicArc;
class igesTransformationMatrix;
// Ellipse: x^2/r^2 + y^2/b^2 = 1

class VEllipse
{
public:
	VEllipse();
	VEllipse(const vreal &xRadius
			, const vreal &yRadius 
			, const CoordinateSystem &cs 
			, const vreal &sweepAngle1,const vreal &sweepAngle2); 
	VEllipse (const VEllipse &toCopy);
	VEllipse (const vreal &xRadius
				,const vreal &yRadius);
	VEllipse (const vreal &xRadius 
			, const vreal &yRadius
			, const vreal &sweepAngle1
			, const vreal &sweepAngle2);
	VEllipse & operator = (const VEllipse &toCopy);
	~VEllipse();
	const vreal & GetXRadius()const;
	const vreal & GetYRadius()const;
	const vreal & GetSweepAngle1()const;
	const vreal & GetSweepAngle2()const;
	const CoordinateSystem &GetCoordinateSystem()const;
	void Serialize (Bifstream &infile);
	void Serialize (Bofstream &outfile)const;
	void Transform (const TransformationMatrix &tm);
	bool operator == (const VEllipse &toCompare)const;
	bool operator != (const VEllipse &toCompare)const;
	bool IsFull()const;
	void GetIgesEllipse (ConicArc &conicarc , igesTransformationMatrix &tm , bool &isTransformed)const;
private:
	vreal m_xRadius;
	vreal m_yRadius;
	CoordinateSystem m_cs;
	vreal m_sweepAngle1;
	vreal m_sweepAngle2;
};

#endif
