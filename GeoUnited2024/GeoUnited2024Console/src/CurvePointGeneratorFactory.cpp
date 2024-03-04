// CurvePointGeneratorFactory.cpp

#include "stdafx.h"
#include "CurvePointGeneratorFactory.h"
#include "BSplineCurvePointGenerator.h"
#include "LinePointGenerator.h"
#include "ArcPointGenerator.h"
#include "CopiousDataPointGenerator.h"
#include "ParametricSplinePointGenerator.h"
#include "ConicArcPointGenerator.h"
#include "IgesEntity.h"
#include "IgesEntitySet.h"
#include "Edge.h"
#include "TrimmedCurvePointGenerator.h"

CurvePointGenerator * CurvePointGeneratorFactory::MakeConicGenerator ( IgesEntity *pcurve , IgesEntitySet &allent)
{
	ConicArc *myarc = (ConicArc *)(pcurve); 
	ConicArcPointGenerator *tmp = new ConicArcPointGenerator(*myarc,allent);
	//return tmp.MakeGenerator (allent);
	return tmp;
}

CurvePointGenerator * CurvePointGeneratorFactory::MakeCurve ( IgesEntity *pcurve ,  IgesEntitySet &allent)
{
	CurvePointGenerator *digi = 0;		
	if ( pcurve->entitytype == 126 )
		digi = new BSplineCurvePointGenerator ( ( BSplineCurve&) (*pcurve) , allent);
	if ( pcurve->entitytype == 110 )
		digi = new LinePointGenerator ( (Line &)(*pcurve ) , allent);
	if ( pcurve->entitytype == 100 )
		digi= new ArcPointGenerator ( (igesArc &)(*pcurve) , allent );
	if ( pcurve->entitytype == 106)
		digi = new CopiousDataPointGenerator ( (CopiousData &)(*pcurve) , allent);
	if ( pcurve->entitytype == 104 )
		digi = MakeConicGenerator (pcurve , allent);
	if ( pcurve->entitytype == 112 )
		digi = new ParametricSplinePointGenerator ((ParametricSpline &)(*pcurve) , &allent);
	if (digi == 0)
		throw VException (( IDS_CurMakUnkCurTyp),1);
	return digi;
}

CurvePointGenerator * CurvePointGeneratorFactory::MakeCurve (Edge *pcurve , IgesEntitySet &allent , int ei , double tol)
{
	CurvePointGenerator *digi = new TrimmedCurvePointGenerator (pcurve , &allent , ei , tol);
	return digi;
}
