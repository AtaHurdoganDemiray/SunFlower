// GlobalSection.cpp

#include "stdafx.h"
#include "GlobalSection.h"

#include "IgesFileWrite.h"

GlobalSection::GlobalSection ()
{
	m_parameterdelimiter = ( IDS_H2);
	m_recorddelimiter = ( IDS_H3);
	authorname = ( IDS_HBELIRSIZ);
	authororganization = ( IDS_HBELIRSIZ);
	datemodellastmodified = ( IDS_HBELIRSIZ);
	dateoffile = ( IDS_HBELIRSIZ);
	vreal_digitno = VGlobalValues::DOUBLE_SIGNIFICANT_DIGITS;
	vreal_maxpowerof10 = VGlobalValues::DOUBLE_MAX_POWER_OF_TEN;
	draftingstandard = 0;
	filename = ( IDS_HBELIRSIZ);
	float_digitno = VGlobalValues::FLOAT_SIGNIFICANT_DIGITS;
	float_maxpowerof10 = VGlobalValues::FLOAT_MAX_POWER_OF_TEN;
	integer_binarybits = 32;
	maxlineweight = 1;
	maxmodelspacedist = 1.e2;
	minmodelspacedist = 1.e-8;
	modelscale = 1;
	preprocessorversion = ( IDS_H4);
	productidentificationReciever = ( IDS_HBELIRSIZ);
	productidentificationSender = ( IDS_HBELIRSIZ);
	specificationversion = 5;
	systemid = ( IDS_HPoBakAci);
	unitflag = 2;//mm
	units = ( IDS_HMM);
	widthofmaxlineweight = 0.125;
}

void GlobalSection::FillGlobalSection ( FileLines & filelines )
{
	int linelength=0;
	VStr theline;
	for ( unsigned int i=0; i<filelines.size(); i++ )
	{
		ProcessLine * myline = filelines[i];
		
		for ( int i=0; i<72; i++ )
		{
			theline+=myline->GetChar(i);
			linelength++;
		}
	
	}
	
	std::vector<VStr> params;	
	GetAllGlobalData(params,theline.data(),linelength);
	
	VChr * stp;
	
	if (params.size()<25)
		throw(VException(( IDS_GloSecExc),0));

	m_parameterdelimiter=params[0];
	m_recorddelimiter=params[1];
	productidentificationSender=params[2];
	filename=params[3];
	systemid=params[4];
	preprocessorversion=params[5];
	integer_binarybits=strtol(params[6].data(),&stp,10);
	float_maxpowerof10=strtol(params[7].data(),&stp,10);
	float_digitno=strtol(params[8].data(),&stp,10);
	vreal_maxpowerof10=strtol(params[9].data(),&stp,10);
	vreal_digitno=strtol(params[10].data(),&stp,10);
	productidentificationReciever=params[11];
	modelscale=strtod(params[12].data(),&stp);
	unitflag=strtol(params[13].data(),&stp,10);
	units=params[14];
	maxlineweight=strtol(params[15].data(),&stp,10);
	widthofmaxlineweight=strtod(params[16].data(),&stp);
	dateoffile=params[17];
	minmodelspacedist=strtod(params[18].data(),&stp);
	maxmodelspacedist=strtod(params[19].data(),&stp);
	authorname=params[20];
	authororganization=params[21];
	specificationversion=strtol(params[22].data(),&stp,10);
	draftingstandard=strtol(params[23].data(),&stp,10);
	datemodellastmodified=params[24];

}

void GlobalSection::GetAllGlobalData ( std::vector<VStr> & params , const VChr theline[] , int linelength )
{
	int counter =0;
	VChr currVChr = ' ';
	bool gettingstring = false;
	VStr currstring;
	int stringlength=0;
	int stringcursor=0;
	VChr *stp;

	while ( counter < linelength )
	{
		currVChr = 	theline[counter];
		if ( gettingstring == false && currVChr == 'H' )
		{
			stringlength = strtol (currstring.data() , &stp , 10 );
			currstring.clear();
			gettingstring = true;
		}
		else if ( gettingstring == false 
			&& (currVChr == ',' || currVChr == ';' ))
		{
			params.push_back ( currstring );
			currstring.clear();
		}
		else
		{
			currstring += currVChr;
			if ( gettingstring )
			{
				stringcursor++;
			}

			if ( gettingstring && stringcursor == stringlength )
			{
				gettingstring = false;
				stringcursor = 0;
			}
		}
		counter++;
	};
	
}

int GlobalSection::WriteGlobalSectionOut (VofStream &outfile)
{
	int linespent = 0;
	bool lineHasEnded;
	int lineNumber = 1;	
	lineHasEnded = IgesFileWrite::WriteStringOut (m_parameterdelimiter , outfile , linespent);
	if (lineHasEnded)
	{
		IgesFileWrite::WriteLineEnd (( IDS_G),lineNumber , outfile);
		outfile<<( IDS_N);
		IgesFileWrite::WriteStringOut (m_parameterdelimiter , outfile , linespent);
		++lineNumber;
	}	
	lineHasEnded = IgesFileWrite::WriteStringOut (m_recorddelimiter , outfile , linespent);
	if (lineHasEnded)
	{
		IgesFileWrite::WriteLineEnd (( IDS_G),lineNumber , outfile);
		outfile<<( IDS_N);
		IgesFileWrite::WriteStringOut (m_recorddelimiter , outfile , linespent);
		++lineNumber;
	}
	
	lineHasEnded = IgesFileWrite::WriteStringOut (productidentificationSender , outfile , linespent);
	if (lineHasEnded)
	{
		IgesFileWrite::WriteLineEnd (( IDS_G),lineNumber , outfile);
		outfile<<( IDS_N);		
		IgesFileWrite::WriteStringOut (productidentificationSender , outfile , linespent);
		++lineNumber;
	}
	lineHasEnded = IgesFileWrite::WriteStringOut (filename , outfile , linespent);
	if (lineHasEnded)
	{
		IgesFileWrite::WriteLineEnd (( IDS_G),lineNumber , outfile);
		outfile<<( IDS_N);		
		IgesFileWrite::WriteStringOut (filename , outfile , linespent);
		++lineNumber;
	}
	lineHasEnded = IgesFileWrite::WriteStringOut (systemid , outfile , linespent);
	if (lineHasEnded)
	{
		IgesFileWrite::WriteLineEnd (( IDS_G),lineNumber,outfile);
		outfile<<( IDS_N);
		IgesFileWrite::WriteStringOut (systemid , outfile , linespent);
		++lineNumber;
	}
	lineHasEnded = IgesFileWrite::WriteStringOut (preprocessorversion , outfile , linespent);
	if (lineHasEnded)
	{
		IgesFileWrite::WriteLineEnd (( IDS_G),lineNumber , outfile);
		outfile<<( IDS_N);		
		IgesFileWrite::WriteStringOut (preprocessorversion , outfile , linespent);
		++lineNumber;
	}
	lineHasEnded = IgesFileWrite::WriteIntegerOut (integer_binarybits , outfile , linespent);
	if (lineHasEnded)
	{
		IgesFileWrite::WriteLineEnd (( IDS_G),lineNumber , outfile);
		outfile<<( IDS_N);		
		IgesFileWrite::WriteIntegerOut (integer_binarybits , outfile , linespent);
		++lineNumber;
	}
	lineHasEnded = IgesFileWrite::WriteIntegerOut (float_maxpowerof10 , outfile , linespent);
	if (lineHasEnded)
	{
		IgesFileWrite::WriteLineEnd (( IDS_G),lineNumber , outfile);
		outfile<<( IDS_N);		
		lineHasEnded = IgesFileWrite::WriteIntegerOut (float_maxpowerof10 , outfile , linespent);
		++lineNumber;
	}
	lineHasEnded = IgesFileWrite::WriteIntegerOut (float_digitno , outfile , linespent);
	if (lineHasEnded)
	{
		IgesFileWrite::WriteLineEnd (( IDS_G),lineNumber , outfile);
		outfile<<( IDS_N);		
		IgesFileWrite::WriteIntegerOut (float_digitno , outfile , linespent);
		++lineNumber;
	}
	lineHasEnded = IgesFileWrite::WriteIntegerOut (vreal_maxpowerof10 , outfile , linespent);
	if (lineHasEnded)
	{
		IgesFileWrite::WriteLineEnd (( IDS_G),lineNumber , outfile);
		outfile<<( IDS_N);		
		IgesFileWrite::WriteIntegerOut (vreal_maxpowerof10 , outfile , linespent);
		++lineNumber;
	}	
	lineHasEnded = IgesFileWrite::WriteIntegerOut (vreal_digitno , outfile , linespent);
	if (lineHasEnded)
	{
		IgesFileWrite::WriteLineEnd (( IDS_G),lineNumber , outfile);
		outfile<<( IDS_N);		
		IgesFileWrite::WriteIntegerOut (vreal_digitno , outfile , linespent);
		++lineNumber;
	}
	lineHasEnded = IgesFileWrite::WriteStringOut (productidentificationReciever , outfile , linespent);
	if (lineHasEnded)
	{
		IgesFileWrite::WriteLineEnd (( IDS_G),lineNumber , outfile);
		outfile<<( IDS_N);		
		IgesFileWrite::WriteStringOut (productidentificationReciever , outfile , linespent);
		++lineNumber;
	}	
	lineHasEnded = IgesFileWrite::WritevrealOut (modelscale , outfile , linespent);
	if (lineHasEnded)
	{
		IgesFileWrite::WriteLineEnd (( IDS_G),lineNumber , outfile);
		outfile<<( IDS_N);		
		IgesFileWrite::WritevrealOut (modelscale , outfile , linespent);
		++lineNumber;
	}	
	lineHasEnded = IgesFileWrite::WriteIntegerOut (unitflag , outfile , linespent);
	if (lineHasEnded)
	{
		IgesFileWrite::WriteLineEnd (( IDS_G),lineNumber , outfile);
		outfile<<( IDS_N);		
		IgesFileWrite::WriteIntegerOut (unitflag , outfile , linespent);		
		++lineNumber;
	}	
	lineHasEnded = IgesFileWrite::WriteStringOut (units , outfile , linespent);
	if (lineHasEnded)
	{
		IgesFileWrite::WriteLineEnd (( IDS_G),lineNumber , outfile);
		outfile<<( IDS_N);		
		IgesFileWrite::WriteStringOut (units , outfile , linespent);
		++lineNumber;
	}
	lineHasEnded = IgesFileWrite::WriteIntegerOut (maxlineweight , outfile , linespent);
	if (lineHasEnded)
	{
		IgesFileWrite::WriteLineEnd (( IDS_G) , lineNumber , outfile);
		outfile<<( IDS_N);
		IgesFileWrite::WriteIntegerOut (maxlineweight , outfile , linespent);
		++lineNumber;
	}
	lineHasEnded = IgesFileWrite::WritevrealOut (widthofmaxlineweight , outfile , linespent);
	if (lineHasEnded)
	{
		IgesFileWrite::WriteLineEnd (( IDS_G),lineNumber , outfile);
		outfile<<( IDS_N);		
		IgesFileWrite::WritevrealOut (widthofmaxlineweight , outfile , linespent);
		++lineNumber;
	}	
	lineHasEnded = IgesFileWrite::WriteStringOut (dateoffile , outfile , linespent);	
	if (lineHasEnded)
	{
		IgesFileWrite::WriteLineEnd (( IDS_G),lineNumber , outfile);
		outfile<<( IDS_N);		
		IgesFileWrite::WriteStringOut (dateoffile , outfile , linespent);	
		++lineNumber;
	}			
	lineHasEnded = IgesFileWrite::WritevrealOut (minmodelspacedist , outfile , linespent);
	if (lineHasEnded)
	{
		IgesFileWrite::WriteLineEnd (( IDS_G),lineNumber , outfile);
		outfile<<( IDS_N);		
		IgesFileWrite::WritevrealOut (minmodelspacedist , outfile , linespent);
		++lineNumber;
	}		
	lineHasEnded = IgesFileWrite::WritevrealOut (maxmodelspacedist , outfile , linespent);
	if (lineHasEnded)
	{
		IgesFileWrite::WriteLineEnd (( IDS_G),lineNumber , outfile);
		outfile<<( IDS_N);		
		IgesFileWrite::WritevrealOut (maxmodelspacedist , outfile , linespent);		
		++lineNumber;
	}	
	lineHasEnded = IgesFileWrite::WriteStringOut (authorname , outfile , linespent);
	if (lineHasEnded)
	{
		IgesFileWrite::WriteLineEnd (( IDS_G),lineNumber , outfile);
		outfile<<( IDS_N);		
		IgesFileWrite::WriteStringOut (authorname , outfile , linespent);	
		++lineNumber;
	}		
	lineHasEnded = IgesFileWrite::WriteStringOut (authororganization , outfile , linespent);
	if (lineHasEnded)
	{
		IgesFileWrite::WriteLineEnd (( IDS_G),lineNumber , outfile);
		outfile<<( IDS_N);		
		IgesFileWrite::WriteStringOut (authororganization , outfile , linespent);		
		++lineNumber;
	}
	lineHasEnded = IgesFileWrite::WriteIntegerOut (specificationversion , outfile , linespent);
	if (lineHasEnded)
	{
		IgesFileWrite::WriteLineEnd (( IDS_G),lineNumber,outfile);
		outfile<<( IDS_N);
		IgesFileWrite::WriteIntegerOut (specificationversion , outfile , linespent);
		++lineNumber;
	}
	lineHasEnded = IgesFileWrite::WriteIntegerOut (draftingstandard , outfile , linespent);
	if (lineHasEnded)
	{
		IgesFileWrite::WriteLineEnd (( IDS_G),lineNumber,outfile);
		outfile<<( IDS_N);
		IgesFileWrite::WriteIntegerOut(draftingstandard , outfile,linespent);
		++lineNumber;
	}
	lineHasEnded = IgesFileWrite::WriteStringOut (datemodellastmodified , outfile , linespent , IgesFileWrite::IGES_LINE_END , (";"));
	if (lineHasEnded)
	{
		IgesFileWrite::WriteLineEnd (( IDS_G),lineNumber , outfile);
		outfile<<( IDS_N);		
		IgesFileWrite::WriteStringOut (datemodellastmodified , outfile , linespent , IgesFileWrite::IGES_LINE_END , (";"));		
		++lineNumber;
	}
	VStr tmp (("                                                                                        "));
	IgesFileWrite::WriteStringOut (tmp , outfile , linespent);
	IgesFileWrite::WriteLineEnd (( IDS_G) , lineNumber , outfile);
	outfile<<( IDS_N);
	return lineNumber;
}
