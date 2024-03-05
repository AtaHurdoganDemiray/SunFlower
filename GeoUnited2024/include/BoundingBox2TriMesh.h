// BoundingBox2TriMesh.h

#ifndef __BoundingBox2TriMesh_h__
#define __BoundingBox2TriMesh_h__

class BoundingBox;
class TriangleMesh;

class BoundingBox2TriMesh
{
public:
	BoundingBox2TriMesh ( const BoundingBox &newbox );
	void FillTriMesh ( TriangleMesh &trimesh ) const;	
private:
	const BoundingBox &mbox;
};

#endif //__BoundingBox2TriMesh_h__
