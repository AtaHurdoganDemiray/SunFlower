// ThinTransformShell.h

#ifndef __ThinTransformShell_h__
#define __ThinTransformShell_h__

#include "VTransformable.h"
#include "VPoint.h"

class Model;

class ThinTransformShell : public VTransformable
{
public:
	ThinTransformShell (const VPoint &mn , const VPoint &mx ,Model *md); 
	virtual void OnTransform (const TransformationMatrix &tm);
	virtual const VPoint GetMinPoint()const;
	virtual const VPoint GetMaxPoint()const;
	virtual const VPoint GetMinPointInCs (const CoordinateSystem &cs)const;
	virtual const VPoint GetMaxPointInCs (const CoordinateSystem &cs)const;
	virtual Model * GetTrModel ();	
	void GetMinMaxInCs (const CoordinateSystem &cs , VPoint &csMin , VPoint &csMax)const;
private:
	VPoint m_min,m_max;
	Model * m_model;
};

#endif