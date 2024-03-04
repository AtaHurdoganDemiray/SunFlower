// VTransformable.h

#ifndef __VTransformable_h__
#define __VTransformable_h__

struct TransformData;
class TransformationMatrix;
//class IgesEntitySet;
struct VPoint;
struct CoordinateSystem;
class Model;
#include "TransformationMatrix.h"
class OperationTransformList;

class VTransformable
{
public:
	static OperationTransformList *m_transformManager;
	VTransformable();
	virtual ~VTransformable ();
	//void AddTransformation (TransformData *ptm);
	//TransformationMatrix GetOverallMatrix ()const;
	//TransformationMatrix GetOverallInverse()const;
	//std::vector <TransformData *> &GetVector();
	//void DoLastTransform (IgesEntitySet &entset);
	//virtual void DoTransform (const TransformationMatrix &tm,IgesEntitySet &entset) = 0;
	//void ClearTransform(IgesEntitySet &entset);
	//void UpdateTransform(IgesEntitySet &entset);
	//void SetInitialTransformationMatrix (const TransformationMatrix &tm);
	virtual void OnTransform (const TransformationMatrix &tm) = 0;
	virtual const VPoint GetMinPoint()const =0;
	virtual const VPoint GetMaxPoint()const =0;
	virtual const VPoint GetMinPointInCs (const CoordinateSystem &cs)const =0;
	virtual const VPoint GetMaxPointInCs (const CoordinateSystem &cs)const =0;
	virtual Model * GetTrModel () = 0;
	const int GetMatModRef()const;
	void SetMatModRef(const int matModRef);
	static double MAX_COORD_LIMIT;
	int GetIgesX ()const;
	void SetIgesX (int igesX);
	static void GetMinMax (const std::vector<VTransformable *> &trVec,VPoint &mn , VPoint &mx);
	static void GetMinMaxInCs (const CoordinateSystem &cs , const std::vector<VTransformable *> &trVec,VPoint &mn , VPoint &mx);

protected:
	//std::vector <TransformData *> m_transformationHistory;
	//TransformationMatrix *m_initialTransform;
	int m_matModRef;
	int m_igesX;
	TransformationMatrix m_tm;
public:
	const TransformationMatrix & GetTm()const;
	void SetTm (const TransformationMatrix &tm);
	virtual TransformationMatrix GetBaseTm()const;
	TransformationMatrix GetDynamicTm()const;
	TransformationMatrix GetOverallTm ()const;
	virtual TransformationMatrix GetTmForIgesExport ()const;
	virtual void UpdateMinMaxPointAlongCsX (const CoordinateSystem &cs , VPoint &mnx, VPoint &mxx)const;
	static void UpdateMinMaxPointAlongCsX (const CoordinateSystem &cs , std::vector<VTransformable *> &trVec , VPoint &mnx , VPoint &mxx);
};

#endif