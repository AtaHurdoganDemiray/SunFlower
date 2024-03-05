// MinkowskiSum.cpp

#include "stdafx.h"
#include "MinkowskiSum.h"
#include "VTriangle.h"
#include "TriangleMesh.h"
#include "CoordinateSystem.h"
#include "RotationMatrix.h"
#include "MirrorMatrix.h"

void MinkowskiSum::Tri2TriAllInABS(const VTriangle &tmoving, const VTriangle &tstatic , TriangleMesh &sum)
{
	// static is tool triangle
	// moving is part triangle 
	

	// for all combinations of edges
	for (int i = 0; i < 3; ++i)
	{
		const VPoint &tmoveV0 = tmoving.GetCorner(i);
		const VPoint &tmoveV1 = tmoving.GetCorner((i + 1) % 3);
		VPoint lvecmoving = tmoveV1 - tmoveV0;
		VPoint tstaticV[3];

		for (int j = 0; j < 3; ++j)
		{
			VPoint tstaticV0 = tstatic.GetCorner(j)+tmoveV0;//moves the tool triangle edge start from tool reference point at absolute 0 to part vertex tmoveV0 
			VPoint tstaticV1 = tstatic.GetCorner((j + 1) % 3)+tmoveV0;//moves the tool triangle edge end from tool reference point at absolute 0 to part vertex tmoveV0 
			Add2TriToMinkowskiSumIf(tmoveV0, tmoveV1, lvecmoving,tstaticV0, tstaticV1 , sum);
			tstaticV[j] = tstaticV0;
		}
		sum.AddTriangle(VTriangleE(tstaticV[0],tstaticV[1],tstaticV[2]));
		VPoint moveVec = tstatic.GetCorner(i);
		sum.AddTriangle(VTriangleE(tmoving.GetCorner1() + moveVec, tmoving.GetCorner2() + moveVec, tmoving.GetCorner3() + moveVec));
	}	
}

void MinkowskiSum::Add2TriToMinkowskiSumIf(const VPoint &tmoveV0, const VPoint &tmoveV1, const VPoint &lvecmoving,const VPoint &tstaticV0, const VPoint &tstaticV1, TriangleMesh &minkowskiSum)
{
	try
	{
		// Check if lines are parallel
		VPoint lvecstat = tstaticV1 - tstaticV0;
		vreal dotck = lvecmoving % lvecstat;
		if (fabs(1.-fabs(dotck)) < VGlobalValues::ONEMICRON_TO_VUNIT * 0.01)
		{
			// lines are parallel
			return;
		}
		//VPoint ckpoint = tstaticV0 + lvecmoving;
		//VArc ack(tstaticV0, tstaticV1, tstaticV1 + lvecmoving);
		//vreal rck2 = (ckpoint - ack.GetCenter()).GetLength2();
		//// Delaunay check
		//if (rck2 < ack.GetRadius() * ack.GetRadius())
		//{
			// Delaunay passed
			VTriangleE mt1(tstaticV0, tstaticV1, tstaticV1 + lvecmoving);
			VTriangleE mt2(tstaticV1 + lvecmoving, tstaticV0 + lvecmoving, tstaticV0);
			minkowskiSum.AddTriangle(mt1);
			minkowskiSum.AddTriangle(mt2);
		//}
		//else
		//{
		//	// swapped triangles
		//	VTriangleE mt1(tstaticV0, tstaticV1, tstaticV0 + lvecmoving);
		//	VTriangleE mt2(tstaticV1, tstaticV1 + lvecmoving, tstaticV0 + lvecmoving);
		//	//minkowskiSum.AddTriangle(mt1);
		//	//minkowskiSum.AddTriangle(mt2);
		//}
	}
	catch (VException &e)
	{
		LogFileWriter::WriteExceptionLog(e,"MinkowskiSum::Add2TriToMinkowskiSumIf");
	}
	catch (...)
	{
		LogFileWriter::WriteUnknownExceptionLog("UE ", "MinkowskiSum::Add2TriToMinkowskiSumIf");
	}
}

void MinkowskiSum::Mesh2MeshAllInABS(TriangleMesh &toolMesh, const TriangleMesh &partMesh, TriangleMesh &sum)
{
	// Transform tool 
	RotationMatrix R(VPoint(0.0), UZ, 180.0);
	toolMesh.Transform(R);
	CoordinateSystem cs;
	MirrorMatrix M(cs, 2);
	toolMesh.Transform(M);

	// for all part triangles 
			// loop all transformed tool triangles and add to sum
	int psize = partMesh.Size();
	int tsize = toolMesh.Size();
	CString fordebug;
	fordebug.Format("part trisize = %d tool tri size:%d  total loop count: %d",psize,tsize,psize*tsize);
	LogFileWriter::WriteUnknownExceptionLog("LOOP START !!!!" , (LPCTSTR)fordebug);
	int counter = 0;
	for (int pi = 0; pi < psize; ++pi)
	{	

		const VTriangleE *ptri = partMesh.operator[](pi);
		for (int ti = 0; ti < tsize; ++ti)
		{
			const VTriangleE *ttri = toolMesh.operator[](ti);
			Tri2TriAllInABS(ptri->GetTri(),ttri->GetTri(),sum);
			++counter;
			if (counter % 1000 == 0)
			{
				fordebug.Format("count: %d",counter);
				LogFileWriter::WriteUnknownExceptionLog("LOOP 1000 milestone",(LPCTSTR)fordebug);
				LogFileWriter::WriteUnknownExceptionLog("...","...");
				if (counter > 1000000)
					return;
			}
		}
	}
}
