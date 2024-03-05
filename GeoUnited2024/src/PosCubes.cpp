/******************************************************************************
*               File: PosCubes.cpp											  *
*******************************************************************************
*               Description:                                                  *
*                                                                             *
*******************************************************************************
*               History:                                                      *
*  02.07.2003 18:44:39 Created by:    Ata Hurdogan Demiray                    *
*******************************************************************************
*               (C) 2003 by Pozitif GmbH                                      *
******************************************************************************/

// PosCubes.cpp

#include "stdafx.h"
#include "PosCubes.h"
#include "TriangleMesh.h"
#include "LineSet.h"
#include "PointSet.h"
#include "MultiTriangles.h"
#include "MultiLines.h"
#include "MultiPoints.h"
#include "SequentialLineSet.h"
#include "Triangles.h"

#define GIVE_ALL_INDEX 0

PosCubes::~PosCubes()
{
	PosCubesCoreDelete();
}

//#############################################################################

PosCubes::PosCubes ( const PointSet &mypointset )
{
	VPoint min = mypointset.GetMin();
	VPoint max = mypointset.GetMax();
	long setcount = mypointset.Size();
	long nox , noy , noz;
	DecideCubes ( min , max , setcount , nox , noy , noz);

	//mcore = new 
		PosCubesCore ( min , max , nox , noy , noz , setcount );
	//geohandler = new PosCubesGeoHandling ( (*mcore) );

	FillCubes ( mypointset );

}

//#############################################################################

PosCubes::PosCubes ( const LineSet &mylineset )
{
	VPoint min = mylineset.GetMin();
	VPoint max = mylineset.GetMax();
	long setcount = mylineset.Size();
	long nox , noy , noz;
	DecideCubes ( min , max , setcount , nox , noy , noz);

	//mcore = new 
	PosCubesCore ( min , max , nox , noy , noz , setcount );
	//geohandler = new PosCubesGeoHandling ( (*mcore) );

	FillCubes ( mylineset );

}

//#############################################################################
PosCubes::PosCubes ( const SequentialLineSet &mylineset )
{
	VPoint min = mylineset.GetMin();
	VPoint max = mylineset.GetMax();
	long setcount = mylineset.Size();
	long nox , noy , noz;
	DecideCubes ( min , max , setcount , nox , noy , noz);

	//mcore = new 
	PosCubesCore ( min , max , nox , noy , noz , setcount );
	//geohandler = new PosCubesGeoHandling ( (*mcore) );

	FillCubes ( mylineset );

}

//#############################################################################

PosCubes::PosCubes ( const TriangleMesh &newtrimesh )
{
	VPoint min = newtrimesh.GetMin();
	VPoint max = newtrimesh.GetMax();
	long setcount = newtrimesh.Size();
	long nox , noy , noz;
	DecideCubes ( min , max , setcount , nox , noy , noz);

	//mcore = new 
	PosCubesCore ( min , max , nox , noy , noz , setcount );
	//geohandler = new PosCubesGeoHandling ( (*mcore) );

	FillCubes ( newtrimesh );

}

//#############################################################################

PosCubes::PosCubes (const Triangles &tri)
{
	VPoint min = tri.GetMin();
	VPoint max = tri.GetMax();
	long setcount = tri.Size();
	long nox , noy , noz;
	DecideCubes ( min , max , setcount , nox , noy , noz);

	//mcore = new 
	PosCubesCore ( min , max , nox , noy , noz , setcount );
	//geohandler = new PosCubesGeoHandling ( (*mcore) );

	FillCubes ( tri );
}

PosCubes::PosCubes ( const MultiTriangles &newtriangles )
{
	VPoint min = newtriangles.GetMin();
	VPoint max = newtriangles.GetMax();
	long setcount = newtriangles.Size();
	long nox , noy , noz;
	DecideCubes ( min , max , setcount , nox , noy , noz);

	//mcore = new 
	PosCubesCore ( min , max , nox , noy , noz , setcount );
	//geohandler = new PosCubesGeoHandling ( (*mcore) );

	FillCubes ( newtriangles );

}

//#############################################################################

PosCubes::PosCubes (const MultiTriangles &tri 
					, const vreal &xyGridLength)
{
	VPoint min = tri.GetMin();
	VPoint max = tri.GetMax();
	int mycount = tri.Size();
	long nox,noy,noz = 1;
	DecideCubesXY (min,max,xyGridLength,nox,noy);
	
	//mcore = new 
	PosCubesCore (min,max,nox,noy,noz,mycount);
	//geohandler = new PosCubesGeoHandling (*mcore);
	FillCubes (tri);
}

//#############################################################################

PosCubes::PosCubes ( const MultiLines &newlines )
{
	VPoint min = newlines.GetMin();
	VPoint max = newlines.GetMax();
	long setcount = newlines.Size();
	long nox , noy , noz;
	DecideCubes ( min , max , setcount , nox , noy , noz);

	//mcore = new 
	PosCubesCore ( min , max , nox , noy , noz , setcount );
	//geohandler = new PosCubesGeoHandling ( (*mcore) );

	FillCubes ( newlines );

}

//#############################################################################

PosCubes::PosCubes ( const MultiPoints &newpoints )
{
	VPoint min = newpoints.GetMin();
	VPoint max = newpoints.GetMax();
	long setcount = newpoints.Size();
	long nox , noy , noz;
	DecideCubes ( min , max , setcount , nox , noy , noz);

	//mcore = new
	PosCubesCore ( min , max , nox , noy , noz , setcount );
	//geohandler = new PosCubesGeoHandling ( (*mcore) );

	FillCubes ( newpoints );

}

//#############################################################################

PosCubes::PosCubes ( const VPoint & overallmin , const VPoint & overallmax  )
{
	long nox, noy,noz;
	long setcount = 0;
	DecideCubes(overallmin,overallmax,setcount,nox,noy,noz);
	//mcore = new 
	PosCubesCore ( overallmin , overallmax , nox , noy , noz , setcount );
	//geohandler = new PosCubesGeoHandling ( (*mcore) );
}

//#############################################################################

void PosCubes::AddItem ( const VPoint & newitem , long index )
{
	AddItem();
	RecordPointToRelCubes ( newitem , index );
}

//#############################################################################

void PosCubes::AddItem ( const VLine & newitem , long index )
{
	AddItem();
	RecordLineToRelCubes ( newitem , index );
}

//#############################################################################

void PosCubes::AddItem ( const VTriangleE & newitem , long index )
{
	AddItem();
	RecordTriToRelCubes ( newitem , index );
}

//#############################################################################

void PosCubes::FillCubes ( const PointSet &mypointset )
{
	int mysize = mypointset.Size();
	for ( long i = 0; i < mysize; i++ )
	{
		const VPoint *currpoint = mypointset[i];
		RecordPointToRelCubes ( (*currpoint) , i );
	}
}

//#############################################################################

void PosCubes::FillCubes ( const LineSet &mylineset )
{
	int mysize = mylineset.Size();
	for ( long i = 0; i < mysize; i++ )
	{
		const VLine *currline = mylineset[i];
		RecordLineToRelCubes ( (*currline) , i );
	}
}

//#############################################################################

void PosCubes::FillCubes ( const SequentialLineSet &mylineset )
{
	int mysize = mylineset.Size();
	for ( long i = 0; i < mysize; i++ )
	{
		VLine currline = mylineset[i];
		RecordLineToRelCubes ( currline , i );
	}
}

//#############################################################################

void PosCubes::FillCubes ( const TriangleMesh &newtrimesh )
{
	int mysize = newtrimesh.Size();
	for ( long i = 0; i < mysize; i++ )
	{
		const VTriangleE * currtri = newtrimesh[i];
		RecordTriToRelCubes ( (*currtri) , i );
	}

}

//#############################################################################

void PosCubes::FillCubes (const Triangles &tri)
{
	int mysize = tri.Size();
	for (int i=0;i<mysize;++i)
	{
		VTriangleE currtri = tri.operator[] (i);
		RecordTriToRelCubes (currtri , i);
	}
}

void PosCubes::FillCubes ( const MultiTriangles &newtriangels )
{
	int mysize = newtriangels.Size();
	for ( long i = 0; i < mysize; i++ )
	{
		const VTriangleE  currtri = newtriangels[i];
		RecordTriToRelCubes ( currtri , i );
	}

}

//#############################################################################

void PosCubes::FillCubes ( const MultiLines &newlines )
{
	int mysize = newlines.Size();
	for ( long i = 0; i < mysize; i++ )
	{
		const VLine & currline = newlines[i];
		RecordLineToRelCubes ( currline , i );
	}

}

//#############################################################################

void PosCubes::FillCubes ( const MultiPoints &newpoints )
{
	int mysize = newpoints.Size();
	for ( long i = 0; i < mysize; i++ )
	{
		const VPoint &currpoint = newpoints[i];
		RecordPointToRelCubes ( currpoint , i );
	}

}

//#############################################################################

const vreal POSCUBES_DIVIDE_FACTOR = 20.;

void PosCubes::DecideCubes ( const VPoint &min , const VPoint &max
							  , const long &setcount , long &nox , long &noy , long &noz)
{
	// decide grid no along x and y
	
	VPoint diff = max - min;
	vreal maxComponent = diff.x();
	if (diff.y()>maxComponent)
		maxComponent=diff.y();
	if (diff.z()>maxComponent)
		maxComponent=diff.z();

	vreal cubeEdgeLength = maxComponent/POSCUBES_DIVIDE_FACTOR;
	nox= (long)(diff.x()/cubeEdgeLength) + 1;
	noy= (long)(diff.y()/cubeEdgeLength) + 1;
	noz= (long)(diff.z()/cubeEdgeLength) + 1;

	////////////////////////////////////////////////////////////
	/*
	nox = 12;
	noy = 12;
	noz = 12;

	if ( diff.x() < 5e-6 )
	{
		nox = 1;
		//noy = (long)( setcount / 5 );
	}

	if ( diff.y() < 5e-6 )
	{
		noy = 1;
		//nox = (long)( setcount / 5 );
	}
	
	if ( diff.z() < 5e-6 )
	{
		noz = 1;
		//nox = (long)( setcount / 5 );
	}
	*/
}

//#############################################################################

void PosCubes::DecideCubesXY (const VPoint &min
							  ,const VPoint &max
							  ,const vreal &xystep
							  ,long &nox
							  ,long &noy)
{
	VPoint diff = max - min;
	double xs = diff.x() / xystep;
	xs = xs + 1.0;
	double ys = diff.y() / xystep;
	ys = ys + 1.0;
	nox = static_cast<int>(xs);
	noy = static_cast<int>(ys);
}

//#############################################################################

void PosCubes::GetAllIndices ( LongVec &relatedindices ) const
{
	VPoint mymin=min;
	VPoint mymax=max;
	LongVec tmp;
	SetRelatedIndiceListForBox(mymin,mymax,relatedindices,tmp);		
}
