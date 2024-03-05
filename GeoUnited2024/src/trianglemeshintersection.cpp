// TriangleMeshIntersection.cpp

#include "stdafx.h"
#include "TriangleMeshIntersection.h"
#include "TriangleMesh.h"
#include "LineSet.h"
#include "PosCubes.h"
#include "TriangleIntersection.h"
#include "VLine.h"
#include "VInterval.h"
#include "VPlane.h"
#include "MultiTriangles.h"

void TriangleMeshIntersection::Intersect 
(const TriangleMesh &m1 , const TriangleMesh &m2 , LineSet &res)
{
	PosCubes cubes (m2);
	TriangleIntersection calc;
	int size1 = m1.Size();
	for (int i=0;i<size1;i++)
	{
		const VTriangleE *t1 = m1[i];
		LongVec relind,nii;
		cubes.SetRelatedIndiceListForTri (*t1 , relind , nii);
		int size2 = relind.Size();
		for (int j=0;j<size2;j++)
		{
			int ind = relind[j];
			const VTriangleE *t2 = m2[ind];
			VLine intL;
			bool intrsct =false;
			TriangleIntersection::IntType ck = calc.Int (*t1,*t2,intrsct,intL);				
			if (ck == TriangleIntersection::REGULAR && intrsct == true)
			{
				res.AddLine (intL);
			}
		}
	}
}

void TriangleMeshIntersection::Intersect (const TriangleMesh &m1 
										  , const VPlane &p 
										  , const vreal &offs
										  , LineSet &res)
{
	VVector diru,dirv;
	diru = p.normal^UX;
	if (diru.GetLength2() < EPSILON)
	{
		diru = p.normal^UY;
	}
	diru.NormThis();
	dirv = p.normal^diru;
	VPoint mx = m1.GetMax();
	VPoint mn = m1.GetMin();
	vreal deltaz = mx.z() - mn.z();
	VVector diagonal = (diru+dirv)*offs + p.normal + deltaz * UZ;
	VPoint c1 = p.pointonplane - diagonal;
	VPoint c2 = p.pointonplane + diagonal;
	VLine tmp (c1,c2);

	VPlane cp1 (c1,-dirv);
	VPlane cp2 (c2,dirv);

	TriangleIntersection calc;
	PosCubes cubes(m1);
	LongVec relind,nii;
	cubes.SetRelatedIndiceListForLine (tmp ,relind,nii);
	int size1 = relind.Size();
	for (int i=0;i<size1;i++)
	{
		int ind = relind[i];
		const VTriangleE *t = m1[ind];
		VLine intL;
		bool ck = calc.Int (*t,p,intL);
		if (ck)
		{
			VLine remaining;
			bool totallyTrimmed;
			cp1.ClipLine (intL,remaining,totallyTrimmed);
			if (totallyTrimmed == false)
			{
				VLine remaining2;
				cp2.ClipLine (remaining , remaining2 , totallyTrimmed);
				if (totallyTrimmed == false)
				{
					res.AddLine (remaining2);			
				}
			}
		}
	}
}
