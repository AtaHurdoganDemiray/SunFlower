// LineVisualAtt.cpp

#include "stdafx.h"
#include "LineVisualAtt.h"

#include "ViewPointDefaults.h"

LineVisualAtt::LineVisualAtt()
	: wirecolor ( VGlobalValues::viewpointdefaults.m_defaultWireframeColor )
{
	//linewidth = 1.0;  // mid
	//linewidth = 2.0;  // thick 
	//linewidth = 0.25; // thin
	linewidth = VGlobalValues::viewpointdefaults.m_defaultLineWidth; // normal
	linefont = (LineFont)(VGlobalValues::viewpointdefaults.m_defaultLineFont);//LINEFONT_NORMAL;
	draw = true;
}

const VColor & LineVisualAtt::GetWireColor () const 
{
	return wirecolor;
}

void LineVisualAtt::ChangeWireColor ( const VColor & newcolor )
{
	wirecolor = newcolor;
}

void LineVisualAtt::SetLineWidth ( const vreal & newwidth )
{
	linewidth = newwidth;
}

LineVisualAtt::LineFont LineVisualAtt::GetLineFont() const
{
	return linefont;
}

void LineVisualAtt::SetLineFont ( LineVisualAtt::LineFont newfont )
{
	linefont = newfont;
}

const vreal & LineVisualAtt::GetLineWidth() const
{
	return linewidth;
}

void LineVisualAtt::SetDraw ( bool newdraw )
{
	draw = newdraw;
}

bool LineVisualAtt::GetDraw () const
{
	return draw;
}

//LineVisualAtt::LineVisualAtt ( VIFStream & infile )
//{
//	(*this)<<infile;
//}

//void LineVisualAtt::operator >> ( VofStream & outfile ) const
//{
//	wirecolor>>outfile;
//	outfile<<draw;
//	outfile<<linewidth;
//	outfile<<linefont;
//}

//void LineVisualAtt::operator << ( VIFStream & infile ) 
//{
//	wirecolor<<infile;
//	infile>>draw;
//	infile>>linewidth;
//	int tmp;
//	infile>>tmp;
//	linefont = static_cast<LineFont>(tmp);
//}

void LineVisualAtt::SetLineWidth (LineWidth widthtype)
{
	switch (widthtype)
	{
	case(thin):
		SetLineWidth(0.25);
	case(normal):
		SetLineWidth(0.5);
		break;
	case(mid):
		SetLineWidth(1.0);
		break;
	case(thick):
		SetLineWidth(2.0);
		break;
	};
}

void LineVisualAtt::Serialize(Bofstream &outfile)const
{
	int version=1;
	outfile.write((VChr*)&version,sizeof(int));
	if(version==1)
	{
		outfile.write((VChr*)&draw,sizeof(bool));
		outfile.write((VChr*)&linefont,sizeof(LineFont));
		outfile.write((VChr*)&linewidth,sizeof(vreal));
		wirecolor.Serialize(outfile);
	}
	else
		throw VException(( IDS_LinSerExc),1);
}
void LineVisualAtt::Serialize(Bifstream &infile)
{
	int version;
	infile.read((VChr*)&version,sizeof(int));
	if(version==1)
	{
		infile.read((VChr*)&draw,sizeof(bool));
		infile.read((VChr*)&linefont,sizeof(LineFont));
		infile.read((VChr*)&linewidth,sizeof(vreal));
		wirecolor.Serialize(infile);
	}
	else
		throw VException(( IDS_LinSerExc),2);
}

const bool LineVisualAtt::operator ==(const LineVisualAtt &tock) const
{
	if (draw == tock.draw && linefont == tock.linefont && linewidth == tock.linewidth && wirecolor == tock.wirecolor)
		return true;
	else
		return false;
}
