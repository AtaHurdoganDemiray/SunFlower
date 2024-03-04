// FacetVisualAtt.cpp

#include "stdafx.h"
#include "FacetVisualAtt.h"

#include "ViewPointDefaults.h"
FacetVisualAtt::FacetVisualAtt ()
//: shadecolor ( 0.0 , 128.0/255.0 , 0.0 ) 
: shadecolor (VGlobalValues::viewpointdefaults.m_defaultSurfaceColor)
{
	translucent = VGlobalValues::viewpointdefaults.m_defaultIsTranslucent == 0 ? false:true;
	draw = true;
}

const VColor & FacetVisualAtt::GetShadeColor () const
{
	return shadecolor;
}

void FacetVisualAtt::ChangeShadeColor ( const VColor & newcolor )
{
	shadecolor = newcolor;
}

void FacetVisualAtt::SetTranslucent( bool newval )
{
	translucent = newval;
}

bool FacetVisualAtt::GetTranslucent() const
{
	return translucent;
}

void FacetVisualAtt::SetDraw ( bool newdraw )
{
	draw = newdraw;
}

bool FacetVisualAtt::GetDraw () const
{
	return draw;
}

//const VUChr * FacetVisualAtt::GetHalfTone () const
//{
//	return halftone;
//}

//FacetVisualAtt::FacetVisualAtt ( VIFStream & infile )
//{
//	(*this)<<infile;
//	
//	for ( int i=0; i < 16; i++ )
//		for ( int j = 0; j < 8; j++ )
//		{
//			if ( j<4 )
//				halftone[i*8 + j] = 170;
//			else
//				halftone[i*8 + j] = 85;
//		}	
//}

//void FacetVisualAtt::operator >> ( VofStream & outfile ) const
//{
//	shadecolor>>outfile;
//	outfile<<draw;
//	outfile<<translucent;
//}

//void FacetVisualAtt::operator << ( VIFStream & infile )
//{
//	shadecolor<<infile;
//	infile>>draw;
//	infile>>translucent;
//}

void FacetVisualAtt::Serialize (Bofstream &outfile)const
{
	int version=1;
	outfile.write((VChr*)&version,sizeof(int));
	if(version==1)
	{
		outfile.write((VChr*)&draw,sizeof(bool));
		outfile.write((VChr*)&translucent,sizeof(bool));
		shadecolor.Serialize(outfile);
	}
	else
		throw VException (( IDS_FacSerExc),2);
}
void FacetVisualAtt::Serialize (Bifstream &infile)
{
	int version;
	infile.read ((VChr*)&version,sizeof(int));
	if(version==1)
	{
		infile.read((VChr*)&draw,sizeof(bool));
		infile.read((VChr*)&translucent,sizeof(bool));
		shadecolor.Serialize(infile);
	}
	else
		throw VException (( IDS_FacSerExc),2);
}

const bool FacetVisualAtt::operator == (const FacetVisualAtt &tock)const
{
	if (draw == tock.draw && shadecolor == tock.shadecolor &&translucent==tock.translucent)
		return true;
	else
		return false;
}
