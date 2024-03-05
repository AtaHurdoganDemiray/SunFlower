// VVoxel.cpp

#include "stdafx.h"
#include "VVoxel.h"
#include "VTransformable.h"
#include "VVoxelList.h"
#include "OperationTransform.h"

VVoxel::VVoxel()
{
	m_cubeLen = 400.0;// 100
	m_halfCubeLen = m_cubeLen*0.5;
	m_invXYZRes = 1. / m_cubeLen;
	m_maxItemPerCube = 2000;//200;// 1000000;//200
	m_maxItemCountStep = 500;
}

bool VVoxel::AddVTransformableIf(VTransformable *tr)
{
	VPoint cen(0.0);
	vreal rad(0.0);
	if (tr != 0)
	{
		VPoint mn = tr->GetMinPoint();
		VPoint mx = tr->GetMaxPoint();
		cen = (mn + mx)*0.5;
		rad = (mx - mn).GetLength() * 0.5;
	}
	else
		return false;
	
	int xi = (int)((cen.x()+m_halfCubeLen) * m_invXYZRes);
	int yi = (int)((cen.y()+m_halfCubeLen) * m_invXYZRes);
	int zi = (int)((cen.z()+m_halfCubeLen) * m_invXYZRes);
	VPoint voxelCenter(xi * m_cubeLen, yi * m_cubeLen, zi * m_cubeLen);
	itrx fix = m_xyzModelMap.find(xi);
	if (fix == m_xyzModelMap.end())
	{
		std::map<int, std::map<int, VVoxelList *>>tempYZ;
		std::map<int, VVoxelList *> tempZ;
		VVoxelList *newlist = new VVoxelList(voxelCenter,m_cubeLen);
		bool ck = newlist->AddModelIf(tr);
		if (ck)
		{
			m_allModel.push_back(tr);
			int cubeItemCount = newlist->GetModelCount();
			if (cubeItemCount > m_maxItemPerCube)
			{
				RefillVoxelListsByReducingCubeLen();
			}
		}
		tempZ.insert(std::pair<int, VVoxelList *>(zi,newlist));
		tempYZ.insert (std::pair<int, std::map<int, VVoxelList *>>(yi,tempZ));
		m_xyzModelMap.insert(std::pair<int, std::map<int, std::map<int, VVoxelList *>>>(xi, tempYZ));
		return ck;
	}
	else
	{
		itry fiy = fix->second.find(yi);
		if (fiy == fix->second.end())
		{
			std::map<int, VVoxelList *> tempZ;
			VVoxelList *newList = new VVoxelList(voxelCenter,m_cubeLen);
			bool ck = newList->AddModelIf(tr);
			tempZ.insert(std::pair<int, VVoxelList *>(zi, newList));
			fix->second.insert(std::pair<int,std::map<int,VVoxelList *>>(yi, tempZ));
			if (ck)
			{
				m_allModel.push_back(tr);
				int cubeItemCount = newList->GetModelCount();
				if (cubeItemCount > m_maxItemPerCube)
				{
					RefillVoxelListsByReducingCubeLen();
				}
			}
			return ck;
		}
		else
		{
			itrz fiz = fiy->second.find(zi);
			if (fiz == fiy->second.end())
			{
				VVoxelList *newList = new VVoxelList(voxelCenter,m_cubeLen);
				bool ck = newList->AddModelIf(tr);
				fiy->second.insert(std::pair<int, VVoxelList *>(zi, newList));
				if (ck)
				{
					m_allModel.push_back(tr);
					int cubeItemCount = newList->GetModelCount();
					if (cubeItemCount > m_maxItemPerCube)
					{
						RefillVoxelListsByReducingCubeLen();
					}
				}
				return ck;
			}
			else
			{
				VVoxelList *curList = fiz->second;
				bool ck = curList->AddModelIf(tr);
				if (ck)
				{
					m_allModel.push_back(tr);
					int cubeItemCount = curList->GetModelCount();
					if (cubeItemCount > m_maxItemPerCube)
					{
						RefillVoxelListsByReducingCubeLen();
					}
				}
				return ck;
			}
		}
	}
	return false;
}

void VVoxel::CollectVTransformable(const VLine &hitLine, std::vector<VTransformable *> &potentialHits)
{
	itrx xi = m_xyzModelMap.begin();
	itrx xe = m_xyzModelMap.end();
	for (xi; xi != xe; ++xi)
	{
		itry yi = xi->second.begin();
		itry ye = xi->second.end();
		for (yi; yi != ye; ++yi)
		{
			itrz zi = yi->second.begin();
			itrz ze = yi->second.end();
			for (zi; zi != ze; ++zi)
			{
				VVoxelList *vl = zi->second;
				vl->CollectModelIf(hitLine, potentialHits);
			}
		}
	}
}

void VVoxel::CollectVTransformableForBox(const VLine &hitLine, vreal boxRadius ,std::vector<VTransformable *> &potentialHits)
{
	itrx xi = m_xyzModelMap.begin();
	itrx xe = m_xyzModelMap.end();
	for (xi; xi != xe; ++xi)
	{
		itry yi = xi->second.begin();
		itry ye = xi->second.end();
		for (yi; yi != ye; ++yi)
		{
			itrz zi = yi->second.begin();
			itrz ze = yi->second.end();
			for (zi; zi != ze; ++zi)
			{
				VVoxelList *vl = zi->second;
				vl->CollectModelForBoxIf(hitLine, boxRadius , potentialHits);
			}
		}
	}
}

void VVoxel::RefillVoxelListsByReducingCubeLen()
{
	m_cubeLen *= 0.5;
	m_halfCubeLen = m_cubeLen * 0.5;
	m_invXYZRes = 1. / m_cubeLen;
	m_maxItemPerCube += m_maxItemCountStep;
	m_xyzModelMap.clear();
	std::list<VTransformable *> allModel = m_allModel;
	m_allModel.clear();
	std::list<VTransformable *>::iterator li = allModel.begin();
	std::list<VTransformable *>::iterator le = allModel.end(); 
	for (li; li != le; ++li)
	{
		AddVTransformableIf(*li);
	}
}

void VVoxel::UpdateTransform(OperationTransform *optr)
{
	int elsize = (int)optr->m_selitems.size();
	for (int eli = 0; eli < elsize; ++eli)
	{
		VTransformable *tr = optr->m_selitems.at(eli);
		itrx xi = m_xyzModelMap.begin();
		itrx xe = m_xyzModelMap.end();
		bool ck = false;
		for (xi; xi != xe; ++xi)
		{
			if (ck)
				break;
			itry yi = xi->second.begin();
			itry ye = xi->second.end();
			for (yi; yi != ye; ++yi)
			{
				if (ck)
					break;
				itrz zi = yi->second.begin();
				itrz ze = yi->second.end();
				for (zi; zi != ze; ++zi)
				{
					VVoxelList *vl = zi->second;
					ck = vl->RemoveTransformIf(tr);
					if (ck)
						break;
				}
			}
		}
		this->AddVTransformableIf(tr);
	}
}

void VVoxel::RemoveTr(std::vector<VTransformable*>& trVec)
{
	int elsize = (int)trVec.size();
	for (int eli = 0; eli < elsize; ++eli)
	{
		VTransformable* tr = trVec.at(eli);
		itrx xi = m_xyzModelMap.begin();
		itrx xe = m_xyzModelMap.end();
		bool ck = false;
		for (xi; xi != xe; ++xi)
		{
			if (ck)
				break;
			itry yi = xi->second.begin();
			itry ye = xi->second.end();
			for (yi; yi != ye; ++yi)
			{
				if (ck)
					break;
				itrz zi = yi->second.begin();
				itrz ze = yi->second.end();
				for (zi; zi != ze; ++zi)
				{
					VVoxelList* vl = zi->second;
					ck = vl->RemoveTransformIf(tr);
					if (ck)
						break;
				}
			}
		}
	}
}
