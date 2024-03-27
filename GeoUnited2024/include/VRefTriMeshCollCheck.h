// VRefTriMeshCollCheck.h

#ifndef __VRefTriMeshCollCheck_h__
#define __VRefTriMeshCollCheck_h__

#include "vTool.h"
#include "CoordinateSystem.h"


class VRefTriangleMesh;

class VRefTriMeshCollCheck
{
public:
	VRefTriMeshCollCheck (const VRefTriangleMesh &toCheck);
	bool IsColl (const vTool<float> &tool);
	void CalcMinVolBoxForMesh (CoordinateSystem &boxCenter
								,vreal &boxx
								,vreal &boxy
								,vreal &boxz);
	bool InNonColl (const ToolPrim<float> &toolprim);
private:
	const VRefTriangleMesh &m_toCheck;
	CoordinateSystem m_boxCenter;
	vreal m_boxx,m_boxy,m_boxz;
	
};

#endif