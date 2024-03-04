// ColorDefinition.cpp

#include "stdafx.h"
#include "ColorDefinition.h"
#include "Test_IgesSerialize.h"
#include "IgesEntityVisitor.h"
#include "IgesEntitySet.h"
#include "VColor.h"

ColorDefinition::ColorDefinition()
{
	entitytype = 314;	
}
ColorDefinition::ColorDefinition(int r255, int g255, int b255)
{
	entitytype = 314;
	m_CC1 = r255  *0.3921568627450980392156862745098; //* 100 / 255;
	m_CC2 = g255 * 0.3921568627450980392156862745098;
	m_CC3 = b255 * 0.3921568627450980392156862745098;
}
void ColorDefinition::GetEntityIn( FileLines & mylines , int poffs)
{
	DoubleVector myvect;
	GetAllParam ( myvect , mylines );
	if (myvect.size() <= 3)
		throw VException (( IDS_ColGetExcNotEno),1);
	m_CC1 = static_cast<vreal>(myvect[1]);
	m_CC2 = static_cast<vreal>(myvect[2]);
	m_CC3 = static_cast<vreal>(myvect[3]);
	m_CNAME = GetColorString(mylines);
#ifdef IGES_TEST_MODE
	Test_IgesSerialize mytester;
	mytester.Test_WriteEntityOut (*this,myvect);
#endif
}

void ColorDefinition::WriteEntityOut (std::vector <VStr> &strvec)const
{
	WriteParam (entitytype , strvec );
	WriteParam (m_CC1 , strvec);
	WriteParam (m_CC2,strvec);
	WriteParam (m_CC3,strvec);
	if (m_CNAME.length())
		WriteParam(m_CNAME , strvec);
}

void ColorDefinition::GetChildren (std::vector<int> &childVec)const
{

}

void ColorDefinition::AcceptVisitor (IgesEntityVisitor &visitor)const
{
	visitor.AcceptVirtual (*this);
}

//bool ColorDefinition::IsPrimitiveEntity()const
//{
//	return true;
//}

//double ColorDefinition::GetSortNo ()const
//{
//	return 2.0;
//}

IgesEntity * ColorDefinition::MakeCopyAndAddToSet (const IgesEntitySet &mainSet , IgesEntitySet &newSet)const
{
	if (m_subsetIndex !=0)
		return newSet.GetEntityPointer(m_subsetIndex);

	ColorDefinition *mycopy = new ColorDefinition (*this);
	CopyDE (*mycopy,mainSet,newSet);
	newSet.InsertEntity(mycopy);
	m_subsetIndex = mycopy->directorylineno;
	return mycopy;
}

bool ColorDefinition::IsEqualavent(const ColorDefinition &toComp)const
{
	if ( fabs(m_CC1 - toComp.m_CC1) < 1.e-6
		&& fabs (m_CC2 - toComp.m_CC2 ) < 1.e-6
		&& fabs (m_CC3 - toComp.m_CC3 ) < 1.e-6
		&& m_CNAME == toComp.m_CNAME)
		return true;
	else 
		return false;

}

VStr ColorDefinition::GetColorString(FileLines &mylines)
{
	VStr ts;
	size_t mysize = mylines.size();
	for (size_t i = 0;i<mysize; ++i )
	{
		ProcessLine *pl = mylines.at(i);
		VStr tmp (pl->GetArray());
		size_t nn = tmp.length() < 64 ? tmp.length() : 64;
		for (size_t ii=0 ; ii<nn; ++ii)
		{
			VChr mychr = tmp.at (ii);
			//VStr tmp2  = mychr;
			ts+=mychr;
		}
	}
	size_t mysize2 = ts.length();
	int comacount = 0;
	string res;
	for (size_t j=0; j<mysize2; ++j)
	{
		VChr  cc = ts.at(j);
		if (cc == ProcessLine::GetParamDelimiter())
		{
			comacount ++;
			if (comacount == 4)
			{
				string cnt;
				size_t w = j+1;
				for (w; w<mysize2; ++w)
				{
					VChr cc2 = ts.at(w);
					if (cc2 == 'H' || cc2 == 'h')
					{	
						VStrStream mystr;
						mystr << cnt;
						size_t mycount;
						mystr >> mycount;
						size_t k = w + 1;
						for (k; k< (w+1+mycount); ++k)
						{
							res += ts.at(k);
						}
						return res;
					}
					else
					{
						cnt += cc2;
					}
				}
			}
		}
	}
	return res;
}

VColor ColorDefinition::GetVColor()const
{
	return VColor(m_CC1 * 0.01, m_CC2 * 0.01, m_CC3 * 0.01);
}