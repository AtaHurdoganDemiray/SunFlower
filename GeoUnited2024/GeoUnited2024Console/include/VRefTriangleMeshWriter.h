// VRefTriangleMeshWriter.h

#ifndef __VRefTriangleMeshWriter_h__
#define __VRefTriangleMeshWriter_h__
class VRefTriangleMesh;
class VRefTriangleMeshWriter
{
public:
	static void Write (const VRefTriangleMesh &m
						, VofStream &f);
};


#endif