// OglRefMesh.h

#ifndef __OglRefMesh_h__
#define __OglRefMesh_h__

class VRefTriangleMesh;

class OglRefMesh
{
public:
	OglRefMesh();
	~OglRefMesh();
	
	void FormMesh ( const VRefTriangleMesh & tri );
	bool IsReversed ( int index , const VRefTriangleMesh & trimesh );

	int * references;
	GLfloat * normals , * modelpoints;
	
	int refarraysize , vertexarraysize;

};

#endif