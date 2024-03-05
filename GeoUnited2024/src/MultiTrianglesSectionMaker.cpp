// MultiTrianglesSectionMaker.cpp

#include "stdafx.h"
#include "MultiTrianglesSectionMaker.h"
#include "VLine.h"
#include "PosCubes.h"
#include "MultiTriangles.h"
#include "LineSet.h"
#include "TriangleIntersection.h"
#include "VPlane.h"
#include "LinePlaneRelation.h"

void MultiTrianglesSectionMaker::MakeSectionOfOnePlane (const VLine &currline 
											   , PosCubes & mycube
											   , MultiTriangles &multiTri
											   , LineSet &sectionLines )
{
	VPoint mymn = multiTri.GetMin();
	VPoint mymx = multiTri.GetMax();
	VVector sectionNormal = currline.GetV() ^ UZ;
	if (sectionNormal.GetLength() < EPSILON)
	{
		//VpAfxMessageBox ( SELWRN1.data() );
		return; 
	}
	sectionNormal.NormThis();
	VPlane mypln (currline.GetP1(),sectionNormal);
	VVector lineDir1 = currline.GetV().Unit();
	VPlane clipPlane1 (currline.GetP1() , lineDir1);
	VPlane clipPlane2 (currline.GetP2() , -lineDir1);
	LinePlaneRelation clipif;
	VPoint linemn (currline.GetP1());
	linemn.ReplaceMin (currline.GetP2());
	VPoint linemx (currline.GetP1());
	linemx.ReplaceMax(currline.GetP2());
	VPoint boxmin (linemn.x(),linemn.y(),mymn.z());
	VPoint boxmax (linemx.x(),linemx.y(),mymx.z());
	LongVec relind , nii;
	mycube.SetRelatedIndiceListForBox (boxmin , boxmax , relind , nii);		
	int tricount = relind.Size();
	TriangleIntersection triPlaneIntersector;
	for (int w =0;w<tricount;w++)
	{
		int currindex = relind[w];
		VTriangleE currTri = multiTri[currindex];
		VLine intL;
		bool ck = triPlaneIntersector.Int (currTri , mypln , intL);
		if (ck)
		{
			VLine subpos1 , subneg1 , subpos2 , subneg2; 
			LinePlaneRelation::Type ck1 = clipif.Clasify (intL , clipPlane1,subpos1,subneg1);
			if (ck1 == LinePlaneRelation::INTERSECTING)
				intL = subpos1;
			LinePlaneRelation::Type ck2 = clipif.Clasify (intL , clipPlane2 , subpos2,subneg2);
			if (ck2 == LinePlaneRelation::INTERSECTING)
				intL = subpos2;
			if (ck1 != LinePlaneRelation::NEGATIVE && ck2 != LinePlaneRelation::NEGATIVE)
				sectionLines.AddLine (intL);
			
		}
	}
}

void MultiTrianglesSectionMaker::MakeSectionOfZPlane (const vreal z
													, PosCubes &mycube
													, MultiTriangles &multitri
													, LineSet &sectionLines )
{
	VPoint mymn = multitri.GetMin();
	VPoint mymx = multitri.GetMax();	
	VPlane mypln (VPoint (0.,0.,z),UZ);
	VPoint boxmin (mymn.x(),mymn.y(),z-1.);
	VPoint boxmax (mymx.x(),mymx.y(),z+1.);
	LongVec relind , nii;
	mycube.SetRelatedIndiceListForBox (boxmin , boxmax , relind , nii);		
	int tricount = relind.Size();
	TriangleIntersection triPlaneIntersector;
	for (int w =0;w<tricount;w++)
	{
		int currindex = relind[w];
		VTriangleE currTri = multitri[currindex];
		VLine intL;
		bool ck = triPlaneIntersector.Int (currTri , mypln , intL);
		if (ck)
		{
			if ( (currTri.GetNormal() ^ intL.GetV()) % UZ < 0)
				intL.Reverse(); // line orientation is corrected
			
			sectionLines.AddLine (intL);	
		}
	}
}
