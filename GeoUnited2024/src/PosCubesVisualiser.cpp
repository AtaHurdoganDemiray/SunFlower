// PosCubesVisualiser.cpp

#include "stdafx.h"
#include "PosCubesVisualiser.h"
#include "PosCubes.h"
//#include "PosCubesCore.h"

PosCubesVisualiser::PosCubesVisualiser ( PosCubes &poscubes )
:m_poscubes(poscubes),m_cubecenters (poscubes.cubeno)
{
	VPoint diff = m_poscubes.max-m_poscubes.min;
	VPoint cubeVector ( diff.x()/m_poscubes.nox , diff.y()/m_poscubes.noy , diff.z()/m_poscubes.noz );
	VPoint onlyX(cubeVector.x(),0.0,0.0);
	VPoint onlyY(0.0,cubeVector.y(),0.0);
	VPoint onlyZ(0.0,0.0,cubeVector.z());

	long relcoord[3];
	long cubeindex;
	
	for ( long i1 = 0; i1 < m_poscubes.nox; i1++ )
		for ( long i2 = 0; i2 < m_poscubes.noy; i2++ )
			for ( long i3 = 0; i3 < m_poscubes.noz; i3++ )
			{
				relcoord[0] = i1;
				relcoord[1] = i2;
				relcoord[2] = i3;
				cubeindex = m_poscubes.GetCubeIndexForRelCoord ( relcoord );
				VPoint currCenter = m_poscubes.GetCenterPointOfCube(cubeindex);
				m_cubecenters.InsertPoint (currCenter);
			}

	for ( int i=0; i<m_cubecenters.Size();i++ )
	{
		VPoint *currPoint = m_cubecenters[i];
		VPoint c[8];
		c[0]=*currPoint-(cubeVector/2.);
		c[1]=c[0]+onlyY;
		c[2]=c[1]+onlyX;
		c[3]=c[2]-onlyY;
		c[4]=c[0]+onlyZ;
		c[5]=c[4]+onlyY;
		c[6]=c[5]+onlyX;
		c[7]=c[6]-onlyY;
		VLine l[12];
		l[0]=VLine(c[0],c[1]);
		l[1]=VLine(c[1],c[2]);
		l[2]=VLine(c[2],c[3]);
		l[3]=VLine(c[3],c[0]);
		
		l[4]=VLine(c[4],c[5]);
		l[5]=VLine(c[5],c[6]);
		l[6]=VLine(c[6],c[7]);
		l[7]=VLine(c[7],c[4]);
		
		l[8]=VLine(c[0],c[4]);
		l[9]=VLine(c[1],c[5]);
		l[10]=VLine(c[2],c[6]);
		l[11]=VLine(c[3],c[7]);

		for (int i=0;i<12;i++)
		{
			m_cubeedges.AddLine(l[i]);
		}
	}

}
