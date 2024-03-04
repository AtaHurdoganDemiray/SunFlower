// OperationTransform.h

#ifndef __OperationTransform_h__
#define __OperationTransform_h__

#include "Model.h"
#include "TransformData.h"
//#include "HandleSet.h"

class Project;
class IgesEntitySet;
class VpEntitySet;
class CommandToViewPoint;
class VTransformable;
class ModelList;
class OperationTransformList;

class OperationTransform:public Model 
{
public:
	OperationTransform ();
	OperationTransform  ( const TransformationMatrix &tm
						, const std::vector<VTransformable *> & sel_items 
						, VStr type , VStr name); 
	virtual void OnTreeViewClicked();
	virtual void OnTreeViewDoubleClicked();
	virtual void OnTreeViewRightClicked(const VStr &commandcap);
	virtual const VStr GetModelTypeEng()const;
	//void TransformSelectedModel(Project &prj,CommandToViewPoint &ap);
	//void VisualTransform (Project &prj);
	//void SurfaceTransform (CommandToViewPoint &ap);
	//void CurveTransform (CommandToViewPoint &ap);
	//void PointTransform (CommandToViewPoint &ap);
	//void Clear (Project &prj,CommandToViewPoint &ap);
	//void Update (Project &prj,CommandToViewPoint &ap);
	//void ClearCurveTransform (CommandToViewPoint &ap);
	//void ClearSurfaceTransform (CommandToViewPoint &ap);
	//void ClearPointTransform (CommandToViewPoint &ap);
	//void UpdateCurve (CommandToViewPoint &ap);
	//void UpdateSurface (CommandToViewPoint &ap);
	//void UpdatePoint (CommandToViewPoint &ap);
	//void DoLastTransform (CommandToViewPoint &ap);
	//void DoLastTransformSurf (CommandToViewPoint &ap);
	//void DoLastTransformCurv (CommandToViewPoint &ap);
	//void DoLastTransformPoint (CommandToViewPoint &ap);
	std::vector<VTransformable *> m_selitems;
	TransformData m_data;
	//bool m_active;
	static bool IsExist (std::vector<VTransformable *> &vec , VTransformable *searchitem);
	friend class ContainerSheet;
	friend class VpAddTypeToVp;
private:
	static void Serialize (Bofstream &f);
	static void Serialize (Bifstream &f);
	static OperationTransform * MakeOperationTransform (ModelList &mlist,OperationTransformList &trlist);
	static HandleSet s_hs;
	static TransformationMatrix s_tm;
};

#endif