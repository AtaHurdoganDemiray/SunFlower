// VVoxelList.h

#ifndef __VVoxelList_h__
#define __VVoxelList_h__

#include "VPoint.h"
struct VLine;

class VTransformable;

class VVoxelList
{
public:
	VVoxelList(const VPoint &center,vreal cubeLen);
	const VPoint & GetCenter()const;
	void SetCenter(const VPoint &center);
	vreal GetMaxRadius()const;
	bool AddModelIf(VTransformable *model);
	void CollectModelIf(const VLine &hitLine , std::vector<VTransformable *> &potentialHits)const;
	void CollectModelForBoxIf(const VLine &hitLine, vreal boxRadius ,std::vector<VTransformable *> &potentialHits)const;
	int GetModelCount()const;
	bool IsAlreadyInListBrudeForceCheck(VTransformable *tr)const;
private:
	VPoint m_center;
	vreal m_maxRadius;
	vreal m_cubeLen;
	vreal m_cubeRadius;
	std::list<VTransformable *> m_modelList;
	typedef std::list<VTransformable *>::iterator itr;
	typedef std::list<VTransformable *>::const_iterator citr;
public:
	// returns true if exist and removed
	bool RemoveTransformIf(VTransformable *tr);
};

#endif