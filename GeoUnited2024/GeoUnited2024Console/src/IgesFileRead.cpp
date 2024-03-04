/******************************************************************************
*               File: IgesFileRead.cpp											  *
*******************************************************************************
*               Description:                                                  *
*                                                                             *
*******************************************************************************
*               History:                                                      *
*  27.11.2003 21:52:10 Created by:    Ata Hurdogan Demiray                    *
*******************************************************************************
*               (C) 2003 by Pozitif GmbH	                                  *
******************************************************************************/

#include "stdafx.h"
#include "IgesFileRead.h"
#include "IgesEntity.h"
#include "GlobalSection.h"
#include "Output.h"
#include "UntypedException.h"
#include "ExceptionLogger.h"
#include <stdexcept>

IgesFileRead::IgesFileRead ( VStr newfilename , IgesEntitySet &entset)
:allentities (entset)
{
	filename = newfilename;
}

//#############################################################################

IgesFileRead::~IgesFileRead()
{

}

//#############################################################################

void IgesFileRead::FileRead (GlobalSection & myglobalsection)
{
	try
	{
	int poffs = allentities.GetPointerOffset();
	// open the file
	VIFStream theiges ( filename.c_str() );
	if (theiges.is_open() == false)
	{
		VStr msg (filename + ( IDS_AdrDosYok));
		throw VException (msg,1);
	}
	// vector for file line processing
	FileLines mylines;

	ProcessLine * aline = new ProcessLine;
	aline->GetLine ( theiges );
	
	while ( theiges.eof() == false )
	{
		if ( aline->GetSection() == 'G' ) // Global Section
		{
			while (aline->GetSection()=='G')
			{
				mylines.push_back (aline);
				aline=new ProcessLine;
				aline->GetLine ( theiges );
			}
			try
			{
				myglobalsection.FillGlobalSection(mylines);
			}
			catch ( exception & exc )
			{
				ExceptionLogger::log(exc, __FILE__, __LINE__);				
			}
			catch (...)
			{
				UntypedException uexc(( IDS_UnkExcInIgeFil),999);
				ExceptionLogger::log(uexc, __FILE__, __LINE__);
			}

			EmptyFileLines (mylines);
		}
		if ( aline->GetSection() == 'D' )// Directory Entry part 
		{
			mylines.push_back ( aline );
			aline = new ProcessLine;
			aline->GetLine ( theiges );
			mylines.push_back ( aline );
			CreateEntity ( mylines , poffs);
			EmptyFileLines( mylines );
			aline = new ProcessLine;
			aline->GetLine ( theiges );
		}
		else if ( aline->GetSection() == 'P' )// Parameter Data part 
		{
			mylines.push_back ( aline );
			aline = GetRestOfTheParameters ( theiges , mylines , poffs);

			IgesEntity *thenewentity =  
				allentities.GetEntityPointer ( mylines[0]->GetDEPointer(poffs) );
		
			if ( thenewentity->entitytype != 0)
				thenewentity->GetEntityIn ( mylines , poffs);

			EmptyFileLines ( mylines );
		}
		else
			aline->GetLine ( theiges );
	}
	theiges.close();
	}
	catch (std::exception &exc)
	{
		throw exc;
	}
	catch (...)
	{
		throw UntypedException (( IDS_IgeFilUnkExcOcc),1);
	}
}

//#############################################################################

void IgesFileRead::CreateEntity ( FileLines &mylines , int poffs)
{
	long myde, mype , mytype;
	mylines[0]->GetDeAndPeAndType ( myde , mype , mytype , poffs);
	DirectoryEntry tmp;
	tmp.GetDirectoryEntryIn ( mylines , poffs);

	if ( mytype == 126 )
		allentities.AddBSplineCurve ( myde , mype , tmp);
	else if ( mytype == 128 )
		allentities.AddBSplineSurface ( myde , mype ,tmp);
	else if ( mytype == 122 )
		allentities.AddTabulatedCylinder ( myde , mype ,tmp);	
	else if ( mytype == 120 )
		allentities.AddSurfaceOfRevolution ( myde , mype ,tmp);	
	else if ( mytype == 144 )
		allentities.AddTrimmedSurface ( myde , mype ,tmp);
	else if ( mytype == 142 )
		allentities.AddCurveOnParametricSurface ( myde , mype ,tmp);
	else if ( mytype == 102 )
		allentities.AddCompositeCurve ( myde , mype ,tmp);
	else if ( mytype == 141 )
		allentities.AddBoundary ( myde , mype ,tmp);
	else if ( mytype == 143 )
		allentities.AddBoundedSurface ( myde , mype ,tmp);
	else if ( mytype == 110 )
		allentities.AddLine ( myde , mype ,tmp);
	else if ( mytype == 116 )
		allentities.AddPoint ( myde , mype ,tmp);
	else if ( mytype == 100 )
		allentities.AddArc ( myde , mype ,tmp);
	else if ( mytype == 124 )
		allentities.AddTransformationMatrix ( myde , mype , tmp);
	else if ( mytype == 140 )
		allentities.AddOffsetSurface ( myde , mype , tmp );
	else if ( mytype == 314 )
		allentities.AddColorDefinition ( myde , mype , tmp );
	else if ( mytype == 106 )
		allentities.AddCopiousData ( myde , mype , tmp );
	else if ( mytype == 104 )
		allentities.AddConicArc ( myde , mype , tmp );
	else if ( mytype == 112 )
		allentities.AddParametricSpline ( myde , mype , tmp );
	else if ( mytype == 108 )
		allentities.AddPlaneEntity ( myde , mype , tmp );
	else if ( mytype == 118 )
		allentities.AddRuledSurface ( myde , mype , tmp );
	else if ( mytype == 212)
		allentities.AddGeneralNote (myde,mype,tmp);
	else if (mytype == 114)
		allentities.AddParametricSplineSurface (myde , mype , tmp);
	else if (mytype == 502)
		allentities.AddVertexEntity(myde , mype , tmp);
	else if (mytype == 504)
		allentities.AddEdgeEntity(myde , mype , tmp);
	else if (mytype == 308)
		allentities.AddSubFigureDefinition (myde,mype,tmp);
	else if (mytype == 408)
		allentities.AddSingularSubFigureInstance(myde, mype, tmp);
	else if (mytype == 190)
		allentities.AddPlaneSurfaceEntity(myde, mype, tmp);
	else if (mytype == 192)
		allentities.AddRightCircularCylindricalSurfaceEntity(myde, mype, tmp);
	else if (mytype == 194)
		allentities.AddRightCircularConicalSurfaceEntity(myde, mype, tmp);
	else if (mytype == 196)
		allentities.AddSphericalSurfaceEntity(myde, mype, tmp);
	else if (mytype == 198)
		allentities.AddToroidalSurfaceEntity(myde, mype, tmp);
	else if (mytype == 508)
		allentities.AddLoopEntity(myde, mype, tmp);
	else if (mytype == 510)
		allentities.AddFaceEntity(myde, mype, tmp);
	else if (mytype == 123)
		allentities.AddDirectionEntity(myde, mype, tmp);
	else
		allentities.AddNullElement(myde,mype,tmp);

}

//#############################################################################

ProcessLine * IgesFileRead::GetRestOfTheParameters ( VIFStream &theiges , FileLines &mylines , int poffs)
{
	bool goon = true;
	long comp = mylines[0]->GetDEPointer(poffs);
	ProcessLine * newline;
	
	while ( goon && !theiges.eof() )
	{
		newline = new ProcessLine;
		newline->GetLine ( theiges );
		
		long ck = newline->GetDEPointer (poffs);

		if ( ck == comp )
			mylines.push_back ( newline );
		else
			goon = false;

	}

	return newline;

}

//#############################################################################

void IgesFileRead::EmptyFileLines (FileLines &mylines)
{
	for ( unsigned int i = 0; i < mylines.size(); i++ )
	{
		delete mylines[i];
	}

	mylines.clear();
}

//#############################################################################
