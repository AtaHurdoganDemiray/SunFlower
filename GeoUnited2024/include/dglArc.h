// dglArc.h

#ifndef __dglArc_h__
#define __dglArc_h__

#include "VArc.h"
#include "SequentialLineSet.h"
#include "LineVisualAtt.h"
struct CoordinateSystem;

class dglArc
{
public:
	dglArc ();
	dglArc (const VArc &a,double deltaAngle);
	const VArc & GetArc ()const;
	void SetArc (const VArc &a);
	void Draw ()const;
	void SetDeltaAngle (double deltaAngle);
	double GetDeltaAngle ()const;
	const LineVisualAtt &GetVisAtt()const;
	void SetVisAtt (const LineVisualAtt &visatt);
	void HideShow (bool isHide);
	void Transform (const TransformationMatrix &tm);
	const VPoint GetMinPoint()const;
	const VPoint GetMaxPoint()const;
	const VPoint GetMinPointInCs(const CoordinateSystem &cs)const;
	const VPoint GetMaxPointInCs(const CoordinateSystem &cs)const;
private:
	VArc m_a;
	SequentialLineSet m_sl;
	void Digitise ();
	double m_da;
	LineVisualAtt m_visAtt;
public:
	void Reverse();
};

#endif