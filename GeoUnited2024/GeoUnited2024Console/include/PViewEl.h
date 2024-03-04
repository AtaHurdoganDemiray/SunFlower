// PViewEl.h

#ifndef __PViewEl_h__
#define __PViewEl_h__

#include "VTransformable.h"
#include "TransformationMatrix.h"
#include "VPoint.h"
#include "Model.h"

class PViewEl : public VTransformable
{
public:
	PViewEl();
	PViewEl(Model *source , TransformationMatrix &tm , const VPoint &mn , const VPoint &mx , const VStr &name);
	void UpdateOpenBox  (std::vector<const Display *> linesDispVec, std::vector<OglMatrix> &oglTmVec , const VPoint &mn , const VPoint &mx , const VStr &name);

	// virtuals of VTransformable
	virtual void OnTransform(const TransformationMatrix &tm);
	virtual const VPoint GetMinPoint()const;
	virtual const VPoint GetMaxPoint()const;
	virtual const VPoint GetMinPointInCs(const CoordinateSystem &cs)const;
	virtual const VPoint GetMaxPointInCs(const CoordinateSystem &cs)const;
	virtual Model * GetTrModel();
	Model m_model;
	VPoint m_min;
	VPoint m_max;
	TransformationMatrix m_tm;
	Model *m_source;
	void Delete();
};

#endif