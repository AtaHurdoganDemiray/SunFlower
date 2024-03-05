// PosCubes2d.cpp

#include "stdafx.h"
#include "PosCubes2d.h"
#include "TriangleMesh2d.h"
#include "LineSet2d.h"
#include "PointSet2d.h"
#include "SequentialLineSet2d.h"
//#include "PosCubes2dGeoHandling.h"
//#include "PosCubes2dCore.h"
#include "gmQuadSet.h"
#include "VRefTriangleMesh.h"
#include "gmQuad2d.h"

PosCubes2d::~PosCubes2d()
{

}

//#############################################################################

PosCubes2d::PosCubes2d ( const PointSet2d &mypointset )
{
	VPoint2d min = mypointset.GetMin();
	VPoint2d max = mypointset.GetMax();
	long setcount = mypointset.Size();
	long nox , noy;
	DecideCubes ( min , max , setcount , nox , noy );

	PosCubes2dCore ( min , max , nox , noy , setcount );
	FillCubes ( mypointset );

}

//#############################################################################

PosCubes2d::PosCubes2d ( const LineSet2d &mylineset )
{
	VPoint2d min = mylineset.GetMin();
	VPoint2d max = mylineset.GetMax();
	long setcount = mylineset.Size();
	long nox , noy;
	DecideCubes ( min , max , setcount , nox , noy );

	PosCubes2dCore ( min , max , nox , noy , setcount );
	FillCubes ( mylineset );

}

//#############################################################################

PosCubes2d::PosCubes2d ( const TriangleMesh2d &newtrimesh )
{
	VPoint2d min = newtrimesh.GetMin();
	VPoint2d max = newtrimesh.GetMax();
	long setcount = newtrimesh.Size();
	long nox , noy;
	DecideCubes ( min , max , setcount , nox , noy );

	PosCubes2dCore ( min , max , nox , noy , setcount );
	FillCubes ( newtrimesh );

}

//#############################################################################

PosCubes2d::PosCubes2d ( const VRefTriangleMesh &refmesh )
{
	VPoint2d min = refmesh.surfpack.parampoints.GetMin();
	VPoint2d max = refmesh.surfpack.parampoints.GetMax();
	long setcount = refmesh.Size();
	long nox , noy;
	DecideCubes ( min , max , setcount , nox , noy );

	PosCubes2dCore ( min , max , nox , noy , setcount );
	FillCubes ( refmesh );

}

//#############################################################################

PosCubes2d::PosCubes2d ( const SequentialLineSet2d &seqlines )
{
	VPoint2d min = seqlines.GetMin();
	VPoint2d max = seqlines.GetMax();
	long setcount = seqlines.Size();
	long nox , noy;
	DecideCubes ( min , max , setcount , nox , noy );

	PosCubes2dCore ( min , max , nox , noy , setcount );

	FillCubes ( seqlines );

}

//#############################################################################

PosCubes2d::PosCubes2d ( const gmQuadSet & qset )
{
	VPoint2d min = qset.GetMin2d();
	VPoint2d max = qset.GetMax2d();
	long setcount = qset.Size();
	long nox , noy;
	DecideCubes ( min , max , setcount , nox , noy );

	PosCubes2dCore ( min , max , nox , noy , setcount );

	FillCubes ( qset );

}

//#############################################################################

PosCubes2d::PosCubes2d ( const VPoint2d & overallmin , const VPoint2d & overallmax  )
{
	long nox = 15, noy = 15;
	long setcount = 0;
	PosCubes2dCore ( overallmin , overallmax , nox , noy , setcount );

}

//#############################################################################

void PosCubes2d::AddItem ( const VPoint2d & newitem , long index )
{
	AddItem();
	RecordPointToRelCubes ( newitem , index );
}

//#############################################################################

void PosCubes2d::AddItem ( const VLine2d & newitem , long index )
{
	AddItem();
	RecordLineToRelCubes ( newitem , index );
}

//#############################################################################

void PosCubes2d::AddItem ( const VTriangle2dE & newitem , long index )
{
	AddItem();
	RecordTriToRelCubes ( newitem , index );
}

//#############################################################################

void PosCubes2d::AddItem ( const gmQuad2d & newitem , long index )
{
	AddItem();
	RecordQuadToRelCubes ( newitem , index );
}

//#############################################################################

void PosCubes2d::FillCubes ( const gmQuadSet &qset )
{
	int mysize = qset.Size();
	for ( int i = 0; i < mysize; i++ )
	{
		gmQuad2d currquad = qset.GetQuad2d(i);
		RecordQuadToRelCubes ( currquad , i );
	}
}

//#############################################################################

void PosCubes2d::FillCubes ( const PointSet2d &mypointset )
{
	int mysize = mypointset.Size();
	for ( long i = 0; i < mysize; i++ )
	{
		const VPoint2d *currpoint = mypointset[i];
		RecordPointToRelCubes ( (*currpoint) , i );
	}
}

//#############################################################################

void PosCubes2d::FillCubes ( const LineSet2d &mylineset )
{
	int mysize =  mylineset.Size();
	for ( long i = 0; i <mysize; i++ )
	{
		const VLine2d *currline = mylineset[i];
		RecordLineToRelCubes ( (*currline) , i );
	}

}

//#############################################################################

void PosCubes2d::FillCubes ( const SequentialLineSet2d &seqlines )
{
	int mysize = seqlines.Size();
	for ( long i = 0; i < mysize; i++ )
	{
		VLine2d currline = seqlines[i];
		RecordLineToRelCubes ( (currline) , i );
	}
}

//#############################################################################

void PosCubes2d::FillCubes ( const TriangleMesh2d &newtrimesh )
{
	int mysize = newtrimesh.Size();
	for ( long i = 0; i < mysize; i++ )
	{
		VTriangle2dE * currtri = newtrimesh[i];
		RecordTriToRelCubes ( (*currtri) , i );
	}

}

//#############################################################################

void PosCubes2d::FillCubes ( const VRefTriangleMesh &refmesh )
{
	int mysize = refmesh.Size();
	for ( long i = 0; i < mysize; i++ )
	{
		VTriangle2dE currtri 
		( refmesh.GetParamTriCor1(i)
		, refmesh.GetParamTriCor2(i)
		, refmesh.GetParamTriCor3(i) );
		RecordTriToRelCubes ( currtri , i );
	}
}

//#############################################################################

void PosCubes2d::DecideCubes ( const VPoint2d &min , const VPoint2d &max
							  , const long &setcount , long &nox , long &noy)
{
	// decide grid no along x and y
	
	VPoint2d diff = max - min;
/*	
	if ( setcount < 100 )
	{
		nox = 4;
		noy = 4;
	}
	else if ( setcount < 400 )
	{
		nox = 10;
		noy = 10;
	}
	else if ( setcount < 600 )
	{
		nox = 20;
		noy = 20;
	}
	else
	{
		nox = 20;
		noy = 20;
	}
*/
	nox = 20;
	noy = 20;
/*
	if ( setcount < 1 )
	{
		nox = 1;
		noy = 1;
		return;
	}

	long homgridno = (long) sqrt ( setcount );

	nox = (long) ( homgridno / 2 );
	noy = (long) ( homgridno / 2 );
*/	
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
}

//#############################################################################

