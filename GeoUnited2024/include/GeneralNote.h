// GeneralNote.h
//(TYPE 212)
//Index__      Name____     Type___   Description___
// 1           NS           Integer   Number of text strings in General Note
// 2           NC1         Integer   Number of VChracters in first VStr (TEXT1) or zero. The num-
//                                   ber of VChracters (NCn) must always be equal to the VChracter
//                                   count of its corresponding text VStr (TEXTn)
// 3           WT1         Real      Box width
// 4           HT1         Real      Box height
// 5           FC1         Integer   Font code (default = 1)
//                          or
//                          Pointer   Pointer to the DE of the Text Font Definition Entity if negative
// 6           SL1         Real      Slant angle of TEXT1 in radians (ss=2 is the value for no slant
//                                   angle and is the default value)
// 7           A1           Real      Rotation angle in radians for TEXT1
// 8           M1           Integer   Mirror flag:
//                                       0 = no mirroring
//                                       1 = mirror axis is perpendicular to text base line
//                                       2 = mirror axis is text base line
// 9           VH1         Integer   Rotate internal text flag:
//                                       0 = text horizontal
//                                       1 = text vertical
//10           XS1         Real      First text start point
//11           YS1         Real
//12           ZS1         Real      Z depth from XT, YT plane
//13           TEXT1       VStr    First text string
//14           NC2         Integer   Number of VChracters in second text string
//..           .            .
//.            ..           ..
//-10+12*NS  NCNS        Integer   Number of VChracters in last text string
//..           .            .
//.            ..           ..
//1+12*NS    TEXTNS      VStr    Last text string


#ifndef __GeneralNote_h__
#define __GeneralNote_h__
#include "IgesEntity.h"

class GeneralNote:public IgesEntity
{
public:
	GeneralNote();
	GeneralNote (int textLen 
				, vreal boxWidth
				, vreal boxHeight
				, int fontCode
				, vreal slantAngle
				, vreal rotAngle
				, int mirrorFlag // 0 no mirror
				, int horVerFlag // 0 horizantal 1 vertical
				, vreal xc
				, vreal yc 
				, vreal zc
				, const VStr &txt); 
	GeneralNote(int nStrings
		, std::vector<int> &allTxtLen
		, std::vector<vreal> &allBoxW
		, std::vector<vreal> &allBoxH
		, std::vector<int> &allFontC
		, std::vector<vreal> &allSlantAngle
		, std::vector<vreal> &allRotAngle
		, std::vector<int> &allMirrorFlag
		, std::vector<int> &allHorVerFlag
		, std::vector<vreal> &allXc
		, std::vector<vreal> &allYc
		, std::vector<vreal> &allZc
		, std::vector<VStr> &allTxt
		);
	GeneralNote(std::vector<VStr> manyStrings
		, vreal boxWidth
		, vreal boxHeight
		, int fontCode
		, vreal slantAngle
		//, vreal rotAngle
		//, int mirrorFlag // 0 no mirror
		//, int horVerFlag // 0 horizantal 1 vertical
		, vreal firstxc
		, vreal firstyc
		, vreal firstzc);
	virtual void GetEntityIn (FileLines &mylines,int pofs);
	virtual void WriteEntityOut (std::vector<VStr> &strvec)const;
	virtual void GetChildren (std::vector<int> &childVec)const;
	virtual void AcceptVisitor (IgesEntityVisitor &visitor)const;
	//virtual bool IsPrimitiveEntity()const;	
	//virtual double GetSortNo ()const;
	virtual IgesEntity * MakeCopyAndAddToSet (const IgesEntitySet &mainSet , IgesEntitySet &newSet)const;

	int NS;
	std::vector<int>NC;
	std::vector<vreal>WT;//box width
	std::vector<vreal>HT;//box height
	std::vector<int>FC;//font code
	std::vector<vreal>SL;//slant angle of text in radians
	std::vector<vreal>A;//rotation angle in radians
	std::vector<int>M;//mirror flag
	std::vector<int>VH;//rotate horizantal vertical flag 0 horizantal
	std::vector<vreal>XS;//text start X coord
	std::vector<vreal>YS;//text start Y coord
	std::vector<vreal>ZS;//text start Z coord
	std::vector<VStr>TEXT;
	std::map<VStr, VStr> GetKeyValMap()const;
	bool IsVTransformationGroupNote(const VStr &typeName)const;
};

#endif