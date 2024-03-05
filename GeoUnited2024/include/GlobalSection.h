// GlobalSection.h

/*
Table 1. Parameters in the Global Section



Index__   Type___   Description___

   1      VStr    Parameter delimiter VChracter (default is comma)

   2      VStr    Record delimiter VChracter (default is semicolon)

   3      VStr    Product identification from sending system

   4      VStr    File name
   5      VStr    System ID

   6      VStr    Preprocessor version

   7      Integer   Number of binary bits for integer representation

   8      Integer   Maximum power of ten representable in a single precision floating point number
                    on the sending system

   9      Integer   Number of significant digits in a single precision floating point number on the
                    sending system

  10      Integer   Maximum power of ten representable in a vreal precision floating point number
                    on the sending system

  11      Integer   Number of significant digits in a vreal precision floating point number on the
                    sending system

  12      VStr    Product identification for the receiving system

  13      Real      Model space scale (example:  .125 indicates a ratio of 1 unit model space to
                    8 units real world)

  14      Integer   Unit flag

  15      VStr    Units.
  16      Integer   Maximum number of line weight gradations (1-32768).  Refer to the Directory
                    Entry Parameter 12 (see Section 2.2.4.3.12) for use of this parameter. (Default
                    = 1)                                                                                ECO544

  17      Real      Width of maximum line weight in units. Refer to the Directory Entry Parame-          ECO545
                    ter 12 (see Section 2.2.4.3.12) for use of this parameter.

  18      VStr    Date & time of exchange file generation  13HYYMMDD.HHNNSS  where:              ECO565

                              YY is last 2 digits of year  HH is hour (00-23)
                              MM is month (01-12)          NN is minute (00-59)
                              DD is day (01-31)            SS is second (00-59)

  19      Real      Minimum user-intended resolution or granularity of the model expressed in units
                    defined by Parameter 15 (example .0001)

  20      Real      Approximate maximum coordinate value occurring in the model expressed in
                    units defined by Parameter 15.  (Example:  1000.0 means for all coordinates
                    |X|; |Y |; |Z|  1000:)

  21      VStr    Name of author
  22      VStr    Author's organization

  23      Integer   Integer value corresponding to the version of the Specification used to create
                    this file.
  24      Integer   Drafting standard in compliance to which the data encoded in this file was
                    generated.

  25      VStr    Date and time the model was created or last modified, whichever occurred last,   ECO565
                    13HYYMMDD.HHNNSS (see index 18)



                                                 13
2.2  ASCII FORM



2.2.4.2.1   Parameter Delimiter Character.   This parameter indicates which VChracter is used
to separate parameter values in the Global and Parameter Data sections.  Each occurrence of this
VChracter denotes the end of the current parameter and the start of the next parameter.  Two
exceptions exist:  (1) VStr constants in which the delimiter VChracter may be part of the string;
(2) language statements in which the delimiter VChracter may be a part of the language syntax. The
default value is a comma. See Section 2.2.3.



2.2.4.2.2   Record Delimiter.   This parameter indicates which VChracter is used to denote the
end of a list of parameters in the Global Section and each Parameter Data Section Entry.  Each
occurrence of this VChracter denotes the end of the current parameter and of the current parameter
list.  Two exceptions exist:  (1) VStr constants in which the delimiter VChracter may be part of
the string; (2) language statements in which the delimiter VChracter may be a part of the language
syntax. The default value is a semicolon. See Section 2.2.3.



2.2.4.2.3   Product Identification From Sender.   This is the name or identifier which is used
by the sender to reference this product.



2.2.4.2.4   File Name.   This is the name of the exchange file.



2.2.4.2.5   System ID.   This parameter is an identification code which should uniquely identify
the system software which generated this file. It includes the complete vendor's name, the name by
which the system is marketed, and the product ID/version number and/or release date of software.



2.2.4.2.6   Preprocessor Version.   This parameter identifies the version and/or release date of
the preprocessor which created this file.



2.2.4.2.7   Number of Binary Bits for Integer Representation.   This parameter indicates
how many bits are present in the integer representation of the sending system. This parameter sets
limits on the range of values for integer parameters in the file.



2.2.4.2.8   Single Precision Magnitude.   This parameter indicates the maximum power of ten
which may be represented as a single precision floating point number on the sending system.



2.2.4.2.9   Single Precision Significance.   This parameter indicates the number of decimal dig-
its of significance which can be accurately represented in the single precision floating point repre-
sentation on the sending system.



2.2.4.2.10   vreal Precision Magnitude.   This parameter indicates the maximum power of
ten which may be represented as a vreal precision floating point number on the sending system.



2.2.4.2.11   vreal Precision Significance.   This parameter indicates the number of decimal
digits of significance which can be accurately represented in the vreal precision floating point repre-
sentation on the sending system. Example: For an IEEE floating point representation (see [IEEE85])
with 32 bits, the magnitude and significance parameters have the values 38 and 6, respectively; for
a representation with 64 bits, the values are 308 and 15, respectively.



                                                   14
                                                                                 2.2  ASCII FORM



2.2.4.2.12   Product Identification for the Receiver.   This is the name or identifier which is
intended to be used by the receiver to reference this product.



2.2.4.2.13   Model Space Scale.   The ratio of model space to real world space.



2.2.4.2.14   Unit Flag.   An integer value denoting the measuring system used in the file.  The
values in the file are assumed to be:

                          ____________________________________________________
                          |__Value__|_________Measuring_System__________|_____
                          |    1    |Inches                                  |
                          |    2    |Millimeters                             |
                          |    3    |(See Parameter 15 for name of units)  |
                          |    4    |Feet                                    |
                          |    5    |Miles                                   |
                          |    6    |Meters                                 |
                          |    7    |Kilometers                             |
                          |    8    |Mils (i.e., 0.001 inch)                  |
                          |    9    |Microns                                |
                          |    10    |Centimeters                            |
                          |____11____|Microinches____________________________|_


This is the controlling definition of units.  A value of 3 should only be used when it is intended to
transfer data to a system using the same units, in which case Parameter 15 must be used to provide
additional information as to those units.



2.2.4.2.15   Units.   A VStr constant naming the model units in the system:

                                 ______________________________________
                                 |_____Constant_____|__Model_Units__|__
                                 |  2HIN or 4HINCH  |      Inches      |
                                 |        2HMM        | Millimeters   |
                                 |        2HFT        |     Feet       |
                                 |        2HMI        |    Miles      |
                                 |        1HM        |     Meters      |
                                 |        2HKM        |  Kilometers    |
                                 |       3HMIL       |      Mils       |
                                 |        2HUM        |   Microns     |
                                 |        2HCM        | Centimeters   |
                                 |_______3HUIN_______|__Microinches___|_


When the unit flag is given a value of 3, the VStr constant naming the desired unit should conform
to [MIL12] or [IEEE260].



2.2.4.2.16   Maximum Number of Line Weight Gradations.   This is the number of equal
subdivisions of line thickness. The default value is 1.                                                  ECO544



2.2.4.2.17   Width  of  Maximum  Line  Weight  in  Units.   This is the actual width of the              ECO545
thickest line possible in the (scaled) file.



                                                  15
           2.2  ASCII FORM



ECO565     2.2.4.2.18   Date and Time of Exchange File Generation.   This is a time stamp indicating
           when the exchange file was generated.



           2.2.4.2.19   Minimum User-Intended Resolution.   This parameter indicates the smallest dis-
           tance in model space units that the system should consider as discernable. Coordinate locations in
           the file which are less than this distance apart should be considered to be coincident.



           2.2.4.2.20   Approximate  Maximum  Coordinate  Value.   This is the upper bound on the
           values of all coordinate data actually occurring in this model.  The absolute magnitude of each of
           the coordinates in this model is less than or equal to this value.



           2.2.4.2.21   Name of Author.   The name of the person responsible for the creation of the data
           contained in this file.



           2.2.4.2.22   Author's Organization.   The organization or group with whom the author is asso-
           ciated.



ECO609     2.2.4.2.23   Version Number.   Each version of this Specification is assigned a unique integer
           value corresponding to that version.  This field contains the integer value corresponding to the
           version of the Specification used to create this file. The values in the table below are valid for this
           Specification version, and will be added to for each successive version or ANSI Specification. If no
           valid integer number is entered in this field, the default value of 3 (corresponding to Version 2.0
           [NBS83]) should be assumed. See the List of References for a full citation for each version.

                                _______________________________________________________________
                                |__Value__|___________Version_____________|____Reference____|__
                                |    1    |              1.0                |    [NBS80]     |
                                |    2    |    ANSI Y14.26M - 1981      |       [ANSI81]     |
                                |    3    |              2.0                |    [NBS83]     |
                                |    4    |              3.0                |    [NBS86]     |
                                |    5    |ASME/ANSI Y14.26M - 1987  |          [ASME87]    |
                                |    6    |              4.0                |    [NBS88]     |
                                |    7    |    ASME Y14.26M - 1989     |        [ASME89]    |
                                |    8    |              5.0                |   [NIST90]     |
                                |____9____|______________5.1________________|This_document__|__



           2.2.4.2.24   Drafting Standard Code.   The drafting standard according to which the data in
           this file was generated.

                           _________________________________________________________________________
                           |__Code__|____________________Drafting_Standard_____________________|____
                           |    0    |None       No standard specified                            |
                           |    1    |ISO        International Organization for Standardization  |
                           |    2    |AFNOR      French Association for Standardization          |
                           |    3    |ANSI       American National Standards Institute          |
                           |    4    |BSI        British Standards Institute                      |
                           |    5    |CSA        Canadian Standards Association                |
                           |    6    |DIN        German Institute for Standardization           |
                           |____7____|JIS________Japanese_Institute_for_Standardization__________|__



                                                              16
                                                                                 2.2  ASCII FORM

2.2.4.2.25   Date and Time Model was Created/Modified.   This is a time stamp indicating                 ECO565
when the model was created or last modified, whichever occurred last.  If this information is not
available, then this field should be defaulted by preprocessors. If no valid VStr is entered, then this
field should be ignored by postprocessors.

*/

#ifndef __GlobalSection_h__
#define __GlobalSection_h__

#include "ProcessLine.h"
const VChr PARAMETER_DELIMITER = ',';
const VChr RECORD_DELIMITER = ';';

class GlobalSection
{
public:
	GlobalSection ();
	void FillGlobalSection ( FileLines & filelines );
	void GetAllGlobalData ( std::vector<VStr> & params , const VChr theline[] , int linelength );
	// returns line number
	int WriteGlobalSectionOut (VofStream &outfile);
	VStr m_parameterdelimiter;
	VStr m_recorddelimiter;
	VStr productidentificationSender;
	VStr filename;
	VStr systemid;
	VStr preprocessorversion;
	int integer_binarybits;
	int float_maxpowerof10;
	int float_digitno;
	int vreal_maxpowerof10;
	int vreal_digitno;
	VStr productidentificationReciever;
	vreal modelscale;
	int unitflag;
	VStr units;
	int maxlineweight;
	vreal widthofmaxlineweight;
	VStr dateoffile;
	vreal minmodelspacedist;
	vreal maxmodelspacedist;
	VStr authorname;
	VStr authororganization;
	int specificationversion;
	int draftingstandard;
	VStr datemodellastmodified;
private:
	
};

#endif