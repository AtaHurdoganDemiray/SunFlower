// OperationTransformList.h

#ifndef __OperationTransformList_h__
#define __OperationTransformList_h__
#include "OperationTransform.h"

class ModelList;
//class CMyTreeCtrl;
class IgesEntitySet;
class VpEntitySet;
class ViewPointItem;
class VTransformable;
class VVoxel;

class OperationTransformList
{
public:
	struct Mat_Mod
	{
	public:
		Mat_Mod();
		TransformData m_tm;
		std::vector<VTransformable *> m_mvec;
		//std::vector<int>m_opvec;
		//void AddOperationIf (const int opRef);
		static void RemoveItem (std::vector<VTransformable *> &vec , VTransformable *itemToRemove);
		bool m_isOpen;
	};
	OperationTransformList ();
	~OperationTransformList();
	void SetModelList (ModelList *modellist);
	void SetVoxel(VVoxel *voxel);
	typedef std::vector<OperationTransform *>::iterator Itr;	
	OperationTransform & MakeOperationTransform (const TransformationMatrix &tm , std::vector<VTransformable *> &hs , bool addToTreeView = true);
	int MakeDynamicTransform (const std::vector<VTransformable *> &hs);
	//void GroupSameMatMod (const std::vector<VTransformable *> &hs,std::vector<std::vector<VTransformable *>> &groupedVec);
	//int DivideMatMod_ToMove_NotToMove (std::vector<VTransformable *> &hs,Mat_Mod &tomove);

	void ReverseOperation (ViewPointItem *clicked);
	//void SetTreeCtrl (CMyTreeCtrl *treecontrol);
	OperationTransform & GetLastOperation();
	void AddUntransformed (VTransformable *tr);
	const std::vector<VTransformable *> & GetUntransformed()const;
	const std::vector<Mat_Mod *> & GetAllMat()const;
	std::vector <Mat_Mod *> & GetAllMat();
	//void FinishDynamicTransform ();
	//const bool IsDynamicTransformActive()const;
	OperationTransform * FinishDynamicTransform (int matModNo);//(OperationTransform &op);
	void IncrementDynamicTransformation (int matModNo , const TransformationMatrix &tm);
	// doesnot increment but sets the matrix to tm
	void ChangeDynamicTransformation (int matModNo ,const TransformationMatrix &tm);
	//void UpdateAllHs ();
	void CancelDynamicTransformation (int matmodi);//(std::vector<VTransformable *> &toMove);
private:
	ModelList *m_modellist;
	void UpdateMatModForOperation (const int otRef);
	//CMyTreeCtrl *m_treecontrol;
	//OperationTransform * GetTransform (const ModelID &id);
	void ReverseOperation (const int operationNo);
	std::vector<OperationTransform *> m_allop;
	std::vector<Mat_Mod *> m_allmat;
	std::vector<VTransformable *> m_untransformed;
	int MakeMatMod (const std::vector<VTransformable *> &mvec);//,const std::vector<int> &opRefVec);
	//int GetMatModRef(const TransformationMatrix &tm)const;
	//void GetRelatedOperationTransform (const OperationTransform &t , std::vector<int> &relVec);
	//void GetRelatedMatMod (const std::vector<int> &otRefVec,std::vector<int> &matmodRefVec);
	//void UndoMatMod (const int matmodref);
	void UpdateMatModRefOfModel (const std::vector<VTransformable *> &hs,const int matmodRef);
	//int m_latestMatModRef;
	//void UpdateSingleHs (HandleSet &hs);
	void NewTransform (const TransformationMatrix &tm , std::vector<VTransformable *> &trVec);
	int m_activeDynamicTransformation;
public:
	bool IsThereDynamicTransformation()const;
	void SuspendDynamicTransformation (int matmodi);
private:
	bool RemoveAnyModelTransformedDynamic (std::vector<VTransformable *> &trVec)const;//returns true if any VTransformable is used any existing Mat_Mod
	VVoxel * m_voxel;
public:
	void HideShowOperationTransform(int matmodi, bool isVisible);
	void AddModelToDynamicTransformation(int matmodi , std::vector<VTransformable*>& newModel);
	void AddModelToDynamicTransformationIfNotExisting(int matmodi, std::vector<VTransformable*>& newModel);
	const TransformationMatrix& GetTransformationOf(int matmodi)const;
};

#endif
