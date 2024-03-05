// PointVisualAtt.cpp

#include "stdafx.h"
#include "PointVisualAtt.h"
#include "ViewPointDefaults.h"

PointVisualAtt::PointVisualAtt()
	: pointcolor ( VGlobalValues::viewpointdefaults.m_defaultPointColor)
{		
	pointsize = VGlobalValues::viewpointdefaults.m_defaultPointSize;
	draw = true;
}

const VColor & PointVisualAtt::GetPointColor () const
{
	return pointcolor;
}

void PointVisualAtt::ChangePointColor ( const VColor & newcolor )
{
	pointcolor = newcolor;
}

void PointVisualAtt::SetPointSize ( const vreal & newsize )
{
	pointsize = newsize;
}

const vreal & PointVisualAtt::GetPointSize() const
{
	return pointsize;
}

void PointVisualAtt::SetDraw ( bool newdraw )
{
	draw = newdraw;
}

bool PointVisualAtt::GetDraw () const
{
	return draw;
}

//PointVisualAtt::PointVisualAtt ( VIFStream & infile )
//{
//	(*this)<<infile;
//}

//void PointVisualAtt::operator >> ( VofStream & outfile ) const
//{
//	pointcolor>>outfile;
//	outfile<<draw;
//	outfile<<pointsize;
//}

//void PointVisualAtt::operator << ( VIFStream & infile )
//{
//	pointcolor<<infile;
//	infile>>draw;
//	infile>>pointsize;
//}

void PointVisualAtt::Serialize(Bofstream &outfile)const
{
	int version=1;
	outfile.write((VChr*)&version,sizeof(int));
	if(version==1)
	{
		outfile.write((VChr*)&draw,sizeof(bool));
		outfile.write((VChr*)&pointsize,sizeof(vreal));
		pointcolor.Serialize(outfile);
	}
	else
		throw VException(( IDS_PoiSerExc),1);
}	
void PointVisualAtt::Serialize(Bifstream &infile)
{
	int version;
	infile.read((VChr*)&version,sizeof(int));
	if(version==1)
	{
		infile.read((VChr*)&draw,sizeof(bool));
		infile.read((VChr*)&pointsize,sizeof(vreal));
		pointcolor.Serialize(infile);
	}
	else
		throw VException (( IDS_PoiSerExc1),2);
}

const bool PointVisualAtt::operator ==(const PointVisualAtt &tock) const
{
	if (draw == tock.draw && pointcolor == tock.pointcolor && pointsize == tock.pointsize)
		return true;
	else
		return false;
}
