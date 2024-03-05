/******************************************************************************
*               File: Stipple.cpp											  *
*******************************************************************************
*               Description:                                                  *
*                                                                             *
*******************************************************************************
*               History:                                                      *
*  08.10.2003 15:41:49 Created by:    Ata Hurdogan Demiray                    *
*******************************************************************************
*               (C) 2003 by ModuleWorks GmbH                                  *
******************************************************************************/

#include "stdafx.h"
#include "Stipple.h"
#include <cmath>

Stipple::Stipple()
{
	patno = 0;
	pat[0] = new GLubyte[128] ;
	int i,j;
	for ( i = 0; i < 16; i++ )
		for ( j = 0; j < 8; j++ )
		{
			if ( j < 4 )
				pat[0][i*8+j] = 76;
			else
				pat[0][i*8+j] = 51;
		}
	
	pat[1] = new GLubyte[128] ;
	
	for ( i = 0; i < 16; i++ )
		for ( j = 0; j < 8; j++ )
		{
			if ( j < 4 )
				pat[1][i*8+j] = 153;
			else
				pat[1][i*8+j] = 102;
		}

	pat[2] = new GLubyte[128] ;
	
	for ( i = 0; i < 16; i++ )
		for ( j = 0; j < 8; j++ )
		{
			if ( j < 4 )
				pat[2][i*8+j] = 170;
			else
				pat[2][i*8+j] = 85;
		}
	
}

//#############################################################################

Stipple::~Stipple()
{

}

//#############################################################################

double Stipple::GetByteValue ( int tdegree )
{
	//tdegree can be 1 min 7 max
	if ( tdegree < 1 || tdegree > 7 )
		return -1;

	double val[8];
	double randn;

	for ( int i= 0; i < tdegree; i++ )
	{
		bool doit = true;

		while ( doit )
		{
			randn = rand () % 8;
			doit = false;

			for ( int j = 0; j < i; j++ )
				if ( val[j]==randn )
					doit = true;
		}
		val[i] = randn;
	}

	double byteval = 0.0;

	for ( i = 0; i < tdegree; i++ )
		byteval = byteval + pow ( 2 , val[i] );

	return byteval;
}

GLubyte * Stipple::CreatePattern( int tdegree )
{
		//tdegree can be 1 min 7 max
	if ( tdegree < 1 || tdegree > 7 )
		return NULL;

	double val;
	GLubyte * patt = new GLubyte[128];

	for ( int i = 0; i < 128; i++ )
	{
		if ( tdegree < 5 )
			val = GetByteValue ( tdegree );
		else
			val = 255 - GetByteValue( 8 - tdegree );
		
		patt[i] = val;
	}

	return patt;
}

//#############################################################################

GLubyte * Stipple::GetPattern()
{
	int retno = patno % 3;
	patno++;
	return pat[retno];
}

//#############################################################################



