/******************************************************************************
*               File: SetMerger.cpp											  *
*******************************************************************************
*               Description:                                                  *
*                                                                             *
*******************************************************************************
*               History:                                                      *
*  04.03.2004 13:47:55 Created by:    Ata Hurdogan Demiray                    *
*******************************************************************************
*               (C) 2003 by Positive GmbH	                                 *
******************************************************************************/

#include "stdafx.h"
#include "SetMerger.h"
#include "LongVec.h"
#include "DoubleVec.h"
#include "PointSet2d.h"
#include "PointSet.h"
#include "LineSet2d.h"
#include "LineSet.h"
#include "TriangleMesh2d.h"
#include "TriangleMesh.h"
#include "RoughPolarPSet2d.h"



void SetMerger::Merge ( const LongVec &source , LongVec &destiny )
{
	for ( long i = 0; i < source.Size(); i++ )
	{
		long temp = source[i];
		destiny.AddLong ( temp );
	}

}

//#############################################################################

void SetMerger::Merge ( const DoubleVec &source , DoubleVec &destiny )
{
	for ( long i = 0; i < source.Size(); i++ )
	{
		vreal temp = source[i];
		destiny.Addvreal ( temp );
	}
}

//#############################################################################

void SetMerger::Merge ( const RoughPolarPSet2d &source , RoughPolarPSet2d &destiny )
{
	for ( long i = 0; i < source.Size(); i++ )
	{
		RoughPolarP2d *temp = source[i];
		destiny.InsertPoint ( *temp );
	}
}

//#############################################################################

void SetMerger::Merge ( const PointSet2d &source , PointSet2d &destiny )
{
	for ( long i = 0; i < source.Size(); i++ )
	{
		const VPoint2d *temp = source[i];
		destiny.InsertPoint ( temp->GetArray() );
	}
	//destiny.UpdateMinMax();
}

//#############################################################################

void SetMerger::Merge ( const PointSet &source , PointSet &destiny )	
{
	for ( long i = 0; i < source.Size(); i++ )
	{
		const VPoint *temp = source[i];
		destiny.InsertPoint ( temp->GetArray() );
	}
	//destiny.UpdateMinMax();
}

//#############################################################################

void SetMerger::Merge ( const LineSet2d &source , LineSet2d &destiny )
{
	for ( long i = 0; i < source.Size(); i++ )
	{
		const VLine2d *temp = source[i];
		destiny.AddLine ( temp->GetP1() , temp->GetP2() );
	}
	//destiny.UpdateMinMax();
}

//#############################################################################

void SetMerger::Merge ( const LineSet &source , LineSet &destiny )
{
	for ( long i = 0; i < source.Size(); i++ )
	{
		const VLine *temp = source[i];
		destiny.AddLine ( temp->GetP1() , temp->GetP2() );
	}
	//destiny.UpdateMinMax();
}

//#############################################################################

void SetMerger::Merge ( const TriangleMesh2d &source , TriangleMesh2d &destiny )
{
	for ( long i = 0; i < source.Size(); i++ )
	{
		VTriangle2dE *temp = source[i];
		destiny.AddTriangle ( temp->GetCorner1() , temp->GetCorner2() , temp->GetCorner3() );
	}
	destiny.UpdateMinMax();
}

//#############################################################################

void SetMerger::Merge ( const TriangleMesh &source , TriangleMesh &destiny )
{
	for ( long i = 0; i < source.Size(); i++ )
	{
		const VTriangleE *temp = source[i];
		destiny.AddTriangle ( temp->GetCorner1() , temp->GetCorner2() , temp->GetCorner3() );
	}
	//destiny.UpdateMinMax();
}

//#############################################################################

void SetMerger::MergeNoRepeat ( const PointSet2d & source , PointSet2d & destiny )
{
	vreal comptol = 5.0e-6;
	for ( int i=0; i < source.Size(); i++ )
	{
		const VPoint2d * currpoint = source[i];

		bool repeat = false;
		for ( int j=0; j < destiny.Size(); j++ )
		{	
			VPoint2d *tmp = destiny[j];
			if ( currpoint->Compare ( *tmp , comptol ) )
			{
				repeat = true;
				break;
			}
		}
		if ( repeat == false )
			destiny.InsertPoint(currpoint->GetArray());
	}
	//destiny.UpdateMinMax();
}
