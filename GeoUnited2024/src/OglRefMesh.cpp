// OglRefMesh.cpp

#include "stdafx.h"
#include "OglRefMesh.h"
#include "VRefTriangleMesh.h"
#include "RefTriangle.h"


OglRefMesh::OglRefMesh()
{
	refarraysize = 0;
	vertexarraysize = 0;
	modelpoints = 0;
	normals = 0;
	references = 0;
}

OglRefMesh::~OglRefMesh()
{
	delete [] references;
	delete [] modelpoints;
	delete [] normals;
}

void OglRefMesh::FormMesh ( const VRefTriangleMesh & tri )
{
	int mysize = tri.Size();
	//int mysize3 = (int)tri.m_quad.size();
	//std::vector<bool> isquadtri (mysize,false);
	//for (int i=0;i<mysize3;++i)
	//{
	//	isquadtri[tri.m_quad[i].t[0]] = true;
	//	isquadtri[tri.m_quad[i].t[1]] = true;
	//}

	refarraysize = mysize * 3;
	references = new int [ refarraysize ];

	int i;
	int counter(0);
	for ( long i= 0; i < mysize; ++i )
	{
		//if (isquadtri[i]==true)
		//	continue;
		const RefTriangle * currreftri = tri.rdata[i];

		int istart = counter * 3;
		
		if ( !IsReversed ( i , tri ) )
		{
			references [istart] = currreftri->ref0;
			references [istart+1] = currreftri->ref1;
			references [istart+2] = currreftri->ref2;
		}
		else
		{
			references [istart] = currreftri->ref2;
			references [istart+1] = currreftri->ref1;
			references [istart+2] = currreftri->ref0;
		}
		counter++;
	}
	//refarraysize = counter*3;
	vertexarraysize = tri.surfpack.modelpoints.Size() * 3;
	modelpoints = new GLfloat [vertexarraysize];
	normals = new GLfloat [vertexarraysize];
	int mysize2 = tri.surfpack.modelpoints.Size();
	for ( i = 0; i < mysize2 ; ++i )
	{
		const VPoint * currp = tri.surfpack.modelpoints[i];
		const VPoint * currn = tri.surfpack.normals[i];
	
		int istart = i * 3;

		modelpoints[istart] = static_cast<GLfloat> (currp->x());
		modelpoints[istart+1] = static_cast<GLfloat> (currp->y());
		modelpoints[istart+2] = static_cast<GLfloat> (currp->z());
		normals[istart] = static_cast<GLfloat> (currn->x());
		normals[istart+1] = static_cast<GLfloat> (currn->y());
		normals[istart+2] = static_cast<GLfloat> (currn->z());
	}
	
}

bool OglRefMesh::IsReversed ( int index , const VRefTriangleMesh & trimesh )
{
	VPoint c1 = trimesh.GetModelTriCor1 ( index );
	VPoint c2 = trimesh.GetModelTriCor2 ( index );
	VPoint c3 = trimesh.GetModelTriCor3 ( index );
	VPoint n = trimesh.GetCor1Normal ( index );

	VPoint e1 = c2 - c1;
	VPoint e2 = c3 - c2;

	VPoint crossp = e1 ^ e2;
	vreal dotp = n % crossp;
	
	if ( dotp >= 0 )
		return false;
	else
		return true;
	
}
