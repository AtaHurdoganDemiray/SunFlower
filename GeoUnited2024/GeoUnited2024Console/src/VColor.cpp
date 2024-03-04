// VColor.cpp

#include "stdafx.h"
#include "VColor.h"
#include "ColorDefinition.h"

VColor::VColor ()
{

}

VColor::VColor ( const vreal &_R , const vreal &_G , const vreal &_B )
{
	r = _R;
	g = _G;
	b = _B;
}

VColor::VColor (COLORREF color)
{
	r = GetRValue (color) / 255.0;
	g = GetGValue (color) / 255.0;
	b = GetBValue (color) / 255.0;
}

const vreal & VColor::R() const
{
	return r;
}
const vreal & VColor::G() const
{
	return g;
}
const vreal & VColor::B() const
{
	return b;
}

void VColor::R ( const vreal & _R )
{
	r = _R;
}

void VColor::G ( const vreal & _G )
{
	g = _G;
}

void VColor::B ( const vreal & _B )
{
	b = _B;
}

VColor::VColor ( VIFStream & infile )
{
	(*this)<<infile;
}

void VColor::operator >> ( VofStream & outfile ) const
{
	outfile<<r;
	outfile<<g;
	outfile<<b;
}

void VColor::operator << ( VIFStream & infile )
{
	infile>>r;
	infile>>g;
	infile>>b;
}

void VColor::Serialize (Bofstream &binfile)const
{
	binfile.write ( (VChr *) &(r) , sizeof (vreal) );
	binfile.write ( (VChr *) &(g) , sizeof (vreal) );
	binfile.write ( (VChr *) &(b) , sizeof (vreal) );
}

void VColor::Serialize(Bifstream &binfile) 
{
	binfile.read ( (VChr *) &(r) , sizeof (vreal) );
	binfile.read ( (VChr *) &(g) , sizeof (vreal) );
	binfile.read ( (VChr *) &(b) , sizeof (vreal) );
};

const ColorDefinition VColor::GetColorDefinition()const
{
	ColorDefinition col;
	col.m_CC1 = 100 * r;
	col.m_CC2 = 100 * g;
	col.m_CC3 = 100 * b;
	return col;
}

const bool VColor::operator ==(const VColor &tock) const
{
	if (fabs(b-tock.b)<1.e-3 && fabs(g-tock.g)<1.e-3 && fabs(r-tock.r)<1.e-3)
		return true;
	else
		return false;
}

COLORREF VColor::Convert ()const
{
	COLORREF res = RGB (r*255,g*255,b*255);
	return res;
}

VColor VColor::GetInverseColor()const
{
	VColor c (1.0-r,1.0-g,1.0-b);
	return c;
}

int VColor::Ri()const
{
	return (int)(r * 255.0);
}
int VColor::Gi()const
{
	return (int)(g * 255.0);
}
int VColor::Bi()const
{
	return (int)(b * 255.0);
}

VColor::VColor (int rgb[3])
{
	vreal invc = 1. / 255.;
	r = (vreal)rgb[0] * invc;
	g = (vreal)rgb[1] * invc;
	b = (vreal)rgb[2] * invc;
}