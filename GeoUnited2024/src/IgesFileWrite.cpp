// IgesFileWrite.cpp

#include "stdafx.h"
#include "IgesFileWrite.h"
#include "IgesEntitySet.h"
#include "IgesEntity.h"
#include <iomanip>

#include "TypeConverters.h"
#include "GlobalSection.h"
#include "LongVec.h"

int IgesFileWrite::IGES_LINE_END = 72;
int IgesFileWrite::PARAMETER_LINE_END = 64;

IgesFileWrite::IgesFileWrite ()
{
	m_directorylineno = 1;
	m_parameterlineno = 1;
}

void IgesFileWrite::WriteOut (const IgesEntitySet &entset , VofStream &outfile)
{
	WriteStartSection (outfile);
	WriteGlobalSection (outfile);
	WriteDirectoryEntrySection (entset , outfile);
	WriteParameterDataSection (entset , outfile);
	WriteTerminateSection (outfile);
}

//void IgesFileWrite::WriteOut (const IgesEntitySet &entset , VofStream &outfile , const LongVec &subset)
//{
//	WriteStartSection (outfile);
//	WriteGlobalSection (outfile);
//	WriteDirectoryEntrySection (entset , outfile , subset);
//	WriteParameterDataSection (entset , outfile , subset);
//	WriteTerminateSection (outfile);
//}

void IgesFileWrite::WriteGlobalSection (VofStream &outfile)
{
	GlobalSection temp;//with default parameters
	//write as default
	m_globallineno = temp.WriteGlobalSectionOut (outfile);
}

void IgesFileWrite::WriteDirectoryEntrySection (const IgesEntitySet &entset , VofStream &outfile)
{
	for (int i=0; i<entset.Size();i++)
	{
		IgesEntity *currEntity = entset[i];
		WriteDirectoryEntry (*currEntity , outfile);
	}
}

void IgesFileWrite::WriteDirectoryEntrySection (const IgesEntitySet &entset , VofStream &outfile , const LongVec &subset)
{
	int mysize = subset.Size();
	for (int i=0; i<mysize;i++)
	{
		int currde = subset[i];
		IgesEntity *currEntity = entset.GetEntityPointer(currde);
		WriteDirectoryEntry (*currEntity , outfile);
	}
}

void IgesFileWrite::WriteParameterDataSection (const IgesEntitySet &entset , VofStream &outfile)
{
	for (int i=0;i<entset.Size();i++)
	{
		IgesEntity *currEntity = entset[i];
		WriteParameterData (*currEntity , outfile);
	}
}

void IgesFileWrite::WriteParameterDataSection (const IgesEntitySet &entset , VofStream &outfile , const LongVec &subset)
{
	int mysize = subset.Size();

	for (int i=0;i<mysize;i++)
	{
		int currde = subset[i];
		IgesEntity *currEntity = entset.GetEntityPointer(currde);
		WriteParameterData (*currEntity , outfile);
	}
}

void IgesFileWrite::WriteDirectoryEntry (const IgesEntity &ent , VofStream &outfile)
{
	ProcessLine temp;
	//First line
	//Entity no
	temp.WriteIntAsEightChar (ent.entitytype , outfile); 
	//Parameter line no
	temp.WriteIntAsEightChar (ent.DE.m_parameterBegin , outfile);
	//Structure
	temp.WriteIntAsEightChar (1 , outfile);
	//Font pattern
	temp.WriteIntAsEightChar (1 , outfile);
	//Level
	temp.WriteIntAsEightChar (ent.DE.mlayer , outfile);
	//View
	temp.WriteIntAsEightChar (0 , outfile);
	//Transformation matrix
	temp.WriteIntAsEightChar (ent.DE.pXform , outfile);
	//Label display
	temp.WriteIntAsEightChar (0 , outfile);
	//Status no
	temp.WriteIntAsEightChar (ent.DE.m_statusNumber , outfile,'0');
	//Sequence no
	outfile << 'D';
	outfile << setw (7);
	outfile << ent.directorylineno;//m_directorylineno;
	outfile << ( IDS_N);
	++ m_directorylineno;
	//Second line
	//Entitytype no
	temp.WriteIntAsEightChar (ent.entitytype , outfile); 
	//Line weight no
	temp.WriteIntAsEightChar (0 , outfile); 
	//Color no
	temp.WriteIntAsEightChar (ent.DE.pColor , outfile); 
	//Parameter line count
	temp.WriteIntAsEightChar (ent.DE.m_parameterLineCount , outfile);
	// Form no
	temp.WriteIntAsEightChar (ent.DE.m_formNo , outfile);
	// 8x3 empty space
	for(int i=0;i<24;i++)
		outfile << ' ';
	//Status no
	temp.WriteIntAsEightChar (0,outfile);
	//Sequence no
	outfile << 'D';
	outfile << setw (7);
	outfile << (ent.directorylineno +1);//m_directorylineno;
	outfile << ( IDS_N);
	++ m_directorylineno;
}

void IgesFileWrite::WriteParameterData (const IgesEntity &ent , VofStream &outfile)
{
	//int mydeno = no;//* 2 + 1;

	std::vector<VStr> strvec;
	ent.WriteEntityOut (strvec);
	int deNo = ent.directorylineno;
	std::vector<VStr>::const_iterator i = strvec.begin();
	int linespent = 0;
	for(unsigned int j=0;j<strvec.size();j++)
	{
		VStr delimiter = (",");
		if (j==strvec.size()-1)
			delimiter = (";");
		bool lineHasEnded = WriteStringOut (*i , outfile , linespent,PARAMETER_LINE_END,delimiter);
		if (lineHasEnded)
		{
			WriteLineEnd ((" "),ent.directorylineno,outfile);
			WriteLineEnd (( IDS_P2) , m_parameterlineno,outfile);
			outfile<<( IDS_N);
			WriteStringOut (*i,outfile,linespent,PARAMETER_LINE_END,delimiter);
			m_parameterlineno++;
		}
		i++;
	}
	VStr tmp (("                                                                                        "));
	WriteStringOut (tmp , outfile , linespent , PARAMETER_LINE_END );
	WriteLineEnd ((" "),ent.directorylineno , outfile);
	WriteLineEnd (( IDS_P2) , m_parameterlineno , outfile);
	m_parameterlineno++;
	outfile<<( IDS_N);
}

void IgesFileWrite::WriteStartSection (VofStream &outfile)
{
	outfile << ( IDS_BakAciIgeCikDos);	
}

void IgesFileWrite::WriteTerminateSection (VofStream &outfile)
{
	WriteLineEnd (( IDS_S) , 1 , outfile);
	WriteLineEnd (( IDS_G) , m_globallineno , outfile);
	WriteLineEnd (( IDS_D1) , m_directorylineno , outfile);
	WriteLineEnd (( IDS_P2) , m_parameterlineno , outfile);
	outfile << ("                                        ");//40 empty spaces
	outfile << ( IDS_T1);
}

//returns true when line ends
bool IgesFileWrite::WriteStringOut (const VStr &toWrite , VofStream &outfile , int &linespent , const int lineend,const VStr delimiter)
{
	bool lineHasEnded = false;
	//end of line
	if (linespent + (int)(toWrite.length()+1) > lineend)
	{
		for (int j=0;j<lineend - linespent;j++)
		{
			outfile<<(" ");
		}
		//outfile << ("\n");
		linespent = 0;
		lineHasEnded = true;
		return lineHasEnded;
	}
	//exceptionally long parameter
	if ((int)(toWrite.length()+1) > lineend)
	{
		throw VException (( IDS_IgeWriExc),1);
	}
	//regular writing
	outfile << toWrite.data() << delimiter.data();
	linespent+=(toWrite.length()+1);
	return lineHasEnded;
}

bool IgesFileWrite::WriteIntegerOut (const int toWrite , VofStream &outfile, int &linespent , const int lineend,const VStr delimiter)
{
	VStr mystr = TypeConverters::ConvertIntToString (toWrite); 
	return WriteStringOut (mystr , outfile , linespent,lineend);
}

bool IgesFileWrite::WritevrealOut (const vreal &toWrite , VofStream &outfile , int &linespent , const int lineend,const VStr delimiter)
{
	VStr mystr = TypeConverters::ConvertvrealToString (toWrite);
	return WriteStringOut (mystr , outfile , linespent,lineend);
}

bool IgesFileWrite::WriteFloatOut (const float &toWrite , VofStream &outfile , int &linespent,const int lineend,const VStr delimiter)
{
	VStr mystr = TypeConverters::ConvertFloatToString (toWrite);
	return WriteStringOut (mystr , outfile , linespent,lineend);
}

void IgesFileWrite::WriteLineEnd (VStr sectionHeader , const int lineNumber , VofStream &outfile)
{
	VStr lineNo = TypeConverters::ConvertIntToString (lineNumber);
	VStr emptyspace = ("0");
	int spaceNumber = 7 - lineNo.length();
	VStr lineEnd (sectionHeader); 
	for (int j=0;j<spaceNumber;j++)
		lineEnd+=emptyspace;
	lineEnd += lineNo;
	outfile << lineEnd.data();
}

int IgesFileWrite::GetParameterLineNo()const
{
	return m_parameterlineno;
}

void IgesFileWrite::Clear()
{
	m_directorylineno = 1;
	m_parameterlineno = 1;
	//m_globallineno = 0;
}
