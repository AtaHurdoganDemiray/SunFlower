// FreeCurveSet.h

#include "stdafx.h"
#include "FreeCurveSet.h"
#include "IgesEntitySet.h"
#include "gmTrimmedSurfaceSet.h"
#include "gmNurbsTesselator.h"
#include "gmTrimmedSurfaceData.h"
#include "LineSet.h"
#include "gmGeneralIgesInfo.h"

FreeCurveSet::FreeCurveSet 
( const IgesEntitySet &allentities , const gmTrimmedSurfaceSet &alltrimmedsurf )
{
	gmGeneralIgesInfo igsinfo ( allentities );

	for ( long i=0; i<allentities.Size(); i++)
	{
		IgesEntity *pe = allentities[i];
		if ( pe->entitytype == 126 )
		{
			bool record = true;
			for ( long j=0; j < alltrimmedsurf.Size(); j++ )
			{
				gmTrimmedSurfaceData *pts = alltrimmedsurf[j];
				
				bool ck = igsinfo.IsCurveIn 
					( pe->directorylineno , *(pts->m_trimmedsurface) );
				
				if ( ck == true )
					record = false;
				
			}
			if ( record == true )
				allsurfaces.push_back ( (BSplineCurve*) pe );
		}
	}
			
}				
					

//#############################################################################

void FreeCurveSet::CalcLineSetForCurve 
( const BSplineCurve &curve , LineSet &curvelines , DoubleVec &params ) const 
{
	PointSet curvepoints ( NURBS_CURVE_TESSELATION_POINTS_PER_DIR + 5 );
	gmNurbsTesselator calc;
	calc.BSplineCurve2PointSet ( curve , curvepoints , params ); 

	for ( long i=0; i < (curvepoints.Size()-1); i++ )
	{
		curvelines.AddLine ( *(curvepoints[i]) , *(curvepoints[i+1]) );
	}

}

//#############################################################################
