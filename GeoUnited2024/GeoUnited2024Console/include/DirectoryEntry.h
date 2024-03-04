/******************************************************************************
*               File: DirectoryEntry.h											  *
*******************************************************************************
*               Description:                                                  *
*                                                                             *
*******************************************************************************
*               History:                                                      *
*  27.11.2003 07:34:24 Created by:    Ata Hurdogan Demiray                    *
*******************************************************************************
*               (C) 2003 by Pozitif GmbH	                                  *
******************************************************************************/

#ifndef __DirectoryEntry_h__
#define __DirectoryEntry_h__

/* INFO

|1||     8 9|||   16 17|||  24 |25||   32|33||   40 41|||  48 |49||   56|57||   64 65|||  72 |73||   80|||
|__________|_________|_________|_________|__________|_________|_________|__________|_________|_________|_
|          |         |         |         |          |         |         |          |         |         |
|   (1)    |  (2)    |   (3)   |   (4)   |   (5)    |  (6)    |   (7)   |   (8)    |  (9)    |  (10)   |
| Entity   | Para-   |Structure|  Line   |  Level   | View    |Transfor-|  Label   | Status  |Sequence |
|  Type    | meter   |         |         |          |         |         | Display  |         |         |
|          |         |         |  Font   |          |         | mation  |          |Number   |Number   |
|Number    | Data    |         |Pattern  |          |         | Matrix  | Assoc.   |         |         |
|   #      |   )     | #; )    |  #; )   |  #; )    |  0; )   |  0; )   |  0; )    |   #     |  D #    |
|          |         |         |         |          |         |         |          |         |         |
|__________|_________|_________|_________|__________|_________|_________|__________|_________|_________|_
|          |         |         |         |          |         |         |          |         |         |
|  (11)    |  (12)   |  (13)   |  (14)   |   (15)   |  (16)   |  (17)   |   (18)   |  (19)   |  (20)   |
| Entity   |         |         |         |          |         |         | Entity   | Entity  |Sequence |
|          | Line    | Color   |  Para-  |  Form    |Reserved |Reserved |          |         |         |
|  Type    |Weight   |Number   | meter   |Number    |         |         |  Label   |Subscript|Number   |
|Number    |Number   |         |  Line   |          |         |         |          |Number   |         |
|          |         |         | Count   |          |         |         |          |         |         |
|          |         |         |         |          |         |         |          |         |         |
|   #      |   #     | #; )    |   #     |    #     |         |         |          |   #     |D # + 1  |
|__________|_________|_________|_________|__________|_________|_________|__________|_________|_________|_

Nomenclature:
     (n)    - Field number n
     #      - Integer
     )      - Pointer
     #; )   - Integer or pointer (pointer has negative sign)
     0; )   - Zero or pointer


             Figure 3. Format of the Directory Entry (DE) Section in the ASCII Form



                                                   18
                                                                                 2.2  ASCII FORM




                               Table 2. Directory Entry (DE) Section



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

*/

#include "ProcessLine.h"

class DirectoryEntry
{
public:
	DirectoryEntry();
	DirectoryEntry ( const DirectoryEntry & Foo );
	//~DirectoryEntry();
	void GetDirectoryEntryIn ( FileLines & mylines , int poffs);
	//void SetStandardColor (int mycolor);
	bool IsVisible ()const;
	void GetStatus (VChr status[8]);
	void SetStatusNumber (VChr blankStatus[2] , VChr subordinateEntitySwitch[2] , VChr entityUseFlag[2] , VChr hierarchy[2]);
	void SetVisible (bool isvisible);
//private:
	//vreal cRed , cGreen , cBlue;
	int pColor;
	int mlayer;
	int pXform;
	int m_parameterBegin;
	int m_parameterLineCount;
	int m_formNo;
	int m_statusNumber;
};

#endif //__DirectoryEntry_h__
