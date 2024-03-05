// ColorDefinition.h

/*
 Color Definition Entity (Type 314)

Directory Entry

|_________|_________|__________|_________|_________|__________|_________|_________|__________|_________|_
|         |         |          |         |         |          |         |         |          |         |
|   (1)   |   (2)   |   (3)    |   (4)   |   (5)   |   (6)    |  (7)    |   (8)   |   (9)    |  (10)   |
|Entity Ty|peParamet|erStructur|eLine Fon|t Level  |   View   |Xformatio|n Label  |  Status  | Sequence|
| Number  |   Data  |          | Pattern |         |          | Matrix  |  Display|  Number  | Number  |
|         |         |          |         |         |          |         |         |          |         |
|  314    |   )     |< n:a: >  |< n:a: > |< n:a: > |< n:a: >  |< n:a: > |< n:a: > |**0002**  |  D #    |
|_________|_________|__________|_________|_________|__________|_________|_________|__________|_________|_
|         |         |          |         |         |          |         |         |          |         |
|  (11)   |  (12)   |   (13)   |  (14)   |  (15)   |   (16)   |  (17)   |  (18)   |   (19)   |  (20)   |
|Entity Ty|pe Line  |   Color  |Parameter|  Form   | Reserved | Reserved|  Entity |  Entity  | Sequence|
| Number  |  Weight |  Number  |Line Coun|tNumber  |          |         |  Label  | Subscript| Number  |
|         |         |          |         |         |          |         |         |          |         |
|  314    |< n:a: > |    #     |   #     |    0    |          |         |         |    #     |D # + 1  |
|_________|_________|__________|_________|_________|__________|_________|_________|__________|_________|_



Note: Color Number value must be in the range 1 through 8; 0 is not allowed.

Parameter Data


Index__      Name____     Type___   Description___
1            CC1         Real      First color coordinate (red) as a percent of full intensity (range
                                   0. to 100.)
2            CC2         Real      Second color coordinate (green) as a percent of full intensity
                                   (range 0. to 100.)
3            CC3         Real      Third color coordinate (blue) as a percent of full intensity (range
                                   0. to 100.)
4            CNAME       VStr    Color name; this is an optional VChracter VStr which may con-
                                   tain some verbal description of the color.  If the color name is
                                   not provided and additional pointers are required, a placekeeper
                                   must be supplied between CC3 and the first additional pointer.
*/

#ifndef __ColorDefinition_h__
#define __ColorDefinition_h__

#include "IgesEntity.h"
struct VColor;

class ColorDefinition : public IgesEntity
{
public:
	ColorDefinition();
	ColorDefinition (int r255 , int g255 , int b255);
	virtual void GetEntityIn( FileLines & mylines , int poffs);
	virtual void WriteEntityOut (std::vector <VStr> &strvec)const;	
	virtual void GetChildren (std::vector<int> &childVec)const;
	virtual void AcceptVisitor (IgesEntityVisitor &visitor)const;
	//virtual bool IsPrimitiveEntity()const;
	//virtual double GetSortNo ()const;
	virtual IgesEntity * MakeCopyAndAddToSet (const IgesEntitySet &mainSet , IgesEntitySet &newSet)const;
	bool IsEqualavent (const ColorDefinition &toComp)const;
	vreal m_CC1 , m_CC2 , m_CC3;
	VStr m_CNAME;
	VStr GetColorString (FileLines & mylines);
	VColor GetVColor()const;
};

#endif