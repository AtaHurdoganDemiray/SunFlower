// VRefTriangleMeshWriter.cpp

#include "stdafx.h"
#include "VRefTriangleMeshWriter.h"
#include "VRefTriangleMesh.h"
#include "SerializeVector.h"

void VRefTriangleMeshWriter::Write (const VRefTriangleMesh &m
									,VofStream &f)
{
    std::vector<sVector<double,3> > poi;
    std::vector<sVector<double,3> > nor;
    std::vector<sVector<int,3> > ind;

	std::vector<VPoint>::const_iterator i = m.surfpack.modelpoints.GetVec().begin();
	std::vector<VPoint>::const_iterator e = m.surfpack.modelpoints.GetVec().end();
	std::vector<VPoint>::const_iterator ni = m.surfpack.normals.GetVec().begin();

	for (i; i!=e; ++i)
	{
		poi.push_back (i->GetArray());
		nor.push_back (ni->GetArray());
		++ni;
	}

	int mysize = m.Size();
	for (int j=0; j<mysize; ++j)
	{
		const RefTriangle &curref = m.GetRefTri(j);
		int tmp[3] = {curref.ref0
					,curref.ref1
					,curref.ref2};
		ind.push_back (tmp);
	}

	SerializeVector myser;
	myser.write (poi
				,nor
				,ind
				,f);

}

