/******************************************************************************
*               File: DrawShapes.h											  *
*******************************************************************************
*               Description:                                                  *
*                                                                             *
*******************************************************************************
*               History:                                                      *
*  01.08.2003 15:15:25 Created by:    Ata Hurdogan Demiray                    *
*******************************************************************************
*               (C) 2003 by Pozitif GmbH                                      *
******************************************************************************/

#include "stdafx.h"
#include "VPoint.h"
#include "DrawShapes.h"
#include "BasicDefinitions.h"
//#include "LinesDisplay.h"
#include "ViewVolume.h"
#include "VPlane.h"
//#include "FacetDisplay.h"
#include "Display.h"
#include "VisualAttLoader.h"


DrawShapes::DrawShapes()
:planetri ( 2 )
{
	VTriangleE t1 ( VPoint ( -5,-5,0 ) , VPoint (-5,5,0) , VPoint ( 5,5,0 ) );
	VTriangleE t2 ( VPoint ( 5,5,0) ,VPoint (5,-5,0) , VPoint ( -5,-5,0 ) );
	planetri.AddTriangle ( t1 );
	planetri.AddTriangle ( t2 );
	planecolor.R(0.2);
	planecolor.G(0.2);
	planecolor.B(0.9);
}

//#############################################################################

DrawShapes::~DrawShapes()
{

}

//#############################################################################

void DrawShapes::DrawVector( VPoint &vecttodraw , VPoint &basepoint , vreal scalefactor )
{
	//if ( fabs(vecttodraw%vecttodraw) < EPSILON )
	//	return;
	//if (m_vectorToDraw.Compare (vecttodraw,VGlobalValues::ONEMICRON_TO_VUNIT) && m_basePoint.Compare (basepoint,VGlobalValues::ONEMICRON_TO_VUNIT) )
	//	return;
	m_vectorToDraw = vecttodraw;
	m_basePoint = basepoint;
	CalculateArrowLines ( vecttodraw , basepoint , scalefactor );
	//LinesDisplay linesdisp( vectordr );
	Display disp (vectordr);
	//linesdisp.Draw();
	disp.DrawLines(false);
	vectordr.Clear();
	//vectordr.Clear();
}

//#############################################################################

void DrawShapes::CalculateArrowLines ( VPoint &vect , VPoint &basepoint , vreal scalefactor )
{
	VPoint temp = vect.Unit();
	VPoint xdir ( 1.0 , 0.0 , 0.0 );
	VPoint ydir ( 0.0 , 1.0 , 0.0 );
	VPoint zdir ( 0.0 , 0.0 , 1.0 );
	
	// for arrow line offsets
	VPoint nd1 = temp ^ xdir;
	VPoint nd2 = temp ^ ydir;
	VPoint nd3 = temp ^ zdir;
	VPoint d1 , d2;

	vreal mag1 = nd1%nd1;
	vreal mag2 = nd2%nd2;
	vreal mag3 = nd3%nd3;

	if ( mag1 >= mag3 && mag2 >= mag3 )
	{
		d1 = nd1;
		d2 = nd2;
	}
	else if ( mag1 >= mag2 && mag3 >= mag2 )
	{
		d1 = nd1;
		d2 = nd3;
	}
	else if ( mag2 >= mag1 && mag3 >= mag1 )
	{
		d1 = nd2;
		d2 = nd3;
	}

	d1 = d1.Unit() * ( 0.10 * scalefactor );
	d2 = d2.Unit() * ( 0.10 * scalefactor );
	//d3 = d3.Unit() * ( 0.15 * scalefactor );

	VPoint endp = basepoint + (vect.Unit() * scalefactor);
	VPoint endp2 = basepoint + (vect.Unit() * ( scalefactor * 4 / 5 ));

	// creating the lines
	VPoint mypoints[14];
	mypoints[0] = basepoint;
	mypoints[1] = endp ;

	mypoints[2] = endp;
	mypoints[4] = endp;
	mypoints[6] = endp;
	mypoints[8] = endp;
//	mypoints[10] = endp;
//	mypoints[12] = endp;

	mypoints[3] = endp2 + d1 ;
	mypoints[5] = endp2 + d2 ;
	mypoints[7] = endp2 - d1 ;
	mypoints[9] = endp2 - d2 ;
//	mypoints[11] = endp2 - d2 ;
//	mypoints[13] = endp2 - d3 ;

	for ( int i = 0; i < 5; i++ )
		vectordr.AddLine ( mypoints[i*2] , mypoints[i*2+1] );

}

//#############################################################################

void DrawShapes::CreateTriOfPlane ( VPoint basepoint , VPoint normal , vreal scalefactor )
{
	// Create 2 triangles for plane 
	// find perp to normal
	VPoint perp1 , perp2;
	VPoint xdir ( 1.0 , 0.0 , 0.0 );
	VPoint ydir ( 0.0 , 1.0 , 0.0 );

	perp1 = normal ^ xdir;
	if ( fabs(perp1%perp1) < EPSILON )
		perp1 = normal ^ ydir;

	perp2 = normal ^ perp1;

	perp1 = perp1.Unit()*(scalefactor/2.0);
	perp2 = perp2.Unit()*(scalefactor/2.0);

	VPoint corner1 , corner2 , corner3 , corner4;
	
	corner1 = basepoint + (perp1 + perp2);
	corner2 = basepoint + (perp2 - perp1);
	corner3 = basepoint - (perp1 + perp2);
	corner4 = basepoint + (perp1 - perp2);
	
	VTriangleE t1 ( corner1 , corner2 , corner3 );
	VTriangleE t2 ( corner1 , corner3 , corner4 );

	*(planetri[0]) = t1;
	*(planetri[1]) = t2;

}

//#############################################################################

void DrawShapes::DrawPlane ( const VPlane & pln , const ViewVolume & vol ) 
{
	vreal scalefactor = 0.7*vol.GetDiaogonalRadius();
	this->CreateTriOfPlane ( pln.pointonplane , pln.normal , scalefactor );
	
	//FacetDisplay fd ( planetri );
	Display fd (planetri);
	FacetVisualAtt fva;
	fva.ChangeShadeColor(planecolor);
	fva.SetTranslucent(true);
	fd.SetVisualAttFacet(fva);
	VisualAttLoader vload;
	vload.Load ( fd.GetVisualAttFacet() , false , VColor(1,0,0));
	fd.DrawFacet();
}
	



	
