// ScaleMatrix.h

#ifndef __ScaleMatrix_h__
#define __ScaleMatrix_h__
#include "TransformationMatrix.h"
#include "CoordinateSystem.h"

class ScaleMatrix : public TransformationMatrix
{
public:
	ScaleMatrix ();
	ScaleMatrix (const vreal s);
	ScaleMatrix (const vreal sx , const vreal sy , const vreal sz);
	ScaleMatrix (const ScaleMatrix &tocopy);
	ScaleMatrix & operator = (const ScaleMatrix &tocopy);
	virtual ~ScaleMatrix ();
	virtual void Serialize (Bofstream &f)const;
	virtual void Serialize (Bifstream &f);

private:
	vreal m_sx,m_sy,m_sz;
public:
	ScaleMatrix (const CoordinateSystem &cs , vreal sx , vreal sy , vreal sz);
private:
	CoordinateSystem m_cs;// scale is done in this work relative to this cs axis x-y-z
public:
	const CoordinateSystem & GetCs()const;
};

#endif