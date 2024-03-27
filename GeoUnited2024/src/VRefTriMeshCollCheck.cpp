// VRefTriMeshCollCheck.cpp

#include "stdafx.h"
#include "VRefTriMeshCollCheck.h"
#include "VRefTriangleMesh.h"
#include "LineCast.h"
#include "ToolLine.h"

VRefTriMeshCollCheck::VRefTriMeshCollCheck(const VRefTriangleMesh &toCheck)
:m_toCheck(toCheck)
{

}

bool VRefTriMeshCollCheck::IsColl(const vTool<float> &tool)
{
	VPoint Po,v,tipc;
	LineCast<vreal> lc (Po.GetArray(),v.GetArray(),tipc.GetArray());
	
	vreal lp[2]={5.,0.};
	vreal up[2]={8.,7.};
	ToolLine <vreal> cone (lp,up);	
	cone.IsColl (lc);
	ToolVerticalLine<vreal> cylinder (lp,20.);
	cylinder.IsColl (lc);
	return false;
}

bool VRefTriMeshCollCheck::InNonColl(const ToolPrim<float> &toolprim) 
{
	return true;
}

void VRefTriMeshCollCheck::CalcMinVolBoxForMesh(CoordinateSystem &boxCenter
												, vreal &boxx
												, vreal &boxy
												, vreal &boxz)
{

}
