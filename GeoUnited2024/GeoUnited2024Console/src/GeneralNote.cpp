// GeneralNote.cpp
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

#include "stdafx.h"
#include "GeneralNote.h"
#include "IgesEntitySet.h"
#include "IgesEntityVisitor.h"
#include "GlobalSection.h"

GeneralNote::GeneralNote()
{
	entitytype = 212;

}
GeneralNote::GeneralNote (int textLen 
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
						, const VStr &txt)
{
	entitytype = 212;
	NS = 1;
	NC.resize (1);
	NC[0] = textLen;
	WT.resize(1);
	WT[0] = boxWidth;
	HT.resize(1);
	HT[0] = boxHeight;
	FC.resize(1);
	FC[0] = fontCode;
	SL.resize(1);
	SL[0] = slantAngle;
	A.resize(1);
	A[0] = rotAngle;
	M.resize(1);
	M[0] = mirrorFlag;
	VH.resize(1);
	VH[0] = horVerFlag;
	XS.resize(1);
	XS[0] = xc;
	YS.resize(1);
	YS[0] = yc;
	ZS.resize(1);
	ZS[0] = zc;
	TEXT.resize(1);
	VOStrStream mystream;
	mystream << txt.length();
	mystream << 'H';
	mystream << txt;
	TEXT[0] = mystream.str();
}
GeneralNote::GeneralNote
(int nStrings
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
)
{
	entitytype = 212;
	NS = nStrings;
	NC.resize(NS);
	WT.resize(NS);
	HT.resize(NS);
	FC.resize(NS);
	SL.resize(NS);
	A.resize(NS);
	M.resize(NS);
	VH.resize(NS);
	XS.resize(NS);
	YS.resize(NS);
	ZS.resize(NS);
	TEXT.resize(NS);

	for (size_t i = 0; i < NS; ++i)
	{
		NC[i] = allTxtLen.at(i);
		WT[i] = allBoxW.at(i);
		HT[i] = allBoxH.at(i);
		FC[i] = allFontC.at(i);
		SL[i] = allSlantAngle.at(i);
		A[i] = allRotAngle.at(i);
		M[i] = allMirrorFlag.at(i);
		VH[i] = allHorVerFlag.at(i);
		XS[i] = allXc.at(i);
		YS[i] = allYc.at(i);
		ZS[i] = allZc.at(i);
		VOStrStream mystream;
		mystream << allTxt.at(i).length();
		mystream << 'H';
		mystream << allTxt.at(i);
		TEXT[i] = mystream.str();
	}
}
// 212,1,8,11.3198820326678,2.07,1,1.5707963267949,0.0,0,0,148.5,      1225P   1185
// 30.6500000000002,0.0,8HMATERIAL,0,0;                                1225P   1186

// NC1(int,VChrno),WT1(real,boxw),HT1(real,boxh),FC1(int,fontcode),SL1(real,slant angle)
//,A1(real,rotation a),M1(int,mirror flag),VH1(int,rot flag 0HOR-1VER)
//,XS1(real,first txt start p)YS1 ZS1,TEXT1 (string ,first text string)

void GeneralNote::GetEntityIn (FileLines &mylines,int pofs)
{
	int mysize = (int)mylines.size();
	std::vector<VStr> strVec;
	VStr tmp; 
	for (int i = 0;i<mysize;i++)
	{
		ProcessLine *currline = mylines[i];
		for (int w=0;w<64;w++)//cutting directory and parameter line no
		{
			VChr currVChr = currline->GetChar(w);
			if (currVChr == 'H')
			{
				// reading str len
				VStr::reverse_iterator jj = tmp.rbegin();
				VStr::reverse_iterator je = tmp.rend();
				VStr tmp3;
				VStr tmp2;
				for (jj; jj != je; ++jj)
				{
					if (*jj != PARAMETER_DELIMITER && *jj != RECORD_DELIMITER)
					{
						tmp2 += *jj;
					}
					else
					{
						break;
					}
				}
				VStr::reverse_iterator jjj = tmp2.rbegin();
				VStr::reverse_iterator jje = tmp2.rend();
				for (jjj; jjj != jje; ++jjj)
				{
					tmp3 += *jjj;
				}
				tmp.clear();
				int strLen = TypeConverters::ConvertStringToInt(tmp3);
				for (int ij = 0; ij < strLen; ++ij)
				{
					currVChr = currline->GetChar(w + 1 + ij);
					tmp.push_back(currVChr);
				}
				w += (strLen + 1);
				currVChr = currline->GetChar(w);
				
			}
			if (currVChr != PARAMETER_DELIMITER && currVChr != RECORD_DELIMITER)
				tmp.push_back(currVChr);
			else
			{
				//tmp.push_back(0);
				strVec.push_back(tmp);
				tmp.clear();
			}
		}		
	}
	double temp;
	VChr ec = ',';
	VChr * pec = &ec;

	temp = strtod(strVec[0].data(),&pec);
	entitytype = (int)(temp);
	temp = strtod(strVec[1].data(),&pec);
	NS = (int)(temp);
	NC.resize(NS);
	WT.resize(NS);
	HT.resize(NS);
	FC.resize(NS);
	SL.resize(NS);
	A.resize(NS);
	M.resize(NS);
	VH.resize(NS);
	XS.resize(NS);
	YS.resize(NS);
	ZS.resize(NS);
	TEXT.resize(NS);
	for (int j = 0;j<NS;j++)
	{
		temp = strtod(strVec[j*12+2].data(),&pec);
		NC[j] = (int)temp;
		temp = strtod(strVec[j*12+3].data(),&pec);
		WT[j] = temp;
		temp = strtod(strVec[j*12+4].data(),&pec);
		HT[j]=temp;
		temp = strtod(strVec[j*12+5].data(),&pec);
		FC[j]=(int)temp;
		temp = strtod(strVec[j*12+6].data(),&pec);
		SL[j] = temp;
		temp = strtod (strVec[j*12+7].data(),&pec);
		A[j] = temp;
		temp = strtod (strVec[j*12+8].data(),&pec);
		M[j]= (int)temp;
		temp = strtod (strVec[j*12+9].data(),&pec);
		VH[j] = (int)temp;
		temp = strtod (strVec[j*12+10].data(),&pec);
		XS[j] = temp;
		temp = strtod (strVec[j*12+11].data(),&pec);
		YS[j] = temp;
		temp = strtod (strVec[j*12+12].data(),&pec);
		ZS[j] = temp;
		//TEXT[j] =strVec[j*12+13];
	}
}
void GeneralNote::WriteEntityOut (std::vector<VStr> &strvec)const
{
	WriteParam (entitytype,strvec);
	WriteParam (NS,strvec);
	for (int j = 0;j<NS;j++)
	{
		WriteParam (NC[j],strvec);
		WriteParam (WT[j],strvec);
		WriteParam (HT[j],strvec);
		WriteParam (FC[j],strvec);
		WriteParam (SL[j],strvec);
		WriteParam (A[j],strvec);
		WriteParam (M[j],strvec);
		WriteParam (VH[j],strvec);
		WriteParam (XS[j],strvec);
		WriteParam (YS[j],strvec);
		WriteParam (ZS[j],strvec);
		strvec.push_back (TEXT[j]);
	}
}

void GeneralNote::GetChildren (std::vector<int> &childVec)const
{
	GetDirectoryChild (childVec);
}

//bool GeneralNote::IsPrimitiveEntity()const
//{
//	return false;
//}

//double GeneralNote::GetSortNo ()const
//{
//	return 9.5;
//}

IgesEntity * GeneralNote::MakeCopyAndAddToSet (const IgesEntitySet &mainSet , IgesEntitySet &newSet)const
{
	if (m_subsetIndex != 0)
		return newSet.GetEntityPointer (m_subsetIndex);
	GeneralNote *mycopy = new GeneralNote (*this);
	CopyDE (*mycopy,mainSet,newSet);
	newSet.InsertEntity (mycopy);
	m_subsetIndex = mycopy->directorylineno;
	return mycopy;
}

void GeneralNote::AcceptVisitor (IgesEntityVisitor &visitor)const
{
	visitor.AcceptVirtual (*this);
}

GeneralNote::GeneralNote(std::vector<VStr> manyStrings
	, vreal boxWidth
	, vreal boxHeight
	, int fontCode
	, vreal slantAngle
	//, vreal rotAngle
	//, int mirrorFlag // 0 no mirror
	//, int horVerFlag // 0 horizantal 1 vertical
	, vreal firstxc
	, vreal firstyc
	, vreal firstzc)
{
	entitytype = 212;
	NS = (int)manyStrings.size();
	NC.resize(NS);
	WT.resize(NS);
	HT.resize(NS);
	FC.resize(NS);
	SL.resize(NS);
	A.resize(NS);
	M.resize(NS);
	VH.resize(NS);
	XS.resize(NS);
	YS.resize(NS);
	ZS.resize(NS);
	TEXT.resize(NS);

	for (size_t i = 0; i < NS; ++i)
	{
		NC[i] = (int)manyStrings.at(i).length();
		WT[i] = boxWidth;
		HT[i] = boxHeight;
		FC[i] = fontCode;
		SL[i] = slantAngle;
		A[i] = 0.0;
		M[i] = 0;
		VH[i] = 0;
		XS[i] = firstxc;
		YS[i] = firstyc + boxHeight * i;
		ZS[i] = firstzc;
		VOStrStream mystream;
		mystream << manyStrings.at(i).length();
		mystream << 'H';
		mystream << manyStrings.at(i);
		TEXT[i] = mystream.str();
	}
}

std::map<VStr, VStr> GeneralNote::GetKeyValMap()const
{
	if (NS % 2 != 0)
		throw VException("GeneralNote::GetKeyValMap exception , text count is not even",1);
	std::map<VStr, VStr> keyValMap;
	for (int i = 0; i < NS; i+=2)
	{
		keyValMap[TEXT[i]] = TEXT[i + 1];
	}
	return keyValMap;
}

bool GeneralNote::IsVTransformationGroupNote(const VStr &typeName)const
{
	if (NS > 0)
	{
		if (TEXT[0] == typeName)
			return true;
	}
	return false;
}