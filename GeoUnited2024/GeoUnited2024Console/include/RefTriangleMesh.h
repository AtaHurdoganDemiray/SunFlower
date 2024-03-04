// RefTriangleMesh.h

#ifndef __RefTriangleMesh_h__
#define __RefTriangleMesh_h__
#include "BasicDefinitions.h"
#include "RefTriangle.h"

class RefTriangleMesh
{
public:
	RefTriangleMesh ( const int &initialsize );
	RefTriangleMesh ( const RefTriangleMesh & Foo );
	~RefTriangleMesh ();
	long Size() const;
	const RefTriangle * operator[] ( const long &index ) const;
	RefTriangle * operator[] ( const long &index );
	void AddTriangle ( const RefTriangle &source );
	bool operator == (const RefTriangleMesh &toCompare)const;
	bool operator != (const RefTriangleMesh &toCompare)const;
	void Serialize (Bofstream &outfile)const;
	void Serialize (Bifstream &infile);
	void Clear();
private:
	std::vector<RefTriangle> mdata;
};

#endif //__RefTriangleMesh_h__
