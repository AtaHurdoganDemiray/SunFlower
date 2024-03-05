// SolidPosCube2d.cpp

#include "stdafx.h"
#include "SolidPosCube2d.h"
#include "PosCubes2d.h"
#include "gmQuad2d.h"
#include "VLine2d.h"
#include "VTriangle2dE.h"
#include "BasicGeoIntr2dUI.h"



SolidPosCube2d::SolidPosCube2d ( const PosCubes2d &newcore , const long &cubeindex )
{
	//long relcoord[2];

	//newcore.GetRelCoordForCubeIndex ( relcoord , cubeindex );

	//VPoint2d qmin ( (newcore.cubelengthx * relcoord[0]) 
	//				, ( newcore.cubelengthy * relcoord[1]) );
	//
	//VPoint2d qmax ( (newcore.cubelengthx * (relcoord[0]+1) )
	//				, (newcore.cubelengthy * (relcoord[1]+1) ) );
	//
	//qmin += newcore.min;
	//qmax += newcore.max;

	//VPoint2d c2 ( qmin.x() , qmax.y() );

	//VPoint2d c4 ( qmax.x() , qmin.y() );

	//quad = new gmQuad2d ( qmin , c2 , qmax , c4 );
}

SolidPosCube2d::~SolidPosCube2d()
{
	//delete quad;
}

bool SolidPosCube2d::IsLineRelated ( const VLine2d &myline )
{return true;
	//BasicGeoIntr2dUI calcint;
	//BasicIntrInfo2d intrinfo1;
	//VLine2d qline1 ( quad->GetCor1() , quad->GetCor2() ); 
	//calcint.FindIntr ( qline1 , myline , intrinfo1 );
	//if ( intrinfo1.isintr )
	//	return true;
	//
	//BasicIntrInfo2d intrinfo2;
	//VLine2d qline2 ( quad->GetCor2() , quad->GetCor3() ); 
	//calcint.FindIntr ( qline2 , myline , intrinfo2 );
	//if ( intrinfo2.isintr )
	//	return true;

	//BasicIntrInfo2d intrinfo3;
	//VLine2d qline3 ( quad->GetCor3() , quad->GetCor4() ); 
	//calcint.FindIntr ( qline3 , myline , intrinfo3 );
	//if ( intrinfo3.isintr )
	//	return true;

	//BasicIntrInfo2d intrinfo4;
	//VLine2d qline4 ( quad->GetCor4() , quad->GetCor1() ); 
	//calcint.FindIntr (  qline4 , myline , intrinfo4 );
	//if ( intrinfo4.isintr )
	//	return true;

	//if ( quad->IsInQuad ( myline.GetP1() ) )
	//	return true;

	//return false;
}

bool SolidPosCube2d::IsTriRelated ( const VTriangle2dE &mytri )
{
	return true;
}

