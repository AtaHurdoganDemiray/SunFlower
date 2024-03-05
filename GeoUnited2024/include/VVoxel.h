// VVoxel.h

#ifndef __VVoxel_h__
#define __VVoxel_h__

class VVoxelList;
class VTransformable;
class OperationTransform;

class VVoxel
{
public:
	VVoxel();
	bool AddVTransformableIf(VTransformable *tr);
	void CollectVTransformable(const VLine &hitLine, std::vector<VTransformable *> &potentialHits);
	void CollectVTransformableForBox(const VLine &hitLine, vreal boxRadius , std::vector<VTransformable *> &potentialHits);
private:
	std::map<int, std::map<int, std::map<int, VVoxelList *>>> m_xyzModelMap;
	std::list<VTransformable *> m_allModel;
	typedef std::map<int, std::map<int, std::map<int, VVoxelList *>>>::iterator itrx;
	typedef std::map<int, std::map<int, VVoxelList *>>::iterator itry;
	typedef std::map<int, VVoxelList *>::iterator itrz;
	typedef std::map<int, std::map<int, std::map<int, VVoxelList *>>>::const_iterator citrx;
	typedef std::map<int, std::map<int, VVoxelList *>>::const_iterator citry;
	typedef std::map<int, VVoxelList *>::const_iterator citrz;
	vreal m_invXYZRes;
	vreal m_cubeLen;
	vreal m_halfCubeLen;
	int m_maxItemPerCube;
	int m_maxItemCountStep;
	void RefillVoxelListsByReducingCubeLen();
public:
	void UpdateTransform(OperationTransform *optr);
	void RemoveTr(std::vector<VTransformable*>& trVec);

};

#endif