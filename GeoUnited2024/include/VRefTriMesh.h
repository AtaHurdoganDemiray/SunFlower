// VRefTriMesh.h

#ifndef __VRefTriMesh_h__
#define __VRefTriMesh_h__
#include "PointSet.h"
#include "RefTriangleMesh.h"
class VRefTriMesh
{
public:
	VRefTriMesh()
	:m_reftris(100),m_pointpool(100)
	{
	}
	VRefTriMesh(int initialsize)
	:m_reftris(initialsize),m_pointpool(initialsize)
	{
		
	};

	PointSet m_pointpool;
	RefTriangleMesh m_reftris;
};

#endif
