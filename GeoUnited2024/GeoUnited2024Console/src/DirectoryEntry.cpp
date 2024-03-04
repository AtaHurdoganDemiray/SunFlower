// DirectoryEntry.h
/*
No.__ Field_Name____                    Meaning_and_Notes_______

 1    Entity Type Number                Identifies the entity type.
 2    Parameter Data                    Pointer to the first line of the parameter data record for
                                        the entity. The letter P is not included.
 3    Structure                         Negated pointer to the directory entry of the definition
                                        entity that specifies this entity's meaning. The letter D is
                                        not included. The integer values 0, 1, and 2 are permissible
                                        in this field but should be disregarded.
 4    Line Font Pattern                 Line font pattern or negated pointer to the directory entry
                                        of a Line Font Definition Entity (Type 304).
 5    Level                             Number of the level upon which the entity resides, or a
                                        negated pointer to the directory entry of a Definition Levels
                                        Property Entity (Type 406, Form 1) which contains a list
                                        of levels upon which the entity resides.
 6    View                              Pointer to the directory entry of a View Entity (Type 410),
                                        or pointer to a Views Visible Associativity Instance (Type
                                        402, Form 3 or 4), or integer zero (default).
 7    Transformation Matrix             Pointer to the directory entry of a Transformation Matrix
                                        Entity (Type 124) used in defining this entity; zero (de-
                                        fault) implies the identity transformation matrix and zero
                                        translation vector will be used.
 8    Label Display Associativity       Pointer to the directory entry of a label Display Associa-
                                        tivity (Type 402, Form 5). The value of zero indicates no
                                        label display associativity.
 9    Status Number                     Provides four two-digit status values which are entered
                                        from left to right in the status number field in the order
                                        given below.
                                        1-2 Blank Status
                                             00 Visible
                                             01 Blanked
                                        3-4 Subordinate Entity Switch
                                             00 Independent
                                             01 Physically Dependent
                                             02 Logically Dependent
                                             03 Both (01) and (02)
                                        5-6 Entity Use Flag
                                             00 Geometry
                                             01 Annotation
                                             02 Definition
                                             03 Other
                                             04 Logical/Positional
                                             05 2D Parametric
                                        7-8 Hierarchy
                                             00 Global top down
                                             01 Global defer
                                             02 Use hierarchy property
 10   Section  Code  and  Sequence      Physical count of this line from the beginning of the Direc-
      Number                            tory Entry Section, preceded by the letter D (odd number).
 11   Entity Type Number               (Same as Field 1.)



                                                  19
           2.2  ASCII FORM



           12    Line Weight Number                System display thickness; given as a gradation value in the
                                                   range of 0 to the maximum (Parameter 16 of the Global
                                                   Section).
           13    Color Number                      Color number or negated pointer to the directory entry of
                                                   a Color Definition Entity (Type 314).
           14    Parameter Line Count              Number of lines in the parameter data record for this en-
                 Number                            tity.
           15    Form Number                       Certain entities have different interpretations.  These in-
                                                   terpretations are uniquely identified by a form number.
                                                   Possible form numbers are listed within each entity de-
                                                   scription.
           16    Reserved for future use
           17    Reserved for future use
           18    Entity Label                      Up to eight alphanumeric VChracters (right justified).
           19    Entity Subscript Number           1 to 8 digit unsigned number associated with the label.
           20    Section  Code  and  Sequence      Same meaning as Field 10 (even number).
                 Number

                               _______________________________________
                                |__Color_No.__|________Color_________|_|||

                                |       0       |No color assigned    |
                                |       1       |Black                |
                                |       2       |Red                  |
                                |       3       |Green                |
                                |       4       |Blue                 |
                                ||      5       |Yellow|              ||

                                ||      6       |Magenta|             ||

                                |       7       |Cyan                 |
                                |_______8_______|White________________|_

*/


#include "stdafx.h"
#include "DirectoryEntry.h"


DirectoryEntry::DirectoryEntry()
{
	mlayer = 1;
	pXform = 0;

	pColor = 3;//default color green
	//cRed = 0.;
	//cGreen = 1.;
	//cBlue = 0.;

	m_parameterLineCount = 0;
	m_formNo = 0;
	m_statusNumber = 0;
}

DirectoryEntry::DirectoryEntry ( const DirectoryEntry & Foo )
{
	//cRed = Foo.cRed;
	//cGreen = Foo.cGreen;
	//cBlue = Foo.cBlue;
	pColor = Foo.pColor;
	mlayer = Foo.mlayer;
	pXform = Foo.pXform;
	m_parameterLineCount = Foo.m_parameterLineCount;
	m_formNo = Foo.m_formNo;
	m_statusNumber = Foo.m_statusNumber;
}

void DirectoryEntry::GetDirectoryEntryIn ( FileLines & mylines , int poffs)
{
	ProcessLine * currline = mylines[0];
	DoubleVector myvect1;
	currline->GetDirectoryEntry(myvect1);
	if (myvect1.size()<=8)
		throw VException (( IDS_DirGetExcNotEno),1);

	mlayer = static_cast<int>(myvect1[4]);
	pXform = static_cast<int>(myvect1[6]);
	m_statusNumber = static_cast<int>(myvect1[8]);
	if (pXform)
		pXform += poffs;
	
	DoubleVector myvect2;
	currline = mylines[1];
	currline->GetDirectoryEntry(myvect2);

	int mycolor = static_cast<int>(myvect2[2]);	
	m_parameterLineCount = static_cast<int>(myvect2[3]);
	m_formNo = static_cast<int>(myvect2[4]);

	if ( mycolor > 8  || mycolor == 0)
	{
		pColor = 3;
	}
	else if ( mycolor < 0 )
	{
		pColor = mycolor - poffs;
	}
	else
	{
		pColor = mycolor;
	}
}

//void DirectoryEntry::SetStandardColor (int mycolor)
//{
//	switch (mycolor)
//	{
//	case 0:
//		// no color
//		cRed = 0.0;
//		cGreen = 1.0;
//		cBlue = 0.0; // default color green
//		break;
//	case 1:
//		// black
//		cRed = 0.0;
//		cGreen = 0.0;
//		cBlue = 0.0;
//		break;
//	case 2:
//		// red
//		cRed = 1.0;
//		cGreen = 0.0;
//		cBlue = 0.0;
//		break;
//	case 3:
//	// green
//		cRed = 0.0;
//		cGreen = 1.0;
//		cBlue = 0.0;
//		break;
//	case 4:
//		// blue
//		cRed = 0.0;
//		cGreen = 0.0;
//		cBlue = 1.0;
//		break;
//	case 5:
//		// yellow
//		cRed = 1.0;
//		cGreen = 1.0;
//		cBlue = 0.0;
//		break;
//	case 6:
//		// magenta
//		cRed = 1.0;
//		cGreen = 0.0;
//		cBlue = 1.0;
//		break;
//	case 7:
//		// cyan
//		cRed = 0.0;
//		cGreen = 1.0;
//		cBlue = 1.0;
//		break;
//	case 8:
//		// white
//		cRed = 1.0;
//		cGreen = 1.0;
//		cBlue = 1.0;
//		break;
//
//	}
//}

bool DirectoryEntry::IsVisible ()const
{
	if (m_statusNumber == 0)
		return true;
	else
	{
		VOStrStream str;
		str << m_statusNumber;
		VStr vis(str.str());
		if (vis.length() < 7)
			return true;
		if (vis.length() == 7 && vis[0] == '1')
			return false;
		if (vis.length() == 8 && vis[0]=='0' && vis[1] == '1')
			return false;
		else return true;
	}
}

void DirectoryEntry::SetStatusNumber (VChr blankStatus[2] , VChr subordinateEntitySwitch[2] , VChr entityUseFlag[2] , VChr hierarchy[2])
{
	VStrStream str;
	str << blankStatus[0] << blankStatus[1] << subordinateEntitySwitch[0] << subordinateEntitySwitch[1] << entityUseFlag[0] << entityUseFlag[1] << hierarchy[0] << hierarchy[1];
	str >> m_statusNumber;
}

void DirectoryEntry::GetStatus(VChr status[])
{
	VOStrStream str;
	str << setfill ('0');
	str << setw(8);
	str << m_statusNumber;
	VStr s(str.str());
	for (int i=0;i<8;++i)
		status[i]=s[i];
}

void DirectoryEntry::SetVisible (bool isvisible)
{
	VChr status[8];
	GetStatus(status);
	VChr a('0');
	if (isvisible == false)
		a = '1';
	VChr blankStatus[2] = {'0',a};
	VChr subordinateEntitySwitch[2] = {status[2],status[3]};
	VChr entityUseFlag [2] = {status[4],status[5]};
	VChr hierarchy [2] = {status[6],status[7]};
	SetStatusNumber(blankStatus,subordinateEntitySwitch,entityUseFlag,hierarchy);
}
