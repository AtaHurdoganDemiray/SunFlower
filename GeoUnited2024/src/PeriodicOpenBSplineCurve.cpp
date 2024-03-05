// PeriodicOpenBSplineCurve.cpp

#include "stdafx.h"
#include "PeriodicOpenBSplineCurve.h"
#include "BSplineM3.h"
#include "BSplineM4.h"
#include "BSplineCurve.h"
#include "VPoint.h"
#include "VException.h"
#include "BSplineKnots.h"

PeriodicOpenBSplineCurve::PeriodicOpenBSplineCurve ( const BSplineCurve & basecurve )
: _basecurve ( basecurve )
{
	if ( _basecurve.IsClosed () == false 
		//&& _basecurve.IsPeriodic () == true 
		) 
	{
		degree = _basecurve.GetDegreeOfBasisFunctions ();

		if ( degree == 2 )
			this->m = new BSplineM3;
		else if ( degree == 3 )
			this->m = new BSplineM4;
		else 
			m = 0;					
	}
	
	delta_v = _basecurve.m_V1 - _basecurve.m_V0;
	knots = new BSplineKnots ( (_basecurve.m_M + 1) , (_basecurve.m_K) , _basecurve.m_V0 , _basecurve.m_V1 , _basecurve.m_T );
}

//#############################################################################

PeriodicOpenBSplineCurve::~PeriodicOpenBSplineCurve ()
{
	delete m;
	delete knots;
}

//#############################################################################

VPoint PeriodicOpenBSplineCurve::GetPoint ( const float & f_v ) const
{
	if ( m == 0 )
		throw ( VException ( ( IDS_BSpCurExc) , 1 ) ); 
		
	int i = knots->GetStarti ( f_v );
	float baseknot = knots->GetKnot ( i );
	float v = f_v - baseknot;

	if ( degree == 2 )
	{
		int pointno = (i - knots->Getk()) + 1;
		Matrix cpoints ( 3 , 3 );
		
		for ( int row = 1; row <= 3; row++ )
		{
			cpoints.SetElement ( _basecurve.controlpoints[pointno*3] , row , 1 );
			cpoints.SetElement ( _basecurve.controlpoints[pointno*3+1] , row , 2 );
			cpoints.SetElement ( _basecurve.controlpoints[pointno*3+2] , row , 3 );
			pointno++;
		}
		
		Matrix temp = m->m * cpoints;

		Matrix vm ( 1 , 3 );
		vm.SetElement ( v * v , 1 , 1 );
		vm.SetElement ( v , 1 , 2 );
		vm.SetElement ( 1.0 , 1 , 3 );
		Matrix resultm = vm * temp;
		
		return ( VPoint ( resultm.GetElement ( 1 , 1 ) , resultm.GetElement ( 1 , 2 ) , resultm.GetElement ( 1 , 3 ) ) );
	}
	else if ( degree == 3 )
	{
		int pointno = (i - knots->Getk()) + 1;
		Matrix cpoints ( 4 , 3 );
		
		for ( int row = 1; row <= 4; row++ )
		{
			cpoints.SetElement ( _basecurve.controlpoints[pointno*3] , row , 1 );
			cpoints.SetElement ( _basecurve.controlpoints[pointno*3+1] , row , 2 );
			cpoints.SetElement ( _basecurve.controlpoints[pointno*3+2] , row , 3 );
			pointno++;
		}
		
		Matrix temp = m->m * cpoints;

		Matrix vm ( 1 , 4 );
		vm.SetElement ( v * v * v , 1 , 1 );
		vm.SetElement ( v * v , 1 , 2 );
		vm.SetElement ( v , 1 , 3 );
		vm.SetElement ( 1.0 , 1 , 4 );
		Matrix resultm = vm * temp;
		
		return ( VPoint ( resultm.GetElement ( 1 , 1 ) , resultm.GetElement ( 1 , 2 ) , resultm.GetElement ( 1 , 3 ) ) );
	}

	
	return ( VPoint ( 0 , 0 , 0 ) );
}

//#############################################################################

const BSplineKnots & PeriodicOpenBSplineCurve::GetKnots () const
{
	return (*knots);
}
